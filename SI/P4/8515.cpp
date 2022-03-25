#include<bits/stdc++.h>
#include<sys/time.h>
#include <unistd.h>
using namespace std;
#define pb push_back
#define THROUGH_BOARD(i, j)	for(int i = 0; i < 9; i++)for(int j = 0; j < 7; j++)
#define FOURSIDES(i, j)	for(int i = -1; i <= 1; i++) for(int j = -1; j <= 1; j++) if(abs(i - j) == 1)

const int INF = 1e9;

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
		int me;
		string mode;

		char get(int xx, int yy, vector<string> &arr)
		{
			if(0 <= xx && xx < 9 && 0 <= yy && yy < 7)
				return arr[xx][yy];
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

		void move_piece(movement move, int player, vector<string> &arr, bool s)
		{
			THROUGH_BOARD(i, j)
			{
				if(arr[i][j] == move.animal)
				{
					arr[i][j] = raw_board[i][j];
					if(piece_of(arr[move.x][move.y], 1 - player))
					{
						char opponent = arr[move.x][move.y];
						char me = move.animal;
						if(raw_board[move.x][move.y] == '#' || beats[Id[me]][Id[opponent]])
						{
							arr[move.x][move.y] = move.animal;
							if(s)
								this->moves_without_beating = 0;
							return;
						}
						if(beats[Id[opponent]][Id[me]] && s)
						{
							this->moves_without_beating = 0;
							return;
						}
					}
					else
					{
						arr[move.x][move.y] = move.animal;
						if(s)
							this->moves_without_beating++;
					}
				}
			}
		}

		movement valid_move(int a, int b, movement m, int player, vector<string> &arr)
		{
			int xx = m.x + a, yy = m.y + b;
			if(get(xx, yy, arr) == '!')
				return {1, -1, -1};
			if(piece_of(get(xx, yy, arr), player))
				return {1, -1, -1};
			if(player == 1 && xx == 0 && yy == 3)
				return {1, -1, -1};
			if(player == 0 && xx == 8 && yy == 3)
				return {1, -1, -1};
			if(m.animal == 'T' || m.animal == 't' || m.animal == 'L' || m.animal == 'l')
			{
				if(get(xx, yy, raw_board) == '~')
				{
					while(get(xx, yy, raw_board) == '~')
						xx += a, yy += b;

					if(get(xx, yy, raw_board) == '!')
						return {1, -1, -1};
					else
						return valid_move(0, 0, {m.animal, xx, yy}, player, arr);
				}
			}
			else
				if(m.animal != 'R' && m.animal != 'r' && get(xx, yy, raw_board) == '~')
					return {1, -1, -1};

			return {m.animal, xx, yy};
		}
		vector<movement> gen_moves(vector<movement> &ToMove, int player, vector<string> &arr)
		{
			vector<movement> Ans;
			for(auto m: ToMove)
			{
				FOURSIDES(i, j)
				{
					movement k = valid_move(i, j, m, player, arr);
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
				else
					if(P1 > P2)
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

		Game simulation(vector<movement> &moves, int player, vector<string> &arr, int mv, movement m)
		{
			int pl = player;
			Game RandomGame = Game(arr, INF, "simulation", this->second, this->me); // wrzucenie zamiast INF
			RandomGame.total_moves = mv + 1;
			RandomGame.move_piece(m, pl, RandomGame.board, 1);
			pl = 1 - pl;
			while(true)
			{
				RandomGame.move(pl);
				pl = 1 - pl;
				if(RandomGame.end())
					return RandomGame;
			}
		}

		movement simulate(vector<movement> &moves, int player, vector<string> &arr, int mv)
		{
			movement best;
			int val = -INF;
			for(auto m: moves)
			{
				int cnt = 0;
				int n = 20000 / moves.size();
				while(n > 0)
				{
					Game RandomGame = simulation(moves, player, arr, mv,  m); // wrzucenie zamiast INF
					int c = RandomGame.winner();
					n -= RandomGame.total_moves;
					if(c == player)
						cnt++;
				}
				if(val < cnt)
				{
					val = cnt;
					best = m;
				}
			}
			return best;
		}
		int BFS(int x, int y, int pl, vector<string> &b)
		{
			int arr[9][7];
			char my_an = b[x][y];
			THROUGH_BOARD(i, j)
			{
				if((piece_of(b[i][j], 1 - pl) && beats[Id[b[i][j]]][Id[b[x][y]]]) || piece_of(b[i][j], pl))
					arr[i][j] = -1;
				else
					arr[i][j] = 1000;
			}
			arr[x][y] = 0;
			if(pl == 1)
				arr[0][3] = -1;
			else
				arr[8][3] = -1;
			queue<pair<int, int>> Q;
			Q.push(make_pair(x, y));
			while(!Q.empty())
			{
				pair<int, int> v = Q.front();
				Q.pop();
				FOURSIDES(i, j)
				{
					int new_x = v.first + i, new_y = v.second + j;
					if(pl == 1 && new_x == 0 && new_y == 3)
						continue;
					if(pl == 0 && new_x == 8 && new_y == 3)
						continue;
					if(get(new_x, new_y, raw_board) == '!')
						continue;
					if(my_an != 'T' && my_an != 'L' && my_an != 'R' && get(new_x, new_y, raw_board) == '~')
						continue;
					if(my_an != 't' && my_an != 'l' && my_an != 'r' && get(new_x, new_y, raw_board) == '~')
						continue;
					if(arr[new_x][new_y] <= arr[v.first][v.second] + 1)
						continue;
					if((my_an == 'T' || my_an == 't' || my_an == 'L' || my_an == 'l') && get(new_x, new_y, raw_board) == '~')
					{
						int ii = new_x, jj = new_y;
						while(get(ii, jj, raw_board) == '~')
							ii += i, jj += j;
						if(get(ii, jj, raw_board) != '!' && arr[ii][jj] > arr[v.first][v.second] + 1)
						{
							arr[ii][jj] = arr[v.first][v.second] + 1;
							Q.push(make_pair(ii, jj));
						}
					}
					else
					{
						arr[new_x][new_y] = arr[v.first][v.second] + 1;
						Q.push(make_pair(new_x, new_y));
					}
				}
			}
			if(pl == 1)
				return arr[8][3];
			else
				return arr[0][3];

		}
		double heuristic_value(int player, Game &AB, int k)
		{
			double p = 0, dist = 0;
			if(AB.end())
			{
				if(AB.winner() == k)
					return -INF + 1;
				else
					return INF - 1;
			}
			THROUGH_BOARD(i, j)
			{
				// if(piece_of(AB.board[i][j],  1 - second))
				// {
				// 	p += pow(2, Id[AB.board[i][j]]);
				// 	int odl = BFS(i, j,  1 - second, AB.board);
				//  	odl = min(odl, 20);
				// 	dist += pow(21 - odl, 2);
				// 	// if(debug or 1)
				// 	// {
				// 	// 	cerr << i << " " << j << " " << AB.board[i][j] << " " << odl << endl;
				// 	// }
				// }
				if(piece_of(AB.board[i][j], second))
				{
					p -= pow(2, Id[AB.board[i][j]]);
					int odl = BFS(i, j, second, AB.board);
					odl = min(odl, 20);
					dist -= pow(21 - odl, 2);
					// if(debug or 1)
					// {
					// 	cerr << i << " " << j << " " << AB.board[i][j] << " " << odl << endl;
					// }
				}
			}
			// if(debug)
			// 	cout << p << " " << dist << endl;
			double ans = p + dist * 10;
			return dist;
		}
		pair<movement, double> minimax(int player, int depth, Game AB, pair<movement, double> alfa, pair<movement, double> beta, int k)
		{
			vector<movement> ToGen;
			THROUGH_BOARD(i, j)
			{
				if(piece_of(AB.board[i][j], player))
					ToGen.pb({AB.board[i][j], i, j});
			}
			vector<movement> Avb = gen_moves(ToGen, player, AB.board);
			if(depth <= 0 || Avb.size() == 0)
			{
			//	cout << heuristic_value(player, arr) << endl;
				return {{1, 1, 1}, heuristic_value(player, AB, k)};
			}
			if(player != second)
			{
				double v = -INF;
				for(auto m: Avb)
				{
					Game New_AB = AB;
					New_AB.move_piece(m, player, New_AB.board, 0);
					auto val =  minimax(1 - player, depth - 1, New_AB, alfa, beta, k);
					if(val.second > v)
						v = val.second;
					if(v > alfa.second)
					{
						alfa.second = v;
						alfa.first = m;
					}
					if(beta.second <= alfa.second)
					{
						//cout << beta.second << " " << alfa.second << endl;
						break;
					}

				}
			//	cout << alfa.first.animal << " " << alfa.second << endl;
				return alfa;
			}
			else
			{
				double v = INF;
				for(auto m: Avb)
				{
					Game New_AB = AB;
					New_AB.move_piece(m, player, New_AB.board, 0);
					auto val =  minimax(1 - player, depth - 1, New_AB, alfa, beta, k);
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
		movement best_move(int player)
		{
			vector<movement> ToGen;
			THROUGH_BOARD(i, j)
			{
				if(piece_of(board[i][j], player))
					ToGen.pb({board[i][j], i, j});
			}
			vector<movement> Avb = gen_moves(ToGen, player, board);
			movement best;

			if(this->mode == "simulation" )//|| player == second)//|| player == second)
				best = Avb[rand() % Avb.size()];
			if(this->mode == "actual game" && player != second)
				best = simulate(Avb, player, board, total_moves);
			if(this->mode == "actual game" && player == second)
			{
				pair<movement, double> al = {{1,1,1}, -INF};
				pair<movement, double> be = {{1,1,1}, INF};
				auto v = minimax(player, 4, *this, al, be, me);
				best = v.first;
			}
			// if(this->mode == "actual game")
			// 	cout << best.animal << " " << best.x << " " << best.y << endl;
			return best;
		}

	public:
		Game(vector<string> arr, int v, string m, int sec, int k)
		{
			for(int i = 0; i < 8; i++)
				for(int j = 0; j < 8; j++)
					beats[i][j] = false;

			for(int i = 0; i < 8; i++)
				for(int j = i - 1; j >= 0; j--)
					beats[i][j] = true;

			this->max_moves = v;
			this->mode = m;
			this->second = sec;
			this->me = k;
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
			{
				return true;
			}

			int p1 = 0, p2 = 0;
			THROUGH_BOARD(i, j)
			{
				if(piece_of(board[i][j], 1))
					p1++;
				if(piece_of(board[i][j], 0))
					p2++;
			}

			if(p1 == 0 || p2 == 0)
			{
				return true;
			}
			return false;
		}
		void move(int player)
		{
			movement bm = this->best_move(player);
			this->move_piece(bm, player, board, 1);
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
	timeval czas;
    gettimeofday(&czas, 0);
    srand(czas.tv_sec * 1000 + czas.tv_usec % 1000);
	//srand(2);
	int cnt1 = 0, cnt2 = 0;
	int q = 20;
	while(q--)
	{
		vector<string> v;
		int pl = 1;
		Game Jungle = Game(v, INF, "actual game", 1 - pl, pl);
	//	cout << "NOWA GRA\n";
	//	Jungle.print_board();

		while(true)
		{
			Jungle.move(pl); // jedynki to duże, zera to małe
			pl = 1 - pl;
		//	Jungle.print_board();
			//exit(0);
			if(Jungle.end())
			{
				int k = Jungle.win();
				if(k == 1)
					cnt1 ++;
				else cnt2++;
				cout << Jungle.win() << endl;;
				break;
			}
			//usleep(900000);
		}
	}
	cout << cnt1 << " " << cnt2 << endl;
}
