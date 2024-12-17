#include "WaweManager.h"
#include <math.h>
#include <stdlib.h>

WaweManager create_wawe_manager() {
    
    WaweManager wm = { 0, NULL, 0, NULL, 0, 0 };

    return wm;
}

void get_logarithmic_range(int input, int *start, int *end) {

    if (input <= 0) {
        *start = 0;
        *end = 0;
        return;
    }

    float base = 2.0f; 

    int start_value = (int)(pow(base, input) + 2);
    
    int end_value = start_value + input;

    *start = start_value;
    *end = end_value;
}

void generate_random_split(int input, int *part1, int *part2) {

    int start, end;

    get_logarithmic_range(input, &start, &end);

    if (start >= end) {
        *part1 = start;
        *part2 = 0;
        return;
    }

    int total = rand() % (end - start + 1) + start;
    float ratio = (input * 0.2f) + 0.5f;
    int split_point = (int)(total * ratio);

    if (split_point > total) {
        split_point = total;
    }

    *part1 = split_point;
    *part2 = total - split_point;
}

void wwm_create_wawe(WaweManager* wm) {

    wm->current_wawe += 1;
    int split1, split2;

    generate_random_split(wm->current_wawe, &split1, &split2);

    int wawe_total = split1 + split2;
    wm->wawe_left = wawe_total;
    wm->e_left = split1;
    wm->es_left = split2;

    if (wm->e_list) {

        //free(wm->e_list);
        //free(wm->es_list);

        wm->e_list = (WWMEnemyEntry*) malloc(sizeof(WWMEnemyEntry) * split1);
        wm->es_list = (WWMEnemySEntry*) malloc(sizeof(WWMEnemySEntry) * split2);
    }
    else {
        wm->e_list = (WWMEnemyEntry*) malloc(sizeof(WWMEnemyEntry) * split1);
        wm->es_list = (WWMEnemySEntry*) malloc(sizeof(WWMEnemySEntry) * split2);
    }

    //Spawn normal
    for (int i = 0; i < split1; ++i) {

        int raise_h = ((rand() % 10) > 9 - i) ? 1 : 0;
        int raise_p = ((rand() % 10) > 9 - i) ? 1 : 0;

        Enemy e = create_enemy(0, 0, wm->current_wawe + raise_p, wm->current_wawe + raise_h);
        WWMEnemyEntry ee = {e, i}; 

        wm->e_list[i] = ee;
    }
    
    //spawn shielded
    for (int i = 0; i < split2; ++i) {

        int raise_h = ((rand() % 7) > 9 - i) ? 1 : 0;
        int raise_p = ((rand() % 14) > 9 - i) ? 1 : 0;

        EnemyShield es = create_enemy_shield(0, 0, wm->current_wawe + raise_p, 
                                        wm->current_wawe + raise_h, i);
        WWMEnemySEntry ee = { es, i }; 

        wm->es_list[i] = ee;
    }
}
