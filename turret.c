#include "turret.h"

Turret new_turret(int pos_row, int pos_col, int id) {
    Turret t = {1, 1, Default, pos_row, pos_col, 50, 50, id };
    return t;
}

void upgrade_turret(Turret* t) {

    if (!t) {
        return;
    }

    t->level += 1;

    if (t->level == 2) {
        t->power += 1; 
    } else if (t->level == 3) {
        t->tts -= 10;
        t->tts_c = t->tts;
    }
}

void re_type_turret(Turret* t, TurretType tt) {
    
    if (!t || t->level > 3) {
        return;
    }

    switch (tt) {
        case MorePower:
            t->power *= 2;
            t->type = tt;
            break;
        case BackToStart:
            t->tts = 100;
            t->tts_c = 100;
            t->type = tt;
            break;
        case MoreTTL:
            t->type = tt;
            t->tts = 20;
            t->tts_c = 20;
            break;
        case Default:
            t->type = tt;
            break;
    }
}

bool clicked_on_turret(Turret* t, int c_row, int c_col) {
    return c_row >= t->pos_row && c_row <= t->pos_row + 3
        && c_col >= t->pos_col && c_col <= t->pos_col + 16;
}

Bullet new_bullet(int power, int ttl, BulletType bt,
        int pos_row, int pos_col, int id) {

    Bullet b = { power, ttl, bt, pos_row, pos_col, id };

    return b;
}
