#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

char grid[1000][1000];

pair<int, int> start;
int width;
int height;
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
			if(dist2 >= dist1 - 10)
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
            if(grid[i][j] != '#')
                grid[i][j] = ' ';
}
priority_queue <pos, vector<pos>, Compare> QP;

void BFS(pos w)
{
    queue <pos> Q;
    Q.push(w);
    grid[w.cy][w.cx] = 'B';
    while(!Q.empty())
    {
		if(QP.size() >= 10)
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
                        QP.push({x, y, 1, v.OnWay + 1});
                        Q.push({x, y, 0, v.OnWay + 1});
                        grid[y][x%(width - 1)] = 'B';
                    }
					else
                    if(grid[y][x%(width - 1)] == 'O')
                    {
                        QP.push({x, y, 10, v.OnWay + 1});
                        Q.push({x, y, 0, v.OnWay + 1});
                        grid[y][x%(width - 1)] = 'B';
                    }
					else
                    if(grid[y][x%(width - 1)] == ' ')
                    {
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
            grid[i][j] = row[j];
    }

    while (1)
    {
        int myScore;
        int opponentScore;
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
            if(!mine)
                grid[y][x] = 'X';
            else
            {
                myId = pacId;
                start = {x, y};
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
            if(value == 10)
                grid[y][x] = 'O';
        }
        cerr << start.first << " " << start.second << endl;
        BFS({start.first, start.second, 0, 0});
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        pair<int, int> go = {QP.top().cx, QP.top().cy};
        cerr << go.first << " " << go.second << endl;
        while(!QP.empty()) QP.pop();
        klar();
        cout << "MOVE " << myId << " " << go.first << " " << go.second << endl; // MOVE <pacId> <x> <y>
    }
}
