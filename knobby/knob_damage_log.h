#ifndef _KNOB_DAMAGE_LOG_H
#define _KNOB_DAMAGE_LOG_H

#include "knob_types.h"

#define DAMAGE_LOG_MAX 256
#define DAMAGE_LOG_SCROLL_PX 30

extern lv_obj_t *screen_damage_log;

void damage_log_add(int player, int delta);
void damage_log_reset(void);
void damage_log_scroll(int direction);

void build_damage_log_screen(void);
void open_damage_log_screen(void);
void pause_damage_log_timer(void);

#endif // _KNOB_DAMAGE_LOG_H
