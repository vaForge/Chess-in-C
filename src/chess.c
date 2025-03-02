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
}Piece;

typedef struct {
    Piece piece[size][size];
}Chess;


void init(Chess *chess){
    
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            chess->piece[i][j].type = noplace;
            chess->piece[i][j].color = noncolor;
        }
    }
    for(int i=0;i<size;i++){                                     //setting the pawns and colors
        chess->piece[1][i].type = pawn;
        chess->piece[1][i].color = white;
        chess->piece[6][i].type = pawn;
        chess->piece[6][i].color = black;
    }

    chess->piece[0][0].type = chess->piece[0][7].type = rook;            //Ok black pieces' types  are done  
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
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(chess->piece[i][j].type == noplace){
                printf("  ");
            }
            else{
                switch(chess->piece[i][j].type){
                    case pawn:
                        if(chess->piece[i][j].color == white){
                            printf("p ");
                        }
                        else{
                            printf("P ");
                        }
                        break;
                    case knight:
                        if(chess->piece[i][j].color == white){
                            printf("N ");
                        }
                        else{
                            printf("n ");
                        }
                        break;
                    case bishop:
                        if(chess->piece[i][j].color == white){
                            printf("B ");
                        }
                        else{
                            printf("b ");
                        }
                        break;
                    case rook:
                        if(chess->piece[i][j].color == white){
                            printf("R ");
                        }
                        else{
                            printf("r ");
                        }
                        break;
                    case queen:
                        if(chess->piece[i][j].color == white){
                            printf("Q ");
                        }
                        else{
                            printf("q ");
                        }
                        break;
                    case king:
                        if(chess->piece[i][j].color == white){
                            printf("K ");
                        }
                        else{
                            printf("k ");
                        }
                        break;
                }
            }
        }
        printf("\n");
    }
}
 

int main(){
    Chess chess;
    init(&chess);
    printBoard(&chess);
    return 0;
}