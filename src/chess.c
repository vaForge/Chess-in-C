#include<stdio.h>
#include<stdlib.h>
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
        chess->piece[1][i].color = black;     // Fixed: row 1 should be black
        chess->piece[6][i].type = pawn;
        chess->piece[6][i].color = white;     // Fixed: row 6 should be white
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

Move ParseMove(char *str){
    Move move;
    move.fromcol = tolower(str[0]) - 'a';
    move.fromrow = 8 - (str[1] - '0');
    move.tocol = tolower(str[2]) - 'a';
    move.torow = 8 - (str[3] - '0');
    return move;
}

int validMove(Chess *chess, Move move){
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
                int step = coldiff > 0 ? -1 : 1;  // Fixed: step direction
                for(int i = move.fromcol + step; i != move.tocol; i += step){
                    if(chess->piece[move.fromrow][i].type != noplace){
                        return 0;
                    }
                }
            }
            else{
                int step = rowdiff > 0 ? -1 : 1;  // Fixed: step direction
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
    
    return 0;  // Added default return value for unexpected piece types
}
 
int makeMove(Chess *chess, Move move){
    if(validMove(chess, move) == 0) return 0;
    chess->piece[move.torow][move.tocol] = chess->piece[move.fromrow][move.fromcol];
    chess->piece[move.fromrow][move.fromcol].type = noplace;
    chess->piece[move.fromrow][move.fromcol].color = noncolor;
    return 1;
}
int isKingInCheck(Chess *chess, Color kingColor) {
    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (chess->piece[r][c].type == king && chess->piece[r][c].color == kingColor) { //king pos found
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (chess->piece[r][c].color != kingColor && chess->piece[r][c].color != noncolor) {
                Move testMove = {r, c, kingRow, kingCol};
                if (validMove(chess, testMove)) {
                    return 1; // King is in check
                }
            }
        }
    }
    return 0; // King is safe
}

int isCheckmate(Chess *chess, Color kingColor) {
    if (!isKingInCheck(chess, kingColor)) return 0; 
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (chess->piece[r][c].color == kingColor) { 
                for (int dr = 0; dr < 8; dr++) {
                    for (int dc = 0; dc < 8; dc++) {
                        Move testMove = {r, c, dr, dc};
                        if (validMove(chess, testMove)) {
                            // Simulate move
                            Piece tempFrom = chess->piece[r][c];
                            Piece tempTo = chess->piece[dr][dc];
                            chess->piece[dr][dc] = chess->piece[r][c];
                            chess->piece[r][c].type = noplace;
                            chess->piece[r][c].color = noncolor;
                            
                            int stillInCheck = isKingInCheck(chess, kingColor);
                            chess->piece[r][c] = tempFrom;        //revet bakc to og move
                            chess->piece[dr][dc] = tempTo;
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
    
        Move m = ParseMove(move);
        if (chess.piece[m.fromrow][m.fromcol].color == turn && makeMove(&chess, m)) { 
            if (isKingInCheck(&chess, (turn == white) ? black : white)) {
                printf("Check!\n");
                Color opposingColor = (turn == white) ? black : white;
                if (isCheckmate(&chess, opposingColor)) {
                    printf("Checkmate! %s wins!\n", turn == white ? "White" : "Black");
                    exit(0); // End game
                }
            }
            turn = (turn == white) ? black : white; // Switch turn
        } else {
            printf("Invalid move! Try again.\n");
        }
        
        
    }
    return 0;
}