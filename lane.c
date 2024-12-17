#include "game_lane.h"

Lane create_lane(int offset_row, int lane_id) {

    Lane lane = { offset_row, 1, lane_id };

    return lane;
}

bool clicked_on_lane(Lane* l, int clicked_row, int clicked_col) {

    if (!l) {
        return false;
    }

    if (clicked_row >= l->offset_row && clicked_row < l->offset_row+3 &&
        clicked_col >= l->left_offset && clicked_col < l->left_offset+16) {
        return true;
    }

    return false;
}
