// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include <string.h>
#include "raw_hid.h"
#include "layer_notify.h"

static uint32_t last_sent = 0;
static bool     has_sent  = false;

static uint32_t current_state(void) {
    return (uint32_t)layer_state | (uint32_t)default_layer_state;
}

static void fill_packet(uint8_t *buf, uint8_t length, uint32_t state) {
    memset(buf, 0, length);
    buf[0] = LAYER_NOTIFY_ID;
    buf[1] = LAYER_NOTIFY_STATE;
    buf[2] = state & 0xFF;
    buf[3] = (state >> 8) & 0xFF;
    buf[4] = (state >> 16) & 0xFF;
    buf[5] = (state >> 24) & 0xFF;
}

void layer_notify_task(void) {
    // スレーブ側は USB に繋がっていないので送らない
    if (!is_keyboard_master()) return;
    uint32_t state = current_state();
    if (has_sent && state == last_sent) return;
    uint8_t buf[LAYER_NOTIFY_PACKET_SIZE];
    fill_packet(buf, sizeof(buf), state);
    raw_hid_send(buf, sizeof(buf));
    last_sent = state;
    has_sent  = true;
}

// VIA の raw_hid_receive より先に呼ばれる。0xF1 だけ処理し、それ以外は VIA に任せる。
bool via_command_kb(uint8_t *data, uint8_t length) {
    if (data[0] != LAYER_NOTIFY_ID) return false;
    fill_packet(data, length, current_state());
    raw_hid_send(data, length);
    return true;
}
