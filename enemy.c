#include "enemy.h"

Enemy create_enemy(int position_row, int position_col,
                   int strength, int health) { 
    Enemy e = { position_row, position_col, strength, health, false };

    return e;
}

EnemyShield create_enemy_shield(int position_row, int position_col,
                   int strength, float health, int dmg_reduce) {
    EnemyShield e = { position_row, position_col, strength, health, dmg_reduce, false };

    return e;
}
