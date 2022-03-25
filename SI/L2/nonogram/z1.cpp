#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

#define pb push_back

int n, m, x;
int arr[100][100];

// legenda:
// -1 - puste
// 1 - zamalowane
// 0 - niewiadomo
// 2 - może czarne
void wypisz()
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			if(arr[i][j] == 1)
				cout << '#';
			if(arr[i][j] == -1)
				cout << 'O';
			if(arr[i][j] == 0)
				cout << '.';
		}
		cout << endl;
	}
	cout << endl;
}

void check(vector<int> &V, vector<int> &N, int s)
{
	int poc = 0;
	int num = N[poc];
	int licz = 0;
	for(int i = 0; i < s; i++)
	{
		if(V[i] == 1 || V[i] == 4)
			licz++;
		if(V[i] != 1 && V[i] != 4 && licz != 0)
			return;
		if(licz == num)
		{
			licz = 0;
			poc++;
			if(poc < N.size())
				num = N[poc];
		}
	}
	//cout << poc << " " << N.size() << endl;
	if(poc != N.size())
		return;
	for(int i = 0; i < V.size(); i++)
	{
		if(V[i] == 1 || V[i] == 4)
			V[i] = 1;
		else
			V[i] = -1;
	}
}

void paint(vector<int> &V, pair<int, int> p, int v)
{
	if(p.first > p.second)
		return;
	for(int i = p.first; i <= p.second; i++)
		V[i] = 1;
}

void inference(vector<int> &V, vector<int> &N, int s)
{
	int poc = 0;
	vector<pair<int, int>> p1;
	vector<pair<int, int>> p2;
	for(int k = 0; k < N.size(); k++)
	{
		int num = N[k];
		int j = poc;
		for(int i = poc; i < s; i++)
		{
			while(j - i + 1 < num && V[j] != -1)
				j++;
			if(j == s - 1 || V[j + 1] != 1)
			{
				p1.pb({i, j});
				break;
			}
		}
		poc = j + 2;
	}
	poc = s - 1;
	for(int k = N.size() - 1; k >= 0; k--)
	{
		int num = N[k];
		int j = poc;
		for(int i = poc; i >= 0; i--)
		{
			while(i - j + 1 < num && V[j] != -1)
				j--;
			if(j == 0 || V[j - 1] != 1)
			{
				p2.pb({j, i});
				break;
			}
		}
		poc = j - 2;
	}
	reverse(p2.begin(), p2.end());
	for(int i = 0; i < N.size(); i++)
	{
		pair<int, int> seg1 = p1[i];
		pair<int, int> seg2 = p2[i];
		paint(V, {max(seg1.first, seg2.first), min(seg1.second, seg2.second)}, 1);
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin >> n >> m;

    vector<vector<int>> Row_Var(n);
    vector<vector<int>> Col_Var(m);

    for(int i = 0; i < n; i++)
	while(true)
	{
		cin >> x;
		Row_Var[i].pb(x);
		if(cin.get() == '\n')
			break;
	}

	for(int i = 0; i < m; i++)
	while(true)
	{
		cin >> x;
		Col_Var[i].pb(x);
		if(cin.get() == '\n')
			break;
	}

	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 100; j++)
			arr[i][j] = 0;

	int q = 2;
	while(q--)
	{
		for(int i = 0; i < n; i++) // wiersze
		{
			vector<int> V;
			for(int j = 0; j < m; j++)
				V.push_back(arr[i][j]);
			inference(V, Row_Var[i], m);
			check(V, Row_Var[i], m);
			for(int j = 0; j < m; j++)
				arr[i][j] = V[j];
		}
		wypisz();
		//exit(0);
		for(int i = 0; i < m; i++) // kolumny
		{
			vector<int> V;
			for(int j = 0; j < n; j++)
				V.push_back(arr[j][i]);
			inference(V, Col_Var[i], n);
			check(V, Col_Var[i], n);
			for(int j = 0; j < n; j++)
				arr[j][i] = V[j];
		}
		wypisz();
	}
}
