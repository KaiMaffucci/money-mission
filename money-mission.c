// game.c
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 50000
#define LONGDELAY 9999999
#define MINX 0
#define MAXX 167
#define MINY 0
#define MAXY 42

// define constants for color pairings
#define TEXT_PAIR 1
#define PLAYER_PAIR 2
#define ENEMY_PAIR 3
#define MONEY_PAIR 4

typedef struct Enemys {
    int x;
    int y;
} enemy;

void movePlayer(int *x, int *y); // function prototype for movePlayer

void moveEnemy(int *x, int *y); // function prototype for moveEnemy

int main() {
    // Setup
    srand(time(NULL)); // sets the random seed based on the current time
    int playerx = 0, playery = 0, moneyx = 0, moneyy = 0, i = 0, alive = 1, lvl = 1, r;
    // sets up location of money
    moneyx = 83 + (rand() % 83);
    moneyy = 21 + (rand() % 21);
    enemy enemys[9]; // enemys array
    int enemysLength = sizeof(enemys) / sizeof(enemys[0]); // variable to hold length of array
    // positions each enemy randomly on the display
    for(i = 0; i <= enemysLength; i++) {
        enemys[i].x = ((rand() % (MAXX - 11)) + 11);
        enemys[i].y = ((rand() % (MAXY - 11)) + 11);
    }
    initscr(); // Initialize the window
    noecho(); // Don't echo any keypresses
    curs_set(FALSE); // Don't display a cursor
    keypad(stdscr, TRUE); // Accepts special keys as one character
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color(); // starts using color
    // initiate color pairs to be available to use
    init_pair(TEXT_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(PLAYER_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(ENEMY_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(MONEY_PAIR, COLOR_GREEN, COLOR_BLACK);
    // displays each enemy
    for(i = 0; i <= enemysLength; i++) {
        mvprintw(enemys[i].y, enemys[i].x, "X");
    }
    while (alive == 1) {
        clear();
        // display text for what level the player is on
        if ( lvl == 1 ) {
            attron(COLOR_PAIR(TEXT_PAIR));
            mvprintw(42, 0, "Level 1 ");
            attroff(COLOR_PAIR(TEXT_PAIR));
        }
        refresh();
        attron(COLOR_PAIR(PLAYER_PAIR));
        mvprintw(playery, playerx, "o"); // prints the player at correct x and y pos
        refresh();
        movePlayer(&playerx, &playery); // gets input from player
        mvprintw(playery, playerx, "o"); // prints the player at correct x and y pos
        refresh();
        attroff(COLOR_PAIR(PLAYER_PAIR));
        // moves each enemy
        attron(COLOR_PAIR(ENEMY_PAIR));
        for ( i = 0; i <= enemysLength; i++ ) {
            moveEnemy(&enemys[i].x, &enemys[i].y);
        }
        attroff(COLOR_PAIR(ENEMY_PAIR));
        refresh();
        // checks to see if the player has lost the game by touching an enemy
        for (i = 0; i <= enemysLength; i++) {
            if ( ( enemys[i].x == playerx ) && ( enemys[i].y == playery ) ) {
                alive = 0;
            }
        }
        attron(COLOR_PAIR(MONEY_PAIR));
        mvprintw(moneyy, moneyx, "$"); // displays the money
        refresh();
        attroff(COLOR_PAIR(MONEY_PAIR));
        // checks to see if the player got the money and displays win message if so
        if ( ( playerx == moneyx ) && ( playery == moneyy ) ) {
            clear();
            attron(COLOR_PAIR(TEXT_PAIR));
            mvprintw(18, 80, " _______ ");
            mvprintw(19, 80, "/       \\");
            mvprintw(20, 80, "|YOU WIN|");
            mvprintw(21, 80, "\\_______/");
            attroff(COLOR_PAIR(TEXT_PAIR));
            refresh();
            usleep(LONGDELAY);
            endwin();
            exit(0);
        }
        usleep(DELAY);
    }
    clear();
    // game over screen
    attron(COLOR_PAIR(TEXT_PAIR));
    mvprintw(18, 80, " _________ ");
    mvprintw(19, 80, "/         \\");
    mvprintw(20, 80, "|GAME OVER|");
    mvprintw(21, 80, "\\_________/");
    attroff(COLOR_PAIR(TEXT_PAIR));
    refresh();
    usleep(LONGDELAY);
    endwin(); // Restore normal terminal behavior
    return 0;
}

// function that checks if the user pressed a key and moves the player accordingly
void movePlayer(int *x, int *y) {
    int ch = getch();
    while(ch == KEY_UP) {
        mvprintw(*y, *x, " ");
        *y -= 1;
        mvprintw(*y, *x, "o");
        refresh();
        ch = getch();
    }
    while(ch == KEY_DOWN) {
        mvprintw(*y, *x, " ");
        *y += 1;
        mvprintw(*y, *x, "o");
        refresh();
        ch = getch();
    }
    while(ch == KEY_LEFT) {
        mvprintw(*y, *x, " ");
        *x -= 1;
        mvprintw(*y, *x, "o");
        refresh();
        ch = getch();
    }
    while(ch == KEY_RIGHT) {
        mvprintw(*y, *x, " ");
        *x += 1;
        mvprintw(*y, *x, "o");
        refresh();
        ch = getch();
    }
    // checks if the player has gone off the display and puts it back on
    if(*x < MINX) {
        *x = MINX;
    }
    if(*x > MAXX) {
        *x = MAXX;
    }
    if(*y < MINY) {
        *y = MINY;
    }
    if(*y > MAXY) {
        *y = MAXY;
    }
}

// a function that moves an enemy's coords randomly one direction
void moveEnemy(int *x, int *y) {
    mvprintw(*y, *x, " "); // clears previous location of enemy
    // moves enemy randomly
    int r = (rand() % 5);
    switch(r) {
        case 1:
            *x += 1;
            break;
        case 2:
            *x -= 1;
            break;
        case 3:
            *y += 1;
            break;
        case 4:
            *y -= 1;
            break;
        default:
            break;
    }
    // checks if the enemy has gone off the display and puts it back on
    if(*x < MINX) {
        *x = MINX;
    }
    if(*x > MAXX) {
        *x = MAXX;
    }
    if(*y < MINY) {
        *y = MINY;
    }
    if(*y > MAXY) {
        *y = MAXY;
    }
    mvprintw(*y, *x, "X");
    refresh();
}
