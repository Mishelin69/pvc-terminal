#include "game.h"

int main() {

    Game* g = create_game(); 
    init_game(g);
    start_game(g);

    getch();
    endwin();

    return 0;
}

