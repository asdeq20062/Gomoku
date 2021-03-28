#include <iostream>
#include <conio.h>
#include "board.h"
#include "value_network.h"

using namespace std;



int main()
{
    system("chcp 65001");
    system("cls");
    int size;
    int player = 1;
    int human;
    int computer;
    int *ai_next;
    cout<<"Please input the size of the board:"<<endl;
    cin>>size;
    cout<<"Please input Player 1 ""○"" or -1""●"":"<<endl;
    cin>>human;
    switch (human)
    {
    case 1:
        human = 1;
        computer = -1;
        break;
    case -1:
        human = -1;
        computer = 1;
        break;
    }
    board chess(size);
    chess.init();
    int **exist_board=chess.get_board();
    value_network ai(size,exist_board);
    chess.print();
    //ai.printout();
    char *result=chess.verdict(player);
    while(result[0]=='g'){
        if(player==human){
            int putx, puty;
            cout<<"Player "<<player<<" - Please input the x coordinate: ";
            cin>>putx;
            cout<<"Player "<<player<<" - Please input the y coordinate: ";
            cin>>puty;
            if(chess.move(putx, puty, player)){
                chess.move(putx, puty, player);
                player = player*-1;
                //exist_board=chess.get_board();
                //ai.refresh(exist_board);
                //ai_next = ai.estimate();
                //cout<<"x: "<<ai_next[0]<<" y: "<<ai_next[1]<< " score: "<<ai_next[2]<<endl;
                chess.print();
                ai.printout();
            } else {
                chess.print();
                cout<<"Illegal movement."<<endl;
            }
        } else if(player == computer){
            exist_board=chess.get_board();
            ai.refresh(exist_board);
            ai_next = ai.estimate();
            chess.move(ai_next[0]+1,ai_next[1]+1,player);
            //*cout<<"ai best x:"<<ai_next[0]<<", ai best y:"<<ai_next[1]<<", ai best score:"<<ai_next[2]<<endl;
            player = player*-1;
            ai.printout();
            chess.print();
        }


        result=chess.verdict(player); //judge the result
    }
    if(result[0]=='w'){
        cout<<"Player "<<player*-1<<" Win!"<<endl;
        system("pause");
    }else if (result[0]=='l'){
        cout<<"You Lose!"<<endl;
        system("pause");  
    }else if (result[0]=='d'){
        cout<<"Draw!"<<endl;
        system("pause");  
    }
    return 0;
}
