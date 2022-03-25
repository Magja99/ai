#include <iostream>
#include <cstdio>
#include <fstream>
#include <queue>
#include <map>

using namespace std;

#define fi first
#define si second

struct state
{
	vector<pair<int, int>> K;
	string moves;
	int dist;
	int heur;
};

struct CustomCompare
{
	bool operator()(const state &a, const state &b){
		return (a.dist + a.heur) > (b.dist + b.heur);
	}
};

int n, m;
int h[200][200];
bool win = 0;

vector <char> Board[100];
vector <pair<int, int>> St;
vector <pair <int, int>> Goal;

priority_queue <state,vector<state>, CustomCompare> QP;
queue <pair<int, int>> Q;

ofstream ans;
ifstream file;

map <vector<pair<int, int>>, int> Vis;
map <pair<int, int>, char> M;

void print_state(vector<pair<int, int>>& V)
{
	int j = 0;
	for(int i = 0; i < V.size(); i++)
	{
		cout << "Komandos "  << i <<": "<< V[i].fi << " " << V[i].si << endl;
	}
}

int move(int i, int j, vector<pair <int, int>>& V)
{
	int moved = 0;
	for(int it = 0; it < V.size(); it++)
	{
		if(Board[V[it].fi + i][V[it].si + j] ==  '#')
			continue;
		else
		{
			V[it].fi += i;
			V[it].si += j;
			moved++;
		}
	}
	return moved;
}

void win_check(vector<pair <int, int>>& V)
{
	int proper = 0;
	for(int i = 0; i < V.size(); i++)
	{
		char pos = Board[V[i].fi][V[i].si];
		if(pos == 'G' ||  pos == 'B')
			proper++;
	}
	if(proper == V.size())
		win = true;
}

int give_h(vector<pair<int, int>>& V)
{
	int ans = 1;
	for(auto i: V)
		ans = max(ans, h[i.fi][i.si]);

	return ans;
}

void BFS(vector <pair<int, int>> &S)
{
	for(auto w: S)
	{
		Q.push(w);
		h[w.fi][w.si] = 0;
		while(!Q.empty())
		{
			pair<int, int> v = Q.front();
			Q.pop();
			for(int i = -1; i <= 1; i++)
				for(int j = -1; j <= 1; j++)
				{
					if(abs(i - j) == 1)
					{
						if(h[v.fi + i][v.si + j] > h[v.fi][v.si] + 1)
						{
							h[v.fi + i][v.si + j] = h[v.fi][v.si] + 1;
							Q.push(make_pair(v.fi + i, v.si + j));
						}
					}
				}
		}
	}
}

void A_star(vector <pair<int, int>> &S)
{
	//print_state(S);
	//cout << endl;
	sort(S.begin(), S.end());
	S.resize(unique(S.begin(), S.end()) - S.begin());
	state w = {S, "", 0, give_h(S)};
	Vis[S] = 0;
	QP.push(w);
	while(!QP.empty())
	{
		state v = QP.top();
		QP.pop();
		//print_state(v.K);
		//cout << endl;
		for(int i = -1; i <= 1; i++)
			for(int j = -1; j <= 1; j++)
			{
				if(abs(i - j) == 1)
				{
					vector<pair<int, int>> new_v = v.K;
					int k = move(i, j, new_v);
					sort(new_v.begin(), new_v.end());
					new_v.resize(unique(new_v.begin(), new_v.end()) - new_v.begin());
					win_check(new_v);
					if(win)
					{
						//cout << "Udało się!!" << endl;
						//cout << "Jesteśmy w: " << endl;
						//print_state(new_v);
						cout << v.moves +  M[{i, j}] << " " << v.moves.size() + 1 << endl;
						ans << v.moves + M[{i, j}] << endl;
						file.close();
						ans.close();
						exit(0);
					}
					if(Vis.find(new_v) == Vis.end() || Vis[new_v] > v.dist + 1)
					{
						state new_state = {new_v, v.moves + M[{i, j}], v.dist + 1, give_h(new_v)};
						QP.push(new_state);
						Vis[new_v] = v.dist + 1;
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

	vector<pair<int, int> > com;
	vector<pair<int, int>> oryginal;

	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 100; j++)
			h[i][j] = 1000000;

	while (getline(file,line))
    {
		m = line.size();
		for(int j = 0; j < line.size(); j++)
		{
			if(line[j] != '\n')
				Board[i].push_back(line[j]);
			if(line[j] == 'S' || line[j] == 'B')
			{
				com.push_back(make_pair(i, j));
				oryginal.push_back(make_pair(i, j));
			}

			if(line[j] == 'G' || line[j] == 'B')
				Goal.push_back(make_pair(i, j));
			if(line[j] == '#')
				h[i][j] = -1;
		}
		i++;
    }
	n = i;
	BFS(Goal);
	A_star(com);
}
