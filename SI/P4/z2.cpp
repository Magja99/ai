#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <sys/time.h>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>
#include <map>

using namespace std;
#define pb push_back
#define THROUGH_BOARD(i, j)	for(int i = 0; i < 9; i++)for(int j = 0; j < 7; j++)
#define FOURSIDES(i, j)	for(int i = -1; i <= 1; i++) for(int j = -1; j <= 1; j++) if(abs(i - j) == 1)

const int INF = 10000;

struct movement
{
	char animal;
	int x;
	int y;
};

class Game
{
	private:
		bool beats[8][8];
		vector<string> board;
		vector<string> raw_board;
		map <char, int> Id;
		int max_moves;
		int total_moves = 0;
		int moves_without_beating = 0;
		int second;
		string mode;
		char get(int xx, int yy, vector<string> &arr)
		{
			if(0 <= xx && xx < 9 && 0 <= yy && yy < 7)
			{
			//	cout << xx << " " << yy << endl;
				return arr[xx][yy];
			}

			return '!';
		}

		bool piece_of(char c, int player)
		{
			if(65 <= c && c <= 90 && player == 1)
				return true;
			if(97 <= c && c <= 122 && player == 0)
				return true;
			return false;
		}

		void move_piece(movement move, int player)
		{
			THROUGH_BOARD(i, j)
			{
				if(board[i][j] == move.animal)
				{
					board[i][j] = raw_board[i][j];

					if(piece_of(board[move.x][move.y], 1 - player))
					{
						char opponent = toupper(board[move.x][move.y]);
						char me = toupper(move.animal);
						if(raw_board[move.x][move.y] == '#' || beats[Id[me]][Id[opponent]])
						{
							board[move.x][move.y] = move.animal;
							this->moves_without_beating = 0;
							return;
						}
						if(beats[Id[opponent]][Id[me]])
						{
							this->moves_without_beating = 0;
							return;
						}
					}
					else
					{
						board[move.x][move.y] = move.animal;
						this->moves_without_beating++;
					}
				}
			}
		}
		movement valid_move(int a, int b, movement m, int player)
		{
			int xx = m.x + a, yy = m.y + b;
			if(get(xx, yy, board) == '!')
				return {1, -1, -1};
			if(piece_of(get(xx, yy, board), player))
				return {1, -1, -1};
			if(player == 1 && xx == 0 && yy == 3)
				return {1, -1, -1};
			if(player == 0 && xx == 8 && yy == 3)
				return {1, -1, -1};
			if(m.animal == 'T' || m.animal == 't' || m.animal == 'L' || m.animal == 'l')
			{
				if(get(xx, yy, board) == '~')
				{
					while(get(xx, yy, board) == '~')
						xx += a, yy += b;

					if(get(xx, yy, board) == '!')
						return {1, -1, -1};
					else
						return valid_move(0, 0, {m.animal, xx, yy}, player);
				}
			}
			else
			if(m.animal != 'R' && m.animal != 'r' && get(xx, yy, board) == '~')
				return {1, -1, -1};
			return {m.animal, xx, yy};
		}
		vector<movement> gen_moves(vector<movement> &ToMove, int player)
		{
			vector<movement> Ans;
			for(auto m: ToMove)
			{
				FOURSIDES(i, j)
				{
					movement k = valid_move(i, j, m, player);
					if(k.x != -1)
						Ans.pb(k);
				}
			}
			return Ans;
		}

		int winner()
		{
			if(this->moves_without_beating >= 30)
			{
				vector<int> P1;
				vector<int> P2;
				THROUGH_BOARD(i, j)
				{
					if(piece_of(board[i][j], 1 - second))
						P1.pb(Id[board[i][j]]);
					if(piece_of(board[i][j], second))
						P2.pb(Id[board[i][j]]);
				}
				sort(P1.begin(), P1.end());
				sort(P2.begin(), P2.end());
				reverse(P1.begin(), P1.end());
				reverse(P2.begin(), P2.end());
				if(P1 == P2)
					return second;
				else if(P1 > P2)
					return 1 - second;
				else
				 if(P1 < P2)
					return second;
			}
			if(board[0][3] != '*')
				return 0;
			if(board[8][3] != '*')
				return 1;
			return -1;
		}
		movement symulate(vector<movement> &moves, int player, vector<string> &arr, int mv)
		{
			movement best;
			int val = -1000000;
			for(auto m: moves)
			{
				int cnt = 0;
				int n = 20000 / moves.size();
				while(n > 0)
				{
					int pl = player;
					Game RandomGame = Game(arr, INF, "symulation", this->second); // wrzucenie zamiast INF
					RandomGame.total_moves = mv + 1;
					RandomGame.move_piece(m, pl);
					n--;
					pl = 1 - pl;
					while(true)
					{
						RandomGame.move(pl);
						n--;
						pl = 1 - pl;
						if(RandomGame.end() || n < 0)
						{
							int c = RandomGame.winner();
							if(c == 1)
								cnt++;
							if(c == 0)
								cnt--;
							break;
						}
					}
				}
				if(val < cnt)
				{
					val = cnt;
					best = m;
				}
			}
			return best;
		}
		movement best_move(int player)
		{
			vector<movement> ToGen;
			THROUGH_BOARD(i, j)
			{
				if(piece_of(board[i][j], player))
					ToGen.pb({board[i][j], i, j});
			}
			vector<movement> Avb = gen_moves(ToGen, player);
			movement best;

			if(this->mode == "symulation" || player == second)
				best = Avb[rand() % Avb.size()];
			else
			if(this->mode == "actual game")
				best = symulate(Avb, player, board, total_moves);
			return best;
		}

	public:
		Game(vector<string> arr, int v, string m, int s)
		{
			for(int i = 0; i < 8; i++)
				for(int j = i - 1; j > 0; j--)
					beats[i][j] = true;
			this->max_moves = v;
			this->mode = m;
			this->second = s;
			beats[0][7] = true;
			beats[7][0] = false;
			Id['R'] = Id['r'] = 0;
			Id['C'] = Id['c'] = 1;
			Id['D'] = Id['d'] = 2;
			Id['W'] = Id['w'] = 3;
			Id['J'] = Id['j'] = 4;
			Id['T'] = Id['t'] = 5;
			Id['L'] = Id['l'] = 6;
			Id['E'] = Id['e'] = 7;
			if(arr.size() != 0)
				board = arr;
			else
			{
				board.pb("L.#*#.T");
				board.pb(".D.#.C.");
				board.pb("R.J.W.E");
				board.pb(".~~.~~.");
				board.pb(".~~.~~.");
				board.pb(".~~.~~.");
				board.pb("e.w.j.r");
				board.pb(".c.#.d.");
				board.pb("t.#*#.l");
			}
			raw_board.pb("..#*#..");
			raw_board.pb("...#...");
			raw_board.pb(".......");
			raw_board.pb(".~~.~~.");
			raw_board.pb(".~~.~~.");
			raw_board.pb(".~~.~~.");
			raw_board.pb(".......");
			raw_board.pb("...#...");
			raw_board.pb("..#*#..");
		}
		~Game(){};
		bool end() // TO DO
		{
			if(this->moves_without_beating >= 30)
				return true;
			if(board[0][3] != '*' || board[8][3] != '*')
				return true;
			int p1 = 0, p2 = 0;
			THROUGH_BOARD(i, j)
			{
				if(piece_of(board[i][j], 1))
					p1++;
				if(piece_of(board[i][j], 0))
					p2++;
			}
			if(p1 == 0 || p2 == 0)
				return true;
			return false;
		}
		void move(int player)
		{
			movement bm = this->best_move(player);
			this->move_piece(bm, player);
			this->total_moves++;

		}
		int win()
		{
			return winner();
		}
		void print_board()
		{
			for(int i = 0; i < 9; i++)
			{
				for(int j = 0; j < 7; j++)
				{
					cout << board[i][j] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
};

int main()
{
	ios::sync_with_stdio();
	timeval czas;
    gettimeofday(&czas, 0);
    srand(czas.tv_sec * 1000 + czas.tv_usec % 1000);
	int cnt1 = 0, cnt2 = 0, cnt3 = 0;
	int q = 100;
	while(q--)
	{
		vector<string> v;
		Game Jungle = Game(v, INF, "actual game", 0);
		int pl = 1;

		while(true)
		{
			Jungle.move(pl); // jedynki to duże, zera to małe
			pl = 1 - pl;
			if(Jungle.end())
			{
				int k = Jungle.win();
				if(k == 0)
					cnt2 ++;
				if(k == 1)
					cnt1++;
				if(k == -1)
					cnt3++;
				break;
			}
		}
	}
	cout << "Wygrane: " << cnt1 << " Przegrane: " << cnt2 << " Remis: " << cnt3 << endl;
}
