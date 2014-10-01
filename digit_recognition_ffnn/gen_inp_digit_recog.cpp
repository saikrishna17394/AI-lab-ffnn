#include <iostream>
#include <map>
using namespace std;

int main() {
	map<int,int> m;
	m[119]=0;
	m[36]=1;
	m[93]=2;
	m[109]=3;
	m[46]=4;
	m[107]=5;
	m[123]=6;
	m[37]=7;
	m[127]=8;
	m[111]=9;

	for(int i=0;i<128;i++) {
		if(m.find(i)==m.end()) {
			for(int j=0;j<10;j++)
				cout<<"0 ";
			cout<<endl;
		}
		else {
			for(int j=0;j<10;j++) {
				if(j==m[i])
					cout<<"1 ";
				else
					cout<<"0 ";
			}
			cout<<endl;
		}
	}
	return 0;
}