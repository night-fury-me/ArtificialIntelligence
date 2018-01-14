#include "headers.h"

using namespace std;

#define INF             (1<<28)

class AIPlayer {

public:
        int intelligenceLevel;

        AIPlayer( ) { }

        AIPlayer( int intelligenceLevel = 5 ) {
                this->intelligenceLevel = intelligenceLevel;
        }

        void setIntelligenceLevel( int intelligenceLevel ) {
                this->intelligenceLevel = intelligenceLevel;
        }

        int evaluation( GameBoard& currentGame ) {
                int alreadyConnectedDif = currentGame.getScore( 2 ) - currentGame.getScore( 1 );
                int possibleConnectedDif = currentGame.possibleFours( 2 ) - currentGame.possibleFours( 1 );
                return ( alreadyConnectedDif + possibleConnectedDif );
        }

        int MAX_VALUE( GameBoard currentGame, int Alpha, int Beta, int intelligenceLevel ) {
                if( intelligenceLevel <= 0 || currentGame.placedDisks == 42 )
                        return evaluation( currentGame );
                int utility = -INF;
                for( int i = 0; i < 7; i ++ ) {
                        if( !currentGame.isValidMove( i ) ) continue;
                        currentGame.playPiece( i );
                        utility = max( utility, MIN_VALUE( currentGame, Alpha, Beta, intelligenceLevel - 1 ) );
                        if( utility >= Beta )
                                return utility;
                        Alpha = max( Alpha, utility );
                }
                return utility;
        }

        int MIN_VALUE( GameBoard currentGame, int Alpha, int Beta, int intelligenceLevel ) {
                if( intelligenceLevel <= 0 || currentGame.placedDisks == 42 )
                        return evaluation( currentGame );
                int utility = INF;
                for( int i = 0; i < 7; i ++ ) {
                        if( !currentGame.isValidMove( i ) ) continue;
                        currentGame.playPiece( i );
                        utility = min( utility, MAX_VALUE( currentGame, Alpha, Beta, intelligenceLevel - 1 ) );
                        if( utility <= Alpha )
                                return utility;
                        Beta = min( Beta, utility );
                }
                return utility;
        }

        int AlphaBeta( GameBoard currentGame, int column, int player ) {
                currentGame.playPiece( column );
                if( player == 1 )
                        return MIN_VALUE( currentGame, -INF, INF, this->intelligenceLevel );
                else
                        return MAX_VALUE( currentGame, -INF, INF, this->intelligenceLevel );
        }

        int findBestMove( GameBoard currentGame ) {
                int selectedMove = 0, player = currentGame.getCurrentPlayer( );
                int utility = player == 1 ? INF : -INF, tempUtility;

                for( int i = 0; i < 7; i ++ ) {
                        if( !currentGame.isValidMove( i ) ) continue;
                        tempUtility = AlphaBeta( currentGame, i, player );
                        if( player == 1 && tempUtility < utility ) {
                                utility = tempUtility;
                                selectedMove = i;
                        }
                        else if( player == 2 && tempUtility > utility ){
                                utility = tempUtility;
                                selectedMove = i;
                        }
                }

                return selectedMove;
        }
};
