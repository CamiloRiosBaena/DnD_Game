#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

struct Player{
    int lives=2;
    int health=100;
};

int win_s=0;
int lose_s=0;
int point=0;
int cnt_mv=0;
char Monster_b = 'M';
char Monster_s = 'm';
char Chest = 'C';
char Player_p = 'P';
char Potion = 'H';
int mode=0;

Player player_s;

void save_s();
void menu();
void arcade();
void function_gm();
void game_f(char mon_b, char mon_s, char chest, char player, char potion, char game[][40]);

void read_s(){
    ifstream stats("stats.txt");
    string line;

    while(getline(stats,line)){
        cout<<line<<endl;
    }  
}

void load_s(){
    ifstream load("stats.txt");
    string line;

     while(getline(load,line)){
        stringstream ss(line);
        string type;
        int value;
        
        ss>>type>>value;

        if(type=="Wins:"){
            win_s=value;
        }
        else if(type=="Loses:"){
            lose_s=value;
        }
    }

    load.close();
}

void load_m(char sv_game[][40]){
    ifstream load("map.txt");
    string line;
    int i=0;

    while(getline(load,line) && i<20){
        int leng = line.length();
        for(int j=0; j<40 && j<leng; j++){
            sv_game[i][j]=line[j];
        }
        i++;
    }  

    load.close();
}

void load_i(){
    ifstream load("info.txt");
    string line;

    while(getline(load,line)){
        stringstream ss(line);
        string type;
        int value;
        
        ss>>type>>value;

        if(type=="health"){
            player_s.health=value;
        }
        if(type=="lives"){
            player_s.lives=value;
        }
    }

    load.close();
}

void win(int i, int j, char movement, char game[][40]){
    int opt;
    if((movement=='R' && j<39 && game[i][j+1]=='C')||
       (movement=='L' && j>0 && game[i][j-1]=='C')||
       (movement=='U' && i>0 && game[i-1][j]=='C')||
       (movement=='D' && i<19 && game[i+1][j]=='C')){

        if(mode==1){
            win_s++;
            save_s();
            system("cls");
             cout<<"############\n";
             cout<<"# You win  #\n";
             cout<<"############\n";
             cin.ignore();
             getchar();

             while(true){
                system("cls");
                cout<<"############################\n";
                cout<<"# Do you want to continue? #\n";
                cout<<"# 1. Go back to the menu   #\n";
                cout<<"# 2. Exit.                 #\n";
                cout<<"############################\n";
                cin>>opt;
                    if(opt==2){
                        cout<<"Goodbye traveler, see you soon.";
                        getchar();
                        exit(0);
                    }
                    else if(opt==1){
                        cout<<"Going back";
                        system("cls");
                        menu();
                        break;
                    }
                    else{
                        system("cls");
                        cout<<"##################\n";
                        cout<<"# Invalid option #\n";
                        cout<<"##################\n";
                        getchar();
                    }
             }
        }
        else if(mode==2){
            if(cnt_mv<=10){point+=1000;}
            else{point+=300;}
            cnt_mv=0;
            cout<<"Your score: "<<point<<"\n";
            cout<<"Starting new round";
            cin.ignore();
            getchar();
            system("cls");
            function_gm();
        }

    }
}

void lose(Player loser){
    if(loser.lives==0){

        lose_s++;
        save_s();
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

    lose(player_s);

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

void lose_hp(char type){
    int hp_lost=0;

    if(type == 'M'){hp_lost=50;}
    else if(type == 'm'){hp_lost=25;}

       if(hp_lost>0){
            if(player_s.health>0){

                player_s.health-=hp_lost;

                if(player_s.health<=0){
                    restore_hp();
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

void monster_mov(int player_i, int player_j, char game[][40]){
    int i,j;
        for(i=0; i<20; i++){
            for(j=0; j<40; j++){
                if((game[i][j]=='M')||game[i][j]=='m'){

                    char type = game[i][j];
                    int new_i = i;
                    int new_j = j;
                    game[i][j] = ' ';
                    int direction = rand()%4;

                    switch(direction){
                        case 0:
                        if(i>0 && game[i-1][j]!='#'){new_i--;}
                        break;
                        case 1:
                        if(i<19 && game[i+1][j]!='#'){new_i++;}
                        break;
                        case 2:
                        if(j>0 && game[i][j-1]!='#'){new_j--;}
                        break;
                        case 3:
                        if(j<39 && game[i][j+1]!='#'){new_j++;}
                        break;
                    }

                    if(new_i==player_i && new_j==player_j){
                        game[new_i][new_j]='P';
                        lose_hp(type);
                    }
                    else if(game[new_i][new_j]=='H'){
                        game[i][j]=type;
                    }
                    else if(game[new_i][new_j]=='C'){
                        game[i][j]=type;
                    }
                    else if((game[new_i][new_j]=='M')||(game[new_i][new_j]=='m')){
                        game[i][j]=type;
                    }
                    else{game[new_i][new_j]=type;}
                }
            }
        }
}

void mov(char movement, char game[][40]){
    int i,j;
    char type = ' ';
    for(i=0;i<20;i++){
        for(j=0;j<40;j++){
            if(game[i][j]=='P'){
                if(movement == 'R' && j < 39 && (game[i][j+1] == 'M' || game[i][j+1] == 'm')){type = game[i][j+1];}
                else if(movement == 'L' && j > 0 && (game[i][j-1] == 'M' || game[i][j-1] == 'm')){type = game[i][j-1];}
                else if(movement == 'U' && i > 0 && (game[i-1][j] == 'M' || game[i-1][j] == 'm')){type = game[i-1][j];}
                else if(movement == 'D' && i < 19 && (game[i+1][j] == 'M' || game[i+1][j] == 'm')){type = game[i+1][j];}

                point+=10;
                cnt_mv++;
                lose_hp(type);
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

void save_f(char game[][40]){
    ofstream save;
    save.open("map.txt");
    for(int i=0;i<20;i++){
        string line;
        for(int j=0; j<40; j++){
            line+=game[i][j];
        }
        save<<line<<"\n";
    }

    save.close();
}

void save_i(){
    ofstream save;
    save.open("info.txt");

    save<<"health"<<" "<<player_s.health<<endl;
    save<<"lives"<<"  "<<player_s.lives;

    save.close();
}

void save_s(){
    ofstream save;
    save.open("stats.txt");

    save<<"Wins: "<<win_s<<endl;
    save<<"Loses: "<<lose_s;

    save.close();
}

void options(char game[][40]){
    int opt;
    system("cls");
    cout<<"####################\n";
    cout<<"# 1. Save and exit #\n";
    cout<<"# 2. Save          #\n";
    cout<<"# 3. Back          #\n";
    cout<<"####################\n";

    cin>>opt;

    switch(opt){
        case 1:
        save_f(game);
        save_i();
        cout<<"Goodbye traveler, see you soon";
        cin.ignore();
        getchar();
        exit(0);
        break;

        case 2:
        save_f(game);
        save_i();
        cout<<"###################################\n";
        cout<<"# Your game was succesfully saved #\n";
        cout<<"###################################\n";
        cin.ignore();
        getchar();
        break;
    }
}

void first_opt(char game[][40]){
        char Movement;
        int player_i, player_j;
        while(true){
            print(game);
            cout<<"Type your movement: ";cin>>Movement;
            Movement=toupper(Movement);
            mov(Movement, game);

            for(int i=0; i<20; i++){
                for(int j=0; j<40; j++){
                    if(game[i][j]=='P'){
                        player_i = i;
                        player_j = j;
                        break;
                    }
                }
            }

            if (Movement=='O'){
                options(game);
            }
            if (Movement=='X'){
                system("cls");
                menu();
            }

            monster_mov(player_i, player_j, game);
            system("cls");
        };
}

void history(){
    cout<<"###################################\n";
    cout<<"# -One day the monsters attacked- #\n";
    cout<<"#                                 #\n";
    cout<<"#     m          MMM         m    #\n";
    cout<<"#             MM     MM           #\n";
    cout<<"#   m       M          M          #\n";
    cout<<"#       m   M     P    M     m    #\n";
    cout<<"#           M          M    m     #\n";
    cout<<"#      m      MM     MM           #\n";
    cout<<"#                MMM          m   #\n";
    cout<<"###################################\n";
    cin.ignore();
    getchar();
    system("cls");

    cout<<"#######################################################################\n";
    cout<<"# -You as the last of your kind, had to look out for the magic chest- #\n";
    cout<<"# -in order to revive all of your kind and to vanish the monsters-    #\n";
    cout<<"#                                                                     #\n";
    cout<<"#                                                                     #\n";
    cout<<"#                                           GGG                       #\n";
    cout<<"#                                         G     G                     #\n";
    cout<<"#                      P _________________   C  G                     #\n";
    cout<<"#                                         G     G                     #\n";
    cout<<"#                                           GGG                       #\n";
    cout<<"#                                                                     #\n";
    cout<<"#######################################################################\n";
    getchar();
    system("cls");

    cout<<"#####################################################################\n";
    cout<<"# -On your journey you will find healing potionts that will restor- #\n";
    cout<<"# -Hp,but you can also take them without needing them, and that-    #\n";
    cout<<"# -and that would trully be a shame-                                #\n";
    cout<<"#                                                                   #\n";
    cout<<"#                                      H                            #\n";
    cout<<"#                 P                      H                          #\n";
    cout<<"#                                    H                              #\n";
    cout<<"#                                        H                          #\n";
    cout<<"#                                                                   #\n";
    cout<<"#####################################################################\n";
    getchar();
    system("cls");

    cout<<"#######################################################\n";
    cout<<"# -Be cautious on your steps, monsters can be tricky- #\n";
    cout<<"# -they may attack you twice in a row so be careful-  #\n";
    cout<<"#######################################################\n";
    getchar();
    system("cls");

    cout<<"#####################\n";
    cout<<"# -R to move right- #\n";
    cout<<"# -L to move left-  #\n";
    cout<<"# -U to move up-    #\n";
    cout<<"# -D to move down-  #\n";
    cout<<"#                   #\n";
    cout<<"# -X main menu-     #\n";
    cout<<"# -O to options-    #\n";
    cout<<"#####################\n";
    getchar();
    system("cls");
}

void menu(){
    int opt;

    cout<<"#############################\n";
    cout<<"#                           #\n";
    cout<<"#           DnD game        #\n";
    cout<<"#                           #\n";
    cout<<"#         1.New game        #\n";
    cout<<"#         2.Load game       #\n";
    cout<<"#         3.Stats           #\n";
    cout<<"#         4.Exit            #\n";
    cout<<"#                           #\n";
    cout<<"#############################\n";
    cin>>opt;

        do{

            switch(opt){
            case 1:
                system("cls");
                cout<<"###################\n";
                cout<<"# 1. History mode #\n";
                cout<<"# 2. Arcade mode  #\n";
                cout<<"###################\n";
                cin>>mode;

                if(mode==1){
                    history();
                    function_gm();
                }
                else if(mode==2){
                    arcade();
                }
                
            break;

            case 2:
                char sv_game[20][40];
                system("cls");
                load_m(sv_game);
                load_i();
                first_opt(sv_game);
            break;

            case 3:
                system("cls");
                read_s();
                system("pause");
            break;

            case 4:
                cout<<"Goodbye traveler, see you soon.";
                cin.ignore();
                getchar();
                exit(0);
            break;

            default:
                cout<<"Invalid option";
            break;
        }
        system("cls");
    }while(opt!=4);
}

void arcade(){
    do{
        system("cls");
        function_gm();
    }while(player_s.lives>0);
}

void function_gm(){
    char game[20][40];
    game_f(Monster_b, Monster_s, Chest, Player_p, Potion, game);
    first_opt(game);
}

int main(){

    load_s();
    srand(time(0));
    menu();

}