#include "headers.h"

#define int64                   long long
#define bitCount(n)             __builtin_popcount(n)
#define MSET(a, v)              memset(a, v, sizeof a)

#define setBit(n, pos)          ( n | (1ll<<pos) )
#define resetBit(n, pos)        ( n & ~(1ll<<pos) )
#define isOn(n, pos)            ( ( n & (1ll<<pos) ) )

using namespace std;

class GameBoard {

public:
        int placedDisks;
        bool computerFirst;
        int64 wholeBoard, playerBoard[ 2 ];

        GameBoard( string nextPlayer ) {
                this->wholeBoard = 0;
                this->placedDisks = 0;
                MSET(this->playerBoard, 0);
                this->computerFirst = nextPlayer == "computer-next";
        }

        GameBoard( ifstream& inFile, string nextPlayer ) {

                this->wholeBoard = 0;
                this->placedDisks = 0;
                MSET(this->playerBoard, 0);
                this->computerFirst = false;

                for( int i = 0; i < 6; i ++ ) {
                        string row; inFile >> row;
                        for( int j = 0; j < 7; j ++ ) {
                                int x = row[ j ] - '0';
                                if( x == 0 ) continue;
                                if( x & 1 ) {
                                        this->playerBoard[ 0 ] = setBit( this->playerBoard[ 0 ], i * 7 + j );
                                        this->wholeBoard = setBit( this->wholeBoard, i * 7 + j );
                                }
                                else {
                                        this->playerBoard[ 1 ] = setBit( this->playerBoard[ 1 ], i * 7 + j );
                                        this->wholeBoard = setBit( this->wholeBoard, i * 7 + j );
                                }
                                if( x > 0 ) this->placedDisks ++;
                        }
                }

                int player; inFile >> player;
                if( nextPlayer == "computer-next" )
                        this->computerFirst = player % 2;
                if( nextPlayer == "human-next" )
                        this->computerFirst = !( player % 2 );
        }

        GameBoard( const GameBoard& parentBoard ) {
                this->placedDisks = parentBoard.placedDisks;
                this->wholeBoard  = parentBoard.wholeBoard;
                this->playerBoard[ 0 ] = parentBoard.playerBoard[ 0 ];
                this->playerBoard[ 1 ] = parentBoard.playerBoard[ 1 ];
        }

        int getScore( int player ) {
                int playerScore = 0;

                // Check Horizontally
                int playerIdx = player == 1 ? 0 : 1;
                for( int i = 0; i < 6; i ++ ) {
			for( int j = 0; j < 4; j ++ )  {
				if( isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j + 1 ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j + 2 ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j + 3 ) ) {
					playerScore++;
				}
			}
                }
                //cout << "---->> " << this->playerBoard[ player & 1 ] << endl;
                //cout << "====>> " << playerScore << endl;
                // Check Vertically
		for( int i = 0; i < 3; i ++ ) {
			for( int j = 0; j < 7; j ++ ) {
				if( isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 1 ) * 7 + j ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 2 ) * 7 + j ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 3 ) * 7 + j ) ) {
					playerScore++;
				}
			}
                }

                // Check Diagonally - backs lash
		for( int i = 0; i < 3; i ++ ){
			for( int j = 0; j < 4; j ++ ) {
				if( isOn( this->playerBoard[ playerIdx ], i * 7 + j ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 1 ) * 7 + j + 1 ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 2 ) * 7 + j + 2 ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 3 ) * 7 + j + 3 ) ) {
					playerScore++;
				}
			}
                }

                // Check Diagonally - forward slash
		for( int i = 0; i < 3; i ++ ){
			for( int j = 0; j < 4; j ++ ) {
				if( isOn( this->playerBoard[ playerIdx ], ( i + 3 ) * 7 + j ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 2 ) * 7 + j + 1 ) &&
                                    isOn( this->playerBoard[ playerIdx ], ( i + 1 ) * 7 + j + 2 ) &&
                                    isOn( this->playerBoard[ playerIdx ], i * 7 + j + 3 ) ) {
					playerScore++;
				}
			}
                }
                return playerScore;
        }

        int possibleFours( int player ) {
                int playerScore = 0, x1, x2;
                int playerIdx = player == 1 ? 0 : 1;

                // Check Horizontally
                for( int i = 0; i < 6; i ++ ) {
                        for( int j = 0; j < 4; j ++ )  {
                                x1 =  isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j + 1 );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j + 2 );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j + 3 );

                                x2 =  isOn( this->wholeBoard, ( i * 7 ) + j );
                                x2 += isOn( this->wholeBoard, ( i * 7 ) + j + 1 );
                                x2 += isOn( this->wholeBoard, ( i * 7 ) + j + 2 );
                                x2 += isOn( this->wholeBoard, ( i * 7 ) + j + 3 );

                                if( x1 == 3 && x2 == 3 ) playerScore ++;
                                x1 = x2 = 0;
                        }
                }
                //cout << "---->> " << this->playerBoard[ player & 1 ] << endl;
                //cout << "====>> " << playerScore << endl;
                // Check Vertically
                x1 = x2 = 0;

                for( int i = 0; i < 3; i ++ ) {
                        for( int j = 0; j < 7; j ++ ) {
                                x1 =  isOn( this->playerBoard[ playerIdx ], ( i * 7 ) + j );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 1 ) * 7 + j );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 2 ) * 7 + j );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 3 ) * 7 + j );

                                x2 =  isOn( this->wholeBoard, ( i * 7 ) + j );
                                x2 += isOn( this->wholeBoard, ( i + 1 ) * 7 + j );
                                x2 += isOn( this->wholeBoard, ( i + 2 ) * 7 + j );
                                x2 += isOn( this->wholeBoard, ( i + 3 ) * 7 + j );

                                if( x1 == 3 && x2 == 3 ) playerScore ++;
                                x1 = x2 = 0;
                        }
                }

                x1 = x2 = 0;
                // Check Diagonally - backs lash
                for( int i = 0; i < 3; i ++ ){
                        for( int j = 0; j < 4; j ++ ) {
                                x1 =  isOn( this->playerBoard[ playerIdx ], i * 7 + j );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 1 ) * 7 + j + 1 );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 2 ) * 7 + j + 2 );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 3 ) * 7 + j + 3 );

                                x2 =  isOn( this->wholeBoard, i * 7 + j );
                                x2 += isOn( this->wholeBoard, ( i + 1 ) * 7 + j + 1 );
                                x2 += isOn( this->wholeBoard, ( i + 2 ) * 7 + j + 2 );
                                x2 += isOn( this->wholeBoard, ( i + 3 ) * 7 + j + 3 );

                                if( x1 == 3 && x2 == 3 ) playerScore ++;
                                x1 = x2 = 0;
                        }
                }

                x1 = x2 = 0;
                // Check Diagonally - forward slash
                for( int i = 0; i < 3; i ++ ){
                        for( int j = 0; j < 4; j ++ ) {
                                x1 =  isOn( this->playerBoard[ playerIdx ], ( i + 3 ) * 7 + j );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 2 ) * 7 + j + 1 );
                                x1 += isOn( this->playerBoard[ playerIdx ], ( i + 1 ) * 7 + j + 2 );
                                x1 += isOn( this->playerBoard[ playerIdx ], i * 7 + j + 3 );

                                x2 =  isOn( this->wholeBoard, ( i + 3 ) * 7 + j );
                                x2 += isOn( this->wholeBoard, ( i + 2 ) * 7 + j + 1 );
                                x2 += isOn( this->wholeBoard, ( i + 1 ) * 7 + j + 2 );
                                x2 += isOn( this->wholeBoard, i * 7 + j + 3 );

                                if( x1 == 3 && x2 == 3 ) playerScore ++;
                        }
                }

                return playerScore;
        }

        int getCurrentPlayer( ) {
                return ( this->placedDisks % 2 ) + 1;
        }

        GameBoard GetGameBoard( ) {
                return (*this);
        }

        bool isValidMove( int column ) {
		if ( !( column >= 0 && column < 7 ) ) return false;
		else if( isOn( this->wholeBoard, column ) ) {
                        //cout << "*******************" << endl;
                        //(*this).PrintWholeBoard( );
                        //cout << "*******************" << endl;
                        return false;

                }
		else    return true;
        }

        bool playPiece( int column ) {
                if( !isValidMove( column ) ) return false;
		else {
			for( int i = 5; i >= 0; i -- ) {
				if( !isOn( this->wholeBoard, i * 7 + column ) ) {
                                        int player = getCurrentPlayer( ) == 1 ? 0 : 1;
                                        this->wholeBoard = setBit( this->wholeBoard, i * 7 + column );
                                        this->playerBoard[ player ] = setBit( this->playerBoard[ player ], i * 7 + column );
                                        this->placedDisks ++;
                                        return true;
				}
			}
                        //cout << "From here ! " << endl;
			return false;
		}
        }

        void setComputerFirst( bool val ) {
                this->computerFirst = val;
        }

        bool isComputerFirst( ) {
                return computerFirst;
        }

        void PrintWholeBoard( ) {
                for( int i = 0; i < 6; i ++ ) {
                        for( int j = 0; j < 7; j ++ ) {
                                if( isOn( this->playerBoard[ 0 ], i * 7 + j ) )
                                        cout << 1 << ' ';
                                else if( isOn( this->playerBoard[ 1 ], i * 7 + j ) )
                                        cout << 2 << ' ';
                                else
                                        cout << 0 << ' ';
                        } cout << "\n";
                }
        }

        void writeOnFile( ofstream& out ) {
                for( int i = 0; i < 6; i ++ ) {
                        for( int j = 0; j < 7; j ++ ) {
                                if( isOn( this->playerBoard[ 0 ], i * 7 + j ) )
                                        out << 1;
                                else if( isOn( this->playerBoard[ 1 ], i * 7 + j ) )
                                        out << 2;
                                else
                                        out << 0;
                        } out << "\n";
                }
                out << this->getCurrentPlayer( ) << endl;
        }

        void PrintFirstPlayersBoard( ) {
                for( int i = 0; i < 6; i ++ ) {
                        for( int j = 0; j < 7; j ++ )
                                cout << isOn( this->playerBoard[ 0 ], i * 7 + j ) << ' ';
                        cout << endl;
                }
        }

        void PrintSecondPlayersBoard( ) {
                for( int i = 0; i < 6; i ++ ) {
                        for( int j = 0; j < 7; j ++ )
                                cout << isOn( this->playerBoard[ 1 ], i * 7 + j ) << ' ';
                        cout << endl;
                }
        }
};
