#ifndef chessboard
#define chessboard

#include <SDL2/SDL.h>

// Tile values for the chess board
const Uint8 EMPTY = 0x00;

const Uint8 WHITE_PAWN = 0x10;
const Uint8 WHITE_KNIGHT = 0x11;
const Uint8 WHITE_BISHOP = 0x12;
const Uint8 WHITE_ROOK = 0x13;
const Uint8 WHITE_QUEEN = 0x14;
const Uint8 WHITE_KING = 0x15;

const Uint8 BLACK_PAWN = 0x20;
const Uint8 BLACK_KNIGHT = 0x21;
const Uint8 BLACK_BISHOP = 0x22;
const Uint8 BLACK_ROOK = 0x23;
const Uint8 BLACK_QUEEN = 0x24;
const Uint8 BLACK_KING = 0x25;

const bool WHITE = true;
const bool BLACK = false;

class Board {
    public:

        // Instance variables
        Uint8* boardState;
        bool* pawnsMoved;
        bool turn;
        int turnCount;

        Board(bool createEmptyBoard = 0) {

            this->boardState = new Uint8[64];
            this->pawnsMoved = new bool[16]; // 0-7 white, 8-15 black
            this->turn = WHITE;
            this->turnCount = 1;

            if (createEmptyBoard) {
                return;
            }

            /* ---- Fill board with pieces ---- */

            // Fill empty spaces with EMPTY
            for (int i = 0; i < 8; i++) {
                for (int j = 2; j < 6; j++) {
                    this->setTile(i, j, EMPTY);
                }
            }

            // Pawns
            for (int i = 0; i < 8; i++) { // loop runs for [0, 1, 2, 3, 4, 5, 6, 7]
                this->setTile(i, 1, BLACK_PAWN);
                this->setTile(i, 6, WHITE_PAWN);

                // Fill pawns moved array
                this->pawnsMoved[i] = false;  // covers first half ( white pieces )
                this->pawnsMoved[i + 8] = false;  // covers second half ( black pieces )
            }

            // Rooks
            for (int i = 0; i < 8; i += 7) { // loop runs for [0, 7]
                this->setTile(i, 0, BLACK_ROOK);
                this->setTile(i, 7, WHITE_ROOK);
            }

            // Knights
            for (int i = 1; i < 7; i += 5) { // loop runs for [1, 6]
                this->setTile(i, 0, BLACK_KNIGHT);
                this->setTile(i, 7, WHITE_KNIGHT);
            }

            // Bishops
            for (int i = 2; i < 6; i += 3) { // loop runs for [2, 5]
                this->setTile(i, 0, BLACK_BISHOP);
                this->setTile(i, 7, WHITE_BISHOP);
            }

            // Queens
            this->setTile(3, 0, BLACK_QUEEN);
            this->setTile(3, 7, WHITE_QUEEN);

            // Kings
            this->setTile(4, 0, BLACK_KING);
            this->setTile(4, 7, WHITE_KING);

        }

        // Destructor
        ~Board() {
            delete[] this->boardState;
            delete[] this->pawnsMoved;
        }

        // Class functions
        static bool isEmpty(Uint8 piece) {
            return (piece >> 4) == 0;  // Left value in the byte is 0
        }

        static bool isWhite(Uint8 piece) {
            return (piece >> 4) == 1;  // Left value in the byte is 1
        }

        static bool isBlack(Uint8 piece) {
            return (piece >> 4) == 2;  // Left value in the byte is 2
        }

        static bool sameColor(Uint8 piece1, Uint8 piece2) {
            return (piece1 >> 4) == (piece2 >> 4);
        }

        // Instance functions
        void changeTurn() {
            this->turn = !(this->turn);
            if (this->turn == WHITE) this->turnCount++;
        }

        bool isEmpty(int x, int y) {
            return isEmpty(this->getTile(x, y));
        }

        bool isWhite(int x, int y) {
            return isWhite(this->getTile(x, y));
        }

        bool isBlack(int x, int y) {
            return isBlack(this->getTile(x, y));
        }

        void setTile(int x, int y, Uint8 piece) {
            if (x > 7 || y > 7) return;
            this->boardState[ (y * 8) + x ] = piece;
        }

        Uint8 getTile(int x, int y) {
            if (x > 7 || y > 7) return -1;
            return this->boardState[ (y * 8) + x ];
        }

        void move(int x1, int y1, int x2, int y2) {
            Uint8 piece = this->getTile(x1, y1);
            this->setTile(x1, y1, EMPTY);
            this->setTile(x2, y2, piece);
        }

        bool canMove(int x1, int y1, int x2, int y2) {

            if (x1 > 7 || y1 > 7 || x2 > 7 || y2 > 7) return false;

            Uint8 piece1 = this->getTile(x1, y1);
            Uint8 piece2 = this->getTile(x2, y2);

            if (sameColor(piece1, piece2)) return false; // same color
            if (isEmpty(piece1)) return false;

            int distx = abs(x2 - x1);
            int distyReal = (y2 - y1);
            int disty = abs(distyReal);

            // Used for checking tiles passed during the move are empty
            int incrementX = 1;
            int incrementY = 1;
            if (x2 < x1) incrementX = -1;
            else if (x2 == x1) incrementX = 0;
            if (y2 < y1) incrementY = -1;
            else if (y2 == y1) incrementY = 0;
            int iX = x1 + incrementX; // Values used in the loops
            int iY = y1 + incrementY; //

            // std::cout<< x1 << " " << y1 << " -> " << x2 << " " << y2 << '\n';
            // std::cout<< iX << " " << iY << " + " << incrementX << " " << incrementY << '\n';

            switch (piece1) {
                case WHITE_PAWN:
                    
                    // Capturing
                    if (distx != 0) {
                        if ((distyReal != -1) || (distx != 1)) return false;
                        return (isBlack(piece2));
                    }
                    
                    // Moving
                    if (!isEmpty(piece2)) return false;

                    if (this->pawnsMoved[x1] || y1 != 6) {
                        return (distyReal == -1);
                    } else {
                        if (distyReal == -2) { // need to check the tile in between the start and end locations
                            if (!this->isEmpty(x1, y1 - 1)) return false;
                        } else if (distyReal != -1) { // any distyReal other than 2 and 1 will hit this
                            return false;
                        }

                        this->pawnsMoved[x1] = true;
                        return true;
                    }

                    return false;

                case BLACK_PAWN:
                    
                    // Capturing
                    if (distx != 0) {
                        if ((distyReal != 1) || (distx != 1)) return false;
                        return (isWhite(piece2));
                    }

                    // Moving
                    if (!isEmpty(piece2)) return false;

                    if (this->pawnsMoved[8 + x1] || y1 != 1) {
                        return (distyReal == 1); // moving down 1
                    } else {
                        if (distyReal == 2) { // need to check the tile in between the start and end locations
                            if (!this->isEmpty(x1, y1 + 1)) return false;
                        } else if (distyReal != 1) { // any distyReal other than 2 and 1 will hit this
                            return false;
                        }

                        this->pawnsMoved[8 + x1] = true;
                        return true;
                    }

                    return false;

                case WHITE_KNIGHT:
                case BLACK_KNIGHT:
                    // Must have either distance (2, 1) or (1, 2) with abs values
                    // Knights can jump pieces so this is all thats needed
                    return (((distx == 1) && (disty == 2)) || ((distx == 2) && (disty == 1)));

                case WHITE_BISHOP:
                case BLACK_BISHOP:
                    // Same distance in both directions
                    if (distx != disty) return false;
                    for (; iX != (x2) || iY != (y2);) { // check all tiles on the path from start to end
                        if (!isEmpty(iX, iY)) return false;
                        iX += incrementX;
                        iY += incrementY;
                    }
                    return true;

                case WHITE_ROOK:
                case BLACK_ROOK:
                    // One of the directions must be 0
                    if ((distx != 0) && (disty != 0)) return false;
                    for (; iX != (x2) || iY != (y2);) { // check all tiles on the path from start to end
                        if (!isEmpty(iX, iY)) return false;
                        iX += incrementX;
                        iY += incrementY;
                    }
                    return true;

                case WHITE_QUEEN:
                case BLACK_QUEEN:
                    // Rook move or bishop move
                    if (((distx != 0) && (disty != 0)) && (distx != disty)) return false;
                    for (; iX != (x2) || iY != (y2);) { // check all tiles on the path from start to end
                        if (!isEmpty(iX, iY)) return false;
                        iX += incrementX;
                        iY += incrementY;
                    }
                    return true;

                case WHITE_KING:
                case BLACK_KING:
                    // Just cant move more than a distance of (1, 1)
                    // This literally cant jump pieces so no more handling needed
                    return ((distx <= 1) && (disty <= 1));

            }

            return false;

        }

    private:
};

#endif