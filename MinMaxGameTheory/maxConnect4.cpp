#include "headers.h"
#include "GameBoard.h"
#include "AIPlayer.h"

using namespace std;

int human, computer;
AIPlayer aiPlayer( 5 );

void aiTurn( GameBoard currentGame );
void humanTurn( GameBoard currentGame );

void showGameStatus( GameBoard currentGame, int& p1, int& p2 ) {

        cout << "---------------------------\n";
        cout << "Current Board State:\n";

        cout << "::Human    = " << human << endl;
        cout << "::Computer = " << computer << "\n" << endl;

        cout << "0 1 2 3 4 5 6\n";
        cout << "--------------\n";
        currentGame.PrintWholeBoard( );
        cout << "--------------\n";
        p1 = currentGame.getScore( 1 );
        p2 = currentGame.getScore( 2 );
        cout << "Score :: " << p1 << " - " << p2 << endl;
        cout << "---------------------------\n";
}

void finalResult( GameBoard currentGame ) {
        int player1Score, player2Score;
        showGameStatus( currentGame, player1Score, player2Score );
        if( player1Score > player2Score ) {
                cout << "Player one is the winner !" << endl;
        }
        else if( player1Score < player2Score ) {
                cout << "Player two is the winner !" << endl;
        }
        else {
                cout << "Game is Tie !" << endl;
        }
}

void humanTurn( GameBoard currentGame ) {
        if( currentGame.placedDisks == 42 ) {
                finalResult( currentGame);
                ofstream humanFile("human.txt");
                currentGame.writeOnFile( humanFile );
                humanFile.close( );
                return;
        }

        int p1, p2;
        showGameStatus( currentGame, p1, p2 );

        cout << "Enter your move :: ";
        int move; cin >> move;

        if( !currentGame.isValidMove( move ) ) {
                cout << "Please Enter a valid move...." << endl;
                humanTurn( currentGame );
        }

        currentGame.playPiece( move );

        ofstream humanFile("human.txt");
        currentGame.writeOnFile( humanFile );
        humanFile.close( );
        aiTurn( currentGame );
}

void aiTurn( GameBoard currentGame ) {
        if( currentGame.placedDisks == 42 ) {
                finalResult( currentGame );
                ofstream computerFile("computer.txt");
                currentGame.writeOnFile( computerFile );
                computerFile.close( );
                return;
        }

        int p1, p2;
        showGameStatus( currentGame, p1, p2 );
        int move = aiPlayer.findBestMove( currentGame );

        //if( aiPlayer.intelligenceLevel < 8 )
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        if( currentGame.playPiece( move ) ) {
                cout << "Computer Placed Disk At : " << move << " !" << endl;
        }

        ofstream computerFile("computer.txt");
        currentGame.writeOnFile( computerFile );
        computerFile.close( );
        humanTurn( currentGame );
}

void aiTurn( GameBoard currentGame, ofstream& outFile ) {

        if( currentGame.placedDisks == 42 ) {
                finalResult( currentGame );
                currentGame.writeOnFile( outFile );
                outFile.close( );
                return;
        }

        int p1, p2;
        showGameStatus( currentGame, p1, p2 );
        int move = aiPlayer.findBestMove( currentGame );

        //if( aiPlayer.intelligenceLevel < 8 )
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

        currentGame.playPiece( move );
        showGameStatus( currentGame, p1, p2 );

        currentGame.writeOnFile( outFile );
        outFile.close( );
}

void setPlayer( GameBoard& game, string nextPlayer ) {
        if( nextPlayer == "computer-next" ) {
                human = game.getCurrentPlayer() == 1 ? 2 : 1;
                computer = game.getCurrentPlayer();
        }
        else {
                human = game.getCurrentPlayer();
                computer = game.getCurrentPlayer() == 1 ? 2 : 1;
        }
}

int main( int argc, char *argv[ ] ) {

        string cmd[ 6 ];
        int intelligenceLevel;


        //{"maxconnect4" ,"interactive" ,"red_next.txt", "human-next" ,"5"};

        cout << ":::Welcome to MaxConnect4:::" << endl;

        if( argc != 5 ) {
                cout << "Four command-line arguments are needed:" << endl;
                cout << "Usage: " << cmd[ 0 ] << " interactive [input_file] [computer-next/human-next] [depth]" << endl;
                cout << "or:  " << cmd[ 0 ] << " one-move [input_file] [output_file] [depth]" << endl;
                return 0;
        }

        cmd[ 0 ] = argv[ 0 ], cmd[ 1 ] = argv[ 1 ];
        cmd[ 2 ] = argv[ 2 ], cmd[ 3 ] = argv[ 3 ], cmd[ 4 ] = argv[ 4 ];

        string gameMode = cmd[ 1 ];

        if( gameMode == "interactive" ) {

                string nextPlayer = cmd[ 3 ];
                string inputFile  = cmd[ 2 ];

                stringstream ss( cmd[ 4 ] );
                ss >> intelligenceLevel;

                ifstream inFile( inputFile );

                if( inFile.is_open( ) ) {
                        GameBoard game( inFile, nextPlayer );
                        setPlayer( game, nextPlayer );
                        aiPlayer.setIntelligenceLevel( intelligenceLevel );

                        if( nextPlayer == "computer-next" )
                                aiTurn( game );
                        else
                                humanTurn( game );
                }
                else {
                        GameBoard game( nextPlayer );
                        setPlayer( game, nextPlayer );
                        aiPlayer.setIntelligenceLevel( intelligenceLevel );

                        if( nextPlayer == "computer-next" )
                                aiTurn( game );
                        else
                                humanTurn( game );
                }

        }
        else if( gameMode == "one-move" ) {
                string inputFile  = cmd[ 2 ];
                string outputFile = cmd[ 3 ];

                ifstream inFile( inputFile );
                ofstream outFile( outputFile );

                stringstream ss( cmd[ 4 ] );
                ss >> intelligenceLevel;

                aiPlayer.setIntelligenceLevel( intelligenceLevel );

                if( inFile.is_open( ) ) {
                        GameBoard game( inFile, "computer-next" );
                        setPlayer( game, "computer-next" );
                        game.PrintWholeBoard( );
                        aiTurn( game, outFile );
                }
                else {
                        cout << "ERROR :: Could not find your specified file!" << endl;
                }
        }


        return 0;
}
