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
map <pair<int, int>, int> Vis_K;
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

int check(int i, int j, pair<int, int>& s, vector<pair<int, int>>& V)
{
	if(Board[s.fi + i][s.si + j] == 'W') // sprawdzamy czy ściana tam gdzie chcemy przesunąć
		return 0;
	if(Board[s.fi - i][s.si - j] == 'W') // sprawdzamy czy ściana w miejscu gdzie pchamy
		return 0;
	if(s.fi + i < 0 || s.si + j < 0 || s.fi + i >= n || s.si + j >= m)
		return 0;

	for(int it = 0; it < V.size(); it++) // sprawdzamy czy skrzynka
	{
		if(V[it].fi == s.fi + i && V[it].si == s.si  + j && V[it] != s) // gdzie chcemy przesunąć
			return 0;
		if(V[it].fi == s.fi - i && V[it].si == s.si - j && V[it] != s) // gdzie pchamy
			return 0;
	}
	return 1; // puste pole
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

string give_path(pair<int, int> start, pair<int, int> end, vector<pair<int, int>>& V)
{
	P.push({start, ""});
	Vis_K[start] = 1;
	if (V[0] == {3, 4}) {
		/* code */
	}
	while(!P.empty())
	{
		Komandos v = P.front();
		P.pop();
		if(v.act == end)
			return v.m;
		//if(end.fi == 1 && end.si == 4)
		//cout << "jesteśmy w: " << v.act.fi << " " <<  v.act.si << endl;
		for(int i = -1; i <= 1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				if(abs(i - j) == 1)
				{
					bool nope = 0;
					pair<int, int> new_v = {v.act.fi + i, v.act.si + j};
				//	if(end.fi == 1 && end.si == 4)
					//cout << "nowe_v: " << new_v.fi << " " <<  new_v.si << endl;
					if(Vis_K[new_v] == 1)
					{
						nope = true;
						//cout << "Vis!" << endl;
					}
					if(Board[new_v.fi][new_v.si] == 'W') //czy idzie na ścianę lub na pudło
					{
						nope = true;
					//	cout << "ściana!" << endl;
					}
					for(auto k: V)
					{
						if(k == new_v)
							nope = true;
					}
					if(!nope)
					{
						if(new_v == end)
						{
						//	cout << "return: " << v.m + M[{i, j}]<< endl;
							return v.m + M[{i, j}];
						}

						//cout << "pusz: " << new_v.fi << " " << new_v.si << endl;
						P.push({new_v, v.m + M[{i, j}]});
					}
					Vis_K[new_v] = 1;
				}
			}
		}
	//	cout << "size: " << P.size() << endl;
		//if(!P.empty()) cout << "jeszcze nie" <<endl;
	}
	Vis_K.clear();
	return "";
}
void Best_first_search(pair<int, int> w, vector <pair<int, int>> S)
{
	sort(S.begin(), S.end());
	state v = {w, S, give_h(S), ""};
	QP.push(v);
	Vis[make_pair(w, S)] = 1;
	while(!QP.empty())
	{
		state v = QP.top();
		QP.pop();
		cout << "Jesteśmy w: " << endl;
		print_state(v.K, v.B);
	//	cout << v.moves;
		cout << endl << endl;
		for(int box = 0; box < v.B.size(); box++) // dla każdej skrzynki sprawdzamy 4 strony gdzie możemy ją ruszyć
		{
			for(int i = -1; i <= 1; i++)
			{
				for(int j = -1; j <= 1; j++)
				{
					if(abs(i - j) == 1)
					{
						if(check(i, j, v.B[box], v.B)) //czy możemy ruszyć
						{
							vector<pair<int, int> > new_b = v.B;
							pair<int, int> new_k = new_b[box];
							new_b[box].fi += i;
							new_b[box].si += j;
							sort(new_b.begin(), new_b.end());
							if(Vis[{new_k, new_b}] == 1)
								continue;
							string com_m = give_path(v.K, make_pair(new_k.fi - i, new_k.si - j), v.B);
							if(com_m == "")
								continue;
							com_m += M[{i, j}];
							win_check(new_b);
							if(win)
							{
								cout << v.moves + com_m << endl;
								ans << v.moves + com_m << endl;
								file.close();
								ans.close();
								exit(0);
							}
							QP.push({new_k, new_b, give_h(new_b), v.moves+com_m});
							Vis[{new_k, new_b}] = 1;
						}
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
	for(auto i: Goal)
		BFS(i);
	n = i;
	Best_first_search(com, St);
}
