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

const double INF = 100000000;

class Board
{
	private:
		pair<int, int> dirs[8] = {{0,1}, {1,0}, {-1,0}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1}};
		vector<vector<char>> board = vector<vector<char>>(8);
		vector<vector<int>> board_val = vector<vector<int>>(8);
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
		Board symulation(vector<vector<char>> arr, char pl)
		{
			char player = pl;
			vector<vector<int>> v;
			Board RandomGame = Board(arr, v);
			while(true)
			{
				RandomGame.random_move(player);
				if(player == 'O')
					player = '#';
				else player = 'O';
				if(RandomGame.end())
					return RandomGame;
			}
		}
		double heuristic_value(vector<vector<char>> &arr, char pl)
		{
			int cnt = 0;
			double pieces1 = 0, pieces2 = 0;
			int cor1 = 0, cor2 = 0;
			int co1, co2 = 0;

			THROUGH_BOARD(i, j)
			{
				if(arr[i][j] == '#')
				{
					if(i == 0 || i == 7 || j == 0 || j == 7)
						co1++;
					pieces1 += 1;
					cor1 += board_val[i][j];
				}
				if(arr[i][j] == 'O')
				{
					if(i == 0 || i == 7 || j == 0 || j == 7)
						co2++;
					pieces2+=  1;
					cor2 += board_val[i][j];
				}
			}

			auto Avb1 = validmoves('#', arr);
			auto Avb2 = validmoves('O', arr);
			double mobility1 = Avb1.size(), mobility2 = Avb2.size();
			double p, m;
			if(mobility1 < mobility2)
				m = -100 * mobility2 / (mobility1 + mobility2);
			else if(mobility1 > mobility2)
				m = 100 * mobility1 / (mobility1 + mobility2);
			else
			 	m = 0;
			if(pieces1 < pieces2)
				p =  -100 *  pieces2/(pieces1 + pieces2);
			else if(pieces1 > pieces2)
				p = 100 * pieces1/(pieces1 + pieces2);
			else
				p = 0;

			return 10 * p + 20 * m + 100 * (cor1 - cor2) + 1000 * (co1 - co2);

		}
		double sym_value(vector<vector<char>> &arr, char pl)
		{
			int cnt = 0;
			for(int i = 0; i < 7; i++)
			{
				Board RandomGame = symulation(arr, pl);
				char c = RandomGame.winner();
				if(c == '#')
					cnt++;
				if(c == 'O')
					cnt--;
			}
			double heu = heuristic_value(arr, pl);
			return cnt * 20 + heu/7000;
		}
		pair<pair<int ,int>, double> minimax(char player, int depth, vector<vector<char>> arr, pair<pair<int ,int>, double> alfa, pair<pair<int ,int>, double> beta)
		{
			auto Avb = validmoves(player, arr);
			if(depth <= 0 || Avb.size() == 0)
				return make_pair(make_pair(1, 1), sym_value(arr, player));
			if(player == '#')
			{
				double v = -INF;
				for(auto m: Avb)
				{
					vector<vector<char>> new_arr = arr;
					move_piece(player, new_arr, m);
					auto val =  minimax('O', depth - 1, new_arr, alfa, beta);
					if(val.second > v)
						v = val.second;
					if(v > alfa.second)
					{
						alfa.second = v;
						alfa.first = m;
					}
					if(beta.second <= alfa.second)
						break;
				}
				return alfa;
			}
			else
			{
				double v = INF;
				for(auto m: Avb)
				{
					vector<vector<char>> new_arr = arr;
					move_piece(player, new_arr, m);
					auto val = minimax('#', depth - 1, new_arr, alfa, beta);
					if(val.second < v)
						v = val.second;
					if(v < beta.second)
					{
						beta.second = v;
						beta.first = m;
					}
					if(beta.second <= alfa.second)
						break;
				}
				return beta;
			}
		}

	public:
		Board(vector<vector<char>> a, vector<vector<int>> b)
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
			if(!b.empty())
				board_val = b;
			else
				THROUGH_BOARD(i, j)
					board_val[i].push_back(0);
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
				pair<pair<int ,int>, double> a = make_pair(make_pair(1, 1), -INF);
				pair<pair<int ,int>, double> b = make_pair(make_pair(1, 1), INF);
				pair<pair<int ,int>, double> move;
				if(done_moves() < 45)
			 		move = this->minimax(player, 1, board, a, b);
				else
				if(done_moves() < 53)
			 		move = this->minimax(player, 2, board, a, b);
				else
					move = this->minimax(player, 4, board, a, b);
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
		vector<vector<int>> make_board_val()
		{
			//cout << "board val zrobione\n";
			for(int i = 0; i < 1000; i++)
			{
				Board RandomGame = symulation(board, '#');
				char c = RandomGame.winner();
				int v = -1;
				if(c == '#')
					v = 1;
				THROUGH_BOARD(i, j)
				{
					if(RandomGame.board[i][j] == '#')
						board_val[i][j] += v;
					if(RandomGame.board[i][j] == 'O')
						board_val[i][j] += v * (-1);
				}
			}
			return board_val;
		}
};

int main()
{
	timeval czas;
    gettimeofday(&czas, 0);
    srand(czas.tv_sec * 1000 + czas.tv_usec % 1000);

	int cnt1 = 0;
	int cnt2 = 0;
	vector<vector<char>> v;
	vector<vector<int>> vb;
	Board G = Board(v, vb);
	vector<vector<int>> b = G.make_board_val();
	for(int i = 0; i < 1000; i++)
	{
		char player = '#';
		Board Game =  Board(v, b);
		int turns = 0;
		while(true)
		{
			char x;
			Game.move(player);
			if(player == 'O')
				player = '#';
			else player = 'O';
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
	}
	cout << cnt1 << " " << cnt2 << endl;
}
