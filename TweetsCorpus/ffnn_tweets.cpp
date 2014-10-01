#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <map>
#include <fstream>

#define ch 2

using namespace std;

struct neuron {
	vector<double> weights;
	vector<double> delta_weights;
	double output;
	double theta;
	double delta;
	double delta_theta;
	int input_count;
	

	// Creates a neuron with n input ports
	void create(int n) {
		input_count=n;
		weights.resize(n);
		delta_weights.resize(n);
		// delta_values.resize(n);
		// Setting weights values randomly in the range [-1,1]
		// cout<<n<<" ";
		for(int i=0;i<n;i++) {
			weights[i]=(double)rand()/(double)RAND_MAX;
			// cout<<weights[i]<< " ";
			delta_weights[i]=0;
		}
		// cout<<endl;
		theta=(double)rand()/(double)RAND_MAX;
		delta_theta=0;
		
		// cout<<theta<<" "<<endl;
		delta=0;
	}

 
};

struct layer {
	vector<neuron> neurons;
	int neuron_count;
	vector<double> layer_input;
	int input_count;

	// Creates a layer with n input ports
	// and m neurons in it
	void create(int n,int m) {
		input_count=n;
		neuron_count=m;
		layer_input.resize(n);
		neurons.resize(m);

		for(int i=0;i<neuron_count;i++)
			neurons[i].create(input_count);
	}

	// Calculates the output value for each 
	// neuron in the current layer
	void calculate() {
		for(int i=0;i<neuron_count;i++) {
			double val=0;
			for(int j=0;j<input_count;j++) {
				val+=neurons[i].weights[j]*layer_input[j];
			}
			val+=neurons[i].theta;
			neurons[i].output=(double)1.0/(double)(1.0+exp(-val));
			// cout<<i<<" man "<<val<<" "<<neurons[i].output<<endl;
		}
	}


};

struct network {
	layer output_layer;
	vector<layer> hidden_layers;
	int hidden_count;

	void create(int input_cnt,int output_cnt,int hidden_cnt,int neurons_cnt) {
		hidden_count=hidden_cnt;
		hidden_layers.resize(hidden_count);

		if(hidden_count) {
			hidden_layers[0].create(input_cnt,neurons_cnt);
			for(int i=1;i<hidden_count;i++) {
				hidden_layers[i].create(neurons_cnt,neurons_cnt);
			}
			output_layer.create(neurons_cnt,output_cnt);		
		}
		else {
			output_layer.create(input_cnt,output_cnt);
		}
	}

	void propagate (vector<double> input) {
		if(hidden_count==0) {
			output_layer.layer_input=input;
			// cout<<"Output Layer :\n";
			output_layer.calculate();
			return ;
		}
		hidden_layers[0].layer_input=input;
		// cout<<"Hidden Layer 0 : \n";
		hidden_layers[0].calculate();

		// Propogating the out values to input of next layer
		update(0);

		for(int i=1;i<hidden_count;i++) {
			hidden_layers[i].calculate();
			update(i);
		}
		output_layer.calculate();
	}

	// Propogates the output values to the next layer as input

	void update(int idx) {
		vector<double> v;

		for(int i=0;i<hidden_layers[idx].neuron_count;i++)
			v.push_back(hidden_layers[idx].neurons[i].output);

		if(idx+1==hidden_count) {
			output_layer.layer_input=v;
		}
		else {
			hidden_layers[idx+1].layer_input=v;
		}

	}

	double train(vector<double> desired_output,vector<double> input,double learn_rate,double momentum) {
		// cout<<"yayyy";
		// cout<<"bro\n";
		propagate(input);
		// cout<<"bro\n";
		double error=0;

		for(int j=0;j<output_layer.neuron_count;j++) {
			// cout<<j<<" "<<output_layer.neuron_count<< endl;

			double out=output_layer.neurons[j].output;
			// cout<<input[1]<<" "<<input[0]<<" k "<<input.size()<<endl;
			// cout<<j<<" "<<desired_output[j]<<" "<<out<<endl;
			double delta=(desired_output[j]-out)*out*(1.0-out);
			error+=(desired_output[j]-out)*(desired_output[j]-out);
					
			int n=output_layer.input_count;

			output_layer.neurons[j].delta=delta;
			// learn_rate*delta*output_layer.layer_input[i];
			for(int i=0;i<output_layer.input_count;i++) {
				// cout<<output_layer.neurons[j].weights[i]<<" kaka "<<delta<<" "<<learn_rate<<" "<<output_layer.layer_input[i]<<" ";
				output_layer.neurons[j].delta_weights[i]=(delta*learn_rate*output_layer.layer_input[i])+(momentum*output_layer.neurons[j].delta_weights[i]);
				output_layer.neurons[j].weights[i]+=output_layer.neurons[j].delta_weights[i];
				// cout<<output_layer.neurons[j].weights[i]<<endl;
			}
			output_layer.neurons[j].delta_theta=(delta*learn_rate)+(momentum*output_layer.neurons[j].delta_theta);
			output_layer.neurons[j].theta+=output_layer.neurons[j].delta_theta;
			// cout<<j<<" ua \n";
		}

		// cout<<"dude\n";

		for(int i=hidden_count-1;i>=0;i--) {
			// cout<<i<<endl;
			for(int j=0;j<hidden_layers[i].neuron_count;j++) {
				double delta=0;
				double val=0;

				if(i+1==hidden_count) {
					for(int k=0;k<output_layer.neuron_count;k++)
						delta+=(output_layer.neurons[k].weights[j])*output_layer.neurons[k].delta;
				}
				else {
					for(int k=0;k<hidden_layers[i+1].neuron_count;k++)
						delta+=(hidden_layers[i+1].neurons[k].weights[j])*hidden_layers[i+1].neurons[k].delta;
				}
				double out=hidden_layers[i].neurons[j].output;
				delta*=out*(1.0-out);
				hidden_layers[i].neurons[j].delta=delta;

				for(int k=0;k<hidden_layers[i].input_count;k++) {
					hidden_layers[i].neurons[j].delta_weights[k]=learn_rate*delta*hidden_layers[i].layer_input[k]+momentum*hidden_layers[i].neurons[j].delta_weights[k];
					hidden_layers[i].neurons[j].weights[k]+=hidden_layers[i].neurons[j].delta_weights[k];
				}
				hidden_layers[i].neurons[j].delta_theta=delta*learn_rate+momentum*hidden_layers[i].neurons[j].delta_theta;
				hidden_layers[i].neurons[j].theta+=hidden_layers[i].neurons[j].delta_theta;
			}
		}
		// cout<<"bro\n";
		return error;
	}
};

int main() {
	int input_ports,output_ports,hid_layers,neuron_cnt;
	double learn_rate,momentum;
	input_ports=443;
	output_ports=3;
	// cout<<"Enter the number of input ports : ";
	// cin>>input_ports;
	// cout<<"Enter the number of output ports : ";
	// cin>>output_ports;
	
	// cout<<"Enter the number of hidden layers : ";
	// cin>>hid_layers;
	hid_layers=0;

	// cout<<"Enter the number of neurons in the hidden layer : ";
	// cin>>neuron_cnt;
	neuron_cnt=10;

	// cout<<"Enter the learning rate : ";
	// cin>>learn_rate;
	// cout<<"Enter the momentum : ";
	// cin>>momentum;
	learn_rate=0.1;
	momentum=0.8;
	// cout<<"Fill the truth tables \n";

	vector<double> truth_table[498];
	vector<double> input_pattern[498];

	// int A[100];

	for(int i=0;i<498;i++) {
		truth_table[i].resize(output_ports);
		input_pattern[i].resize(input_ports);

		int num=i;
		for(int j=0;j<input_ports;j++) {
			// input_pattern[i][j]=num%2;
			// // A[j]=num%2;
			// num/=2;

			cin>>input_pattern[i][j];
			// cout<<input_pattern[i][j]<<" ";
		}

		// for(int j=input_ports-1;j>=0;j--) {
		// 	cout<<input_pattern[i][j]<<" ";
		// }
		// cout<<" : ";

		for(int j=0;j<output_ports;j++) {
			cin>>truth_table[i][j];
			// cout<<truth_table[i][j]<<" ";			
		}
		// cout<<endl;
	}

	network ffnn;
	ffnn.create(input_ports,output_ports,hid_layers,neuron_cnt);

	int cnt=1;
	while(1) {
		bool ok=true;

		double error=0;

		for(int i=0;i<498;i++) {
			if(i%5==ch)
				continue;
			//  cout<<i<<" "<<error<<endl;
			error+=(ffnn.train(truth_table[i],input_pattern[i],learn_rate,momentum)/2.0);
			// cout<<i<<" "<<error<<endl;
			cnt++;
			// if(error > 0.2)
			// 	ok=false;
		}
		cout<<cnt<<" "<<error<<endl;
			
		if(error<8 || cnt>500000)
			break;
	}

	cout<<"\n------------------------------------\n";

	for(int i=0;i<498;i++) {
		cout<<"Input : "<<i<<endl;
		ffnn.propagate(input_pattern[i]);
		cout<<"Output Layer output:\n";
		for(int j=0;j<output_ports;j++) {
			cout<<ffnn.output_layer.neurons[j].output<<" ";
		}
		cout<<endl;
		for(int i=0;i<hid_layers;i++) {
			cout<<"Hidden Layer : "<<i+1<<endl;
			for(int j=1;j<=neuron_cnt;j++) {
				cout<<"Neuron "<<j<<" "<<ffnn.hidden_layers[i].neurons[j-1].output<<endl;
			}
		}
		cout<<endl;
		cout<<"------------------------------------\n";
	}


	int correct=0,wrong=0;

	for(int i=0;i<498;i++) {
		if(i%5!=ch)
			continue;
		cout<<"Input : "<<i<<endl;
		ffnn.propagate(input_pattern[i]);
		cout<<"Output Layer output:\n";
		for(int j=0;j<output_ports;j++) {
			cout<<ffnn.output_layer.neurons[j].output<<" ";
		}
		cout<<endl;

		// for(int i=0;i<hid_layers;i++) {
		// 	cout<<"Hidden Layer : "<<i+1<<endl;
		// 	for(int j=1;j<=neuron_cnt;j++) {
		// 		cout<<"Neuron "<<j<<" "<<ffnn.hidden_layers[i].neurons[j-1].output<<endl;
		// 	}
		// }

		int ans=0,ex;
		double numb=ffnn.output_layer.neurons[0].output;

		if(ffnn.output_layer.neurons[1].output>numb) {
			numb=ffnn.output_layer.neurons[1].output;
			ans=1;
		}

		if(ffnn.output_layer.neurons[2].output>numb) {
			numb=ffnn.output_layer.neurons[2].output;
			ans=2;
		}

		if(i<140)
			ex=0;
		else if(i<280)
			ex=2;
		else if(i<390)
			ex=1;
		else if(i<432)
			ex=0;
		else if(i<469)
			ex=2;
		else
			ex=1;
		if(ans==ex) {
			cout<<"CORRECT ANSWER :) \n";
			correct++;
		}
		else {
			cout<<"WRONG ANSWER :( \n";
			wrong++;
		}
		cout<<endl;
		cout<<"------------------------------------\n";
	}

	cout<<"Learn rate              : "<<learn_rate<<endl;
	cout<<"momentum                : "<<momentum<<endl;
	cout<<"Total correct           : "<<correct<<endl;
	cout<<"Total wrong             : "<<wrong<<endl;
	cout<<"Number of iterations    : "<<cnt<<endl;
	return 0;
}