#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Player{
    int lives=2;
    int health=100;
};

sf::RenderWindow window(sf::VideoMode(800, 600), "DnD Game");
sf::Font font;
int win_s=0;
int lose_s=0;
int point=0;
int temp_s=0;
int cnt_mv=0;
char Monster_b = 'M';
char Monster_s = 'm';
char Chest = 'C';
char Player_p = 'P';
int player_i=0;
int player_j=0;
char Potion = 'H';
int mode=0;

Player player_s;

void menu();
void gameshow();
void gameshow_s();
void game_f(char mon_b, char mon_s, char chest, char player, char potion, char game[][40]);

void verify_font(){
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
}

void center_text(sf::Text& text){
    sf::FloatRect textRect = text.getLocalBounds();
    float centerX = (window.getSize().x - textRect.width) / 2;
    float centerY = (window.getSize().y - textRect.height) / 2;
    text.setPosition(centerX, centerY); 
}

void exit_message(){
    sf::Text text;
    sf::Event event;

    verify_font();

    while(window.isOpen()){
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Num1){
                    menu();
                } else if(event.key.code==sf::Keyboard::Num2){
                    window.close();
                }
            }
        }
        window.clear();
        text.setFont(font);
        text.setCharacterSize(24);
        text.setString("1. Back to menu\n2. Exit");

        center_text(text); 

        window.draw(text);
        window.display();
    }
}

void read_s(){
    sf::Text text;
    sf::Event event;

    ifstream stats("stats.txt");
    string line;
    vector<string>message;
    int i=0;
    while(getline(stats,line)){
        message.push_back(line);
    } 
    while(window.isOpen()){
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Enter){
                    i++;
                    if(i>=message.size()){
                        menu();
                    }
                }
            }
        }

        window.clear();

        text.setFont(font);
        text.setCharacterSize(24);
        text.setString(message[i]);

        center_text(text); 

        window.draw(text);
        window.display();
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
    save<<"Loses: "<<lose_s<<endl;

    if(point>=temp_s){
        save<<"MAX: "<<point;
    } 

    save.close();
}

void print(char game[][40]){
    int i, f;
    for(i=0; i<20; i++){
        for(f=0; f<40; f++){
            cout<<game[i][f];
        }
        cout<<"\n";
    }
}

void win(int i, int j, char movement, char game[][40]){

    sf::Text text;
    sf::Event event;
    
    if((movement=='R' && j<39 && game[i][j+1]=='C')||
       (movement=='L' && j>0 && game[i][j-1]=='C')||
       (movement=='U' && i>0 && game[i-1][j]=='C')||
       (movement=='D' && i<19 && game[i+1][j]=='C')){

        if(mode==1){
            win_s++;
            save_s();

            while(window.isOpen()){
                while(window.pollEvent(event)){
                    if(event.type==sf::Event::Closed){
                        window.close();
                    }
                    if(event.type==sf::Event::KeyPressed){
                        if(event.key.code==sf::Keyboard::Enter){
                            exit_message();
                        }
                    }
                }
                window.clear();

                text.setFont(font);
                text.setCharacterSize(24);
                text.setString("You are now the hero of your kind and a legend");

                center_text(text); 

                window.draw(text);
                window.display();
            }
        }
        else if(mode==2){
            if(cnt_mv<=10){point+=1000;}
            else{point+=300;}
            cnt_mv=0;

            vector<string>message={"Your score is: "+to_string(point),
                                   "Starting new round"};
            int messageindex=0;

            while(window.isOpen()){
                while(window.pollEvent(event)){
                    if(event.type==sf::Event::Closed){
                        window.close();
                    }
                    if(event.type==sf::Event::KeyPressed){
                        if(event.key.code==sf::Keyboard::Enter){
                            messageindex++;
                        }
                    }
                }
                if(messageindex>=message.size()){gameshow();}
                window.clear();

                text.setFont(font);
                text.setCharacterSize(24);
                text.setString(message[messageindex]);

                center_text(text); 

                window.draw(text);
                window.display();
            }
        }

    }
}

void win_hp(int i, int j, char movement, char game[][40]){

    sf::Text text;
    sf::Event event;

    verify_font();

    if(player_s.health<100 && ((movement=='R' && j<39 && game[i][j+1]=='H')||
       (movement=='L' && j>0 && game[i][j-1]=='H')||
       (movement=='U' && i>0 && game[i-1][j]=='H')||
       (movement=='D' && i<19 && game[i+1][j]=='H'))){

        if(player_s.health<75){player_s.health+=50;}
        else{player_s.health+=25;}

            vector<string>message={"You've won 50 hp",
                                   "Your current Hp is "+to_string(player_s.health)};
            int messageindex=0;

            while(window.isOpen()){
                while(window.pollEvent(event)){
                    if(event.type==sf::Event::Closed){
                        window.close();
                    }
                    if(event.type==sf::Event::KeyPressed){
                        if(event.key.code==sf::Keyboard::Enter){
                            messageindex++;
                        }
                    }
                }
                if(messageindex >= message.size()){break;}
                window.clear();

                text.setFont(font);
                text.setCharacterSize(24);
                text.setString(message[messageindex]);

                center_text(text); 

                window.draw(text);
                window.display();
            }
       }
}

void lose(Player loser){
    sf::Text text;
    sf::Event event;

    verify_font();

    if(loser.lives==0){

        lose_s++;
        save_s();

        vector<string>message={"No more lives remaining", "So bad, you have failed the quest and your kind now is doom"};
        int messageindex=0;

        while(window.isOpen()){
            while(window.pollEvent(event)){
                if(event.type==sf::Event::Closed){
                    window.close();
                }
                if(event.type==sf::Event::KeyPressed){
                    if(event.key.code==sf::Keyboard::Enter){
                        messageindex++;
                    }
                }
            }
            if(messageindex >= message.size()){exit_message();}
            window.clear();

            text.setFont(font);
            text.setCharacterSize(24);
            text.setString(message[messageindex]);

            center_text(text); 

            window.draw(text);
            window.display();
        }
        //exit_m();
        window.close();
    }
}

void restore_hp(){
    sf::Text text;
    sf::Event event;

    player_s.lives-=1;
    player_s.health=100;

    lose(player_s);

    vector<string> message={
        "You've lost one live. "+to_string(player_s.lives)+" remaining",
        "Your health has been restored"
    };
    int messageindex=0;
    while(window.isOpen()){
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Enter){
                    messageindex++;
                }
            }
        }
        if(messageindex >= message.size()){break;}
        window.clear();

        text.setFont(font);
        text.setCharacterSize(24);
        text.setString(message[messageindex]);

        center_text(text); 

        window.draw(text);
        window.display();
    }
}

void lose_hp(char type){
    int hp_lost=0;
    sf::Text text;
    sf::Event event;

    verify_font();

    if(type == 'M'){hp_lost=50;}
    else if(type == 'm'){hp_lost=25;}

       if(hp_lost>0){
            if(player_s.health>0){

                player_s.health-=hp_lost;

                if(player_s.health<=0){
                    restore_hp();
                }
                else{
                    vector<string>message={"You've lost: "+to_string(hp_lost), 
                                           "Your current hp is: "+to_string(player_s.health)};
                    int messageindex=0;

                    while(window.isOpen()){
                        while(window.pollEvent(event)){
                            if(event.type==sf::Event::Closed){
                                window.close();
                            }
                            if(event.type==sf::Event::KeyPressed){
                                if(event.key.code==sf::Keyboard::Enter){
                                    messageindex++;
                                }
                            }
                        }
                        if(messageindex >= message.size()){break;}
                        window.clear();

                        text.setFont(font);
                        text.setCharacterSize(24);
                        text.setString(message[messageindex]);

                        center_text(text); 

                        window.draw(text);
                        window.display();
                    }
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

                     player_i=i;
                     player_j=j;
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

void new_history(){
    sf::Text text;
    sf::Event event;

    verify_font();

    vector<string> message={
                "############################\n"
                "# -One day the monsters attacked- #\n"
                "#                                                    #\n"
                "#     m              MMM               m    #\n"
                "#                MM        MM                 #\n"
                "#   m       M                     M             #\n"
                "#       m   M          P         M     m     #\n"
                "#             M                     M    m      #\n"
                "#      m       MM        MM                 #\n"
                "#                     MMM          m          #\n"
                "############################\n",

                "#####################################################\n"
                "# -You as the last of your kind, had to look out for the magic chest-   #\n"
                "# -in order to revive all of your kind and to vanish the monsters-       #\n"
                "#                                                                                                       #\n"
                "#                                                                                                       #\n"
                "#                                                              GGG                                 #\n"
                "#                                                           G         G                             #\n"
                "#                      P _________________   C      G                             #\n"
                "#                                                           G         G                             #\n"
                "#                                                              GGG                                 #\n"
                "#                                                                                                       #\n"
                "####################################################\n",

                "####################################################\n"
                "# -On your journey you will find healing potionts that will restor-       #\n"
                "# -Hp,but you can also take them without needing them, and that-   #\n"
                "# - would trully be a shame-                                                            #\n"
                "#                                                                                                     #\n"
                "#                                                  H                                                #\n"
                "#                               P                      H                                            #\n"
                "#                                               H                                                    #\n"
                "#                                                     H                                              #\n"
                "#                                                                                                     #\n"
                "####################################################\n",

                "##########################################\n"
                "# -Be cautious on your steps, monsters can be tricky- #\n"
                "# -they may attack you twice in a row so be careful-   #\n"
                "##########################################\n",

                "##################\n"
                "# -D to move right-     #\n"
                "# -A to move left-       #\n"
                "# -w to move up-        #\n"
                "# -s to move down-    #\n"
                "#                                #\n"
                "# -X main menu-        #\n"
                "# -O to options-          #\n"
                "##################\n"

    };

    int messageIndex=0; 

    while(window.isOpen()){
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }

            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Enter){
                    messageIndex++;
                    if(messageIndex>=message.size()){
                        gameshow();
                    }
                }
            }
        }

        window.clear();

        text.setFont(font);
        text.setCharacterSize(24);
        text.setString(message[messageIndex]);

        center_text(text); 

        window.draw(text);
        window.display();
    }
}

void menu_exit_message(){
    sf::Text text;

    verify_font();

    if(window.isOpen()){
    window.clear();
    
    text.setFont(font);
    text.setCharacterSize(24);
    text.setString("Goodbye traveler, see you soon");

    center_text(text);

    window.draw(text);
    window.display();
    sf::sleep(sf::seconds(1));
    window.close();
    }

}

void mini_menu(){
    sf::Text text;
    sf::Event event;

    if(!font.loadFromFile("arial.ttf")){
        cerr<<"Error loading font!"<<endl;
    }

    while(window.isOpen()){
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }

            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Num1){
                    mode=1;
                    new_history();
                } else if(event.key.code==sf::Keyboard::Num2){
                    mode=2;
                    gameshow();
                } else if(event.key.code==sf::Keyboard::Num3){
                    menu();
                }
            }
        }

        window.clear();

        text.setFont(font);
        text.setCharacterSize(24);
        text.setString(
            "################\n"
            "# 1. History mode   #\n"
            "# 2. Arcade mode  #\n"
            "# 3. Back               #\n"
            "################\n"
        );

        sf::FloatRect textRect = text.getLocalBounds();
        float centerX = (window.getSize().x - textRect.width) / 2;
        float centerY = (window.getSize().y - textRect.height) / 2;
        text.setPosition(centerX, centerY); 

        window.draw(text);
        window.display();
    }
}

void menu_options(char game[][40]){
    sf::Text text;
    sf::Event event;

    verify_font();
    bool closewindow=false;
    bool message=false;

    while(window.isOpen()&&!closewindow){
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Num1){
                    save_f(game);
                    save_i();
                    window.close();
                    }
                else if(event.key.code == sf::Keyboard::Num2){
                    save_f(game);
                    save_i();
                    closewindow=true;
                    message=true;
                }
                else if(event.key.code == sf::Keyboard::Num3){
                    closewindow=true;
                }
            }
        }
        window.clear();
        text.setFont(font);
        text.setCharacterSize(24);
        
        if(message){
            text.setString("Succesfully saved");
            center_text(text);
            window.draw(text);
            window.display();

            sf::sleep(sf::seconds(1));  
            break;
        } 
        else{
            text.setString("1. Save and Exit\n2. Save\n3. Back");
            center_text(text);
            window.draw(text);
            window.display();
        }

    }
}

void menu(){
    int opt;

    sf::Text tileText;
    sf::Event event;

    verify_font();

    while(window.isOpen()){
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    mini_menu();
                } else if (event.key.code == sf::Keyboard::Num2) {
                    gameshow_s();
                } else if (event.key.code == sf::Keyboard::Num3) {
                    read_s();
                } else if (event.key.code == sf::Keyboard::Num4) {
                    menu_exit_message();
                } 
            }
        }

        window.clear();

                tileText.setFont(font);
                tileText.setCharacterSize(24);
                tileText.setString( 
                                    "###################\n"
                                    "#                                  #\n"
                                    "#         DnD game         #\n"
                                    "#                                  #\n"
                                    "#       1. New game       #\n"
                                    "#       2. Load game      #\n"
                                    "#       3. Stats               #\n"
                                    "#       4. Exit                 #\n"
                                    "#                                 #\n"
                                    "##################\n");
                center_text(tileText);
        
        window.draw(tileText);

        window.display();
    }
}

void game_w(char game[][40]){
    verify_font();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Manejo de las teclas para mover al jugador
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::D) {mov('R', game);} 
                else if (event.key.code == sf::Keyboard::A) {mov('L', game);} 
                else if (event.key.code == sf::Keyboard::W) {mov('U', game);} 
                else if (event.key.code == sf::Keyboard::S) {mov('D', game);} 
                else if (event.key.code == sf::Keyboard::X) {menu();}
                else if (event.key.code == sf::Keyboard::O) {menu_options(game);}
            
                if((event.key.code==sf::Keyboard::W)||
                (event.key.code==sf::Keyboard::A)||
                (event.key.code==sf::Keyboard::S)||
                (event.key.code==sf::Keyboard::D)){
                    monster_mov(player_i, player_j, game);
                }
            }
        }

        window.clear();

        // Aquí dibujamos el mapa de juego con SFML
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 40; ++j) {
                // Asignar una celda del mapa a un carácter de SFML
                sf::Text tileText;
                tileText.setFont(font);
                tileText.setCharacterSize(24);
                tileText.setPosition(j * 20, i * 30);
                tileText.setString(game[i][j]);
                window.draw(tileText);
            }
        }

        window.display();
    }
}

void gameshow_s(){
    char game_s[20][40];
    load_m(game_s);
    load_i();
    game_w(game_s);
}

void gameshow(){
    char game[20][40];
    game_f(Monster_b, Monster_s, Chest, Player_p, Potion, game);
    game_w(game);
}

int main() {
    load_s();
    menu();
    return 0;
}
