// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define LAYER_NOTIFY_ID 0xF1
#define LAYER_NOTIFY_STATE 0x01
#define LAYER_NOTIFY_PACKET_SIZE 32

// レイヤー状態が変わっていたら Raw HID で通知する。housekeeping_task_user から呼ぶ。
void layer_notify_task(void);
