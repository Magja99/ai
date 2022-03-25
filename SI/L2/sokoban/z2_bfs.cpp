#include <iostream>
#include <cstdio>
#include <fstream>
#include <queue>
#include <map>

using namespace std;

vector <char> Board[100];
vector <pair<int, int>> St;
vector <pair <int, int>> Goal;
map <vector<pair<int, int>>, bool> Vis;
map <pair<int, int>, char> M;
map <int, char> Anc;
queue <pair<vector<pair<int, int>>, string>> Q;
bool win = 0;
int vert = 1;
ofstream ans;
ifstream file;
int n, m;
void win_check(vector<pair <int, int>>& V)
{
	int proper = 0;
	for(int i = 1; i < V.size(); i++)
	{
		char pos = Board[V[i].first][V[i].second];
		if(pos == 'G' ||  pos == '*')
			proper++;
	}
	if(proper == V.size() - 1)
		win = true;
}

int check(int i, int j, vector<pair <int, int>>& V)
{
	if(Board[V[0].first + i][V[0].second + j] == 'W') // sprawdzamy czy ściana
		return -1;
	if(V[0].first + i < 0 || V[0].second + j < 0)
		return -1;
	if(V[0].first + i >= n || V[0].second + j >= m)
		return -1;
	for(int it = 1; it < V.size(); it++) // sprawdzamy czy skrzynka
	{
		if(V[it].first == V[0].first + i && V[it].second == V[0].second + j) // jeśli tak sprawdzamy czy możemy przesunąć
		{
			for(int id = 1; id < V.size(); id++)
			{
				if(id != it && V[id].first == V[it].first + i && V[id].second == V[it].second + j) // natrafiamy na inną skrzynkę
					return -1;
			}
			if(Board[V[it].first + i][V[it].second + j] ==  'W') // natrafiamy na ścianę
				return -1;
			if(V[it].first + i < 0 || V[it].second + j < 0)
				return -1;
			if(V[it].first + i >= n|| V[it].second + j >= m)
				return -1;
		//	cout << "przesuwamy na: " << V[it].first + i << " " <<  V[it].second + j << endl;
		//	cout << "Board[i][j]: " << Board[V[it].first + i][V[it].second + j] << endl;
			return it; // możemy przesunąć
		}
	}
	return 0; // puste pole
}

void print_state(vector<pair<int, int>>& V)
{
	for(int i = 0; i < V.size(); i++)
	{
		if(i == 0)
			cout << "Komandos: " << V[i].first << " " << V[i].second << endl;
		else
			cout << "Pudła: " << V[i].first << " " << V[i].second << endl;
	}
	cout << endl;
}
void BFS(pair<vector<pair<int, int>>, string> S)
{
	Vis[S.first] = 1;
	Q.push(S);
	//cerr << "Time for BFS!" << endl;
	while(!Q.empty())
	{
		vector<pair<int, int>> State_1 = Q.front().first;
		string moves_1 = Q.front().second;
		Q.pop();
		vector<pair<int, int>> State_2 = State_1;
		string moves_2 = moves_1;
		//print_state(State_1);
		for(int i = -1; i <= 1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				if(abs(i - j) == 1)
				{
					int it = check(i, j, State_1);
					if(it != -1)
					{
						State_2[0] = State_1[it];
						State_2[it].first += i;
						State_2[it].second += j;
						moves_2 += M[{i, j}];
						win_check(State_2);
						if(win)
						{
							cout << moves_2 << endl;
							ans << moves_2 << endl;
							file.close();
							ans.close();
							exit(0);
						}
						if(Vis[State_2] != 1)
							Q.push(make_pair(State_2, moves_2));
						Vis[State_2] = true;
						State_2[0] = State_1[0];
						State_2[it] = State_1[it];
						moves_2 = moves_1;
					}
				}
			}
		}
	}
}


int main()
{
	ans.open("zad_output.txt");
	file.open("zad_input.txt");
	int i = 0;
	M[{1, 0}] = 'D';
	M[{-1, 0}] = 'U';
	M[{0, 1}] = 'R';
	M[{0, -1}] = 'L';
	string line;
	St.push_back({0,0});
	while (getline(file,line))
    	{
		for(int j = 0; j < line.size(); j++)
		{
			if(line[j] != '\n')
				Board[i].push_back(line[j]);
			if(line[j] == 'K' || line[j] == '+')
				St[0] = make_pair(i, j);
			if(line[j] == 'B' || line[j] == '*')
				St.push_back(make_pair(i, j));
			if(line[j] == 'G' || line[j] == '*' || line[j] == '+')
				Goal.push_back(make_pair(i, j));
			m = line.size();
		}
		i++;
    	}
	n = i;
	BFS(make_pair(St, ""));

}
