#pragma once

#include "enemy.h"
#include <stddef.h>

typedef struct WWMEnemyEntry {
    Enemy enemy;
    int id;
} WWMEnemyEntry;

typedef struct WWMEnemySEntry {
    EnemyShield enemy;
    int id;
} WWMEnemySEntry;

typedef struct WaweManager {

    int current_wawe;

    WWMEnemyEntry* e_list;
    int e_left;

    WWMEnemySEntry* es_list;
    int es_left;

    int wawe_left;

} WaweManager;

WaweManager create_wawe_manager();

void wwm_create_wawe(WaweManager* wm);
