/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <zephyr/drivers/sensor.h>

#include "battery_common.h"

int battery_channel_get(const struct battery_value *value, enum sensor_channel chan,
                        struct sensor_value *val_out) {
    switch (chan) {
    case SENSOR_CHAN_GAUGE_VOLTAGE:
        val_out->val1 = value->millivolts / 1000;
        val_out->val2 = (value->millivolts % 1000) * 1000U;
        break;

    case SENSOR_CHAN_GAUGE_STATE_OF_CHARGE:
        val_out->val1 = value->state_of_charge;
        val_out->val2 = 0;
        break;

    default:
        return -ENOTSUP;
    }

    return 0;
}

uint8_t lithium_ion_mv_to_pct(int16_t bat_mv) {  
    static int16_t mv_history[5] = {0};  
    static uint8_t history_index = 0;  
    static bool history_filled = false;  
      
    // 履歴に追加  
    mv_history[history_index] = bat_mv;  
    history_index = (history_index + 1) % 5;  
    if (history_index == 0) history_filled = true;  
      
    // 平均値を計算  
    int32_t sum = 0;  
    uint8_t count = history_filled ? 5 : history_index;  
    for (uint8_t i = 0; i < count; i++) {  
        sum += mv_history[i];  
    }  
    int16_t avg_mv = sum / count;  
      
    if (avg_mv >= 2350) {  
        return 100;  
    } else if (avg_mv <= 1950) {  
        return 0;  
    }  
    return avg_mv / 4 - 487.5;  
}
