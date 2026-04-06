#include "knob_damage_log.h"
#include "knob_life.h"

// ---------- data ----------
typedef struct {
    uint32_t timestamp_ms;
    int8_t player;      // -1 = single player, 0-3 = multiplayer index
    int16_t delta;
} damage_log_entry_t;

static damage_log_entry_t damage_log[DAMAGE_LOG_MAX];
static int damage_log_count = 0;
static int damage_log_head = 0;

// ---------- screen ----------
lv_obj_t *screen_damage_log = NULL;
static lv_obj_t *damage_log_container = NULL;
static lv_timer_t *damage_log_refresh_timer = NULL;

// ---------- log operations ----------
void damage_log_add(int player, int delta)
{
    if (delta == 0) return;
    damage_log[damage_log_head].timestamp_ms = lv_tick_get();
    damage_log[damage_log_head].player = (int8_t)player;
    damage_log[damage_log_head].delta = (int16_t)delta;
    damage_log_head = (damage_log_head + 1) % DAMAGE_LOG_MAX;
    if (damage_log_count < DAMAGE_LOG_MAX) damage_log_count++;
}

void damage_log_reset(void)
{
    damage_log_count = 0;
    damage_log_head = 0;
}

void damage_log_scroll(int direction)
{
    lv_obj_scroll_by(damage_log_container, 0,
        direction < 0 ? -DAMAGE_LOG_SCROLL_PX : DAMAGE_LOG_SCROLL_PX,
        LV_ANIM_ON);
}

// ---------- UI ----------
static void format_elapsed(uint32_t elapsed_s, char *out, size_t out_sz)
{
    if (elapsed_s >= 60) {
        snprintf(out, out_sz, "%2lum ago", (unsigned long)(elapsed_s / 60));
    } else {
        snprintf(out, out_sz, "%2lus ago", (unsigned long)elapsed_s);
    }
}

static void format_log_line(damage_log_entry_t *entry, char *buf, size_t buf_sz)
{
    uint32_t elapsed_s = lv_tick_elaps(entry->timestamp_ms) / 1000;
    const char *action = entry->delta > 0 ? "gained" : "lost";
    int abs_delta = entry->delta > 0 ? entry->delta : -entry->delta;
    char time_str[16];

    format_elapsed(elapsed_s, time_str, sizeof(time_str));

    if (entry->player < 0) {
        snprintf(buf, buf_sz, "%s: %s %d life", time_str, action, abs_delta);
    } else {
        snprintf(buf, buf_sz, "%s: %s %s %d life",
                 time_str, multiplayer_names[entry->player], action, abs_delta);
    }
}

static void damage_log_update_timestamps(void)
{
    int i;
    uint32_t child_count = lv_obj_get_child_cnt(damage_log_container);

    if (damage_log_count == 0 || child_count == 0) return;

    for (i = 0; i < (int)child_count && i < damage_log_count; i++) {
        int idx = (damage_log_head - 1 - i + DAMAGE_LOG_MAX) % DAMAGE_LOG_MAX;
        lv_obj_t *lbl = lv_obj_get_child(damage_log_container, i);
        char buf[80];

        format_log_line(&damage_log[idx], buf, sizeof(buf));
        lv_label_set_text(lbl, buf);
    }
}

static void refresh_damage_log_ui(void)
{
    int i, idx;
    char buf[80];

    lv_obj_clean(damage_log_container);

    if (damage_log_count == 0) {
        lv_obj_t *lbl = lv_label_create(damage_log_container);
        lv_label_set_text(lbl, "No events yet");
        lv_obj_set_style_text_color(lbl, lv_color_hex(0x7A7A7A), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_16, 0);
        return;
    }

    for (i = 0; i < damage_log_count; i++) {
        idx = (damage_log_head - 1 - i + DAMAGE_LOG_MAX) % DAMAGE_LOG_MAX;

        format_log_line(&damage_log[idx], buf, sizeof(buf));

        lv_obj_t *lbl = lv_label_create(damage_log_container);
        lv_label_set_text(lbl, buf);
        lv_obj_set_width(lbl, 280);
        lv_obj_set_style_text_color(lbl,
            damage_log[idx].delta > 0 ? lv_color_hex(0x4CAF50) : lv_color_hex(0xFF5252), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
    }

    lv_obj_scroll_to_y(damage_log_container, 0, LV_ANIM_OFF);
}

static void damage_log_refresh_cb(lv_timer_t *timer)
{
    (void)timer;
    if (lv_scr_act() == screen_damage_log) {
        damage_log_update_timestamps();
    }
}

// ---------- navigation ----------
void open_damage_log_screen(void)
{
    refresh_damage_log_ui();
    load_screen_if_needed(screen_damage_log);
    if (damage_log_refresh_timer != NULL) lv_timer_resume(damage_log_refresh_timer);
}

void pause_damage_log_timer(void)
{
    if (damage_log_refresh_timer != NULL) lv_timer_pause(damage_log_refresh_timer);
}

static void event_open_damage_log(lv_event_t *e)
{
    (void)e;
    open_damage_log_screen();
}

// ---------- build ----------
void build_damage_log_screen(void)
{
    screen_damage_log = lv_obj_create(NULL);
    lv_obj_set_size(screen_damage_log, 360, 360);
    lv_obj_set_style_bg_color(screen_damage_log, lv_color_black(), 0);
    lv_obj_set_style_border_width(screen_damage_log, 0, 0);
    lv_obj_set_scrollbar_mode(screen_damage_log, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *title = lv_label_create(screen_damage_log);
    lv_label_set_text(title, "Event Log");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 24);

    damage_log_container = lv_obj_create(screen_damage_log);
    lv_obj_remove_style_all(damage_log_container);
    lv_obj_set_size(damage_log_container, 300, 220);
    lv_obj_align(damage_log_container, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_flex_flow(damage_log_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(damage_log_container, 6, 0);
    lv_obj_set_scrollbar_mode(damage_log_container, LV_SCROLLBAR_MODE_OFF);

    damage_log_refresh_timer = lv_timer_create(damage_log_refresh_cb, 1000, NULL);
    if (damage_log_refresh_timer != NULL) {
        lv_timer_pause(damage_log_refresh_timer);
    }
}
