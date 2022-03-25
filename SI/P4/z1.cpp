#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <sys/time.h>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>

using namespace std;

#define THROUGH_BOARD(i, j)	for(int i = 0; i < 8; i++)for(int j = 0; j < 8; j++)

const int INF = 1000;

class Board
{
	private:
		pair<int, int> dirs[8] = {{0,1}, {1,0}, {-1,0}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1}};
		vector<vector<char>> board = vector<vector<char>>(8);
		int done_moves()
		{
			int result = 0;
			for(auto &i: board)
				for(auto &j: i)
					result += j != '.';
			return result;
		}
		int points()
		{
			int result = 0;
			for(auto &i: board)
				for(auto &j: i)
					result += j == '#' ? 1 : (j == 'O' ? -1 : 0);
			return result;
		}
		char get(int x, int y, vector<vector<char>> &arr)
		{
			if(0 <= x && x < 8 && 0 <= y && y < 8)
				return arr[x][y];
			return '!';
		}
		bool beats(char pl, int x, int y, pair<int, int> d, vector<vector<char>> &arr)
		{
			int dx = d.first, dy = d.second;
			x += dx;
			y += dy;
			char dest = '#';
			if(pl == '#')
				dest = 'O';
			int cnt = 0;
			while(get(x, y, arr) == dest)
			{
				x += dx;
				y += dy;
				cnt++;
			}
			if(cnt > 0 && get(x, y, arr) == pl)
				return true;
			return false;
		}
		vector <pair<int, int>> validmoves(char player, vector<vector<char>> &arr)
		{
			vector<pair<int, int>> Ans;
			THROUGH_BOARD(i, j)
			{
				if(arr[i][j] == '.')
					for(auto d: dirs)
					{
						if(beats(player, i, j, d, arr))
						{
							Ans.push_back({i, j});
							break;
						}
					}
			}
			return Ans;
		}
		void move_piece(char player, vector<vector<char>> &arr, pair<int, int> move)
		{
			int x = move.first, y = move.second;
			arr[x][y] = player;
			char dest = '#';
			if(player == '#')
				dest = 'O';
			for(auto d: dirs)
			{
				int xx = x, yy = y;
				int dx = d.first;
				int dy = d.second;
				xx += dx;
				yy += dy;
				vector<pair<int, int>> beat;
	            while(get(xx, yy, arr) == dest)
				{
					beat.push_back({xx, yy});
	  	            xx += dx;
	  	            yy += dy;

				}
				if(get(xx, yy, arr) == player)
					for(auto i: beat)
						arr[i.first][i.second] = player;
			}
		}
		void random_move(char player)
		{
			auto Avb = validmoves(player, board);
			if(Avb.empty())
				return;
			pair<int, int> move = Avb[rand() % Avb.size()];
			move_piece(player, board, move);
		}
		int value(vector<vector<char>> &arr, char pl)
		{
			int cnt = 0;
			for(int i = 0; i < 20; i++)
			{
				char player = pl;
				Board RandomGame = Board(arr);
			//	RandomGame.print_board();
				while(true)
				{
					RandomGame.random_move(player);
					if(player == 'O')
						player = '#';
					else player = 'O';
					if(RandomGame.end())
					{
						char c = RandomGame.winner();
						if(c == '#')
							cnt++;
						if(c == 'O')
							cnt--;
						break;
					}
					//usleep(150000);
				//	RandomGame.print_board();
				}
			}
			//cout << "koniec value\n";
			//usleep(1500000);
			return cnt;
		}
		pair<pair<int ,int>, int> expectiminimax(char player, int depth, vector<vector<char>> arr)
		{
			pair<pair<int ,int>, int> a;
			auto Avb = validmoves(player, arr);
			//cout << "gracz: " << player << "\nMożliwe ruchy: ";
			// for(auto m: Avb)
			// {
			// 	cout <<"("<< m.first << " " <<m.second <<  "), ";
			// }
			//cout << endl;
			if(depth <= 0 || Avb.size() == 0)
			{
				//cout << "wyszło\n";
				return make_pair(make_pair(1, 1), value(arr, player));
			}

			if(player == '#')
			{
				a.second = -INF;
				for(auto m: Avb)
				{
					vector<vector<char>> new_arr = arr;
					move_piece(player, new_arr, m);
					auto val = expectiminimax('O', depth - 1, new_arr);
				//	cout << "wartość: " <<val.second  << " w porównaniu: " << a.second<< "\n";
					if(val.second > a.second)
					{
					//	cout << "lepsze!\n";
						a.second = val.second;
						a.first = m;
					}
				}
			}
			else
			{
				a.second = INF;
				for(auto m: Avb)
				{
					vector<vector<char>> new_arr = arr;
					move_piece(player, new_arr, m);
					auto val = expectiminimax('#', depth - 1, new_arr);
					//cout << "wartość: " <<val.second << "\n";
					if(val.second < a.second)
					{
					//	cout << "lepsze!\n";
						a.second = val.second;
						a.first = m;
					}
				}
			}
			//cout << "ruch: " << a.first.first << " " << a.first.second << endl;
			return a;
		}

	public:
		Board(vector<vector<char>> a)
		{
			if(!a.empty())
				board = a;
			else
			{
				THROUGH_BOARD(i, j)
					board[i].push_back('.');

				board[3][3] = board[4][4] = 'O';
				board[3][4] = board[4][3] = '#';
			}
		}
		~Board(){};
		void print_board()
		{
			for(int i = 0; i < 8; i++)
			{
				for(int j = 0; j < 8; j++)
					cout << board[i][j] << " ";
				cout << endl;
			}
			cout << endl;
		}
		void move(char player)
		{
			if(player == 'O')
				this->random_move(player);
		 	else
			{
		 		auto move = this->expectiminimax(player, done_moves() < 50 ? 1 : 3, board);
				move_piece(player, board, move.first);
			}

		}
		bool end()
		{
			auto Avb1 = validmoves('O', board);
			auto Avb2 = validmoves('#', board);
			if(Avb1.size() == 0 && Avb2.size() == 0)
				return true;
			return false;
		}
		char winner()
		{
			int cnt1 = 0;
			int cnt2 = 0;
			for(int i = 0; i < 8; i++)
				for(int j = 0; j < 8; j++)
				{
					if(board[i][j] == '#')
						cnt1++;
					else if(board[i][j] == 'O')
						cnt2++;
				}
			if(cnt1 > cnt2)
				return '#';
			if(cnt2 > cnt1)
				return 'O';
			return '!';
		}
};

int main()
{
	timeval czas;
    gettimeofday(&czas, 0);
    srand(czas.tv_sec * 1000 + czas.tv_usec % 1000);

	int cnt1 = 0;
	int cnt2 = 0;
	for(int i = 0; i < 1000; i++)
	{
		char player = '#';
		vector<vector<char>> v;
		Board Game =  Board(v);
		int turns = 0;
		while(true)
		{
			char x;
			Game.move(player);
			if(player == 'O')
				player = '#';
			else player = 'O';
//			Game.print_board();
			//usleep(1000000);
			if(Game.end())
			{
				char c = Game.winner();
				cout << "Wygrywa: " << c << " ja: " << cnt1 << " on:" << cnt2 << endl;
				if(c == '#')
					cnt1++;
				if(c == 'O')
					cnt2++;
				cout << turns << endl;
				break;
			}
			turns++;
		}
//		usleep(1000000);
	}
	cout << cnt1 << " " << cnt2 << endl;
}
