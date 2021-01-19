#include <iostream>
#include <array>
#include <set>
#include <chrono>

using namespace std;

#define SIZE 8
#define LC (SIZE/2)-1
#define UC (SIZE/2)


int32_t total = 0;


class Board {

    private:
        void initializeGrid() {
            board[LC][LC] = 1;
            board[UC][UC] = 1;
            board[LC][UC] = -1;
            board[UC][LC] = -1;
        }

        bool checkSquare(int x, int y, bool turn) {
            if (0 > x || SIZE-1 < x || 0 > y || SIZE-1 < y)
                return false;
            
            int target = (turn ? -1 : 1);
            int adj = (turn ? 1 : -1); 

            // check horizontal before
            if (x > 1 && board[y][x-1] == adj) {
                for (int i = x-2; i > -1; i--) {
                    if (board[y][i] == 0) break;
                    if (board[y][i] == target) return true;
                }
            }
            // check horizontal after
            if (x < SIZE-2 && board[y][x+1] == adj) {
                for (int i = x+2; i < SIZE; i++) {
                    if (board[y][i] == 0) break;
                    if (board[y][i] == target) return true;                    
                }
            }

            // check vertical before
            if (y > 1 && board[y-1][x] == adj) {
                for (int i = y-2; i > -1; i--) {
                    if (board[i][x] == 0) break;
                    if (board[i][x] == target) return true;
                }
            }

            // check vertical after
            if (y < SIZE-2 && board[y+1][x] == adj) {
                for (int i = y+2; i < SIZE; i++) {
                    if (board[i][x] == 0) break;
                    if (board[i][x] == target) return true;
                }
            }

            // check diagonal upper left;
            if (x > 1 && y > 1 && board[y-1][x-1] == adj) {
                int currentX = x-2;
                int currentY = y-2;
                while (currentX > -1 && currentY > -1) {
                    if (board[currentY][currentX] == 0) break;
                    if (board[currentY][currentX] == target) return true;
                    currentX--;
                    currentY--;
                }
            }

            // check diagonal upper right;
            if (x < SIZE-2 && y > 1 && board[y-1][x+1] == adj) {
                int currentX = x+2;
                int currentY = y-2;
                while (currentX < SIZE && currentY > -1) {
                    if (board[currentY][currentX] == 0) break;
                    if (board[currentY][currentX] == target) return true;
                    currentX++;
                    currentY--;
                }
            }

            // check diagonal bottom left;
            if (x > 1 && y < SIZE-2 && board[y+1][x-1] == adj) {
                int currentX = x-2;
                int currentY = y+2;
                while (currentX > -1 && currentY < SIZE) {
                    if (board[currentY][currentX] == 0) break;
                    if (board[currentY][currentX] == target) return true;
                    currentX--;
                    currentY++;
                }
            }

            // check diagonal bottom right;
            if (x < SIZE-2 && y < SIZE-2 && board[y+1][x+1] == adj) {
                int currentX = x+2;
                int currentY = y+2;
                while (currentX < SIZE && currentY < SIZE) {
                    if (board[currentY][currentX] == 0) break;
                    if (board[currentY][currentX] == target) return true;
                    currentX++;
                    currentY++;
                }
            }


            return false;
        }

    public:
        array<array<int, SIZE>, SIZE> board = {0};
        array<pair<int,int>, 8> neighbors = {make_pair(-1,-1),make_pair(-1,0),make_pair(-1,1),make_pair(0,-1),make_pair(0,1),make_pair(1,-1),make_pair(1,0),make_pair(1,1)};


        Board() {initializeGrid();}

        Board(array<array<int, SIZE>, SIZE> theBoard) {
            board = theBoard;
        }

        void displayBoard() {
            cout << string(2*SIZE+3,'-') << endl;
            for (int i = 0; i < SIZE; i++) {
                cout << "| ";
                for (int j = 0; j < SIZE; j++) {
                    if (board[i][j] == -1) {
                        cout << "B";
                    } else if (board[i][j] == 1) {
                        cout << "W";
                    } else {
                        cout << " ";
                    }
                    cout << " ";
                }
                cout << "|" << endl;
            }
            cout << string(2*SIZE+3,'-') << endl;
        }

        void displayPossibleMoves(bool turn) {
            set<pair<int,int>> valid = getValidMoves(turn);
            set<pair<int,int>>::const_iterator itr;

            for (itr = valid.begin(); itr != valid.end(); itr++) {
                pair<int, int> square = *itr;

                cout << "(" << square.first << ", " << square.second << ") ";
            }
            cout << endl;
        }

        void displayScore() {
            int blackScore = 0;
            int whiteScore = 0;
            
            for (int y = 0; y < SIZE; y++) {
                for (int x = 0; x < SIZE; x++) {
                    if (board[y][x] == 1) 
                        whiteScore++;
                    else if (board[y][x] == -1)
                        blackScore++;
                }
            }

            if (blackScore > whiteScore)
                cout << "Black wins! " << endl << blackScore << " - " << whiteScore << endl;
            else if (whiteScore > blackScore)
                cout << "White wins! " << endl << whiteScore << " - " << blackScore << endl;
            else
                cout << "Tie!" << endl << blackScore << " - " << whiteScore << endl;
        }

        int getScore() {
            int score = 0;
            for (int y = 0; y < SIZE; y++)
                for (int x = 0; x < SIZE; x++)
                    score -= board[y][x]; // -= because black is the player trying to maximize score
            return score;
        }

        bool isFull() {
            for (int i = 0; i < SIZE; i++) 
                for (int j = 0; j < SIZE; j++)
                    if (board[i][j] == 0)
                        return false;
            return true;
        }

        set<pair<int,int>> getValidMoves(bool turn) {
            
            set<pair<int, int>> possible;
            for (int y = 0; y < SIZE; y++) {
                for (int x = 0; x < SIZE; x++) {
                    if (board[y][x] != 0) {
                        for (pair<int,int> neighbor : neighbors) {                        
                            int xVal = max(min(SIZE-1,x+neighbor.first),0);
                            int yVal = max(min(SIZE-1,y+neighbor.second),0);
                            if (board[yVal][xVal] == 0 && checkSquare(xVal,yVal,turn)) {
                                // cout << xVal << " " << yVal << endl;
                                pair<int, int> square(xVal,yVal);
                                possible.insert(square);
                            }
                        }
                    } 
                }
            }


            return possible;
        }

        void makeMove(int x, int y, bool turn) {

            if (!checkSquare(x,y,turn)) {
                cout << "Invalid move\n";
                return;
            }

            int target = (turn ? -1 : 1);
            int adj = (turn ? 1 : -1); 

            // change horizontal before
            if (x > 1 && board[y][x-1] == adj) {
                for (int i = x-2; i > -1; i--) {
                    if (board[y][i] == target) {
                        for (int j = i; j < x; j++) {
                            board[y][j] = target;
                        }
                        break;
                    }
                }
            }
            // change horizontal after
            if (x < SIZE-2 && board[y][x+1] == adj) {
                for (int i = x+2; i < SIZE; i++) {
                    if (board[y][i] == target) {
                        for (int j = i; j > x; j--) {
                            board[y][j] = target;
                        }
                        break;
                    }
                               
                }
            }

            // change vertical before
            if (y > 1 && board[y-1][x] == adj) {
                for (int i = y-2; i > -1; i--) {
                    if (board[i][x] == target) {
                        for (int j = i; j < y; j++) {
                            board[j][x] = target;
                        }
                        break;
                    }
                }
            }

            // change vertical after
            if (y < SIZE-2 && board[y+1][x] == adj) {
                for (int i = y+2; i < SIZE; i++) {
                    if (board[i][x] == target)  {
                        for (int j = i; j > y; j--) {
                            board[j][x] = target;
                        }
                        break;
                    }
                }
            }

            // change diagonal upper left;
            if (x > 1 && y > 1 && board[y-1][x-1] == adj) {
                for (int i = 2; i < min(x,y); i++) {
                    if (board[y-i][x-i] == target) {
                        for (int j = i; j > -1; j--) {
                            board[y-j][x-j] = target;
                        }
                        break;
                    }
                }
            }


            // change diagonal upper right;
            if (x < SIZE-2 && y > 1 && board[y-1][x+1] == adj) {
                for (int i = 2; i < min(SIZE-x, y); i++) {
                    if (board[y-i][x+i] == target) {
                        for (int j = i; j > -1; j--) {
                            board[y-j][x+j] = target;
                        }
                        break;
                    }
                }
            }

            // change diagonal bottom left;
            if (x > 1 && y < SIZE-2 && board[y+1][x-1] == adj) {
                for (int i = 2; i < min(x, SIZE-y); i++) {
                    if (board[y+i][x-i] == target) {
                        for (int j = i; j > -1; j--) {
                            board[y+j][x-j] = target;
                        }
                        break;
                    }
                }
            }

            // change diagonal bottom right;
            if (x < SIZE-2 && y < SIZE-2 && board[y+1][x+1] == adj) {
                for (int i = 2; i < min(SIZE-x, SIZE-y); i++) {
                    if (board[y+i][x+i] == target) {
                        for (int j = i; j > -1; j--) {
                            board[y+j][x+j] = target;
                        }
                        break;
                    }
                }
            }

            board[y][x] = (turn ? -1 : 1); 
        }        

};

int minimax(Board board, int depth, bool isMax, int alpha, int beta) {

    total++;

    if (board.getValidMoves(isMax).size() == 0 || depth == 0) {
        return board.getScore();
    }


    if (isMax) {
        int bestVal = -1000;

        set<pair<int,int>> possible = board.getValidMoves(true);


        for (pair<int,int> move : possible) {

            array<array<int, SIZE>, SIZE> copyBoard;

            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    copyBoard[i][j] = board.board[i][j];
            

            Board copy = Board(copyBoard);

            copy.makeMove(move.first,move.second,true);

            int value = minimax(copy, depth-1, false, alpha, beta);

            bestVal = max(bestVal,value);

            alpha = max(alpha, bestVal);

            if (beta <= alpha) {
                break;
            }
        }
        return bestVal;
    } else {
        int bestVal = 1000;

        set<pair<int,int>> possible = board.getValidMoves(false);

        for (pair<int,int> move : possible) {

            array<array<int, SIZE>, SIZE> copyBoard;

            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    copyBoard[i][j] = board.board[i][j];
            
            Board copy = Board(copyBoard);

            copy.makeMove(move.first,move.second,false);

            int value = minimax(copy, depth-1, true, alpha, beta);

            bestVal = min(bestVal,value);

            beta = min(beta, bestVal);

            if (beta <= alpha)
                break;
        }
        return bestVal;
    }

}


pair<int,int> findBestMove(Board board, bool turn) {

    int bestVal = (turn) ? -1000 : 1000;
    pair<int,int> bestMove;
    int depth;

    set<pair<int,int>> possible = board.getValidMoves(turn);

    cout << "Possible moves: " << possible.size() << endl;

    if (possible.size() < 6)
        depth = 8;
    else
        depth = 6;

    for (pair<int,int> move : possible) {
        array<array<int, SIZE>, SIZE> copyBoard;

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                copyBoard[i][j] = board.board[i][j];
        
        Board copy = Board(copyBoard);

        copy.makeMove(move.first,move.second,false);

        int moveVal = minimax(copy, depth, !turn, -1000,1000);

        if (turn && moveVal > bestVal) {
            bestVal = moveVal;
            bestMove.first = move.first;
            bestMove.second = move.second;
        } else if (!turn && moveVal < bestVal) {
            bestVal = moveVal;
            bestMove.first = move.first;
            bestMove.second = move.second;            
        }
    }
    cout << "Score estimate: " << bestVal << endl;
    return bestMove;

}


int main() {


    
    Board board;
    bool turn = true;

    while (!board.isFull()) {
        board.displayBoard();
        if (turn) {
            cout << "Black to move: " << endl;

            board.displayPossibleMoves(turn);

            int x, y;

            cin >> x >> y;

            board.makeMove(x,y, turn);
        } else {
            cout << "Computer move: " << endl;

            total = 0;

            uint64_t startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            pair<int,int> compMove = findBestMove(board, false);

            uint64_t endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            int64_t totalTime = endTime - startTime;
            
            int64_t bps = (total*1000) / totalTime;

            cout << "Move: " << compMove.first << " " << compMove.second << endl;
            cout << "Boards searched: " << total << endl;
            cout << "Total time: " << totalTime << endl;
            cout << "Boards per second: " << bps << endl;


            board.makeMove(compMove.first,compMove.second, false);
        }
        
        turn = !turn;
    }
    
    board.displayBoard();

    board.displayScore();

    return 1;
}