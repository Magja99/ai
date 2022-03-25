#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

char grid[1000][1000];
char org[1000][1000];
pair<int, int> start;
map<pair<int, int>, pair<int, int>> anc;
map <int, pair<int, int>> pop;
int width;
int height;

int R(long long a, long long b)
{
        return a + rand() % (b - a + 1);
}

struct pos
{
    int cx;
    int cy;
    int val;
	int OnWay;
};

struct Compare
{
    bool operator()(const pos &a, pos &b)
    {
        int dist1 = abs(a.cx - start.first) + abs(a.cy - start.second);
        int dist2 = abs(b.cx - start.first) + abs(b.cy - start.second);
		if(a.val == b.val)
		{
			if(dist1 < dist2)// && a.OnWay > b.OnWay)
	            return false;
	        if(dist1 > dist2)// && b.OnWay > a.OnWay)
	            return true;
			return false;
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
priority_queue <pos, vector<pos>, Compare> QP;

void BFS(pos w)
{
    queue <pos> Q;
    Q.push(w);
	anc[{w.cx, w.cy}] = {-2, -2};
    grid[w.cy][w.cx] = 'B';
	int cnt = 0;
    while(!Q.empty())
    {
		cnt++;
		if(cnt == 30)
			return;
		if(QP.size() >= 5)
			return;
        pos v = Q.front();
        Q.pop();
		v.cx %= (width - 1);
        for(int i = -1; i <= 1; i++)
            for(int j = -1; j <= 1; j++)
                if(i != j)
                {
                    int x = (v.cx + i);
					int y = v.cy + j;
                    if(grid[y][x%(width - 1)] == 'o')
                    {
						anc[{x%(width - 1), y}] = {v.cx, v.cy};
                        QP.push({x, y, 1, v.OnWay + 1});
                        Q.push({x, y, 0, v.OnWay + 1});
                        grid[y][x%(width - 1)] = 'B';
                    }
					else
                    if(grid[y][x%(width - 1)] == 'O')
                    {
						anc[{x%(width - 1), y}] = {v.cx, v.cy};
                        QP.push({x, y, 10, v.OnWay + 1});
                        Q.push({x, y, 0, v.OnWay + 1});
                        grid[y][x%(width - 1)] = 'B';
                    }
					else
                    if(grid[y][x%(width - 1)] == ' ')
                    {
						anc[{x%(width - 1), y}] = {v.cx, v.cy};
                        Q.push({x, y, 0, v.OnWay});
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
		}
    }

    while (1)
    {
        int myScore;
        int opponentScore;
		vector<pair<pair<int, int>, int>> Start;
        cin >> myScore >> opponentScore; cin.ignore();
        int visiblePacCount; // all your pacs and enemy pacs in sight
        cin >> visiblePacCount; cin.ignore();
        int myId = 0;
        for (int i = 0; i < visiblePacCount; i++) {
            int pacId; // pac number (unique within a team)
            bool mine; // true if this pac is yours
            int x; // position in the grid
            int y; // position in the grid
            string typeId; // unused in wood leagues
            int speedTurnsLeft; // unused in wood leagues
            int abilityCooldown; // unused in wood leagues
            cin >> pacId >> mine >> x >> y >> typeId >> speedTurnsLeft >> abilityCooldown; cin.ignore();
            if(!mine){
				grid[y][x] = 'X';
				org[y][x] = 'X';
			}

            else
            {
                Start.push_back({{x, y}, pacId});
            }
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
            if(value == 10)
			{
				grid[y][x] = 'O';
				org[y][x] = 'O';
			}

        }
		for(int v = 0; v < Start.size(); v++)
		{
			pair<int, int> wyp = {R(0, width - 1), R(0, height - 1)};
			while(grid[wyp.second][wyp.first] == '#')
				wyp = {R(0, width - 1), R(0, height - 1)};
			int ide = Start[v].second;
			start = Start[v].first;
			if(pop[ide] == start)
			{
		        cout << "MOVE " << ide << " " << wyp.first << " " << wyp.second;
				if(v != Start.size() - 1)
					cout << "|";
				wyp = {-1, -1};
				continue;
			}

			pop[ide] = start;
			cerr << start.first << " " << start.second << " " <<  ide << endl;

			BFS({Start[v].first.first, Start[v].first.second, 0, 0});
			pair<int, int> go;
			if(!QP.empty())
			{
	        	go = {QP.top().cx, QP.top().cy};
	        	cerr << go.first << " " << go.second << " " <<  ide << endl;

				for(int i = 0; i < 1000; i++)
			        for(int j = 0; j < 1000; j++)
			            if(grid[i][j] != '.')
							grid[i][j] = org[i][j];

				pair<int, int> k = go;
				int cnt = 0;
				while(k != make_pair(-2, -2) && cnt < 20)
				{
					grid[k.second][k.first] = '.';
					k = anc[k];
					cnt++;
				}
				cout << "MOVE " << ide << " " << go.first << " " << go.second;
				if(v != Start.size() - 1)
					cout << "|";
				wyp = {-1, -1};
			}
			else
			{
				cout << "MOVE " << ide << " " << wyp.first << " " << wyp.second;
				if(v != Start.size() - 1)
					cout << "|";
				wyp = {-1, -1};
			}
			 while(!QP.empty())
			 	QP.pop();
			 anc.clear();
			 for(int i = 0; i < 1000; i++)
				 for(int j = 0; j < 1000; j++)
					 if(grid[i][j] != '.')
						 grid[i][j] = org[i][j];
			if(wyp.first != -1)
			{
				cout << "MOVE " << ide << " " << wyp.first << " " << wyp.second;
				if(v != Start.size() - 1)
					cout << "|";
			}
		}
		cout << endl;
		cerr << "------------" << endl;
        klar();
    }
}
