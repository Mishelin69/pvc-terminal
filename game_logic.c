#include "game.h"
#include "malloc.h"
#include "time.h"
#include "unistd.h"
#include <stdlib.h>

Game* create_game() {

    Game* g = (Game*) malloc(sizeof(Game));

    if (!g) {
        return NULL;
    }

    return g;
}

void init_game(Game* g) {

    if (!g) {
        return;
    }

    g->window = initscr();
    srand(time(NULL));

    //windows settings
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    keypad(g->window, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    nodelay(stdscr, TRUE); //to make getch nonblocking
    getmaxyx(g->window, g->screen_rows, g->screen_cols);

    AsciiModel* cannon = load_model("./cannon_model.dat");
    AsciiModel* enemy = load_model("./enemy.dat");
    AsciiModel* enemy2 = load_model("./enemy2.dat");

    g->models[0] = cannon;
    g->models[1] = enemy;
    g->models[2] = enemy2;
    g->model_n = 2;
    g->screen_data = (Pixel*) malloc(sizeof(Pixel) * g->screen_rows * g->screen_cols);

    g->lives_left = 100;
    g->ttf = 500;
    g->placing_turret = false;
    g->current_money = 999;
    g->drawn_avb_spots = false;
    g->frame_count = 0;
    g->wm = create_wawe_manager();
    g->wm.current_wawe = 0;
    g->enemies_to_dispatch = 0;
    g->allowed_lanes = 3;
    g->to_dispatch_list = NULL;

    g->cur_turret = 0;
    g->turret_max = 100;
    g->turrets = (Turret*) malloc(sizeof(Turret) * g->turret_max);

    g->cur_bullet = 0;
    g->max_bullets = 1000;
    g->bullets = (Bullet*) malloc(sizeof(Bullet) * g->max_bullets);

    g->turret_clicked = false;
    g->selected_turret = false;

    init_ui(g);
}

void start_game(Game* g) {

    if (!g) {
        return;
    }

    draw_game(g);

    while (g->lives_left >= 0) {
        update_game(g);
        draw_game(g);
    }
}

void update_game(Game* g) {

    if (!g) {
        return;
    }

    //Maybe if time upgradable turrets
    perform_collision_checks(g);
    undraw_enemies(g);
    update_bullets(g);

    simulate_enemies(g);
    update_turrets(g);

    draw_enemies(g);

    if (g->wm.wawe_left <= 0) {
        add_wawe(g);
        g->enemies_to_dispatch = g->wm.wawe_left;
        g->init_el = g->wm.wawe_left;
        g->allowed_lanes += 1;
    }

    try_dispatch_enemies(g);

    int c = getch();
    MEVENT event;

    if (c == KEY_MOUSE) {

        if (getmouse(&event) != OK) {
            goto end;
        }

        int clicked_row = event.y;
        int clicked_col = event.x;
        const int BNTTL_Y = g->screen_rows-1-UI_BOTTOM_SCREEN_SIZE+2;

        bool s = false;
        for (int i = 0; i < g->cur_turret; ++i) {

            Turret* t = &g->turrets[i];

            if (clicked_on_turret(t, clicked_row, clicked_col)) {
                g->selected_turret = t;
                g->turret_clicked = true;
                s = true;
                break;
            }
        }

        if (g->turret_clicked) {

            if (clicked_row >= BNTTL_Y && clicked_row <= BNTTL_Y + 3 &&
                clicked_col >= TURRET_MENU_OFFSET && clicked_col <= TURRET_MENU_OFFSET + UPGRADE_WIDTH) {

                if (g->current_money > 200) {
                        
                    upgrade_turret(g->selected_turret);

                    draw_model(g->models[0], g->screen_data, COLOR_MODE_BLUE, g->selected_turret->pos_row, 
                            g->selected_turret->pos_col, g->screen_rows, g->screen_cols);

                    g->current_money -= 200;
                }

                g->turret_clicked = false;
            }

            if (clicked_row >= BNTTL_Y && clicked_row <= BNTTL_Y + 3 &&
                    clicked_col >= TURRET_MENU_OFFSET2 && clicked_col <= TURRET_MENU_OFFSET2 + UPGRADE_WIDTH2) {

                if (g->current_money > 300) {

                    re_type_turret(g->selected_turret, MoreTTL);

                    draw_model(g->models[0], g->screen_data, COLOR_MODE_MAGENTA, g->selected_turret->pos_row, 
                            g->selected_turret->pos_col, g->screen_rows, g->screen_cols);

                    g->current_money -= 300;
                }

                g->turret_clicked = false;
            }

            if (clicked_row >= BNTTL_Y && clicked_row <= BNTTL_Y + 3 &&
                    clicked_col >= TURRET_MENU_OFFSET3 && clicked_col <= TURRET_MENU_OFFSET3 + UPGRADE_WIDTH3) {

                if (g->current_money > 300) {

                    re_type_turret(g->selected_turret, MorePower);

                    draw_model(g->models[0], g->screen_data, COLOR_MODE_GREEN, g->selected_turret->pos_row, 
                            g->selected_turret->pos_col, g->screen_rows, g->screen_cols);

                    g->current_money -= 300;
                }

                g->turret_clicked = false;
            }

            if (clicked_row >= BNTTL_Y && clicked_row <= BNTTL_Y + 3 &&
                    clicked_col >= TURRET_MENU_OFFSET4 && clicked_col <= TURRET_MENU_OFFSET4 + UPGRADE_WIDTH4) {

                if (g->current_money > 300) {

                    re_type_turret(g->selected_turret, BackToStart);

                    draw_model(g->models[0], g->screen_data, COLOR_MODE_YELLOW, g->selected_turret->pos_row, 
                            g->selected_turret->pos_col, g->screen_rows, g->screen_cols);
                    g->current_money -= 300;
                }

                g->turret_clicked = false;
            }

        }

        if (s == false) {
            g->turret_clicked = false;
            undraw_turret_menu(g);
        }

        if (!g->placing_turret && clicked_row >= BNTTL_Y && clicked_row <= BNTTL_Y + 3 &&
                clicked_col >= BNTWL && clicked_col <= BNTWL + BUY_NEW_TURRET_WINDOW_WIDTH) {

            if (g->current_money < TURRET_PRICE) {
                goto end; 
            }

            g->placing_turret = true;

            if (g->drawn_avb_spots) {
                goto end;
            }

            for (int i = 0; i < g->lane_n; ++i) {
                draw_available_lane_spot_turret(g, &g->lanes[i]);
            }

            g->drawn_avb_spots = true;
            goto end;
        }

        if (g->placing_turret) {

            for (int i = 0; i < g->lane_n; ++i) {
                if (clicked_on_lane(&g->lanes[i], clicked_row, clicked_col)) {
                    g->current_money -= TURRET_PRICE;
                    create_turret(g, &g->lanes[i]);
                    place_turret_on_lane(g, &g->lanes[i], COLOR_MODE_DEFAULT);
                } else {
                    undo_draw_avb_lane_spot_turret(g, &g->lanes[i]);
                }
            }

            g->placing_turret = false;
            g->drawn_avb_spots = false;

            goto end;
        }

    } else {
        goto end;
    }

end:
    //function expects micro so we convert the unit to micor 
    //since ttf (time to frame) is in milli

    if (g->current_money < 1000 && g->frame_count % 10 == 0) {
        g->current_money += 1;
    }

    g->frame_count += 1;
    usleep(g->ttf * 300);
}

void add_wawe(Game* g) {

    wwm_create_wawe(&g->wm);

    if (!g->to_dispatch_list) {
        g->to_dispatch_list = (int*) malloc(sizeof(int) * g->wm.wawe_left);
    } else {
        //I know this will leak but it'll also solve my issues :(
        //free(g->to_dispatch_list);
        g->to_dispatch_list = (int*) malloc(sizeof(int) * g->wm.wawe_left);
    }

    g->tdl_e = g->to_dispatch_list;
    g->tdl_es = g->to_dispatch_list + g->wm.e_left;
    g->_enemy_td = g->wm.e_left;
    g->_enemys_td = g->wm.es_left;

    g->enemies_to_dispatch = g->wm.wawe_left;
    g->enemy_split_id = g->wm.e_left;

    for (int i = 0; i < g->wm.wawe_left; ++i) {
        g->to_dispatch_list[i] = rand() % g->allowed_lanes;

        if (i < g->wm.e_left) {
            g->wm.e_list[i].id = i;
            g->wm.e_list[i].enemy.position_row = g->lanes[g->to_dispatch_list[i]].offset_row;
            g->wm.e_list[i].enemy.position_col = g->screen_cols - 1 - 3;
        } else {
            int _i = i - g->wm.e_left;
            g->wm.es_list[_i].id = i;
            g->wm.es_list[_i].enemy.position_row = g->lanes[g->to_dispatch_list[i]].offset_row;
            g->wm.es_list[_i].enemy.position_col = g->screen_cols - 1 - 3;
        }
    }
}

void try_dispatch_enemies(Game* g) {

    if (g->enemies_to_dispatch < 1) {
        return;
    }

    bool lane_avb[g->allowed_lanes];

    for (int i = 0; i < g->allowed_lanes; ++i) {
        lane_avb[i] = false;
    }

    for (int i = 0; i < g->allowed_lanes; ++i) {

        Lane* l = &g->lanes[i];

        for (int j = 0; j < g->wm.e_left; ++j) {
            Enemy* e = &g->wm.e_list[j].enemy;

            if (e->draw == false) {
                continue;
            }

            if (l->offset_row == e->position_row && (e->position_col == (g->screen_cols - 1 - 3) || 
                        e->position_col == (g->screen_cols - 1 - 4) || e->position_col == (g->screen_cols - 1 - 5))
                    || e->position_col == (g->screen_cols - 1 - 5)) {
                lane_avb[i] = true;
            }
        }

        for (int j = 0; j < g->wm.es_left; ++j) {
            EnemyShield* es = &g->wm.es_list[j].enemy;

            if (es->draw == false) {
                continue;
            }

            if (l->offset_row == es->position_row && (es->position_col == (g->screen_cols - 1 - 3) || 
                        es->position_col == (g->screen_cols - 1 - 4) || es->position_col == (g->screen_cols - 1 - 5))
                    || es->position_col == (g->screen_cols - 1 - 5)) {
                lane_avb[i] = true;
            }
        }
    }

    int e_del = 0;
    int es_del = 0;
    for (int i = 0; i < g->_enemy_td; ++i) {

        Enemy* e = &g->wm.e_list[i].enemy;

        if (e->draw == true) {
            continue;
        }

        int lane_id = g->tdl_e[i];

        if (lane_avb[lane_id] == true) {
            continue;
        }

        e->draw = true;
        ++e_del;

        lane_avb[lane_id] = true;
    }

    for (int i = 0; i < g->_enemys_td; ++i) {

        EnemyShield* es = &g->wm.es_list[i].enemy;

        if (es->draw == true) {
            continue;
        }

        int lane_id = g->tdl_es[i];

        if (lane_avb[lane_id] == true) {
            continue;
        }

        es->draw = true;
        ++es_del;

        lane_avb[lane_id] = true;
    }

    g->enemies_to_dispatch -= e_del + es_del;
}

void simulate_enemies(Game* g) {

    if (g->wm.wawe_left < 1) {
        return;
    }

    int ek = 0;

    for (int i = 0; i < g->wm.e_left; ++i) {

        Enemy* e = &g->wm.e_list[i].enemy;

        if (e->draw == false) {
            continue;
        }

        if (e->position_col < 2) {
            g->lives_left -= e->strength * 5;
            destroy_enemy(g, g->wm.e_list[i].id, ek);
            continue;
        }

        e->position_col -= 1;
    }

    ek = 0;
    for (int i = 0; i < g->wm.es_left; ++i) {

        EnemyShield* es = &g->wm.es_list[i].enemy;

        if (es->draw == false) {
            continue;
        }

        if (es->position_col < 2) {
            g->lives_left -= es->strength * 5;
            destroy_enemy(g, g->wm.es_list[i].id, ek);
            continue;
        }

        es->position_col -= 1;
    }
}

void create_turret(Game* g, Lane* l) {

    if (g->cur_turret == g->turret_max - 1) {
        free(NULL);
        g->turrets = realloc(g->turrets, sizeof(Turret) * g->turret_max * 2);
        g->turret_max = g->turret_max * 2;
    }

    Turret t = new_turret(l->offset_row, l->left_offset, g->cur_turret);
    g->turrets[g->cur_turret] = t;

    g->cur_turret += 1;
}

void create_bullet(Game* g, Turret* t) {

    if (g->cur_bullet == g->max_bullets - 1) {
        g->bullets = realloc(g->bullets, sizeof(Bullet) * g->max_bullets * 2);
        g->max_bullets = g->max_bullets * 2;
    }

    int power = t->power;
    int ttl = 1;
    BulletType bt = DefaultB;

    switch (t->type) {
        case Default:
            ttl = 1;
            bt = DefaultB;
            break;
        case MorePower:
            ttl = 1;
            bt = MorePowerB;
            break;
        case MoreTTL:
            ttl = 2;
            bt = MoreTTLB;
            break;
        case BackToStart:
            ttl = 1;
            bt = BackToStartB;
            break;
    }

    Bullet b = new_bullet(power, ttl, bt, t->pos_row + 1, t->pos_col + 17, g->cur_bullet);
    g->bullets[g->cur_bullet] = b;
    g->cur_bullet += 1;
}

void update_bullets(Game* g) {

    int bullets_destroyed = 0;

    for (int i = 0; i < g->cur_bullet; ++i) {

        Bullet* b = &g->bullets[i];

        if (b->ttl <= 0) {
            destroy_bullet(g, b, 0);
            --i;
            mvwprintw(g->window, b->pos_row, b->pos_col, " ");
            continue;
        }

        if (b->pos_col == g->screen_cols-2) {
            mvwprintw(g->window, b->pos_row, b->pos_col, " ");
            destroy_bullet(g, &g->bullets[i], bullets_destroyed);
            --i;
            continue;
        }

        mvwprintw(g->window, b->pos_row, b->pos_col, " ");
        b->pos_col += 1;
        mvwprintw(g->window, b->pos_row, b->pos_col, "*");
    }

}

void update_turrets(Game* g) {

    for (int i = 0; i < g->cur_turret; ++i) {
        Turret* t = &g->turrets[i];
        if (t->tts_c == t->tts) {
            create_bullet(g, t);
            t->tts_c = 0;
            continue;
        }

        t->tts_c += 1;
    }
}

void destroy_bullet(Game* g, Bullet* b, int _) {

    if (g->cur_bullet <= 0) {
        return;
    }

    Bullet _b = *b;

    if (g->cur_bullet == 1) {
        // No swap needed if only one bullet is left
        g->cur_bullet -= 1;
    } else {
        // Swap the current bullet with the last bullet
        g->bullets[_b.id] = g->bullets[g->cur_bullet - 1];
        g->bullets[_b.id].id = _b.id; // Ensure the correct ID is maintained
        g->cur_bullet -= 1;
    }
}

int find_lane(Game* g, int row) {

    for (int i = 0; i < g->lane_n; ++i) {
        if (row == g->lanes[i].offset_row) {
            return i;
        }
    }

    return -1;
}

void perform_collision_checks(Game* g) {

    if (!g) {
        return;
    }

    if (g->cur_bullet > 0) {

        int bullets_destroyed = 0;
        for (int i = 0; i < g->cur_bullet; ++i) {

            Bullet* b = &g->bullets[i];

            if (b->ttl <= 0) {
                destroy_bullet(g, b, 0);
                --i;
                continue;
            }

            int enemies_killed = 0;

            int j;
            for (j = 0; j < g->wm.e_left; ++j) {
                Enemy* e = &g->wm.e_list[j].enemy;

                if (e->draw == false) {
                    continue;
                }

                if (e->position_row + 1 != b->pos_row) {
                    continue;
                }

                if (b->pos_col < e->position_col) {
                    continue;
                }

                if (b->pos_col > e->position_col + 2) {
                    return;
                }

                //hit
                e->health -= b->power;
                if (e->health < 1) {
                    destroy_enemy(g, g->wm.e_list[j].id, enemies_killed);
                    g->current_money += 100;
                }


                mvwprintw(g->window, 0, 0, "%c", '0' + b->bt);
                if (b->bt == BackToStartB) {
                    e->position_col = g->screen_cols - 1 - 3;
                }

                b->ttl -= 1; 
                if (b->ttl <= 0) {
                    destroy_bullet(g, b, bullets_destroyed);
                    --i;
                }
                break;
            }

            if (j != g->wm.e_left) {
                continue;
            }

            enemies_killed = 0;
            for (j = 0; j < g->wm.es_left; ++j) {
                EnemyShield* es = &g->wm.es_list[j].enemy;

                if (es->draw == false) {
                    continue;
                }

                if (es->position_row + 1 != b->pos_row) {
                    continue;
                }

                if (b->pos_col < es->position_col) {
                    continue;
                }

                if (b->pos_col > es->position_col + 2) {
                    return;
                }

                //hit
                b->ttl -= 1;

                if ((rand() % 10 + es->dmg_reduce) < 9) {
                    es->health -= b->power;
                }

                if (es->health <= 0) {
                    destroy_enemy(g, g->wm.es_list[j].id, enemies_killed);
                    g->current_money += 150;
                }

                if (b->bt == BackToStartB) {
                    es->position_col = g->screen_cols - 1 - 3;
                }

                if (b->ttl <= 0) {
                    destroy_bullet(g, b, bullets_destroyed);
                    --i;
                } 
                break;
            }
        }

    }

    if (g->wm.wawe_left > 0) {
        for (int i = 0; i < g->cur_turret; ++i) {

            Turret* t = &g->turrets[i];

            int j;
            for (j = 0; j < g->wm.e_left; ++j) {
                Enemy* e = &g->wm.e_list[j].enemy;

                if (e->draw == false) {
                    continue;
                }

                if (t->pos_row != e->position_row) {
                    continue;
                }

                if (e->position_col - 16 >= t->pos_col) {
                    continue;
                }

                destroy_turret(g, t);
                --i;
                break;
            }

            if (j != g->wm.e_left) {
                continue;
            }

            for (j = 0; j < g->wm.es_left; ++j) {
                EnemyShield* es = &g->wm.es_list[j].enemy;

                if (t->pos_row != es->position_row) {
                    continue;
                }

                if (es->position_col - 16 >= t->pos_col) {
                    continue;
                }

                destroy_turret(g, t);
                --i;
                break;
            }
        }
    }

}

void destroy_enemy(Game* g, int enemy_id, int ek) { 

    if (g->wm.wawe_left < 1) {
        return;
    }

    if (enemy_id < g->enemy_split_id) { 

        g->wm.e_list[enemy_id].enemy.draw = false; 
        g->wm.wawe_left -= 1;

    } else if (enemy_id >= g->enemy_split_id){

        g->wm.es_list[enemy_id-g->enemy_split_id].enemy.draw = false; 
        g->wm.wawe_left -= 1;    
    }    
}

void destroy_turret(Game *g, Turret *t) {

    if (g->placing_turret) {

        for (int i = 0; i < g->lane_n; ++i) {
            undo_draw_avb_lane_spot_turret(g, &g->lanes[i]);
        }
    }

    undraw_turret(g, t);
    Turret _t = *t;
    int f = 0;

    for (int i = 0; i < g->lane_n; ++i) {
        if (g->lanes[i].offset_row == _t.pos_row) {
            g->lanes[i].left_offset -= 16;
            break;
        }
    }

    g->turrets[_t.id] = g->turrets[g->cur_turret - 1];
    g->turrets[_t.id].id = _t.id; 

    g->cur_turret -= 1;
}
