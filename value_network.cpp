#include <iostream>
#include "value_network.h"
#include <string>
#include <math.h>
#include <iomanip>
using namespace std;

value_network::value_network(int s, int **arr){
    size = s;
    board = arr;
    network=new int **[size];

    for(int i=0;i<size;i++){ //create a empty 3d array
        network[i]=new int *[size];
        for(int j=0;j<size;j++){
            network[i][j]=new int [2];
        }
    }
    for(int i=0;i<size;i++){ //initiate the 3d array
        for(int j=0;j<size;j++){
            for(int k=0;k<2;k++){
                network[i][j][k]=0;
            }
        }
    }

    double times=ceil((double)size/(double)2);
    for (int i=1, j=size-2; i<times;i++,j--){ //evaluate each point of the board for p1[0] and p-1[1]
        for(int x=i;x<=j;x++){
            for(int y=i;y<=j;y++){
                network[x][y][0]++;
                network[x][y][1]++;
            }
        }
    }

}

int *value_network::estimate(){
    int arr[5]={};
    int p1_score=0;
    int p2_score=0;
    init_network();
    //avoid duplicate chess
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if (board[i][j]!=0){
                network[i][j][0]= -99999;
                network[i][j][1]= -99999;
            }
        }
    }

    //estimate horiztonally
    for(int i=0;i<size;i++){
        for(int j=0;j<size-4;j++){
            for(int k=j, l=0;k<j+5;k++,l++){
                arr[l]=board[i][k];
            }
            
            p1_score=pattern(arr, 1);
            p2_score=pattern(arr, -1);
            for(int k=j;k<j+5;k++){
                if(board[i][k]==0){
                    network[i][k][0]=network[i][k][0]+p1_score;
                    network[i][k][1]=network[i][k][1]+p2_score;
                }
            }
        }
    }
    //estimate vertically
    for(int i=0;i<size-4;i++){
        for(int j=0;j<size;j++){
            for(int k=i, l=0;k<i+5;k++,l++){
                arr[l]=board[k][j];
            }
            
            p1_score=pattern(arr, 1);
            p2_score=pattern(arr, -1);
            for(int k=i;k<i+5;k++){
                if(board[k][j]==0){
                    network[k][j][0]=network[k][j][0]+p1_score;
                    network[k][j][1]=network[k][j][1]+p2_score;
                }
            }
        }
    }
    //estimate from left to right
    for(int i=0;i<size-4;i++){
        for(int j=0;j<size-4;j++){
            for(int k=i, m=j, l=0;k<i+5;k++,l++,m++){
                arr[l]=board[k][m];
            }
            
            p1_score=pattern(arr, 1);
            p2_score=pattern(arr, -1);
            for(int k=i, m=j;k<i+5;k++,m++){
                if(board[k][m]==0){
                    network[k][m][0]=network[k][m][0]+p1_score;
                    network[k][m][1]=network[k][m][1]+p2_score;
                }
            }
        }
    }
    //estimate from right to left
    for(int i=0;i<size-4;i++){
        for(int j=size-1;j>=0+4;j--){
            for(int k=i, m=j, l=0;k<i+5;k++,l++,m--){
                arr[l]=board[k][m];
            }
            
            p1_score=pattern(arr, 1);
            p2_score=pattern(arr, -1);
            for(int k=i, m=j;k<i+5;k++,m--){
                if(board[k][m]==0){
                    network[k][m][0]=network[k][m][0]+p1_score;
                    network[k][m][1]=network[k][m][1]+p2_score;
                }
            }
        }
    }
    //bouns marks if the point around by same chess
    for(int i=1;i<size-2;i++){
        for(int j=1;j<size-2;j++){
            if((board[i-1][j]==1 && board[i+1][j]==1 && board[i][j+1]==1 && board[i][j-1]==1) ||
               (board[i-1][j-1]==1 && board[i-1][j+1]==1 && board[i+1][j-1]==1 && board[i+1][j+1]==1)){
                   network[i][j][0]=network[i][j][0]+2000;
               }
            if((board[i-1][j]==-1 && board[i+1][j]==-1 && board[i][j+1]==-1 && board[i][j-1]==-1) ||
               (board[i-1][j-1]==-1 && board[i-1][j+1]==-1 && board[i+1][j-1]==-1 && board[i+1][j+1]==-1)){
                   network[i][j][1]=network[i][j][1]+2000;
               }
        }
    }

    //find the greatest marks
    static int p1_best[3]={0,0,-1}; //x,y,score
    static int p2_best[3]={0,0,-1};
    //initi
    for(int i=0;i<3;i++){
        p1_best[i]=-1;
        p2_best[i]=-1;
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(network[i][j][0]>p1_best[2]){
                p1_best[0]=i;
                p1_best[1]=j;
                p1_best[2]=network[i][j][0];
            }
            if(network[i][j][1]>p2_best[2]){
                p2_best[0]=i;
                p2_best[1]=j;
                p2_best[2]=network[i][j][1];
            }
        }
    }
    //*cout<<"P1 best score: "<<p1_best[2]<<", P2 best score: "<<p2_best[2]<<endl;
    //compare the best score with p1 and p2
    if(p1_best[2]>p2_best[2]){
        return p1_best;
    } else {
        return p2_best;
    }
}

int value_network::pattern(int arr[5], int player){ //cal 
    if (player ==-1){ //as this function only calculate as player1, -1 and 1 need to be reversed in the array when calculation as player-1
        for(int i=0; i<5; i++){
            switch (arr[i])
            {
            case 1:
                arr[i]=-1;
                break;
            case -1:
                arr[i]=1;
                break;
            default:
                break;
            }
        }
    }
    int c4[5]={1,1,1,1,0}; //●●●●x
    int c4_2[5]={1,0,1,1,1}; //●x●●●
    int c4_3[5]={1,1,0,1,1}; //●●x●●
    int c3[5]={0,1,1,1,0}; //x●●●x
    int c3_3[5]={1,1,1,0,0}; //●●●xx
    int c3_2[5]={0,1,1,0,1}; //x●●x●
    int s3[5]={-1,1,1,1,0}; //○●●●x
    int c2[5]={1,1,0,0,0}; //●●xxx
    int c2_2[5]={0,1,0,1,0}; //x●x●x
    int s2[5]={-1,1,1,0,0}; //○●●xx
    int s1[5]={1,0,0,0,0}; //●xxxx
    int s1_2[5]={0,1,0,0,0}; //x●xxx
    int s1_3[5]={0,0,1,0,0}; //xx●xx
    int c1[5]={-1,1,0,0,0}; //○●xxx


    int reverse[5];
    for(int i=0, j=4; i<5;i++, j--){
        reverse[i]=arr[j];
    }

    if(compare_arr(arr,c4)==true || compare_arr(reverse,c4)==true){
        return 100000;
    } else if (compare_arr(arr,c4_2)==true || compare_arr(reverse,c4_2)==true){
        return 100000;
    } else if (compare_arr(arr,c4_3)==true || compare_arr(reverse,c4_3)==true){
        return 100000;
    } else if (compare_arr(arr,c3)==true || compare_arr(reverse,c3)==true){
        return 50000;
    } else if (compare_arr(arr,c3_2)==true || compare_arr(reverse,c3_2)==true){
        return 50000;
    } else if (compare_arr(arr,s3)==true || compare_arr(reverse,s3)==true){
        return 1000;
    } else if (compare_arr(arr,c2_2)==true || compare_arr(reverse,c2_2)==true){ 
        return 1000;    
    } else if (compare_arr(arr,s2)==true || compare_arr(reverse,s2)==true){
        return 100;
    } else if (compare_arr(arr,c1)==true || compare_arr(reverse,c1)==true){
        return 10;
    } else if (compare_arr(arr,s1_2)==true || compare_arr(reverse,s1_2)==true){
        return 100;
    } else if (compare_arr(arr,s1_3)==true || compare_arr(reverse,s1_3)==true){
        return 100;
    } else if (compare_arr(arr,c3_3)==true || compare_arr(reverse,c3_3)==true){
        return 100;
    } else if (compare_arr(arr,c2)==true || compare_arr(reverse,c2)==true){
        return 100;
    } else if (compare_arr(arr,s1)==true || compare_arr(reverse,s1)==true){
        return 100;
    }
    return 0;
}

bool value_network::compare_arr(int arr1[5], int arr2[5]){
    int true_number=0;
    for(int i=0;i<5;i++){
        if(arr1[i]==arr2[i]){
            true_number++;
        }
    }
    if(true_number==5){
        return true;
    }
    return false;
}

void value_network::refresh(int **arr){
    board=arr;
}

void value_network::printout(){
    /*cout<<"ai's board:"<<endl;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            cout<<board[i][j]<<",";
        }
        cout<<endl;
    }*/
    cout<<"Player 1's value network:"<<endl;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            cout<<setw(7)<<network[i][j][0]<<", ";
        }
        cout<<endl;
    }
    cout<<"Player 2's value network:"<<endl;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            cout<<setw(7)<<network[i][j][1]<<", ";
        }
        cout<<endl;
    }
}

void value_network::init_network(){
    for(int i=0;i<size;i++){ //initiate the 3d array
        for(int j=0;j<size;j++){
            for(int k=0;k<2;k++){
                network[i][j][k]=0;
            }
        }
    }

    double times=ceil((double)size/(double)2);
    for (int i=1, j=size-2; i<times;i++,j--){ //evaluate each point of the board for p1[0] and p-1[1]
        for(int x=i;x<=j;x++){
            for(int y=i;y<=j;y++){
                network[x][y][0]++;
                network[x][y][1]++;
            }
        }
    }
}