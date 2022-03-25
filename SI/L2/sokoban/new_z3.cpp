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
	pair<int, int> K;
	vector<pair<int, int>> B;
	int heur;
	string moves;
};

struct CustomCompare
{
    bool operator()(const state& a, const state& b)
    {
        return  a.heur >  b.heur;
    }
};

struct Komandos
{
	pair<int, int> act;
	string m;
};

int n, m;
int h[100][100];
bool win = 0;

vector <char> Board[100];
vector <pair<int, int>> St;
vector <pair <int, int>> Goal;

queue <pair<int, int>> Q;
queue <Komandos> P;

ofstream ans;
ifstream file;

priority_queue <state, vector<state>, CustomCompare> QP;

map <pair<pair<int, int>, vector<pair<int, int>>>, int> Vis;
map <pair<int, int>, char> M;

void print_state(pair<int, int> k, vector<pair<int, int>>& V)
{
	cout << "komandos: " << k.fi << " " << k.si << endl;
	for(int i = 0; i < V.size(); i++)
	{
		cout << "Pudła: " << V[i].fi << " " << V[i].si << endl;
	}
}

void BFS(pair<int, int> w)
{
	if(h[w.fi][w.si] == 0)
		return;
	h[w.fi][w.si] = 0;
	Q.push(w);
	while(!Q.empty())
	{
		pair<int, int> v = Q.front();
		Q.pop();
		for(int i = -1; i <= 1; i++)
		{
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

int give_h(vector<pair<int, int>>& V)
{
	int ans = 1000000;
	for(auto i: V)
		ans += h[i.fi][i.si];

	return ans;
}

int check(int i, int j, pair<int, int>& s, vector<pair <int, int>>& V)
{
	if(Board[s.fi + i][s.si + j] == 'W') // sprawdzamy czy ściana w miejsce gdzie przesuwamy
		return -1;
	if(Board[s.fi - i][s.si - j] == 'W') // sprawdzamy czy ściana gdzie pchamy
		return -1;

	if(s.fi + i < 0 || s.si + j < 0 || s.fi + i >= n || s.si + j >= m)
		return -1;

	for(int it = 0; it < V.size(); it++) // sprawdzamy czy skrzynka
	{
		if(V[it].fi == s.fi + i && V[it].si == s.si + j) // jeśli tak sprawdzamy czy możemy przesunąć
			return -1;
		if(V[it].fi == s.fi - i && V[it].si == s.si - j) // jeśli tak sprawdzamy czy możemy przesunąć
			return -1;
	}
	return 0; // puste pole
}

void win_check(vector<pair <int, int>>& V)
{
	int proper = 0;
	for(int i = 0; i < V.size(); i++)
	{
		char pos = Board[V[i].fi][V[i].si];
		if(pos == 'G' ||  pos == '*')
			proper++;
	}
	if(proper == V.size())
		win = true;
}

string give_path(pair<int, int> start, pair<int, int> end, vector <pair<int, int>>& V)
{
	P.push({start, ""});
	char path[100][100];
	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 100; j++)
			path[i][j] = '0';
	path[start.fi][start.si] = 'S';
//	cout << "New Path\n";
	while(!P.empty())
	{
		Komandos v = P.front();
		P.pop();
		if(v.act == end)
		{
			while(!P.empty()) P.pop();
			return v.m;
		}
		for(int i = -1; i <= 1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				bool nope = 0;
				if(abs(i - j) == 1)
				{
					if(Board[v.act.fi + i][v.act.si + j] != 'W' && path[v.act.fi + i][v.act.si + j] == '0')
					{
						// for(int i = 0; i < n; i++){
						// 	for(int j = 0; j < m; j++)
						// 	{
						// 		cout << path[i][j];
						// 	}
						// 	cout << endl;
						// }
						// cout << endl;
						for(auto k: V)
							if(k.fi == v.act.fi + i && k.si == v.act.si + j)
								nope = 1;
						if(nope)
							continue;
						path[v.act.fi + i][v.act.si + j] = M[{i, j}];
						P.push({make_pair(v.act.fi + i, v.act.si + j), v.m + M[{i, j}]});
					}
				}

			}
		}
	}
	return "dead_end";
}

void A_star(pair<int, int> w, vector <pair<int, int>> S)
{
	sort(S.begin(), S.end());
	state v = {w, S, give_h(S), ""};
	QP.push(v);
	Vis[make_pair(w, S)] = 1;
	while(!QP.empty())
	{
		state v = QP.top();
		QP.pop();
		// cout << "Jesteśmy w: " << endl;
		// print_state(v.K, v.B);
		// cout << v.moves;
		// cout << endl << endl;
		for(int box = 0; box < v.B.size(); box++)
			for(int i = -1; i <= 1; i++)
				for(int j = -1; j <= 1; j++)
				{
					if(abs(i - j) == 1)
					{
						int it = check(i, j, v.B[box], v.B);
						if(it != -1) // dozwolone
						{
							vector<pair<int, int> > new_b = v.B;
							pair<int, int> new_k = v.B[box];
							new_b[box].fi += i;
							new_b[box].si += j;
							sort(new_b.begin(), new_b.end());
							string com_m = give_path(v.K, make_pair(new_k.fi - i, new_k.si - j), v.B);
							if(com_m == "dead_end")
								continue;
							com_m += M[{i, j}];
							win_check(new_b);
							if(win)
							{
							//	cout << "Udało się!!" << endl;
								ans << v.moves + com_m << endl;
								file.close();
								ans.close();
								exit(0);
							}
							if(Vis[{new_k, new_b}] == 1)
								continue;
							QP.push({new_k, new_b, give_h(new_b), v.moves+ com_m});
							Vis[{new_k, new_b}] = 1;
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
	pair<int, int> com;

	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 100; j++)
			h[i][j] = 1000000;
	while (getline(file,line))
    	{
		for(int j = 0; j < line.size(); j++)
		{
			if(line[j] != '\n')
				Board[i].push_back(line[j]);
			if(line[j] == 'K' || line[j] == '+')
				com = make_pair(i, j);
			if(line[j] == 'B' || line[j] == '*')
				St.push_back(make_pair(i, j));
			if(line[j] == 'G' || line[j] == '*' || line[j] == '+')
				Goal.push_back(make_pair(i, j));
			if(line[j] == 'W')
				h[i][j] = -1;
			m = line.size();
		}
		i++;
    	}
	n = i;
	for(auto i: Goal)
		BFS(i);
	A_star(com, St);
}
