#pragma once

#include "model.h"
#include "game_lane.h"
#include <ncurses.h>
#include <stddef.h>
#include "pixel.h"
#include "WaweManager.h"
#include "turret.h"

#define UI_BOTTOM_SCREEN_SIZE 5
#define UI_BOTTOM_RIGHT_PART_OFF 150
#define LANE_SIZE 3
#define WINDOW_HEIGHT 44
#define BUY_NEW_TURRET_WINDOW_WIDTH 30
#define BUY_NEW_TURRET_WINDOW_HEIGHT 3
#define BNTWL 10

#define TURRET_PRICE 100
#define TURRET_MODEL_HEIGHT 3
#define TURRET_MODEL_WIDTH 16

#define TURRET_MENU_OFFSET 45
#define UPGRADE_WIDTH 8

#define TURRET_MENU_OFFSET2 55
#define UPGRADE_WIDTH2 11

#define TURRET_MENU_OFFSET3 68
#define UPGRADE_WIDTH3 14

#define TURRET_MENU_OFFSET4 84
#define UPGRADE_WIDTH4 4

typedef struct Game {

    AsciiModel* models[3];
    size_t model_n;

    WINDOW* window;
    struct Pixel* screen_data;
    uint32_t screen_rows;
    uint32_t screen_cols;
    ColorMode current_clr_mode;

    Lane* lanes;
    uint32_t lane_n;
    int lives_left;
    uint32_t ttf;
    bool placing_turret;
    uint32_t current_money;
    bool drawn_avb_spots;
    uint64_t frame_count;
    int allowed_lanes;

    WaweManager wm;
    int init_el;
    int* to_dispatch_list;
    int enemies_to_dispatch;
    int _enemy_td;
    int _enemys_td;
    int* tdl_e;
    int* tdl_es;
    int enemy_split_id;

    Turret* turrets;
    int cur_turret;
    int turret_max;

    Bullet* bullets;
    int cur_bullet;
    int max_bullets;

    bool turret_clicked;
    Turret* selected_turret;
    
} Game;

void switch_color_mode(Game* g, ColorMode mode);

void init_colors();

Game* create_game();

void init_game(Game* g);

void draw_lives(Game* g);

void draw_wawe_stat(Game* g);

void draw_available_lane_spot_turret(Game* g, Lane* l);

void undo_draw_avb_lane_spot_turret(Game* g, Lane* l);

void place_turret_on_lane(Game* g, Lane* l, ColorMode m);

void init_ui(Game* g);

void start_game(Game* g);

void update_game(Game* g);

void draw_game(Game* g);

void try_dispatch_enemies(Game* g);

void simulate_enemies(Game* g);

void perform_collision_checks(Game* g);

void undraw_enemies(Game* g);

void undraw_enemy(Game* g, int enemy_id);

void draw_enemies(Game* g);

void draw_enemy(Game* g, int enemy_id);

void destroy_enemy(Game* g, int enemy_id, int ek);

void update_bullets(Game *g);

void update_turrets(Game *g);

void draw_bullets(Game *g);

void add_wawe(Game *g);

void create_turret(Game* g, Lane* l);

void destroy_turret(Game*g, Turret* t);

void undraw_turret(Game* g, Turret* t);

void create_bullet(Game* g, Turret* t);

void destroy_bullet(Game* g, Bullet* b, int bd);

void draw_turret_menu(Game* g);

void undraw_turret_menu(Game* g);
