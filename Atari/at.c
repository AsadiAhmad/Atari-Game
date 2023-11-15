#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int crash_tank (int x_airplane,int y_airplane,int x_tank,int y_tank);
int crash_jet (int x_airplane, int y_airplane, int x_jet, int y_jet);
int crash_ferry (int x_airplane, int y_airplane, int x_ferry, int y_ferry);

struct window{ // this is struct code for initializing windows
    WINDOW *login_page;
    WINDOW *win1;
    WINDOW *win2;
    WINDOW *win3;
    WINDOW *win4;
    WINDOW *win_end;
};
struct id{ // this is struct code for initializing score
    int score;
};

void creat_border(struct window d){ // this function creat border for game page
    d.win1 = newwin (62, 120, 0, 0);
    d.win2 = newwin (62, 37, 0, 120);
    d.win3 = newwin (62, 15 , 0, 0);
    d.win4 = newwin (62, 15, 0, 105);
    refresh();
    box (d.win1, 0, 0);
    box (d.win2, 0, 0);
    box (d.win3, 0, 0);
    box (d.win4, 0, 0);
    wrefresh(d.win1);
    wrefresh(d.win2);
    wrefresh(d.win3);
    wrefresh(d.win4);
}
void print_airplane_mod_1 (int y_airplane, int x_airplane){ // this function prints airplane
    mvprintw (y_airplane    , x_airplane + 3,    ".  A  _");
    mvprintw (y_airplane + 1, x_airplane    , "(00000000000)");
    mvprintw (y_airplane + 2, x_airplane + 3,    "0  0  0");
    mvprintw (y_airplane + 3, x_airplane + 6,       "0");
    mvprintw (y_airplane + 4, x_airplane + 5,      "<0>");
    mvprintw (y_airplane + 5, x_airplane + 6,       "0");         
}
void print_airplane_mod_2 (int y_airplane, int x_airplane){
    mvprintw (y_airplane    , x_airplane + 3,    "_  A  .");
    mvprintw (y_airplane + 1, x_airplane    , "(00000000000)");
    mvprintw (y_airplane + 2, x_airplane + 3,    "0  0  0");
    mvprintw (y_airplane + 3, x_airplane + 6,       "0");
    mvprintw (y_airplane + 4, x_airplane + 5,      "<0>");
    mvprintw (y_airplane + 5, x_airplane + 6,       "0");
}
void delete_airplane (int y_airplane, int x_airplane){
    mvprintw (y_airplane    , x_airplane + 3,    "       ");
    mvprintw (y_airplane + 1, x_airplane    , "             ");
    mvprintw (y_airplane + 2, x_airplane + 3,    "       ");
    mvprintw (y_airplane + 3, x_airplane + 6,       " ");
    mvprintw (y_airplane + 4, x_airplane + 5,      "   ");
    mvprintw (y_airplane + 5, x_airplane + 6,       " ");    
}
void print_airplane (int x_airplane, int y_airplane, struct window w){
    print_airplane_mod_1(x_airplane, y_airplane);
    refresh(); 
}
void print_tank (int y_tank, int x_tank){ // this function prints tank with green color
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron (COLOR_PAIR(1));
    mvprintw (y_tank    , x_tank + 2,   "/TTT====");
    mvprintw (y_tank + 1, x_tank    , "(0=0=0=0)");
    attroff (COLOR_PAIR(1));
}
void delete_tank (int y_tank, int x_tank){ // this function prints space on old tank so old tank will delete
    mvprintw (y_tank    , x_tank + 2,   "        ");
    mvprintw (y_tank + 1, x_tank    , "         ");
}
void print_ferry (int y_ferry, int x_ferry){ //this function prints ferry with blue color
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    attron (COLOR_PAIR(1));
    mvprintw (y_ferry    , x_ferry + 4,    "||");
    mvprintw (y_ferry + 1, x_ferry + 4,    "||");
    mvprintw (y_ferry + 2, x_ferry    ,"\\TT\\II_");
    mvprintw (y_ferry + 3, x_ferry + 1,  "\\____/");
    attroff (COLOR_PAIR(1));    
}
void delete_ferry (int y_ferry, int x_ferry){ // this function prints space on old ferry so old ferry will delete
    mvprintw (y_ferry    , x_ferry + 4,  "  ");
    mvprintw (y_ferry + 1, x_ferry + 4,  "  ");
    mvprintw (y_ferry + 2, x_ferry    ,"       ");
    mvprintw (y_ferry + 3, x_ferry + 1, "      ");     
}
void print_jet (int y_jet, int x_jet){//this function prints jet with magenta color 
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    attron (COLOR_PAIR(1));
    mvprintw (y_jet    , x_jet + 4,    "A");
    mvprintw (y_jet + 1, x_jet + 1, "__/0\\__");
    mvprintw (y_jet + 2, x_jet    ,"/___0___\\"); 
    mvprintw (y_jet + 3, x_jet + 4,    "0");
    mvprintw (y_jet + 4, x_jet + 3,   "/_\\");
    attroff (COLOR_PAIR(1));
}
void delete_jet (int y_jet, int x_jet){ // this function prints space on old jet so old jet will delete
    mvprintw (y_jet    , x_jet + 4,    " ");
    mvprintw (y_jet + 1, x_jet + 1, "       ");
    mvprintw (y_jet + 2, x_jet    ,"         "); 
    mvprintw (y_jet + 3, x_jet + 4,    " ");
    mvprintw (y_jet + 4, x_jet + 3,   "   ");
}
int random1 (){ // this function creat random number for select random enemy
    int random_number = rand()%3;
    random_number++;
    return random_number;
}
int random2 (){ // this function creat random number for print enemy randomly
    int random_number = rand()%81;
    random_number = random_number +15;
    return random_number;
} // this function is heart of game that name is game play sorry for bad writing 
  //this function. it will get airplane and enemy data chose a random enemy 
  // printing and controlling airplane and enemies and at the end return your score
int game_play (int y_airplane, int x_airplane, int y_tank, int x_tank, int y_ferry, int x_ferry, int y_jet, int x_jet, struct window w, struct id i){
    i.score = 0;
    while (crash_tank(y_airplane, x_airplane, y_tank, x_tank) == 0){
    y_tank = 1;
    y_ferry = 1;
    y_jet = 1;
    int Q;
    int random_number1 = random1(); 
    int random_number2 = random2();
    char key;
    if (random_number1 == 1) {
        x_tank = random_number2;
        y_tank = 1;
        while (crash_tank(y_airplane, x_airplane, y_tank, x_tank) == 0){
            i.score++;
            mvprintw (10, 121, "%d",i.score);            
            print_tank (y_tank, x_tank);
            refresh();
            for (int i=10; i>0; i--){
                Q = 20;
                timeout(0);
                usleep(10000);
                while (Q > 0){
                    Q--;
                    key = getch();
                    switch (key)
                    {
                        case 'D':
                        case 'd':
                        if (x_airplane < 92){
                            delete_airplane(y_airplane, x_airplane);            
                            x_airplane++;
                            print_airplane_mod_1 (y_airplane, x_airplane);
                        }
                        break;
                        case 'A':
                        case 'a':
                        if (x_airplane > 15){
                            delete_airplane(y_airplane, x_airplane);
                            x_airplane --;
                            print_airplane_mod_1 (y_airplane, x_airplane);
                        }
                        break;
                    }
                }
            }
            delete_tank(y_tank, x_tank);
            y_tank++;
            print_tank (y_tank, x_tank);
            if (y_tank > 58){
            delete_tank (y_tank, x_tank);                
            break;
            }
        }
        if (crash_tank(y_airplane, x_airplane, y_tank, x_tank) == 1){
            break;
        }
    }
        if (random_number1 == 2) {
        x_ferry = random_number2;
        y_ferry = 1;
        while (crash_ferry(y_airplane, x_airplane, y_ferry, x_ferry) == 0){
            i.score++;
            mvprintw (10, 121, "%d",i.score);            
            print_ferry (y_ferry, x_ferry);
            refresh();
            for (int i=10; i>0; i--){
                Q = 20;
                timeout(0);
                usleep(10000);
                while (Q > 0){
                    Q--;
                    key = getch();
                    switch (key)
                    {
                        case 'D':
                        case 'd':
                        if (x_airplane < 92){
                            delete_airplane(y_airplane, x_airplane);            
                            x_airplane++;
                            print_airplane_mod_1 (y_airplane, x_airplane);
                        }
                        break;
                        case 'A':
                        case 'a':
                        if (x_airplane > 15){
                            delete_airplane(y_airplane, x_airplane);
                            x_airplane --;
                            print_airplane_mod_1 (y_airplane, x_airplane);
                        }
                        break;
                    }
                }
            }
            delete_ferry(y_ferry, x_ferry);
            y_ferry++;
            print_ferry (y_ferry, x_ferry);
            if (y_ferry > 56){
            delete_ferry (y_ferry, x_ferry);                
            break;
            }
        }
        if (crash_ferry(y_airplane, x_airplane, y_ferry, x_ferry) == 1){
            break;
        }
    }
        if (random_number1 == 3) {
        x_jet = random_number2;
        y_jet = 1;
        while (crash_jet(y_airplane, x_airplane, y_jet, x_jet) == 0){
            i.score++;
            mvprintw (10, 121, "%d",i.score);
            print_jet (y_jet, x_jet);
            refresh();           
            for (int i=10; i>0; i--){
                Q = 20;
                timeout(0);
                usleep(10000);
                while (Q > 0){
                    Q--;
                    key = getch();
                    switch (key)
                    {
                        case 'D':
                        case 'd':
                        if (x_airplane < 92){
                            delete_airplane(y_airplane, x_airplane);            
                            x_airplane++;
                            print_airplane_mod_1 (y_airplane, x_airplane);
                        }
                        break;
                        case 'A':
                        case 'a':
                        if (x_airplane > 15){
                            delete_airplane(y_airplane, x_airplane);
                            x_airplane --;
                            print_airplane_mod_1 (y_airplane, x_airplane);
                        }
                        break;
                    }
                }
            }
            delete_jet(y_jet, x_jet);
            y_jet++;
            print_jet (y_jet, x_jet);
            if (y_jet > 55){
            delete_jet (y_jet, x_jet);                
            break;
            }
        }
        if (crash_jet(y_airplane, x_airplane, y_jet, x_jet) == 1){
            break ;
        }
    }
    } 
    return i.score;  
} // this function reforming airplane has an accident with tank if yes return 1 else return 0
int crash_tank(int y_airplane,int x_airplane,int y_tank,int x_tank){
    int y_airplane2 = y_airplane, x_airplane2 = x_airplane;
    int y_tank2 = y_tank, x_tank2 = x_tank;
    for (int leny2=2; leny2 > 0; leny2--){
        for (int lenx2=10; lenx2>0; lenx2--){
            for (int leny1=6; leny1>0; leny1--){
                for (int lenx2=13; lenx2>0; lenx2--){
                    if (x_airplane2 == x_tank2 && y_airplane2 == y_tank2){
                        return (1);
                    }
                    x_airplane2 ++;
                }
                x_airplane2 = x_airplane;
                y_airplane2 ++;                
            }
            y_airplane2 = y_airplane;
            x_tank2++;            
        }
        x_tank2 = x_tank;
        y_tank2++;
    }
    return (0);            
} // this function reforming airplane has an accident with ferry if yes return 1 else return 0
int crash_ferry (int y_airplane, int x_airplane, int y_ferry, int x_ferry){
    int y_airplane2 = y_airplane, x_airplane2 = x_airplane; 
    int y_ferry2 = y_ferry, x_ferry2 = x_ferry; 
    for (int leny2=4; leny2 > 0; leny2--){
        for (int lenx2=7; lenx2>0; lenx2--){
            for (int leny1=6; leny1>0; leny1--){
                for (int lenx2=13; lenx2>0; lenx2--){
                    if (x_airplane2 == x_ferry2 && y_airplane2 == y_ferry2){
                        return (1);
                    }
                    x_airplane2 ++;
                }
                x_airplane2 = x_airplane;
                y_airplane2 ++;
            }
            y_airplane2 = y_airplane;
            x_ferry2++;
        }
        x_ferry2 = x_ferry;
        y_ferry2++;
    }
    return (0);    
} // this function reforming airplane has an accident with jet if yes return 1 else return 0
int crash_jet (int y_airplane, int x_airplane, int y_jet, int x_jet){
    int y_airplane2 = y_airplane, x_airplane2 = x_airplane;
    int y_jet2 = y_jet, x_jet2 = x_jet;    
    for (int leny2=5; leny2 > 0; leny2--){
        for (int lenx2=9; lenx2>0; lenx2--){
            for (int leny1=6; leny1>0; leny1--){
                for (int lenx2=13; lenx2>0; lenx2--){
                    if (x_airplane2 == x_jet2 && y_airplane2 == y_jet2){
                        return (1);
                    }
                    x_airplane2 ++;
                }
                x_airplane2 = x_airplane;
                y_airplane2 ++;
            }
            y_airplane2 = y_airplane;
            x_jet2++;
        }
        x_jet2 = x_jet;
        y_jet2++;
    } 
    return (0);   
} // this two function acctions when you lose game its preety beutifull with 3D text with red color
void print_game_over_1 (void){
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron (COLOR_PAIR(1));
    mvprintw (5, 10, "    _________               ___              ___            ___      __________   ");
    mvprintw (6, 10, "   / _______ \\             / _ \\            |   \\          /   |\\   |  ________|\\  ");
    mvprintw (7, 10, "  / /\\______\\ \\           / / \\ \\           | |\\ \\        / /| | |  | |\\________\\|  ");
    mvprintw (8, 10, " / / /       \\_\\         / / / \\ \\          | | \\ \\      / / | | |  | | |           ");
    mvprintw (9, 10, "[ | |         \\_|       / / /   \\ \\         | | |\\ \\    / / /| | |  | |_______     ");
    mvprintw (10, 10, "[ | |     _______      / /_______\\ \\        | | | \\ \\  / / / | | |  |  ______|\\     ");
    mvprintw (11, 10, "[ | |    |___  / |    / __________\\ \\       | | |  \\ \\/ / /  | | |  | |\\______\\|    ");
    mvprintw (12, 10, " \\ \\|       / / /    / /\\__________\\ \\      | | |   \\__/ /   | | |  | | |            ");
    mvprintw (13, 10, "  \\ \\______/ / /    / / /           \\ \\     | | |    \\_\\/    | | |  | |________     ");
    mvprintw (14, 10, "   \\________/ /    /_/ /             \\_\\    |_| |            |_| |  |__________|\\   ");
    mvprintw (15, 10, "    \\_______\\/     \\_\\/               \\_\\   \\_\\              \\_\\    \\__________\\|  ");
    attroff (COLOR_PAIR(1));    
}
void print_game_over_2 (void){
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron (COLOR_PAIR(1));
    mvprintw (20, 70, "      ________       __              __    __________      _______   ");
    mvprintw (21, 70, "     / ______ \\     /\\ \\            / /   /|  _______|    /|  ____ \\  ");
    mvprintw (22, 70, "    / /_____/\\ \\    \\ \\ \\          / /   | | |_______/   | | |    \\ \\  ");
    mvprintw (23, 70, "   / /      \\ \\ \\    \\ \\ \\        / /    | | |           | | |     | |  ");
    mvprintw (24, 70, " /| |        \\/| |    \\ \\ \\      / /     | | |_______    | | |____/ /  ");
    mvprintw (25, 70, "| | |        | | |     \\ \\ \\    / /      | |  _______|   | |  _  __/   ");
    mvprintw (26, 70, "| | |        | | |      \\ \\ \\  / /       | | |_______/   | | | \\ \\   ");
    mvprintw (27, 70, " \\ \\ \\        / /        \\ \\ \\/ /        | | |           | | |\\ \\ \\  ");
    mvprintw (28, 70, "  \\ \\ \\______/ /          \\ \\  /         | | |________   | | | \\ \\ \\   ");
    mvprintw (29, 70, "   \\ \\________/            \\ \\/          | |__________|  | |_|  \\ \\_\\  ");
    mvprintw (30, 70, "    \\/_______/              \\/           |/___________/  |/_/    \\/_/   ");   
    attroff (COLOR_PAIR(1)); 
} // when you lose game this function print your score in the last page and has a guide for exiting game
char end_win (struct window w, int score, int high_score){
    char key;
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    delwin(w.win1);
    delwin(w.win2);
    delwin(w.win3);
    delwin(w.win4);
    refresh();
    w.win_end = newwin (62, 157, 0, 0);
    refresh();
    box (w.win_end, 0, 0);
    wrefresh(w.win_end);
    print_game_over_1();
    print_game_over_2();
    attron (COLOR_PAIR(1));
    mvprintw (39, 60, "press q to exit game!");
    mvprintw (41 ,60, "press r to restart game!");
    attroff (COLOR_PAIR(1));
    while (key = getch()){
        attron (COLOR_PAIR(1));
        mvprintw (35, 60, "your score is:%d",score);
        mvprintw (37, 60, "your high score is:%d",high_score);         
        attroff (COLOR_PAIR(1));
        if (key == 'r' || key == 'q'){
            return key;
        }

    }
} // this function creat login page for game
void creat_login_page(WINDOW *win_login_page){
    win_login_page = newwin (62 , 157, 0, 0);
    refresh();
    box (win_login_page, 0, 0);
    wrefresh (win_login_page);
} // this function print some text
void printer (void){
    mvprintw(30, 60,"wellcome to atary game!");
    mvprintw(32, 60,"please enter your name :");
    mvprintw(34, 60,"please enter your user name :");
    mvprintw(36, 60,"please enter your age :");

}
void scaner (void){
    char enter;
    char name[30];
    char user_name[30];
    char age[4];
    move(32, 85);
    while (enter != '\n' && enter != '\r'){
        enter = getchar();
    }
    mvscanw(32, 85, "%s", &name[30]);
    move(34, 90);
    while (enter != '\n' && enter != '\r'){
        enter = getchar();
    }
    mvscanw(34, 90, "%s", &user_name[30]);
    move(36, 84);
    while (enter != '\n' && enter != '\r'){
        enter = getchar();
    }
    mvscanw(36, 84, "%s", &age[4]);
}

void login(void){
    struct window w;
    creat_login_page(w.login_page);
    printer();
    scaner();
}
void game(void){
    int high_score = 0;
    char key2 = 'r';
    struct window w;
    struct id i;
    char key ;
    int x_airplane;int y_airplane;
    int x_tank;int y_tank;
    int x_ferry;int y_ferry;
    int x_jet;int y_jet;
    int score; 
    srand(time(NULL));
    x_airplane = 60;
    y_airplane = 55;
    initscr();
    start_color();
    login(); 
    delwin (w.login_page);
    while (key2 == 'r'){
        curs_set(0);
        noecho();    
        creat_border(w);
        print_airplane(y_airplane, x_airplane, w);
        score = game_play(y_airplane, x_airplane, x_tank, y_tank, x_ferry, y_ferry, x_jet, y_jet, w, i);
        if (high_score < score){
            high_score = score;
        }
        key2 = end_win(w, score, high_score);
        getch();         
        if (key2 == 'q'){
            delwin(w.win_end);
            endwin();            
            break;
        }
    }
}
int main(){
    game();
    return 0;
}