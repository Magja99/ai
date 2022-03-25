#include <iostream>
#include <cstdio>
#include <fstream>
#include <queue>
#include <map>
#include <ctime>
#include <sys/time.h>

using namespace std;

#define fi first
#define si second

struct state
{
	vector<pair<int, int>> K;
	string moves;
};

int n, m;
bool win = 0;

vector <char> Board[100];
vector <pair<int, int>> St;
vector <pair <int, int>> Goal;

queue <state> Q;

ofstream ans;
ifstream file;

map <vector<pair<int, int>>, int> Vis;
map <pair<int, int>, char> M;

int R(long long a, long long b)
{
        return a + rand() % (b - a + 1);
}

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

string shuffle(vector <pair<int, int>> &S)
{
	pair<int, int> t[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	vector<pair<int, int>> V;
	int last_it = -1;
	int it;
	string k = "";
	for(int i = 0; i < 10; i++)
	{
		while(true)
		{
			it = R(0, 3);
			if(it == 0 && last_it == 1)
				continue;
			if(it == 1 && last_it == 0)
				continue;
			if(it == 2 && last_it == 3)
				continue;
			if(it == 3 && last_it == 2)
				continue;
			break;
		}
		for(int j = 0; j < max(n, m); j++)
		{
			int m = move(t[it].fi, t[it].si, S);
			if(m != 0)
				k += M[t[it]];
			else
				break;
		}
		last_it = it;
	}
	S.resize(unique(S.begin(), S.end()) - S.begin());
	return k;
}
void BFS(vector <pair<int, int>> S, string first_moves)
{
	//print_state(S);
	//cout << endl;
	sort(S.begin(), S.end());
	S.resize(unique(S.begin(), S.end()) - S.begin());
	state w = {S, first_moves};
	Vis[S] = 1;
	Q.push(w);
	while(!Q.empty())
	{
		state v = Q.front();
		Q.pop();
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
					if(Vis[new_v] == 1)
						continue;
					win_check(new_v);
					if(win)
					{
						//cout << "Udało się!!" << endl;
						//cout << "Jesteśmy w: " << endl;
						//print_state(new_v);
						cout << v.moves +  M[{i, j}] << " " << v.moves.size() << endl;
						ans << v.moves + M[{i, j}] << endl;
						file.close();
						ans.close();
						exit(0);
					}
					state new_state = {new_v, v.moves + M[{i, j}]};
					Q.push(new_state);
					Vis[new_v] = 1;
				}
			}
	}
}

int main()
{
	timeval czas;
     gettimeofday(&czas, 0);
     srand(czas.tv_sec * 1000 + czas.tv_usec % 1000);

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
	while (getline(file,line))
    	{
		m = line.size();
		for(int j = 0; j < line.size(); j++)
		{
			if(line[j] != '\n')
				Board[i].push_back(line[j]);
			if(line[j] == 'S' || line[j] == 'B')
			{
				//cout << line << endl;
				com.push_back(make_pair(i, j));
				oryginal.push_back(make_pair(i, j));
			}

			if(line[j] == 'G' || line[j] == 'B')
				Goal.push_back(make_pair(i, j));
		}
		i++;
    	}
	n = i;
	string first_moves = shuffle(com);
	//cout << com.size() << endl;
	vector<pair<int, int>> best = com;
	com = oryginal;

	for(int i = 0; i < 1000; i++)
	{
		string fm = shuffle(com);
		if(com.size() < best.size() || (com.size() == best.size() && first_moves.size() > fm.size()))
		{
			best = com;
			first_moves = fm;
		}
		//cout << best.size() << endl;
		com = oryginal;
	}

	BFS(best, first_moves);
}
