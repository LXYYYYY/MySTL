#include<iostream>
#include<unordered_map>
using namespace std;
string isIsomorphic(string s, string t) {
	unordered_map<char, char> smap, tmap;
	string sres = "";
	string tres = "";
	char cate = 'a';
	for (int i = 0; i < s.length(); i++)
	{
		if (smap.count(s[i]) == 0)smap[s[i]] = cate++;
		sres += smap[s[i]];
	}
	cate = 'a';
	for (int i = 0; i < s.length(); i++)
	{
		if (smap.count(t[i]) == 0)smap[t[i]] = cate++;
		tres += smap[t[i]];
	}
	return tres;
}
int main()
{
	string a = isIsomorphic("ab", "ac");
	cout << a;
	return 0;
}