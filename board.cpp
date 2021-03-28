#include <iostream>
#include <iomanip>
#include "board.h"
using namespace std;

board::board(int size){
    board_size = size;
}

void board::init(){
    board_array = new int*[board_size];
    for(int i=0;i<board_size;i++){
        board_array[i] = new int[board_size];
    }
    for(int i=0;i<board_size;i++){
        for(int j=0;j<board_size;j++){
            board_array[i][j]=0;
        }
    }
}

int **board::get_board(){
    return board_array;
}

void board::print(){
    //system("CLS");
    cout<<"Player 1: Black ○"<<endl;
    cout<<"Player -1: White ●"<<endl<<endl;
    cout<<setw(4)<<0;
    for(int i=1;i<=board_size;i++){
        cout<<setw(3)<<i;
    }
    cout<<endl;
    for(int i=0;i<board_size;i++){
        cout<<setw(4)<<i+1<<" ";
        for(int j=0;j<board_size;j++){
            switch(board_array[i][j]){
                case 0:
                cout<<setw(3)<<"十 ";
                break;

                case -1:
                cout<<setw(3)<<"● ";
                break;
                
                case 1:
                cout<<setw(3)<<"○ ";
                break;
            }
        }
        cout<<endl;
    }
}
bool board::move(int x, int y, int player){
    if( x>board_size || x<=0 || y>board_size || y<=0 || (board_array[x-1][y-1]!=0)){
        return false;
    }
    x--;
    y--;
    if(player == 1){
        board_array[x][y]=1;
    } else {
        board_array[x][y]=-1;
    }
    return true;
}

char *board::verdict(int player){
    char nplayer;
    static char result[2]={};
    switch (player)
    {
    case 1:
        nplayer = '1';
        break;
    case -1:
        nplayer = '2';
    }
    for (int i=0;i<board_size;i++){  //sliding horizontally
        for (int j=0;j<board_size-4;j++){
            if(board_array[i][j]!=0 && 
               board_array[i][j]==board_array[i][j+1] &&
               board_array[i][j+1]==board_array[i][j+2] &&
               board_array[i][j+2]==board_array[i][j+3] &&
               board_array[i][j+3]==board_array[i][j+4]
            ){
                result[0] = {'w'};
                result[1] = {nplayer};
                //cout<<result[0]<<" : "<<result[1]<<endl;
                return result;
            }

        }
    }

    for (int i=0;i<board_size-4;i++){  //sliding vertically
        for (int j=0;j<board_size;j++){
            if(board_array[i][j]!=0 && 
               board_array[i][j]==board_array[i+1][j] &&
               board_array[i+1][j]==board_array[i+2][j] &&
               board_array[i+2][j]==board_array[i+3][j] &&
               board_array[i+3][j]==board_array[i+4][j]
            ){
                result[0] = {'w'};
                result[1] = {nplayer};
                //cout<<result[0]<<" : "<<result[1]<<endl;
                return result;
            }

        }
    }

    for (int i=0;i<board_size-4;i++){  //sliding left to right
        for (int j=0;j<board_size-4;j++){
            if(board_array[i][j]!=0 && 
               board_array[i][j]==board_array[i+1][j+1] &&
               board_array[i+1][j+1]==board_array[i+2][j+2] &&
               board_array[i+2][j+2]==board_array[i+3][j+3] &&
               board_array[i+3][j+3]==board_array[i+4][j+4]
            ){
                result[0] = {'w'};
                result[1] = {nplayer};
                //cout<<result[0]<<" : "<<result[1]<<endl;
                return result;
            }

        }
    }

    for (int i=0;i<board_size-4;i++){  //sliding right to left
        for (int j=board_size-1;j>=4;j--){
            if(board_array[i][j]!=0 && 
               board_array[i][j]==board_array[i+1][j-1] &&
               board_array[i+1][j-1]==board_array[i+2][j-2] &&
               board_array[i+2][j-2]==board_array[i+3][j-3] &&
               board_array[i+3][j-3]==board_array[i+4][j-4]
            ){
                result[0] = {'w'};
                result[1] = {nplayer};
                //cout<<result[0]<<" : "<<result[1]<<endl;
                return result;
            }

        }
    }

    int count=0;
    int limit_point=board_size*board_size;
    for(int i=0; i<board_size;i++){ //draw
        for(int j=0; j<board_size;j++){
            if(board_array[i][j]!=0){
                count++;
            }
        }
    }
    if(count==board_size*board_size){
        result[0] = {'d'};
        result[1] = {nplayer};
        //cout<<result[0]<<" : "<<result[1]<<endl;
        return result;
    }

    result[0] = {'g'};
    result[1] = {nplayer};
    //cout<<result[0]<<" : "<<result[1]<<endl;
    return result;
}