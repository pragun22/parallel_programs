#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;
int main(int argc, char **argv)
{
	string line;
    ifstream f(argv[1]);
    getline(f, line);
    string temp = "";
    ll min = -1*LLONG_MAX;
    for (int i = 0; i < line.size(); ++i)
    {
        if(i==line.size()-1)
        {
            temp+=line[i];
            ll t =  stoll(temp);
            if(t < min)
            {
            	cout<<"Wrong"<<endl;
            	return 0;
            }
            min = t;
        }
        else if(line[i]==' ' || line[i]=='\n')
        {
			ll t =stoll(temp);
			if(t < min)
            {
            	cout<<"Wrong"<<endl;
            	return 0;
            }
            min = t;
            temp = "";
        }
        else temp+=line[i];
    }
    cout<<"CORRECT"<<endl;
	return 0;
}