#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct Enemy {
    int position_row;
    int position_col;
    int strength;
    int health;
    bool draw;
} Enemy;

Enemy create_enemy(int position_row, int position_col,
                   int strength, int health);

typedef struct EnemyShield {
    int position_row;
    int position_col;
    int strength;
    float health;
    int dmg_reduce;
    bool draw;
} EnemyShield;

EnemyShield create_enemy_shield(int position_row, int position_col,
                   int strength, float health, int dmg_reduce);
