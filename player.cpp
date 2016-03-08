#include "player.h"



/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    
    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     board = new Board();
     this->side = side;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
     delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    Side otherSide = getOtherSide(side);
    board->doMove(opponentsMove, otherSide);
    int score;
    Move * move = doMinimax(board, side, 3, score);
    board->doMove(move, side);
    return move;
}

int Player::heuristic(Board * board, Side side)
{
    int score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board->getSide(side, i, j))
            {
                score += getPositionScore(i, j);
            }
            else if (board->getSide(getOtherSide(side), i , j))
            {
                score -= getPositionScore(i, j); 
            } 
        }
    }
    //score -= board->getPossibleMoves(getOtherSide(side)).size() / 2;
    return score;
}

int Player::getPositionScore(int x, int y)
{
    bool xEdge = (x == 0) || (x == 7);
    bool yEdge = (y == 0) || (y == 7);

    if (xEdge && yEdge)
    {
        //corner
        return 100;
    } 
    else if (xEdge || yEdge)
    {
        return 4;
    }

    return 3;
}

Side Player::getOtherSide(Side side)
{
    if (side == WHITE) return BLACK;
    else return WHITE;
}

void Player::setBoard(Board * board)
{
    if (this->board != NULL)
        delete this->board;
    this->board = board;
}

Move * Player::doMinimax(Board * currBoard, Side side, int depth, int & score)
{
    std::vector<Move> potentialMoves = currBoard->getPossibleMoves(side);
    if (depth == 0 || potentialMoves.size() == 0)
    {
        score = heuristic(currBoard, side);
        return NULL;
    }

    std::vector<int> scores;
    for (unsigned int i = 0; i < potentialMoves.size(); i++)
    {
        Board * newBoard = board->copy();
        newBoard->doMove(&potentialMoves[i], side);
        int currScore = 0;
        doMinimax(newBoard, getOtherSide(side), depth - 1, currScore);
        scores.push_back(-1*currScore);
        delete newBoard;
    }

    int maxIndex = 0;
    for (unsigned int i = 1; i < scores.size(); i++)
    {
        if(scores[i] > scores[maxIndex])
        {
            maxIndex = i;
        }
    }
    score = scores[maxIndex];
    return new Move(potentialMoves[maxIndex].getX(), potentialMoves[maxIndex].getY());

}
