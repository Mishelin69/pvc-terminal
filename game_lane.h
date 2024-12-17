#pragma once

#include <stdbool.h>
typedef struct Lane {
    int offset_row;
    int left_offset;
    int lane_id;
} Lane;

Lane create_lane(int offset_row, int lane_id);

bool clicked_on_lane(Lane* l, int clicked_row, int clicked_col);
