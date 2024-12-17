#pragma once

#include <stdbool.h>

typedef enum TurretType {
    Default,
    MorePower,
    MoreTTL,
    BackToStart,
} TurretType;

typedef enum BulletType {
    DefaultB,
    MorePowerB,
    MoreTTLB,
    BackToStartB
} BulletType;

typedef struct Turret {

    int level;
    int power;
    TurretType type;
    int pos_row;
    int pos_col;
    int tts;
    int tts_c;
    int id;

} Turret;

Turret new_turret(int pos_row, int pos_col, int id);

bool clicked_on_turret(Turret* t, int c_row, int c_col);

void upgrade_turret(Turret* t);

void re_type_turret(Turret* t, TurretType tt);

typedef struct Bullet {

    int power;
    int ttl;
    BulletType bt;
    int pos_row;
    int pos_col;
    int id;

} Bullet;

Bullet new_bullet(int power, int ttl, BulletType bt,
        int pos_row, int pos_col, int id);
