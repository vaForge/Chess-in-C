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

typedef struct {
    int fromrow , fromcol , torow , tocol;
}Move;


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
    printf("  a  b  c  d  e  f  g  h\n");
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
                            printf("p  ");
                        }
                        else{
                            printf("P  ");
                        }
                        break;
                    case knight:
                        if(chess->piece[i][j].color == white){
                            printf("N  ");
                        }
                        else{
                            printf("n  ");
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
    printf(" a  b  c  d  e  f  g  h\n");
}

Move ParseMove( char *str ){
    Move move;
    move.fromcol = tolower(str[0]) - 'a';
    move.fromrow = 8 - (str[1] - '0');
    move.tocol = tolower(str[2]) - 'a';
    move.torow = 8 - (str[3] - '0');
    return move;
}
 
int makeMove(Chess *chess , Move move){
    chess->piece[move.torow][move.tocol] = chess->piece[move.fromrow][move.fromcol];
    chess->piece[move.fromrow][move.fromcol].type = noplace;
    chess->piece[move.fromrow][move.fromcol].color = noncolor;
    return 1;
}
int validMove(Chess *chess,Move move){
    if(move.fromrow<0 || move.fromcol<0 || move.torow< 0 || move.tocol < 0 || move.fromrow>=size || move.fromcol >=size || move.tocol>=size || move.torow>= size){
        return 0;                   //out of board move
    }
    if(chess->piece[move.fromrow][move.fromcol].type == noplace){
        return 0;                    //moved from emty place 
    }
    if(chess->piece[move.fromrow][move.fromcol].color ==chess->piece[move.torow][move.tocol].color){
        return 0;                    //move to a place with same color 
    }
    Piece Fromtype = chess->piece[move.fromrow][move.fromcol];
    Piece Totype = chess->piece[move.torow][move.tocol];
    int coldiff = move.fromcol - move.tocol;
    int rowdiff = move.fromrow - move.torow;
    //Piece restrictions 1)pawn 
    if(Fromtype.type == pawn){
        if(Fromtype.color == white){
            if(coldiff == 0 && abs(rowdiff)==1 && Totype.type == noplace){
                return 1;
            }
            if(coldiff ==0 && rowdiff == -2 && move.fromrow == 6 && chess->piece[move.fromrow-1][move.fromcol].type == noplace && Totype.type == noplace){
                return 1;
            }
            if(abs(coldiff) == 1 && rowdiff == -1 && Totype.type != noplace && Totype.color == black){
                return 1;
            }
        }
        else{
            if(coldiff == 0 && abs(rowdiff)==1 && Totype.type == noplace){
                return 1;
            }
            if(coldiff ==0 && rowdiff == 2 && move.fromrow == 1 && chess->piece[move.fromrow+1][move.fromcol].type == noplace && Totype.type == noplace){
                return 1;
            }
            if(abs(coldiff) == 1 && rowdiff == 1 && Totype.type != noplace && Totype.color == white){
                return 1;
            }
        }
        return 0;
    }
    if(Fromtype.type == rook){
        if(coldiff == 0 || rowdiff == 0){
            if(rowdiff == 0){
                int step = coldiff>0?1: -1;
                for(int i=move.fromcol+step;i!=move.tocol;i+=step){
                    if(chess->piece[move.fromrow][i].type != noplace){
                        return 0;
                    }
                }
            }
            else{
                int step = rowdiff>0?1: -1;
                for(int i=move.fromrow+step;i!=move.torow;i+=step){
                    if(chess->piece[i][move.fromcol].type != noplace){
                        return 0;
                    }
                }
            }
            return 1;
        }
        return 0;
    }
    if(Fromtype.type == knight){
        if(abs(coldiff) == 2 && abs(rowdiff) == 1 ){
            return 1;
        }
        if(abs(coldiff) == 1 && abs(rowdiff) == 2 ){
            return 1;
        }
        return 0;
    }
    if(Fromtype.type == bishop){
        if(abs(coldiff) == abs(rowdiff)){
            int steprow = rowdiff>0?1:-1;
            int stepcol = coldiff>0?1:-1;
            for(int i = move.fromrow+steprow,j=move.fromcol+stepcol;i!=move.torow;i+=steprow,j+=stepcol){
                if(chess->piece[i][j].type != noplace){
                    return 0;
                }
            }
            return 1;
        }
        return 0;
    }
    if(Fromtype.type == king){
        if(abs(coldiff) <= 1 && abs(rowdiff) <= 1){
            return 1;
        }
        return 0;
    }
    if(Fromtype.type == queen){                       // rook+bihsop
        if(coldiff ==0 || rowdiff == 0 || abs(coldiff) == abs(rowdiff)){
            if(rowdiff == 0){
                int step = coldiff>0?1: -1;                        //horizontal move
                for(int i=move.fromcol+step;i!=move.tocol;i+=step){
                    if(chess->piece[move.fromrow][i].type != noplace){
                        return 0;
                    }
                }
            }
            else if(coldiff == 0){
                int step = rowdiff>0?1: -1;                           // vertical move 
                for(int i=move.fromrow+step;i!=move.torow;i+=step){
                    if(chess->piece[i][move.fromcol].type != noplace){
                        return 0;
                    }
                }
            }
            else{                                 //diagonal move
                int steprow = rowdiff>0?1:-1;
                int stepcol = coldiff>0?1:-1;
                for(int i = move.fromrow+steprow,j=move.fromcol+stepcol;i!=move.torow;i+=steprow,j+=stepcol){
                    if(chess->piece[i][j].type != noplace){
                        return 0;
                    }
                }
            }
            return 1;
        }
        return 0;
    }
}

int main(){
    Chess chess;
    init(&chess);
    printBoard(&chess);
    char str[5];
    printf("Enter move (like e2e4): ");
    scanf("%s",str);
    while(str[0] != 'Z'){
        Move move = ParseMove(str);
        makeMove(&chess,move);
        printBoard(&chess);
        printf("Enter move (like e2e4): ");
        scanf("%s",str);
    }
   
    return 0;
}