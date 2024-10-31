#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Player{
    int lives=2;
    int health=100;
};

Player player_s;

void win(int i, int j, char movement, char game[][40]){
    if((movement=='R' && j<39 && game[i][j+1]=='C')||
       (movement=='L' && j>0 && game[i][j-1]=='C')||
       (movement=='U' && i>0 && game[i-1][j]=='C')||
       (movement=='D' && i<19 && game[i+1][j]=='C')){
            system("cls");
             cout<<"############\n";
             cout<<"# You win  #\n";
             cout<<"############\n";
             cout<<"Press enter to exit...";
             cin.ignore();
             getchar();
             exit(0);
       }
}

void lose(Player loser){
    if(loser.lives==0){

        system("cls");
        cout<<"###########################\n";
        cout<<"# No more lives remaining #\n";
        cout<<"###########################\n";
        cin.ignore();
        getchar();
        system("cls");

        cout<<"############\n";
        cout<<"# You lose #\n";
        cout<<"############\n";
        cout<<"Press enter to exit...";
        getchar();
        exit(0);
    }
}

void win_hp(int i, int j, char movement, char game[][40]){
    if(player_s.health<100 && ((movement=='R' && j<39 && game[i][j+1]=='H')||
       (movement=='L' && j>0 && game[i][j-1]=='H')||
       (movement=='U' && i>0 && game[i-1][j]=='H')||
       (movement=='D' && i<19 && game[i+1][j]=='H'))){

            player_s.health+=50;

            system("cls");
             cout<<"#####################\n";
             cout<<"# You've won 50 Hp  #\n";
             cout<<"#####################\n";
             cout<<"Press enter to continue...";
             cin.ignore();
             getchar();
             system("cls");

             cout<<"###########################\n";
             cout<<"# Your current Hp is "<<player_s.health<<" #\n";
             cout<<"###########################\n";
             cout<<"Press enter to continue...";
             getchar();
       }
}

void restore_hp(){
    player_s.lives-=1;
    player_s.health=100;

    system("cls");
    cout << "#####################################\n";
    cout << "# You've lost one life. " << player_s.lives << " remaining #\n";
    cout << "#####################################\n";
    cout << "Press enter to continue...";
    cin.ignore();
    getchar();
    system("cls");

    cout << "##################################\n";
    cout << "# Your health has been restored  #\n";
    cout << "##################################\n";
     getchar();
}

void lose_hp(int i, int j, char movement, char game[][40]){
    int hp_lost=0;

    if((movement=='R' && j<39 && game[i][j+1]=='M')||
       (movement=='L' && j>0 && game[i][j-1]=='M')||
       (movement=='U' && i>0 && game[i-1][j]=='M')||
       (movement=='D' && i<19 && game[i+1][j]=='M')){

        hp_lost=50;
       }
    else if((movement=='R' && j<39 && game[i][j+1]=='m')||
       (movement=='L' && j>0 && game[i][j-1]=='m')||
       (movement=='U' && i>0 && game[i-1][j]=='m')||
       (movement=='D' && i<19 && game[i+1][j]=='m')){

        hp_lost=25;
       }

       if(hp_lost>0){
            if(player_s.health>0){

                player_s.health-=hp_lost;

                if(player_s.health<=0){
                    restore_hp();
                    lose(player_s);
                }
                else{
                system("cls");
                cout<<"######################\n";
                cout<<"# You've lost "<<hp_lost<<"Hp  #\n";
                cout<<"######################\n";
                cout<<"Press enter to continue...";
                cin.ignore();
                getchar();
                system("cls");

                cout<<"##########################\n";
                cout<<"# Your current Hp is: "<<player_s.health<<" #\n";
                cout<<"##########################\n";
                getchar(); 

                }
            }
        }
}

void mov(char movement, char game[][40]){
    int i,j;
    for(i=0;i<20;i++){
        for(j=0;j<40;j++){
            if(game[i][j]=='P'){
                lose_hp(i,j,movement, game);
                win_hp(i,j,movement,game);
                win(i,j,movement, game);

                game[i][j]=' ';
                     if(movement=='R' && j<39 && game[i][j+1]!='#'){j=j+1;}
                     else if(movement=='L' && j>0 && game[i][j-1]!='#'){j=j-1;}
                     else if(movement=='U' && i>0 && game[i-1][j]!='#'){i=i-1;}
                     else if(movement=='D' && i<19 && game[i+1][j]!='#'){i=i+1;}

                     game[i][j]='P';
            }
        }
    }
}

void pos(char entity, int cnt, char game[][40]){
    int i, f, k;
    
    for(k=0; k<cnt; k++){
            do{
            i=1+rand()%19;
            f=1+rand()%39;
            }while(game[i][f]!=' ');
            
            game[i][f]=entity;
    }
}

void game_f(char mon_b, char mon_s, char chest, char player, char potion, char game[][40]){
    int i, f;

    for(i=0; i<20; i++){
        for(f=0; f<40; f++){
            if(i==0||i==19||f==0||f==39){game[i][f]='#';}
            else{game[i][f]=' ';}   
        }
    }

    pos(mon_b, 5, game);
    pos(mon_s, 10, game);
    pos(chest, 1, game);
    pos(player, 1, game);
    pos(potion, 2, game);
}

void print(char game[][40]){
    int i,f;
    for(i=0; i<20; i++){
        for(f=0; f<40; f++){
            cout<<game[i][f];
        }
        cout<<"\n";
    }
}

int main(){
    char game[20][40];
    char Movement;
    char Monster_b = 'M';
    char Monster_s = 'm';
    char Chest = 'C';
    char Player = 'P';
    char Potion = 'H';

    srand(time(0));

    game_f(Monster_b, Monster_s, Chest, Player, Potion, game);

    do{
        print(game);
        cout<<"Type your movement: ";cin>>Movement;
        Movement=toupper(Movement);
        mov(Movement, game);
        system("cls");
    }while(Movement!='X');
}