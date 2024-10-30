#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void win(int i, int j, char movement, char game[][20]){
    if((movement=='R' && j<19 && game[i][j+1]=='C')||
       (movement=='L' && j>0 && game[i][j-1]=='C')||
       (movement=='U' && i>0 && game[i-1][j]=='C')||
       (movement=='D' && i<9 && game[i+1][j]=='C')){
             cout<<"############\n";
             cout<<"# You win #\n";
             cout<<"############\n";
             cout<<"Presione enter para salir...";
             cin.ignore();
             getchar();
             exit(0);
       }
}

void lose(int i, int j, char movement, char game[][20]){
    if((movement=='R' && j<19 && game[i][j+1]=='M')||
       (movement=='L' && j>0 && game[i][j-1]=='M')||
       (movement=='U' && i>0 && game[i-1][j]=='M')||
       (movement=='D' && i<9 && game[i+1][j]=='M')){
             cout<<"############\n";
             cout<<"# You lose #\n";
             cout<<"############\n";
             cout<<"Presione enter para salir...";
             cin.ignore();
             getchar();
             exit(0);
       }
}

void mov(char movement, char game[][20]){
    int i,j;
    for(i=0;i<10;i++){
        for(j=0;j<20;j++){
            if(game[i][j]=='P'){

                lose(i,j,movement, game);
                win(i,j,movement, game);

                game[i][j]=' ';
                     if(movement=='R' && j<19 && game[i][j+1]==' '){j=j+1;}
                     else if(movement=='L' && j>0 && game[i][j-1]==' '){j=j-1;}
                     else if(movement=='U' && i>0 && game[i-1][j]==' '){i=i-1;}
                     else if(movement=='D' && i<9 && game[i+1][j]==' '){i=i+1;}

                     game[i][j]='P';
            }
        }
    }
}

void pos(char entity, int cnt, char game[][20]){
    int i, f, k;
    
    for(k=0; k<cnt; k++){
            do{
            i=1+rand()%9;
            f=1+rand()%19;
            }while(game[i][f]!=' ');
            
            game[i][f]=entity;
    }
}

void game_f(char mon, char chest, char player, char game[][20]){
    int i, f;

    for(i=0; i<10; i++){
        for(f=0; f<20; f++){
            if(i==0||i==9||f==0||f==19){game[i][f]='#';}
            else{game[i][f]=' ';}   
        }
    }

    pos(mon, 3, game);
    pos(chest, 1, game);
    pos(player, 1, game);
}

void print(char game[][20]){
    int i,f;
    for(i=0; i<10; i++){
        for(f=0; f<20; f++){
            cout<<game[i][f];
        }
        cout<<"\n";
    }
}

int main(){
    char game[10][20];
    char Movement;
    char Monster = 'M';
    char Chest = 'C';
    char Player = 'P';

    srand(time(0));

    game_f(Monster, Chest, Player, game);

    do{
        print(game);
        cout<<"Type your movement: ";cin>>Movement;
        Movement=toupper(Movement);
        mov(Movement, game);
        system("cls");
    }while(Movement!='X');
}