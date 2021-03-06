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
map <pair<int, int>, bool> taken;

map<int, int> last_cool;

int width;
int height;
int manhattan[1000][1000];

vector<pair<int, int>> S;

void klar_man()
{
	for(int i = 0; i <= height; i++)
		for(int j = 0; j <= width; j++)
			if(manhattan[i][j] != -1)
				manhattan[i][j] = 10000000;
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
				manhattan[i][j] = 1000000;
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

void erase_visible(pair<int, int> start)
{
	for(int i = 0; i < 30; i++)
	{
		if(start.first + i < width && grid[start.second][start.first + i] == ' ')
			weight[make_pair(start.first + i, start.second)] = 0;
		if(start.first + i < width && grid[start.second][start.first + i] == '#')
			break;
	}
	for(int i = 0; i < 30; i++)
	{
		if(start.first + i > 0 && grid[start.second][start.first - i] == ' ')
			weight[make_pair(start.first - i, start.second)] = 0;
		if(start.first + i > 0 && grid[start.second][start.first - i] == '#')
			break;
	}
	for(int i = 0; i < 30; i++)
	{
		if(start.second + i < height && grid[start.second + i][start.first] == ' ')
			weight[make_pair(start.first, start.second + i)] = 0;
		if(start.second + i < height && grid[start.second + i][start.first] == '#')
			break;
	}
	for(int i = 0; i < 30; i++)
	{
		if(start.second - i > 0 && grid[start.second - i][start.first] == ' ')
			weight[make_pair(start.first, start.second - i)] = 0;
		if(start.second - i > 0 && grid[start.second - i][start.first] == '#')
			break;
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
		vector<pos> Enemy;
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
			if(typeId == "DEAD")
				continue;
            if(!mine){
				grid[y][x] = typeId[0];
				org[y][x] = typeId[0];
				Enemy.push_back({x, y, pacId, typeId, speedTurnsLeft, abilityCooldown});
				cerr << "wr??g na: " << x << " " << y << endl;
				manhattan[y][x] = -1;
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
				weight[make_pair(x, y)] = 400;
				org[y][x] = 'O';
			}
        }
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				if(grid[i][j] != 'O' && weight[make_pair(j, i)] >= 15)
					weight[make_pair(j, i)] = 0;
			}
		}
		for(int v = 0; v < Start.size(); v++)
			erase_visible({Start[v].cx, Start[v].cy});
// program g????wny -----------------------------------------------------------------------------------------
		taken.clear();
		vector<pos> Rest_Pacs;
		vector<pair<int, int>> Rest_goals;
		for(int v = 0; v < Start.size(); v++)
		{
			klar_man();
			klar_grid();
			anc.clear();


			while(!QP.empty())
			   QP.pop();
			pos Pac = Start[v];
			int ide = Pac.id;

			start = make_pair(Pac.cx, Pac.cy);
			cerr << "Start: " << Pac.cx << " " << Pac.cy << " " << ide << endl;
			weight[start] = 0;

			dist({Pac.cx, Pac.cy}); // liczenie odl na mapie
			sort(S.begin(), S.end(), CompareS);
			//debug(S, start);
			for(int i = 0; i < 5; i++)
			{
				cerr << weight[S[i]] << " " << S[i] << endl;
			}
			cerr << endl;
			bool jezu = 0;

//			----------------------------------------------- czy lecie????za wrogiem
			for(auto i: Enemy)
			{
				if(abs(Pac.cx - i.cx) + abs(Pac.cy - i.cy) <= 2)
				{
					cerr << "mo??e spr??bowa????zje????\n";
					if(attacks(Pac.type, i.type) && i.cooldown != 0)
					{
						if(Pac.cooldown == 0)
							cout << "SPEED " << ide;
						else
							cout << "MOVE " << ide << " " << i.cx << " " << i.cy;
						if(v != Start.size() - 1 || Rest_Pacs.size() != 0)
							cout << "|";
						jezu = true;
						break;
					}
					if(Pac.cooldown == 0 && abs(Pac.cx - i.cx) + abs(Pac.cy - i.cy) <= 1)
					{
						string s;
						if(i.type == "SCISSORS")
							s = "ROCK";
						if(i.type == "PAPER")
							s = "SCISSORS";
						if(i.type == "ROCK")
							s = "PAPER";
						cout << "SWITCH " << ide <<" "<< s;
						if(v != Start.size() - 1 || Rest_Pacs.size() != 0)
							cout << "|";
						jezu = true;
						break;
					}
					if(attacks(i.type, Pac.type) || i.cooldown == 0)
					{
						cerr << "spierdala\n";
						FOURSIDES(k, j)
						{
							int new_x = (Pac.cx + k + width)%width;
							int di_n = abs(new_x - i.cx) + abs(Pac.cy + j - i.cy);
							int di_s = abs(Pac.cx - i.cx) + abs(Pac.cy - i.cy);
							if(di_n <= di_s)
								continue;
							else if(grid[Pac.cy + j][new_x] != '#' && !jezu)
							{
								cout << "MOVE " << ide << " " << new_x << " " << Pac.cy + j;
								jezu = true;
								break;
							}
						}
						if(v != Start.size() - 1 || Rest_Pacs.size() != 0)
							cout << "|";
						break;
					}
				}
			}
//			------------------------------------------------------------------------

			if(jezu) // znale??li??my wroga za kt??rym lecimy
				continue;

			pair<int, int> go;	// wybieramy potencjalny chrupek
			pair<int, int> wp = S[0];
			int z = 1;
			while(taken[wp])
			{
				wp = S[z];
				z++;
			}
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
				if(v != Start.size() - 1 || Rest_Pacs.size() != 0)
					cout << "|";
				go = {-1, -1};
				last_random[ide] = go;
				grid[zam.second][zam.first] = '.';
				continue;
			}
			last_start[ide] = start;
			if(Pac.cooldown == 0 && (tur - last_cool[ide] >= 10 || tur <= 1))
			{
				cerr << "ZIUUUUUUM" << endl;
				cout << "SPEED " << ide;
				if(v != Start.size() - 1 || Rest_Pacs.size() != 0)
					cout << "|";
				last_random[ide] = {-1, -1};
				last_start[ide] = {-1, -1};
				last_cool[ide] = tur;
				continue;
			}
			if(Pac.speed != 0 && manhattan[go.second][go.first] <= 1 && weight[go] < 10)
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
				cerr << "wyszuka??! " << go.first << " " <<go.second << endl;
			}
			Rest_Pacs.push_back(Pac);
			Rest_goals.push_back(go);
			// cout << "MOVE " << ide << " " << go.first << " " << go.second;
			// if(v != Start.size() - 1 || Rest_Pacs.size() != 0)
			// 	cout << "|";
			taken[go] = true;
		}
		for(int i = 0; i < Rest_Pacs.size(); i++)
		{
			klar_man();

			int best = 10000000;
			pair<int, int> g;
			int k;
			int px = Rest_Pacs[i].cx;
			int py = Rest_Pacs[i].cy;
			dist({px, py});
			for(int j = 0; j < Rest_goals.size(); j++)
			{
				int x = Rest_goals[j].first;
				int y = Rest_goals[j].second;
				if(manhattan[y][x] < best && x != -1)
				{
					best = manhattan[y][x];
					k = j;
					g = {x, y};
				}
			}
			cout << "MOVE " << Rest_Pacs[i].id << " " << g.first << " " << g.second;
			if(i != Rest_Pacs.size() - 1)
				cout << "|";
			Rest_goals[k].first = -1;
		}
		for(auto &it: weight)
		{
			it.second *= 0.95;
		}
		cout << endl;
		cerr << "------------" << endl;
        klar();
    }
}
