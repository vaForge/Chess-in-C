#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define size 8
typedef enum {
    noplace , pawn , knight , bishop , rook , queen , king
} Type;

typedef enum {
    noncolor , white , black,
}Color;

typedef struct {
    Type type;
    Color color;
    int moved;
}Piece;

typedef struct {
    Piece piece[size][size];
}Chess;

typedef struct {
    int fromrow , fromcol , torow , tocol;
}Move;


void init(Chess *chess){
    
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            chess->piece[i][j].type = noplace;
            chess->piece[i][j].color = noncolor;
            chess->piece[i][j].moved = 0;
        }
    }
    for(int i=0;i<size;i++){                                     //setting the pawns and colors
        chess->piece[1][i].type = pawn;
        chess->piece[1][i].color = black;     
        chess->piece[6][i].type = pawn;
        chess->piece[6][i].color = white;     
    }

    chess->piece[0][0].type = chess->piece[0][7].type = rook;            //black pieces' types
    chess->piece[0][1].type = chess->piece[0][6].type = knight;
    chess->piece[0][2].type = chess->piece[0][5].type = bishop;
    chess->piece[0][3].type = queen;
    chess->piece[0][4].type = king;

    chess->piece[7][0].type = chess->piece[7][7].type = rook;        //white pieces' types 
    chess->piece[7][1].type = chess->piece[7][6].type = knight;
    chess->piece[7][2].type = chess->piece[7][5].type = bishop;
    chess->piece[7][3].type = queen;
    chess->piece[7][4].type = king;

    for(int i=0;i<size;i++){                                      // setting up their colors too 
        chess->piece[0][i].color = black;
        chess->piece[7][i].color = white;
    }
}

void printBoard(Chess *chess){
    printf("   a  b  c  d  e  f  g  h\n");
    printf("  -----------------------\n");
    for(int i=0;i<size;i++){
        printf("%d| ",size-i);
        for(int j=0;j<size;j++){
            if(chess->piece[i][j].type == noplace){
                printf("_  ");
            }
            else{
                switch(chess->piece[i][j].type){
                    case pawn:
                        if(chess->piece[i][j].color == white){
                            printf("P  ");
                        }
                        else{
                            printf("p  ");
                        }
                        break;
                    case knight:
                        if(chess->piece[i][j].color == white){
                            printf("H  ");
                        }
                        else{
                            printf("h  ");
                        }
                        break;
                    case bishop:
                        if(chess->piece[i][j].color == white){
                            printf("B  ");
                        }
                        else{
                            printf("b  ");
                        }
                        break;
                    case rook:
                        if(chess->piece[i][j].color == white){
                            printf("R  ");
                        }
                        else{
                            printf("r  ");
                        }
                        break;
                    case queen:
                        if(chess->piece[i][j].color == white){
                            printf("Q  ");
                        }
                        else{
                            printf("q  ");
                        }
                        break;
                    case king:
                        if(chess->piece[i][j].color == white){
                            printf("K  ");
                        }
                        else{
                            printf("k  ");
                        }
                        break;
                }
            }
        }
        printf("\n");
    }
    printf(" ------------------------\n");
    printf("   a  b  c  d  e  f  g  h\n");
}
int validMove(Chess *chess, Move move, int checkCastling);
int isKingInCheck(Chess *chess, Color kingColor, int checkCastling);
int validMoveFormat(const char *str) {
    if (strlen(str) != 4) return 0;
    if (str[0] < 'a' || str[0] > 'h' || str[2] < 'a' || str[2] > 'h') return 0;
    if (str[1] < '1' || str[1] > '8' || str[3] < '1' || str[3] > '8') return 0;
    
    return 1;
}

Move ParseMove(char *str){
    Move move;
    move.fromcol = tolower(str[0]) - 'a';
    move.fromrow = 8 - (str[1] - '0');
    move.tocol = tolower(str[2]) - 'a';
    move.torow = 8 - (str[3] - '0');
    return move;
}

int validMove(Chess *chess, Move move, int checkCastling){
    if(move.fromrow<0 || move.fromcol<0 || move.torow< 0 || move.tocol < 0 || move.fromrow>=size || move.fromcol >=size || move.tocol>=size || move.torow>= size){
        return 0;                   //out of board move
    }
    if(chess->piece[move.fromrow][move.fromcol].type == noplace){
        return 0;                    //moved from empty place 
    }
    if(chess->piece[move.fromrow][move.fromcol].color == chess->piece[move.torow][move.tocol].color && 
       chess->piece[move.torow][move.tocol].type != noplace){
        return 0;                    //move to a place with same color 
    }
    Piece Fromtype = chess->piece[move.fromrow][move.fromcol];
    Piece Totype = chess->piece[move.torow][move.tocol];
    int coldiff = move.fromcol - move.tocol;
    int rowdiff = move.fromrow - move.torow;
    
    //Piece restrictions 1) pawn 
    if(Fromtype.type == pawn){
        if(Fromtype.color == white){
            if(coldiff == 0 && rowdiff == 1 && Totype.type == noplace){  
                return 1;
            }
            if(coldiff == 0 && rowdiff == 2 && move.fromrow == 6 && 
               chess->piece[move.fromrow-1][move.fromcol].type == noplace && Totype.type == noplace){
                return 1;
            }
            if(abs(coldiff) == 1 && rowdiff == 1 && Totype.type != noplace && Totype.color == black){
                return 1;
            }
        }
        else{                      // black pawn
            if(coldiff == 0 && rowdiff == -1 && Totype.type == noplace){  
                return 1;
            }
            if(coldiff == 0 && rowdiff == -2 && move.fromrow == 1 && 
               chess->piece[move.fromrow+1][move.fromcol].type == noplace && Totype.type == noplace){
                return 1;
            }
            if(abs(coldiff) == 1 && rowdiff == -1 && Totype.type != noplace && Totype.color == white){
                return 1;
            }
        }
        return 0;
    }
    //2) rook
    if(Fromtype.type == rook){
        if(coldiff == 0 || rowdiff == 0){
            if(rowdiff == 0){
                int step = coldiff > 0 ? -1 : 1;  
                for(int i = move.fromcol + step; i != move.tocol; i += step){
                    if(chess->piece[move.fromrow][i].type != noplace){
                        return 0;
                    }
                }
            }
            else{
                int step = rowdiff > 0 ? -1 : 1;
                for(int i = move.fromrow + step; i != move.torow; i += step){
                    if(chess->piece[i][move.fromcol].type != noplace){
                        return 0;
                    }
                }
            }
            return 1;
        }
        return 0;
    }
    // 3) knight
    if(Fromtype.type == knight){
        if(abs(coldiff) == 2 && abs(rowdiff) == 1){
            return 1;
        }
        if(abs(coldiff) == 1 && abs(rowdiff) == 2){
            return 1;
        }
        return 0;
    }
    //4) bishop
    if(Fromtype.type == bishop){
        if(abs(coldiff) == abs(rowdiff)){
            int steprow = rowdiff > 0 ? -1 : 1;  
            int stepcol = coldiff > 0 ? -1 : 1;  
            for(int i = move.fromrow + steprow, j = move.fromcol + stepcol; i != move.torow; i += steprow, j += stepcol){
                if(chess->piece[i][j].type != noplace){
                    return 0;
                }
            }
            return 1;
        }
        return 0;
    }
    //5) king
    if(Fromtype.type == king){
        if(abs(coldiff) <= 1 && abs(rowdiff) <= 1){
            return 1;
        }
        
        // Check castling only if requested
        if (checkCastling && abs(coldiff) == 2 && rowdiff == 0) {
            if (Fromtype.moved) return 0; //king moved
            int rookCol = (coldiff < 0) ? 7 : 0;  //kingside or queenside
            if (chess->piece[move.fromrow][rookCol].type != rook || chess->piece[move.fromrow][rookCol].moved){
                return 0; // Rook has moved before or not present
            }
            int step = (coldiff < 0) ? 1 : -1;
            for (int i = move.fromcol + step; i != rookCol; i += step) {
                if (chess->piece[move.fromrow][i].type != noplace){
                    return 0;
                }
            }
            // Check if king is in check currently
            if (isKingInCheck(chess, Fromtype.color, 0)){
                return 0;
            }
            
            // Check if king passes through check
            Chess tempChess = *chess;
            tempChess.piece[move.fromrow][move.fromcol + step] = tempChess.piece[move.fromrow][move.fromcol];
            tempChess.piece[move.fromrow][move.fromcol].type = noplace;
            tempChess.piece[move.fromrow][move.fromcol].color = noncolor;
            if (isKingInCheck(&tempChess, Fromtype.color, 0)){
                return 0;
            }
            
            return 1;
        }
        return 0;
    }
    if(Fromtype.type == queen){                                          // rook + bishop = queen
        if(coldiff == 0 || rowdiff == 0 || abs(coldiff) == abs(rowdiff)){
            if(rowdiff == 0){                                 // horizontal move
                int step = coldiff > 0 ? -1 : 1;  
                for(int i = move.fromcol + step; i != move.tocol; i += step){
                    if(chess->piece[move.fromrow][i].type != noplace){
                        return 0;
                    }
                }
            }
            else if(coldiff == 0){               // vertical move 
                int step = rowdiff > 0 ? -1 : 1;  
                for(int i = move.fromrow + step; i != move.torow; i += step){
                    if(chess->piece[i][move.fromcol].type != noplace){
                        return 0;
                    }
                }
            }
            else{                  // diagonal move
                int steprow = rowdiff > 0 ? -1 : 1;  
                int stepcol = coldiff > 0 ? -1 : 1;  
                for(int i = move.fromrow + steprow, j = move.fromcol + stepcol; i != move.torow; i += steprow, j += stepcol){  
                    if(chess->piece[i][j].type != noplace){
                        return 0;
                    }
                }
            }
            return 1;
        }
        return 0;
    }
    return 0;  
}

int makeMove(Chess *chess, Move move) {
    if (validMove(chess, move, 1) == 0) return 0;
    
    Piece fromPiece = chess->piece[move.fromrow][move.fromcol];
    int coldiff = move.fromcol - move.tocol;
    // Handle castling
    if (fromPiece.type == king && abs(coldiff) == 2) {
        int rookCol = (coldiff < 0) ? 7 : 0;
        int newRookCol = (coldiff < 0) ? move.tocol - 1 : move.tocol + 1;
        // Move king
        chess->piece[move.torow][move.tocol] = chess->piece[move.fromrow][move.fromcol];
        chess->piece[move.torow][move.tocol].moved = 1;
        // Move rook
        chess->piece[move.torow][newRookCol] = chess->piece[move.fromrow][rookCol];
        chess->piece[move.torow][newRookCol].moved = 1;
        
        // Clear old positions
        chess->piece[move.fromrow][move.fromcol].type = noplace;
        chess->piece[move.fromrow][move.fromcol].color = noncolor;
        chess->piece[move.fromrow][rookCol].type = noplace;
        chess->piece[move.fromrow][rookCol].color = noncolor;
        
        return 1;
    }

    chess->piece[move.torow][move.tocol] = chess->piece[move.fromrow][move.fromcol];
    chess->piece[move.torow][move.tocol].moved = 1;  // Mark the piece as moved
    chess->piece[move.fromrow][move.fromcol].type = noplace;
    chess->piece[move.fromrow][move.fromcol].color = noncolor;
    
    return 1;
}

int isKingInCheck(Chess *chess, Color kingColor, int checkCastling) {
    int kingRow = -1, kingCol = -1;
    
    // Find the king's position
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (chess->piece[r][c].type == king && chess->piece[r][c].color == kingColor) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
        if (kingRow != -1) break;
    }
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (chess->piece[r][c].color != kingColor && chess->piece[r][c].color != noncolor) {
                Move testMove = {r, c, kingRow, kingCol};
                if (validMove(chess, testMove, checkCastling)) {
                    return 1; // King is in check
                }
            }
        }
    }
    return 0;
}

int isCheckmate(Chess *chess, Color kingColor) {
    if (!isKingInCheck(chess, kingColor, 1)) return 0; 
    
    // Try every possible move for all pieces of the given color
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (chess->piece[r][c].color == kingColor) { 
                for (int dr = 0; dr < 8; dr++) {
                    for (int dc = 0; dc < 8; dc++) {
                        Move testMove = {r, c, dr, dc};
                        if (validMove(chess, testMove, 1)) {
                            // Save the current state
                            Piece savedFrom = chess->piece[r][c];
                            Piece savedTo = chess->piece[dr][dc];
                            
                            // Make the move
                            chess->piece[dr][dc] = chess->piece[r][c];
                            chess->piece[r][c].type = noplace;
                            chess->piece[r][c].color = noncolor;
                            
                            // Check if the king is still in check
                            int stillInCheck = isKingInCheck(chess, kingColor, 0);
                            
                            // Restore the board
                            chess->piece[r][c] = savedFrom;
                            chess->piece[dr][dc] = savedTo;
                            
                            if (!stillInCheck) return 0; // Found an escape move
                        }
                    }
                }
            }
        }
    }
    return 1; // No valid move to escape check
}

int main(){
    Chess chess;
    init(&chess);
    Color turn = white;
    
    while (1) {
        printBoard(&chess);
        char move[5]; 
        printf("%s's turn: Enter move (e.g., e2e4): ", turn == white ? "White" : "Black");
        scanf("%s", move);
        
        // Validate move format
        if (!validMoveFormat(move)) {
            printf("Invalid move format! Use format like 'e2e4'.\n");
            continue;
        }
        
        Move m = ParseMove(move);
        
        // Check if player is moving their own piece
        if (chess.piece[m.fromrow][m.fromcol].color != turn) {
            printf("You can only move your own pieces!\n");
            continue;
        }
        
        if (makeMove(&chess, m)) { 
            Color opcolor = (turn == white) ? black : white;
            if (isKingInCheck(&chess, opcolor, 1)) {
                printf("Check!\n");
                if (isCheckmate(&chess, opcolor)) {
                    printf("Checkmate! %s wins!\n", turn == white ? "White" : "Black");
                    printBoard(&chess);
                    break; // End game
                }
            }
            turn = (turn == white) ? black : white; // Switch turn
        } else {
            printf("Invalid move! Try again.\n");
        }
    }
    return 0;
}