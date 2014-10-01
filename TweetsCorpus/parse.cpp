#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <map>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

map<string,int> m,stop_words;
string ret;

bool check(string s) {
	ret="";
	string http="http:";
	if(s.length()>=1 && s[0]=='@')
		return false;
	// cout<<s.substr(0,5)<<" "<<http<<endl;
	if(s.length()>=5 && http.compare(s.substr(0,5))==0) {
		return false;
	}

	for(int i=0;i<s.length();i++) {
		if(s[i]>='a' && s[i]<='z')
			ret+=s[i];
		else if(s[i]>='A' && s[i]<='Z')
			ret+=(s[i]-'A'+'a');
		else
			;
	}
	if(ret.length()<3)
		return false;
	return true;
}

string st;

bool stem (string s) {
	int len=s.length();
	st="";

	if(len>2 && (s.substr(len-2)).compare("ed")==0 && m.find(s.substr(0,len-2))!=m.end()) {
		st=s.substr(0,len-2);
		return true;
	}

	if(len>2 && (s.substr(len-2)).compare("er")==0 && m.find(s.substr(0,len-2))!=m.end()) {
		st=s.substr(0,len-2);
		return true;
	}

	if(len>2 && (s.substr(len-2)).compare("ly")==0 && m.find(s.substr(0,len-2))!=m.end()) {
		st=s.substr(0,len-2);
		return true;
	}

	if(len>3 && (s.substr(len-3)).compare("ing")==0 && m.find(s.substr(0,len-3))!=m.end()) {
		st=s.substr(0,len-3);
		return true;
	}

	if(len>1 && (s.substr(len-1)).compare("s")==0 && m.find(s.substr(0,len-1))!=m.end()) {
		st=s.substr(0,len-1);
		return true;
	}

	return false;
}

// Stemming cheyi

int main() {
	fstream f("stop_words");
	fstream f1("twitter_positive");
	fstream f2("twitter_negative");
	fstream f3("twitter_objective");

	string s;

	
	while(getline(f,s)) {
		stop_words[s]=1;
		// cout<<s<<endl;
	}

	while(getline(f1,s)) {
		stringstream ss(s);

		string word;

		while(ss>>word) {
			if(check(word) && stop_words.find(ret)==stop_words.end())
				m[ret]++;
			// cout<<word<<endl;
		}
		
	}

	while(getline(f2,s)) {
		stringstream ss(s);

		string word;

		while(ss>>word) {
			if(check(word) && stop_words.find(ret)==stop_words.end())
				m[ret]++;
			// cout<<word<<endl;
		}
		
	}

	while(getline(f3,s)) {
		stringstream ss(s);

		string word;

		while(ss>>word) {
			if(check(word) && stop_words.find(ret)==stop_words.end())
				m[ret]++;
			// cout<<word<<endl;
		}
		
	}

	// cout<<m.size()<<endl;
	vector<string> rem;

	for(map<string,int>::iterator it=m.begin();it!=m.end();it++) {
		// if(stem(it->first)) {
		// 	rem.push_back(it->first);
		// 	m[st]++;
		// 	continue;
		// }
		if(it->second==1)
			rem.push_back(it->first);

	}

	for(int i=0;i<rem.size();i++) {
		m.erase(rem[i]);
	}


	// rem.clear();

	int cnt=0;
	for(map<string,int>::iterator it=m.begin();it!=m.end();it++) {
		// cout<<it->first<<" "<<cnt<<endl;
		it->second=cnt;
		cnt++;
	}

	f1.close();
	f1.open("twitter_positive");
	
	vector<int> v(443);

	int idx=0;
	while(idx<140 && getline(f1,s) ) {
		// cout<<s<<endl;

		stringstream ss(s);

		string word;
		for(int i=0;i<443;i++)
			v[i]=0;

		while(ss>>word) {
			if(check(word)) {
				if(m.find(ret)!=m.end()) {
					v[m[ret]]=1;
				}
			}
		}

		for(int i=0;i<443;i++)
			cout<<v[i]<<" ";
		// cout<<" : ";
		cout<<"1 0 0\n";
		idx++;
	}

	f2.close();
	f2.open("twitter_negative");
	idx=0;
	while(idx<140 && getline(f2,s) ) {
		// cout<<s<<endl;

		stringstream ss(s);

		string word;
		for(int i=0;i<443;i++)
			v[i]=0;

		while(ss>>word) {
			if(check(word)) {
				if(m.find(ret)!=m.end()) {
					v[m[ret]]=1;
				}
			}
		}

		for(int i=0;i<443;i++)
			cout<<v[i]<<" ";
		// cout<<" : ";
		cout<<"0 0 1\n";
		idx++;
	}

	f3.close();
	f3.open("twitter_objective");
	
	idx=0;
	while(idx<110 && getline(f3,s)) {
		// cout<<s<<endl;

		stringstream ss(s);

		string word;
		for(int i=0;i<443;i++)
			v[i]=0;

		while(ss>>word) {
			if(check(word)) {
				if(m.find(ret)!=m.end()) {
					v[m[ret]]=1;
				}
			}
		}

		for(int i=0;i<443;i++)
			cout<<v[i]<<" ";
		// cout<<" : ";
		cout<<"0 1 0\n";
		idx++;
	}


	idx=140;
	while(idx<182 && getline(f1,s) ) {
		// cout<<s<<endl;

		// cout<<s;
		stringstream ss(s);
		string word;
		for(int i=0;i<443;i++)
			v[i]=0;

		while(ss>>word) {
			if(check(word)) {
				if(m.find(ret)!=m.end()) {
					v[m[ret]]=1;
				}
			}
		}

		for(int i=0;i<443;i++)
			cout<<v[i]<<" ";
		// cout<<" : ";
		cout<<"1 0 0\n";
		idx++;
	}

	// f2.close();
	// f2.open("twitter_negative");
	idx=140;
	while(idx<177 && getline(f2,s) ) {
		// cout<<s<<endl;

		// cout<<s;
		stringstream ss(s);
		string word;
		for(int i=0;i<443;i++)
			v[i]=0;

		while(ss>>word) {
			if(check(word)) {
				if(m.find(ret)!=m.end()) {
					v[m[ret]]=1;
				}
			}
		}

		for(int i=0;i<443;i++)
			cout<<v[i]<<" ";
		// cout<<" : ";
		cout<<"0 0 1\n";
		idx++;
	}

	// f3.close();
	// f3.open("twitter_objective");
	
	idx=110;
	while(idx<139 && getline(f3,s) ) {
		// cout<<s<<endl;

		// cout<<s;
		stringstream ss(s);
		string word;
		for(int i=0;i<443;i++)
			v[i]=0;

		while(ss>>word) {
			if(check(word)) {
				if(m.find(ret)!=m.end()) {
					v[m[ret]]=1;
				}
			}
		}

		for(int i=0;i<443;i++)
			cout<<v[i]<<" ";
		// cout<<" : ";
		cout<<"0 1 0\n";
		idx++;
	}

	// cout<<cnt<<endl;
	return 0;
}