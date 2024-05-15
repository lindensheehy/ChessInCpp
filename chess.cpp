#include <iostream>
#include <SDL2/SDL.h>

#include "draw.h"
#include "colors.h"
#include "frame.h"
#include "chessboard.h"

Board* board;
Uint8 heldPiece;
int heldPieceX; // Stores the tile that the held piece came from
int heldPieceY; //

PNG* allPiecespng;

PNG* whitePawnpng;
PNG* whiteKnightpng;
PNG* whiteBishoppng;
PNG* whiteRookpng;
PNG* whiteQueenpng;
PNG* whiteKingpng;

PNG* blackPawnpng;
PNG* blackKnightpng;
PNG* blackBishoppng;
PNG* blackRookpng;
PNG* blackQueenpng;
PNG* blackKingpng;

PNG* piecepngs[12] = {
    whitePawnpng,
    whiteKnightpng,
    whiteBishoppng,
    whiteRookpng,
    whiteQueenpng,
    whiteKingpng,
    blackPawnpng,
    blackKnightpng,
    blackBishoppng,
    blackRookpng,
    blackQueenpng,
    blackKingpng
};

void initBoard() {
    board = new Board();

    const char* pngPath = "textures/pieces.png";
    allPiecespng = new PNG(pngPath);

    // Load white piece pngs
    piecepngs[0] = new PNG(allPiecespng, 1000, 0, 1200, 200);   // pawn
    piecepngs[1] = new PNG(allPiecespng, 600, 0, 800, 200);     // knight
    piecepngs[2] = new PNG(allPiecespng, 400, 0, 600, 200);     // bishop
    piecepngs[3] = new PNG(allPiecespng, 800, 0, 1000, 200);    // rook
    piecepngs[4] = new PNG(allPiecespng, 200, 0, 400, 200);     // queen
    piecepngs[5] = new PNG(allPiecespng, 0, 0, 200, 200);       // king

    // Load black piece pngs
    piecepngs[6] = new PNG(allPiecespng, 1000, 200, 1200, 400);   // pawn
    piecepngs[7] = new PNG(allPiecespng, 600, 200, 800, 400);   // knight
    piecepngs[8] = new PNG(allPiecespng, 400, 200, 600, 400);   // bishop
    piecepngs[9] = new PNG(allPiecespng, 800, 200, 1000, 400);   // rook
    piecepngs[10] = new PNG(allPiecespng, 200, 200, 400, 400);  // queen
    piecepngs[11] = new PNG(allPiecespng, 0, 200, 200, 400);  // king

}

void drawBoard(Drawer* drawer) {
    Uint32 color = Color::CHESS_WHITE;
    for (int i = 0; i < 1600; i += 200) {
        for (int j = 0; j < 1600; j += 200) {
            drawer->drawRect(color, i, j, i + 200, j + 200);
            color ^= 0x00FFFFFF; // flips all the last 24 bits
        }
        color ^= 0x00FFFFFF;
    }
}

void drawPiece(Drawer* drawer, Uint8 piece, int x, int y) {

    int index = 0;
    index += ( 6 * ((piece >> 4) - 1) );   // adds 6 if the piece is black, if the more significant 4 bits = 0010 = 2
    index += ( piece & 0xF );              // adds the 4 less significant bits using a bitwise and operator
    PNG* piecepng = piecepngs[index];

    drawer->drawpng(piecepng, x, y);

    return;
}

void drawPieces(Drawer* drawer, FrameState* frameState) {

    // Draw all pieces on the board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Uint8 piece = board->getTile(j, i);
            if (!Board::isEmpty(piece)) {
                drawPiece(drawer, piece, j * 200, i * 200);
            }
        }
    }

    // Draw held piece
    if (heldPiece != EMPTY) {
        
        // Draw image in the middle of the mouse
        int drawPosX = frameState->mouse->posX - 100;
        int drawPosY = frameState->mouse->posY - 100;

        // avoid attempting to draw off screen
        if (frameState->mouse->posX < 100) drawPosX = 0;
        if (frameState->mouse->posY < 100) drawPosY = 0;
        if (frameState->mouse->posX > 1499) drawPosX = 1398;
        if (frameState->mouse->posY > 1499) drawPosY = 1398;

        drawPiece(drawer, heldPiece, drawPosX, drawPosY);
    }

    return;
}

void startBoard() {
    delete board;
    board = new Board();
    return;
}

void findBoardPos(int* windowX, int* windowY) {
    int boardPosX = (int) ((*windowX) / 200);
    int boardPosY = (int) ((*windowY) / 200);
    *windowX = boardPosX;
    *windowY = boardPosY;
}

void getTileNotation(int tileX, int tileY, char* output1, char* output2) {
    if (tileX != -1) *output1 = (char) (97 + tileX);  // 97 is the char code for a and it goes alphabetically
    if (tileY != -1) *output2 = (char) (49 + tileY);  // 49 is the char code for 1 and it goes in order
}

void mouseDown(int tileX, int tileY) {
    heldPiece = board->getTile(tileX, tileY);
    heldPieceX = tileX;
    heldPieceY = tileY;
    board->setTile(tileX, tileY, EMPTY);
}

void printMove(int fromX, int fromY, int toX, int toY) {

    // Individual pieces of the string in the order they will be printed
    // Ones set to '\0' are conditional and are not always printed
    char pieceMoved;
    char fromTile[2] = {'\0', '\0'}; // Very rarely will both be used but it can happen
    char takes = '\0';
    char toTile[2];
    char promote[2] = {'\0', '\0'};
    char check = '\0';
    char mate = '\0';

    // Get piece type
    Uint8 piece = board->getTile(fromX, fromY);

    /*  ---  Handling all the chars in the same order they appear above  ---  */
    // pieceMoved
    switch (piece) {
        
        // Pawns dont need any prefix letter
        case WHITE_PAWN:
        case BLACK_PAWN:
            if (fromX != toX) getTileNotation(fromX, -1, &pieceMoved, NULL);
            else pieceMoved = '\0';
            break;

        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            pieceMoved = 'N';
            break;

        case WHITE_BISHOP:
        case BLACK_BISHOP:
            pieceMoved = 'B';
            break;

        case WHITE_ROOK:
        case BLACK_ROOK:
            pieceMoved = 'R';
            break;

        case WHITE_QUEEN:
        case BLACK_QUEEN:
            pieceMoved = 'Q';
            break;

        case WHITE_KING:
        case BLACK_KING:
            pieceMoved = 'K';
            break;

    }

    // fromTile
    // nothing yet

    // takes
    if (!board->isEmpty(toX, toY)) takes = 'x';

    // toTile
    getTileNotation(toX, toY, &toTile[0], &toTile[1]); // This function fills the second two values


    // Print stuff
    if (board->turn == WHITE) std::cout<< board->turnCount << ". ";
    std::cout<< pieceMoved;
    if (fromTile[0] != '\0') std::cout<< fromTile;
    if (takes != '\0') std::cout<< takes;
    std::cout<< toTile;
    if (promote[0] != '\0') std::cout<< promote;
    if (check != '\0') std::cout<< check;
    if (mate != '\0') std::cout<< mate;
    std::cout<< " ";
    if (board->turn == BLACK) std::cout<< "\n";

    return;
}

void mouseUp(int tileX, int tileY) {
    if (heldPiece == EMPTY) return;

    // Put the piece back in the board temporarily
    board->setTile(heldPieceX, heldPieceY, heldPiece);

    // Check piece moved against game turn
    bool wrongTurn = board->turn != Board::isWhite(heldPiece); // white turn has this->turn = true
    if (wrongTurn) {
        heldPiece = EMPTY;
        return;
    }

    // Check if legal move
    if (board->canMove(heldPieceX, heldPieceY, tileX, tileY)) {
        printMove(heldPieceX, heldPieceY, tileX, tileY);
        board->setTile(tileX, tileY, heldPiece);
        board->setTile(heldPieceX, heldPieceY, EMPTY);
        heldPiece = EMPTY;
        board->changeTurn();
    }

    heldPiece = EMPTY;
}

// Effectively a main function, gets called every frame
void doFrame(Drawer* drawer, FrameState* frameState) {

    if (drawer == NULL) {
        return;
    }

    int mouseTileX = frameState->mouse->posX;
    int mouseTileY = frameState->mouse->posY;
    findBoardPos(&mouseTileX, &mouseTileY);
    // std::cout<< mouseTileX << ", " << mouseTileY << "\n";

    // Mouse down handle
    if (frameState->wasLeftJustPressed()) {
        mouseDown(mouseTileX, mouseTileY);
    }

    // Mouse up handle
    if (frameState->wasLeftJustReleased()) {
        mouseUp(mouseTileX, mouseTileY);
    }

    drawBoard(drawer);
    drawPieces(drawer, frameState);

    return;
}