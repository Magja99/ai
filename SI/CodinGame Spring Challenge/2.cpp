#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <set>

using namespace std;

#define FOURSIDES(i, j) for(int i = -1; i <= 1; i++) for(int j = -1; j <= 1; j++) if(abs(i-j)==1)
#define sim template<class n
sim, class s> ostream & operator << (ostream &p, pair<n, s> x)
{return p << "<" << x.first << ", " << x.second << ">";}
sim> auto operator << (ostream &p, n y) ->
typename enable_if<!is_same<n, string>::value, decltype(y.begin(), p)>::type
{int o = 0; p << "{"; for(auto c: y) {if(o++) p << ", "; p << c;} return p << "}";}
void dor() {cerr << endl;}
sim, class...s> void dor(n p, s...y) {cerr << p << " "; dor(y...);}
sim, class s> void mini(n &p, s y) {if(p>y) p = y;}
sim, class s> void maxi(n &p, s y) {if(p<y) p = y;}

#define debug(...) dor(__FUNCTION__, ":", __LINE__, ": ", __VA_ARGS__)



char grid[1000][1000];
char org[1000][1000];

pair<int, int> start;

map<pair<int, int>, pair<int, int>> anc;
map <int, pair<int, int>> last_start;
map <int, pair<int, int>> last_random;
map <int, pair<int, int>> last_stack;
map <pair<int, int>, double> weight;
map<int, int> last_cool;

int width;
int height;
int manhattan[1000][1000];

vector<pair<int, int>> S;
void erase_from_S(pair<int, int> x)
{
	int to_erase = -1;
	for(int i = 0; i < S.size(); i++)
	{
		if(S[i] == x)
		{
			to_erase = i;
			break;
		}
	}
	if(to_erase != -1)
		S.erase(S.begin() + to_erase);
}

void klar_man()
{
	for(int i = 0; i <= height; i++)
		for(int j = 0; j <= width; j++)
			if(org[i][j] != '#')
				manhattan[i][j] = 10000;
}

void klar_grid()
{
	for(int i = 0; i <= height; i++)
		for(int j = 0; j <= width; j++)
			if(grid[i][j] != '.')
				grid[i][j] = org[i][j];
}

void dist(pair<int, int> start)
{
	queue<pair<int, int>> Q;
	Q.push(start);
	manhattan[start.second][start.first] = 0;
	while(!Q.empty())
	{
		pair<int, int> v = Q.front();
		Q.pop();
		FOURSIDES(i, j)
		{
			int x = (v.first + i + width)%width;
			int y = v.second + j;
			if(x < 0 || y < 0 || x >= width || y >= height)
				continue;
			if(manhattan[y][x] > manhattan[v.second][v.first] + 1)
			{
				manhattan[y][x] = 1 + manhattan[v.second][v.first];
				Q.push({x, y});
			}
		}
	}
}


int R(long long a, long long b)
{
    return a + rand() % (b - a + 1);
}

struct pos
{
    int cx;
    int cy;
	int id;
    string type;
	int speed;
	int cooldown;
};

struct toQ
{
	int cx;
    int cy;
	int val;
};


struct en
{
	int cx;
    int cy;
	string type;
};

struct CompareQ
{
    bool operator()(const toQ &a, const toQ &b)
    {
        int dist1 = manhattan[a.cy][a.cx];
        int dist2 = manhattan[b.cy][b.cx];
		if(a.val == b.val)
		{
			if(dist1 < dist2)// && a.OnWay > b.OnWay)
	            return false;
	        if(dist1 > dist2)// && b.OnWay > a.OnWay)
	            return true;
			return true;
		}
		if(a.val > b.val)
		{
			if(dist2 > dist1 - 10)
				return false;
			return true;
		}
		if(b.val > a.val)
		{
			if(dist1 >= dist2 - 10)
				return true;
			return false;
		}
		return true;
    }
};

bool CompareS(const pair<int, int> &a, const pair<int, int> &b)
{
    double dist1 = manhattan[a.second][a.first];
    double dist2 = manhattan[b.second][b.first];
	return weight[a]/dist1 > weight[b]/dist2;// && a.OnWay > b.OnWay)
}

void klar()
{
    for(int i = 0; i < 1000; i++)
        for(int j = 0; j < 1000; j++)
            if(org[i][j] != '#')
			{
				grid[i][j] = ' ';
				org[i][j] = ' ';
			}

}
priority_queue <toQ, vector<toQ>, CompareQ> QP;

pair<int, int> Random_point()
{
    pair<int, int> ans = {R(0, width - 1), R(0, height - 1)};
	while(grid[ans.second][ans.first] == '#')
		ans = {R(0, width - 1), R(0, height - 1)};
	return ans;
}

bool attacks(string a, string b)
{
	if(a == "SCISSORS" && b == "PAPER")
		return true;
	if(a == "PAPER" && b == "ROCK")
		return true;
	if(a == "ROCK" && b == "SCISSORS")
		return true;
	return false;
}
void BFS(pos w)
{
	//cerr << "NEW BFS\n";
    queue <pair<int, int>> Q;
    Q.push({w.cx, w.cy});
	anc[{w.cx, w.cy}] = {-2, -2};
    grid[w.cy][w.cx] = 'B';
	int cnt = 0;
    while(!Q.empty())
    {
		cnt++;
		if(cnt == 60)
			return;
		if(QP.size() >= 15)
			return;
        pair<int, int> v = Q.front();
        Q.pop();
		v.first %= (width - 1);

        FOURSIDES(i, j)
        {
            int x = (v.first + i);
			int y = v.second + j;
			if(x < 0 || y < 0 || x >= width || y >= height)
				continue;
			//cerr << grid[y][x] << " " << org[y][x] << " " << x << " " << y  << " " << QP.size() << endl;
            if(grid[y][x%(width - 1)] == 'o')
            {
				anc[{x%(width - 1), y}] = {v.first, v.second};
                QP.push({x, y, 1});
                Q.push({x, y});
                grid[y][x%(width - 1)] = 'B';
            }
			else
            if(grid[y][x%(width - 1)] == 'O')
            {
				anc[{x%(width - 1), y}] = {v.first, v.second};
                QP.push({x, y, 10});
                Q.push({x, y});
                grid[y][x%(width - 1)] = 'B';
            }
			else
            if(grid[y][x%(width - 1)] == ' ')
            {
				anc[{x%(width - 1), y}] = {v.first, v.second};
                Q.push({x, y});
                grid[y][x%(width - 1)] = 'B';
            }
        }
    }
}

int main()
{
    cin >> width >> height; cin.ignore();
    for (int i = 0; i < height; i++)
    {
        string row;
        getline(cin, row); // one line of the grid: space " " is floor, pound "#" is wall
        for(int j = 0; j < row.size(); j++)
		{
            grid[i][j] = row[j];
			org[i][j] = row[j];
			if(row[j] == '#')
			{
				manhattan[i][j] = -1;
				weight[make_pair(j, i)] = 0;
			}
			else
			{
				S.push_back(make_pair(j, i));
				manhattan[i][j] = 1;
				weight[make_pair(j, i)] = 1;
			}
		}
    }

	int tur = 0;
    while (1)
    {
		tur++;
        int myScore;
        int opponentScore;
		vector<pos> Start;
		vector<en> Enemy;
        cin >> myScore >> opponentScore; cin.ignore();
        int visiblePacCount; // all your pacs and enemy pacs in sight
        cin >> visiblePacCount; cin.ignore();
        int myId = 0;
        for (int i = 0; i < visiblePacCount; i++)
		{
            int pacId; // pac number (unique within a team)
            bool mine; // true if this pac is yours
            int x; // position in the grid
            int y; // position in the grid
            string typeId; // unused in wood leagues
            int speedTurnsLeft; // unused in wood leagues
            int abilityCooldown; // unused in wood leagues
            cin >> pacId >> mine >> x >> y >> typeId >> speedTurnsLeft >> abilityCooldown; cin.ignore();
            if(!mine){
				grid[y][x] = typeId[0];
				org[y][x] = typeId[0];
				Enemy.push_back({x, y, typeId});
			//	manhattan[y][x] = -1;
			}
            else
            {
				grid[y][x] = 'M';
				org[y][x] = 'M';
                Start.push_back({x, y, pacId, typeId, speedTurnsLeft, abilityCooldown});
            }
			pair<int, int> er = make_pair(x, y);
			weight[er] = 0;
        }
        int visiblePelletCount; // all pellets in sight
        cin >> visiblePelletCount; cin.ignore();
        for (int i = 0; i < visiblePelletCount; i++)
        {
            int x;
            int y;
            int value; // amount of points this pellet is worth
            cin >> x >> y >> value; cin.ignore();
            grid[y][x] = 'o';
			org[y][x] = 'o';
			weight[make_pair(x, y)] = 1;
            if(value == 10)
			{
				grid[y][x] = 'O';
				weight[make_pair(x, y)] = 60;
				org[y][x] = 'O';
			}
        }

// program główny -----------------------------------------------------------------------------------------

		for(int v = 0; v < Start.size(); v++)
		{
// 			 ---------------- czyszczenie
			klar_man();
			klar_grid();
			anc.clear();
			while(!QP.empty())
			   QP.pop();
//	   		---------------------------

			pos Pac = Start[v];
			int ide = Pac.id;

			start = make_pair(Pac.cx, Pac.cy);
			cerr << "Start: " << Pac.cx << " " << Pac.cy << " " << ide << endl;
			weight[start] = 0;
			int mx[] = {0, 0, 1, -1};
			int my[] = {1, -1, 0, 0};
			for(int dir = 0; dir < 4; dir++)
				for(int i = 0; i < 30; i++)
				{
					int new_x = (start.first + mx[dir] * i + width)%width;
					int new_y = (start.second + my[dir] * i + height)%height;

					if(grid[new_y][new_x] == ' ' || grid[new_y][new_x] == 'M')
					{
						weight[make_pair(new_x, new_y)] = 0;
						cerr << "Zeruje: " << new_x << " " << new_y << endl;
					}
					else
					if(grid[new_y][new_x] == 'o' || grid[new_y][new_x] == 'O')
						continue;
					else break;
				}
			// for(int i = 0; i < 30; i++)
			// {
			// 	if(start.first + i < width && grid[start.second][start.first + i] == ' ')
			// 		weight[make_pair(start.first + i, start.second)] = 0;
			// 	if(start.first + i > 0 && grid[start.second][start.first - i] == ' ')
			// 		weight[make_pair(start.first - i, start.second)] = 0;
			// 	if(start.second + i < height && grid[start.second + i][start.first] == ' ')
			// 		weight[make_pair(start.first, start.second + i)] = 0;
			// 	if(start.second - i > 0 && grid[start.second - i][start.first] == ' ')
			// 		weight[make_pair(start.first, start.second - i)] = 0;
			// }

			if(last_random[ide] == make_pair(Pac.cx, Pac.cy)) // last_random to ostatni randomowy ruch. Jeśli cel został osiągnięty
				last_random[ide] = make_pair(-1, -1); 		// to ustawiami na nic

			dist({Pac.cx, Pac.cy}); // liczenie odl na mapie
			sort(S.begin(), S.end(), CompareS);
			debug(S, start);
			for(auto i: S)
			{
				cerr << weight[i]/manhattan[i.second][i.first] << " ";
			}
			cerr << endl;
			bool jezu = 0;

//			----------------------------------------------- czy lecieć za wrogiem
			for(auto i: Enemy)
			{
				if(manhattan[i.cy][i.cx] <= 2)
				{
					cerr << "może spróbować zjeść\n";
					if(attacks(Pac.type, i.type))
					{
						if(Pac.cooldown == 0)
							cout << "SPEED " << ide;
						else
							cout << "MOVE " << ide << " " << i.cx << " " << i.cy;
						if(v != Start.size() - 1)
							cout << "|";
						grid[i.cy][i.cx] = '.';
						jezu = true;
						break;
					}
					if(Pac.cooldown == 0)
					{
						string s;
						if(i.type == "SCISSORS")
							s = "ROCK";
						if(i.type == "PAPER")
							s = "SCISSORS";
						if(i.type == "ROCK")
							s = "PAPER";
						cout << "SWITCH " << ide <<" "<< s;
						if(v != Start.size() - 1)
							cout << "|";
						jezu = true;
						break;
					}
				}

			}
//			------------------------------------------------------------------------

			if(jezu) // znaleźliśmy wroga za którym lecimy
				continue;

			pair<int, int> go;	// wybieramy potencjalny chrupek
			pair<int, int> wp = S[0];
			cerr << "S.begin() = " << wp.first << " " << wp.second << endl;
			cerr << S.size() << endl;
			if(last_random[ide] != make_pair(-1, -1))
				go = last_random[ide];
			else
				go = wp;

			if(last_start[ide] == start) // jest kolizja
			{
				cerr << "KOLIZA\n";
				pair<int, int> zam = Random_point();
		        cout << "MOVE " << ide << " " << zam.first << " " << zam.second;
				if(v != Start.size() - 1)
					cout << "|";
				go = {-1, -1};
				last_random[ide] = go;
				grid[zam.second][zam.first] = '.';
				continue;
			}
			last_start[ide] = start;
			if(Pac.cooldown == 0 && tur - last_cool[ide] >= 5)
			{
				cerr << "ZIUUUUUUM" << endl;
				cout << "SPEED " << ide;
				if(v != Start.size() - 1)
					cout << "|";
				last_random[ide] = {-1, -1};
				last_start[ide] = {-1, -1};
				last_cool[ide] = tur;
				continue;
			}

			bool bfs_find = 0;
			BFS(Pac);
			if(!QP.empty())
			{
	        	go = {QP.top().cx, QP.top().cy};
				pair<int, int> k = go;
				int cnt = 0;
					while(k != make_pair(-2, -2) && cnt < 15)
					{
						grid[k.second][k.first] = '.';
						//weight[k] = 0;
						k = anc[k];
						cnt++;
					}
				cerr << "BIERZE " << go.first << " " << go.second << " " << ide << " " << manhattan[go.second][go.first] << endl;
				if(v != Start.size() - 1)
					cout << "|";
				last_random[ide] = {-1, -1};
				Pac.cx = go.first;
				Pac.cy = go.second;
				bfs_find = 1;
			}
			else
				last_random[ide] = go;
			if(Pac.speed != 0 && manhattan[go.second][go.first] <= 1)
			{
				cerr << "szuka!" << endl;
				bool found = false;
				FOURSIDES(i, j)
				{
					int x = go.first + j;
					int y = go.second + i;
					if(x < 0 || y < 0 || x >= width || y >= height)
						continue;
					if((org[y][x] == 'o' || org[y][x] == 'O') && make_pair(x, y) != start)
					{
						if(manhattan[y][x] > manhattan[go.second][go.first])
						{
							go.second += i;
							go.first += j;
							found = true;
							cerr << "nowy chrupek ";
							break;
						}
					}
				}
				if(!found)
				FOURSIDES(i, j)
				{
					int x = go.first + j;
					int y = go.second + i;
					if(x < 0 || y < 0 || x >= width || y >= height)
						continue;
					cerr << "tu sie psuje " << manhattan[y][x] << " " << org[y][x]<< endl;
					if(org[y][x] != '#' && make_pair(x, y) != start)
					{
						if(manhattan[y][x] > manhattan[go.second][go.first])
						{
							go.second += i;
							go.first += j;
							break;
						}
					}
				}
				cerr << "wyszukał! " << go.first << " " <<go.second << endl;
			}
			if(!bfs_find)
			{
				go = wp;
			}
				cout << "MOVE " << ide << " " << go.first << " " << go.second;
			grid[go.second][go.first] = '.';
			if(v != Start.size() - 1)
				cout << "|";
		}
		for(auto &it: weight)
		{
			it.second *= 0.99;
		}
		cout << endl;
		cerr << "------------" << endl;
        klar();
    }
}
