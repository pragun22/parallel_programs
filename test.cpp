#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;
const ll mod = 1e9+7;
int main()
{
	srand(time(NULL));
	for (int i = 0; i < 1000000; ++i)
	{
		ll a = rand()%11;
		ll num = rand()% mod;
		ll num2 = rand()%mod;
		if(a < 6) cout<<(-1*num*num2)<<" ";
		else cout<<num<<" ";
	}
	cout<<endl;
	return 0;
}