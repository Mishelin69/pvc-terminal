#include "game.h"
#include <malloc.h>

void init_ui(Game* g) {

    init_colors();
    g->current_clr_mode = COLOR_MODE_DEFAULT;
    switch_color_mode(g, COLOR_MODE_DEFAULT);

    if (!g) {
        return;
    }

    Pixel p = { ' ', COLOR_MODE_DEFAULT }; 
    for (int i = 0; i < g->screen_rows; ++i) {
        for (int j = 0; j < g->screen_cols; ++j) {
            g->screen_data[i*g->screen_cols + j] = p;
        }
    }

    p.c = '+';
    g->screen_data[0] = p;
    g->screen_data[g->screen_cols-1] = p;
    g->screen_data[(g->screen_rows-1)*g->screen_cols] = p;
    g->screen_data[(g->screen_rows-1)*g->screen_cols + g->screen_cols-1] = p;

    p.c = '-';
    for (int i = 1; i < g->screen_cols-1; ++i) {
        g->screen_data[i] = p;
        g->screen_data[(g->screen_rows-1)*g->screen_cols + i] = p;
    }

    p.c = '|';
    for (int i = 1; i < g->screen_rows-1; ++i) {
       g->screen_data[i*g->screen_cols] = p;
       g->screen_data[i*g->screen_cols + g->screen_cols-1] = p;
    }

    //UI BOTTOM SCREEN
    p.c = '~';
    for (int i = 1; i < g->screen_cols-1; ++i) {
        int TOP_OFFSET = g->screen_rows-1-1-UI_BOTTOM_SCREEN_SIZE;
        g->screen_data[TOP_OFFSET * g->screen_cols + i] = p;
    }

    p.c = '|';
    for (int i = 0; i < UI_BOTTOM_SCREEN_SIZE; ++i) {
        int TOP_OFFSET = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE + i;
        g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF] = p;
    }

    draw_lives(g);

    //LANES
    int free_rows = g->screen_rows - 2 - 1 - UI_BOTTOM_SCREEN_SIZE - 1;
    int free_lanes = free_rows / 4;
    g->lane_n = free_lanes;
    g->lanes = (Lane*) malloc(sizeof(Lane) * g->lane_n);

    for (int i = 0; i < free_lanes; ++i) {

        g->lanes[i] = create_lane((2 + i * 4), i);

        for (int j = 1; j < g->screen_cols-1; ++j) {
            Pixel p = { '.', COLOR_MODE_DEFAULT };
            g->screen_data[(1 + i * 4) * g->screen_cols + j] = p;
        }
    }

    for (int j = 1; j < g->screen_cols-1; ++j) {
        Pixel p = { '.', COLOR_MODE_DEFAULT };
        g->screen_data[(1 + free_lanes * 4) * g->screen_cols + j] = p;
    }

    int NEW_TURRET_OFFSET = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE + 1;

    for (int j = 1; j < BUY_NEW_TURRET_WINDOW_WIDTH; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + BNTWL] = p;
    }

    for (int j = 1; j < BUY_NEW_TURRET_WINDOW_WIDTH; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + BNTWL] = p;
    }


    p.c = '|';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + BUY_NEW_TURRET_WINDOW_WIDTH] = p; 

    p.c = '+';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + BUY_NEW_TURRET_WINDOW_WIDTH] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + BUY_NEW_TURRET_WINDOW_WIDTH] = p; 

    p.c = 'N';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 9] = p;
    p.c = 'E';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 10] = p;
    p.c = 'W';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 11] = p;
    p.c = ' ';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 12] = p;
    p.c = 'T';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 13] = p;
    p.c = 'U';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 14] = p;
    p.c = 'R';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 15] = p;
    p.c = 'R';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 16] = p;
    p.c = 'E';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 17] = p;
    p.c = 'T';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + BNTWL + 18] = p;
}

void draw_lives(Game* g) {

    if (!g) {
        return;
    }

    int TOP_OFFSET = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE + 2; 
    int ll_copy = g->lives_left;

    char digit1 = ll_copy % 10 + '0'; 
    ll_copy /= 10;

    char digit2 = ll_copy % 10 + '0'; 
    ll_copy /= 10;

    char digit3 = ll_copy % 10 + '0'; 
    ll_copy /= 10;

    Pixel p = { 'L', COLOR_MODE_DEFAULT };
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 14] = p;
    p.c = 'I';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 15] = p;
    p.c = 'V';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 16] = p;
    p.c = 'E';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 17] = p;
    p.c = 'S';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 18] = p;
    p.c = ' ';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 19] = p;
    p.c = 'L';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 20] = p;
    p.c = 'E';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 21] = p;
    p.c = 'F';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 22] = p;
    p.c = 'T';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 23] = p;
    p.c = ':';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 24] = p;
    p.c = ' ';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 25] = p;
    p.c = digit3;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 26] = p;
    p.c = digit2;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 27] = p;
    p.c = digit1;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 28] = p;
}

void draw_money(Game* g) {

    if (!g) {
        return;
    }

    int TOP_OFFSET = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE + 2; 
    int ll_copy = g->current_money;

    char digit1 = ll_copy % 10 + '0'; 
    ll_copy /= 10;

    char digit2 = ll_copy % 10 + '0'; 
    ll_copy /= 10;

    char digit3 = ll_copy % 10 + '0'; 
    ll_copy /= 10;

    Pixel p = { 'M', COLOR_MODE_DEFAULT };
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 34] = p;
    p.c = 'O';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 35] = p;
    p.c = 'N';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 36] = p;
    p.c = 'E';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 37] = p;
    p.c = 'Y';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 38] = p;
    p.c = ':';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 39] = p;
    p.c = ' ';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 40] = p;
    p.c = digit3;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 41] = p;
    p.c = digit2;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 42] = p;
    p.c = digit1;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF + 43] = p;
}

void draw_wawe_stat(Game* g) {

    if (!g) {
        return;
    }

    int TOP_OFFSET = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE + 2; 
    int init = g->init_el;
    int cur = g->wm.wawe_left;

    char digit1 = init % 10 + '0'; 
    init /= 10;

    char digit2 = init % 10 + '0'; 

    char digit3 = cur % 10 + '0'; 
    cur /= 10;

    char digit4 = cur % 10 + '0'; 

    Pixel p = { 'W', COLOR_MODE_DEFAULT };
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 34] = p;
    p.c = 'A';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 33] = p;
    p.c = 'W';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 32] = p;
    p.c = 'E';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 31] = p;
    p.c = ':';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 30] = p;
    p.c = ' ';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 29] = p;
    p.c = digit4;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 28] = p;
    p.c = digit3;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 27] = p;
    p.c = '/';
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 26] = p;
    p.c = digit2;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 25] = p;
    p.c = digit1;
    g->screen_data[TOP_OFFSET * g->screen_cols + UI_BOTTOM_RIGHT_PART_OFF - 24] = p;
}

void draw_game(Game* g) {

    if (!g) {
        return;
    }

    werase(g->window);
    ColorMode m = g->current_clr_mode;

    for (int i = 0; i < g->screen_rows; ++i) {
        for (int j = 0; j < g->screen_cols; ++j) {

            if (m != g->screen_data[i*g->screen_cols + j].m) {

                m = g->screen_data[i*g->screen_cols + j].m;
                switch_color_mode(g, m);
            }

            mvwprintw(g->window, i, j, "%c", g->screen_data[i*g->screen_cols + j].c);
        }
    }

    draw_lives(g);
    draw_money(g);
    draw_wawe_stat(g);
    if (g->turret_clicked) {
        draw_turret_menu(g);
    }
    doupdate(); 
}

void init_colors() {

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

    init_pair(12, COLOR_WHITE, COLOR_RED);
    init_pair(13, COLOR_GREEN, COLOR_BLUE);
    init_pair(14, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(15, COLOR_CYAN, COLOR_BLACK);
    init_pair(16, COLOR_BLUE, COLOR_GREEN);
    init_pair(17, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(18, COLOR_RED, COLOR_CYAN);
    init_pair(19, COLOR_WHITE, COLOR_BLUE);
}

void switch_color_mode(Game* g, ColorMode mode) {

    switch (g->current_clr_mode) {
        case COLOR_MODE_DEFAULT:
            wattroff(g->window, COLOR_PAIR(1));
            break;
        case COLOR_MODE_RED:
            wattroff(g->window, COLOR_PAIR(2));
            break;
        case COLOR_MODE_GREEN:
            wattroff(g->window, COLOR_PAIR(3));
            break;
        case COLOR_MODE_BLUE:
            wattroff(g->window, COLOR_PAIR(4));
            break;
        case COLOR_MODE_YELLOW:
            wattroff(g->window, COLOR_PAIR(5));
            break;
        case COLOR_MODE_CYAN:
            wattroff(g->window, COLOR_PAIR(6));
            break;
        case COLOR_MODE_MAGENTA:
            wattroff(g->window, COLOR_PAIR(7));
            break;
    }

    g->current_clr_mode = mode;

    switch (mode) {
        case COLOR_MODE_DEFAULT:
            wattron(g->window, COLOR_PAIR(1));
            break;
        case COLOR_MODE_RED:
            wattron(g->window, COLOR_PAIR(2));
            break;
        case COLOR_MODE_GREEN:
            wattron(g->window, COLOR_PAIR(3));
            break;
        case COLOR_MODE_BLUE:
            wattron(g->window, COLOR_PAIR(4));
            break;
        case COLOR_MODE_YELLOW:
            wattron(g->window, COLOR_PAIR(5));
            break;
        case COLOR_MODE_CYAN:
            wattron(g->window, COLOR_PAIR(6));
            break;
        case COLOR_MODE_MAGENTA:
            wattron(g->window, COLOR_PAIR(7));
            break;
    }
}

void draw_available_lane_spot_turret(Game* g, Lane* l) {

    if (!g || !l) {
        return;
    }

    Pixel p = { '+', COLOR_MODE_GREEN };

    for (int i = 0; i < TURRET_MODEL_HEIGHT; ++i) {
        int lane_i = l->offset_row + i;
        for (int j = l->left_offset; j < l->left_offset + TURRET_MODEL_WIDTH; ++j) {
            g->screen_data[lane_i*g->screen_cols + j] = p;
        }
    }
}

void undo_draw_avb_lane_spot_turret(Game *g, Lane* l) {

    if (!g || !l) {
        return;
    }

    Pixel p = { ' ', COLOR_MODE_DEFAULT };

    for (int i = 0; i < TURRET_MODEL_HEIGHT; ++i) {
        int lane_i = l->offset_row + i;
        for (int j = l->left_offset; j < l->left_offset + TURRET_MODEL_WIDTH; ++j) {
            g->screen_data[lane_i*g->screen_cols + j] = p;
        }
    }

}

void place_turret_on_lane(Game* g, Lane* l, ColorMode m) {

    draw_model(g->models[0], g->screen_data, m, l->offset_row, 
            l->left_offset, g->screen_rows, g->screen_cols);

    l->left_offset += 16;
}

bool setup_enemy_draw(Game* g, int enemy_id, int* row, int* left_off, int* hp, AsciiModel** m) {

    //branch 1 enemy normal, branch 2 enemy shield
    if (enemy_id < g->enemy_split_id) {

        Enemy e = g->wm.e_list[enemy_id].enemy;
        *hp = e.health;

        if (e.draw == false) {
            return false;
        }

        *row = e.position_row; 
        *left_off = e.position_col;
        *m = g->models[1];

    } else {

        int _i = enemy_id - g->enemy_split_id;
        EnemyShield es = g->wm.es_list[_i].enemy;
        *hp = es.health;

        if (es.draw == false) {
            return false;
        }

        *row = es.position_row; 
        *left_off = es.position_col;
        *m = g->models[2];
    }

    return true;
}

void undraw_enemies(Game* g) {

    if (!g || g->wm.wawe_left < 1 || !g->to_dispatch_list) {
        return;
    }

    for (int i = 0; i < g->wm.e_left; ++i) {
        undraw_enemy(g, i);
    }

    for (int i = 0; i < g->wm.es_left; ++i) {
        undraw_enemy(g, i + g->enemy_split_id);
    }
}

void draw_enemies(Game* g) {

    if (!g || g->wm.wawe_left < 1 || !g->to_dispatch_list) {
        return;
    }

    for (int i = 0; i < g->wm.e_left; ++i) {
        draw_enemy(g, i);
    }

    for (int i = 0; i < g->wm.es_left; ++i) {
        draw_enemy(g, i + g->enemy_split_id);
    }
}

void draw_enemy(Game* g, int enemy_id) {

    int row;
    int left_off;
    int hp;
    AsciiModel* m;

    if (setup_enemy_draw(g, enemy_id, &row, &left_off, &hp, &m) == false) {
        return;
    }

    wattron(g->window, COLOR_PAIR(10 + g->wm.current_wawe + hp));
    for (int i = 0; i < m->model_rows; ++i) {
        for (int j = 0; j < m->model_cols; ++j) {
            mvwprintw(g->window, row + i, left_off + j, "%c", m->model_data[i*m->model_cols + j]);
        }
    }
    
    wattroff(g->window, COLOR_PAIR(10 + g->wm.current_wawe + hp));
    switch_color_mode(g, COLOR_MODE_DEFAULT);
    wnoutrefresh(g->window);
}

void undraw_enemy(Game* g, int enemy_id) {

    int row;
    int left_off;
    AsciiModel* m;
    int hp;

    if (setup_enemy_draw(g, enemy_id, &row, &left_off, &hp, &m) == false) {
        return;
    }

    //draw D:
    //Have to do this because of my stupid design
    //But I guess it makes sense because its moving? :(
    for (int i = 0; i < m->model_rows; ++i) {
        for (int j = 0; j < m->model_cols; ++j) {
            mvwprintw(g->window, row + i, left_off + j, " ");
        }
    }
}

void undraw_turret(Game* g, Turret* t) {

    AsciiModel* m = g->models[0];

    undraw_model(g->models[0], g->screen_data, COLOR_MODE_DEFAULT, t->pos_row, 
            t->pos_col, g->screen_rows, g->screen_cols);
}

void draw_turret_menu(Game* g) {

    int NEW_TURRET_OFFSET = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE + 1;
 
    Pixel p = { '+', COLOR_MODE_DEFAULT };
    //MENU 1
    p.c = '-';
    for (int j = 1; j < UPGRADE_WIDTH; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET] = p;
    }
    p.c = '|';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET  + UPGRADE_WIDTH] = p; 

    p.c = '+';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET + UPGRADE_WIDTH] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET +  UPGRADE_WIDTH] = p; 

    p.c = 'U';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET] = p;
    p.c = 'P';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET] = p;
    p.c = 'G';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET] = p;
    p.c = 'R';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 4 + TURRET_MENU_OFFSET] = p;
    p.c = 'A';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 5 + TURRET_MENU_OFFSET] = p;
    p.c = 'D';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 6 + TURRET_MENU_OFFSET] = p;
    p.c = 'E';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 7 + TURRET_MENU_OFFSET] = p;

    //MENU 2
    p.c = '-';
    for (int j = 1; j < UPGRADE_WIDTH2; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET2] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH2; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET2] = p;
    }
    p.c = '|';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2  + UPGRADE_WIDTH2] = p; 

    p.c = '+';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2 + UPGRADE_WIDTH2] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2 +  UPGRADE_WIDTH2] = p; 

    p.c = 'B';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET2] = p;
    p.c = 'e';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET2] = p;
    p.c = 't';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET2] = p;
    p.c = 't';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 4 + TURRET_MENU_OFFSET2] = p;
    p.c = 'e';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 5 + TURRET_MENU_OFFSET2] = p;
    p.c = 'r';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 6 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 7 + TURRET_MENU_OFFSET2] = p;
    p.c = 'T';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 8 + TURRET_MENU_OFFSET2] = p;
    p.c = 'T';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 9 + TURRET_MENU_OFFSET2] = p;
    p.c = 'L';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 10 + TURRET_MENU_OFFSET2] = p;

    //MENU 3
    p.c = '-';
    for (int j = 1; j < UPGRADE_WIDTH3; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET3] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH3; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET3] = p;
    }

    p.c = '|';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3  + UPGRADE_WIDTH3] = p; 

    p.c = '+';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3 + UPGRADE_WIDTH3] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3 +  UPGRADE_WIDTH3] = p; 

    p.c = 'B';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET3] = p;
    p.c = 'e';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET3] = p;
    p.c = 't';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET3] = p;
    p.c = 't';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 4 + TURRET_MENU_OFFSET3] = p;
    p.c = 'e';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 5 + TURRET_MENU_OFFSET3] = p;
    p.c = 'r';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 6 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 7 + TURRET_MENU_OFFSET3] = p;
    p.c = 'd';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 8 + TURRET_MENU_OFFSET3] = p;
    p.c = 'a';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 9 + TURRET_MENU_OFFSET3] = p;
    p.c = 'm';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 10 + TURRET_MENU_OFFSET3] = p;
    p.c = 'a';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 11 + TURRET_MENU_OFFSET3] = p;
    p.c = 'g';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 12 + TURRET_MENU_OFFSET3] = p;
    p.c = 'e';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 13 + TURRET_MENU_OFFSET3] = p;

    //MENU 4
    p.c = '-';
    for (int j = 1; j < UPGRADE_WIDTH4; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET4] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH4; ++j) {
        Pixel p = { '-', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET4] = p;
    }

    p.c = '|';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4  + UPGRADE_WIDTH4] = p; 

    p.c = '+';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4 + UPGRADE_WIDTH4] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4 +  UPGRADE_WIDTH4] = p; 

    p.c = 'B';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET4] = p;
    p.c = 'T';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET4] = p;
    p.c = 'S';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET4] = p;

}

void undraw_turret_menu(Game* g) {

    int NEW_TURRET_OFFSET = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE + 1;
 
    Pixel p = { ' ', COLOR_MODE_DEFAULT };
    //MENU 1
    p.c = '-';
    for (int j = 1; j < UPGRADE_WIDTH; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET] = p;
    }
    p.c = ' ';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET  + UPGRADE_WIDTH] = p; 

    p.c = ' ';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET + UPGRADE_WIDTH] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET +  UPGRADE_WIDTH] = p; 

    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 4 + TURRET_MENU_OFFSET] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 5 + TURRET_MENU_OFFSET] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 6 + TURRET_MENU_OFFSET] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 7 + TURRET_MENU_OFFSET] = p;

    //MENU 2
    p.c = ' ';
    for (int j = 1; j < UPGRADE_WIDTH2; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET2] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH2; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET2] = p;
    }
    p.c = ' ';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2  + UPGRADE_WIDTH2] = p; 

    p.c = ' ';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2 + UPGRADE_WIDTH2] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET2 +  UPGRADE_WIDTH2] = p; 

    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 4 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 5 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 6 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 7 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 8 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 9 + TURRET_MENU_OFFSET2] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 10 + TURRET_MENU_OFFSET2] = p;

    //MENU 3
    p.c = ' ';
    for (int j = 1; j < UPGRADE_WIDTH3; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET3] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH3; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET3] = p;
    }

    p.c = ' ';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3  + UPGRADE_WIDTH3] = p; 

    p.c = ' ';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3 + UPGRADE_WIDTH3] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET3 +  UPGRADE_WIDTH3] = p; 

    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 4 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 5 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 6 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 7 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 8 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 9 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 10 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 11 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 12 + TURRET_MENU_OFFSET3] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 13 + TURRET_MENU_OFFSET3] = p;

    //MENU 4
    p.c = ' ';
    for (int j = 1; j < UPGRADE_WIDTH4; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[NEW_TURRET_OFFSET * g->screen_cols + j + TURRET_MENU_OFFSET4] = p;
    }

    for (int j = 1; j < UPGRADE_WIDTH4; ++j) {
        Pixel p = { ' ', COLOR_MODE_DEFAULT };
        g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + j + TURRET_MENU_OFFSET4] = p;
    }

    p.c = ' ';
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4] = p;
    g->screen_data[(1 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4  + UPGRADE_WIDTH4] = p; 

    p.c = ' ';
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4] = p;
    g->screen_data[(0 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4 + UPGRADE_WIDTH4] = p; 

    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4] = p;
    g->screen_data[(2 + NEW_TURRET_OFFSET) * g->screen_cols + TURRET_MENU_OFFSET4 +  UPGRADE_WIDTH4] = p; 

    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 1 + TURRET_MENU_OFFSET4] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 2 + TURRET_MENU_OFFSET4] = p;
    p.c = ' ';
    g->screen_data[(NEW_TURRET_OFFSET+1) * g->screen_cols + 3 + TURRET_MENU_OFFSET4] = p;

}
