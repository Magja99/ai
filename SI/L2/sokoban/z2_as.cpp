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
	int source_dist;
	int heur;
	string moves;
};

struct CustomCompare
{
    bool operator()(const state& a, const state& b)
    {
        return (a.source_dist + a.heur) > (b.source_dist + b.heur);
    }
};

int n, m;
int h[100][100];
bool win = 0;

vector <char> Board[100];
vector <pair<int, int>> St;
vector <pair <int, int>> Goal;

queue <pair<int, int>> Q;

ofstream ans;
ifstream file;

priority_queue <state, vector<state>, CustomCompare> QP;

map <pair<pair<int, int>, vector<pair<int, int>>>, int> Dist;
map <pair<int, int>, char> M;

void print_state(pair<int, int> k, vector<pair<int, int>>& V)
{
	cout << "komandos: " << k.fi << " " << k.si << endl;
	for(int i = 0; i < V.size(); i++)
	{
		cout << "Pudła: " << V[i].fi << " " << V[i].si << endl;
	}
}

void print_h()
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			if(h[i][j] == -1)
				cout << '-' << " ";
			else
			if(h[i][j] == 1000000)
				cout << "x ";
			else
				cout << h[i][j] << " ";
		}
		cout << endl;
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
	if(Board[s.fi + i][s.si + j] == 'W') // sprawdzamy czy ściana
		return -1;
	if(s.fi + i < 0 || s.si + j < 0 || s.fi + i >= n || s.si + j >= m)
		return -1;

	for(int it = 0; it < V.size(); it++) // sprawdzamy czy skrzynka
	{
		if(V[it].fi == s.fi + i && V[it].si == s.si  + j) // jeśli tak sprawdzamy czy możemy przesunąć
		{
			for(int id = 0; id < V.size(); id++)
			{
				if(id != it && V[id].fi == V[it].fi + i && V[id].si == V[it].si + j) // natrafiamy na inną skrzynkę
					return -1;
			}
			if(Board[V[it].fi + i][V[it].si + j] ==  'W') // natrafiamy na ścianę
				return -1;
			if(V[it].fi + i < 0 || V[it].si + j < 0)
				return -1;
			if(V[it].fi + i >= n|| V[it].si + j >= m)
				return -1;
			return it; // możemy przesunąć it
		}
	}
	return 1000; // puste pole
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

void A_star(pair<int, int> w, vector <pair<int, int>> S)
{
	sort(S.begin(), S.end());
	state v = {w, S, 0, give_h(S), ""};
	QP.push(v);
	Dist[make_pair(w, S)] = 0;
	while(!QP.empty())
	{
		state v = QP.top();
		//cout << "Jesteśmy w: " << endl;
		//print_state(v.K, v.B);
		//cout << v.source_dist;
		//cout << endl << endl;
		int c = v.source_dist;
		QP.pop();
		for(int i = -1; i <= 1; i++)
			for(int j = -1; j <= 1; j++)
			{
				if(abs(i - j) == 1)
				{
					int it = check(i, j, v.K, v.B);
					if(it != -1) // dozwolone
					{
						vector<pair<int, int> > new_b = v.B;
						pair<int, int> new_k = {v.K.fi + i, v.K.si + j};
						if(it != 1000)
							new_b[it] = {new_b[it].fi + i, new_b[it].si + j};
						sort(new_b.begin(), new_b.end());
						win_check(new_b);
						if(win)
						{
							// cout << "Udało się!!" << endl;
							// cout << "Jesteśmy w: " << endl;
							// print_state(new_k, new_b);
							cout << v.moves +  M[{i, j}] << endl;
							ans << v.moves + M[{i, j}] << endl;
							file.close();
							ans.close();
							exit(0);
						}
						if(Dist.find({new_k, new_b}) == Dist.end() || Dist[{new_k, new_b}] > c + 1)
						{
							Dist[{new_k, new_b}] = c + 1;
							state new_state = {new_k, new_b, c + 1, give_h(new_b), v.moves + M[{i, j}]};
							QP.push(new_state);
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
