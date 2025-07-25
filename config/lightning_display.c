#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display/status_screen.h>
#include <zmk/events/wpm_state_changed.h>
#include <zmk/display/widgets/peripheral_status.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/wpm_status.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Display dimensions
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

// Animation state
static bool lightning_active = false;
static uint32_t lightning_start_time = 0;
static uint8_t lightning_frame = 0;
static uint8_t wpm_threshold = 30;

// Cloud and lightning drawing functions
static void draw_cloud(struct zmk_display_buffer *buffer, uint8_t x, uint8_t y) {
    // Simple cloud shape
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 8; j++) {
            bool draw = false;
            
            // Cloud outline
            if ((i == 0 || i == 19) && j > 2 && j < 5) draw = true;
            if ((j == 0 || j == 7) && i > 2 && i < 17) draw = true;
            if ((i == 2 || i == 17) && (j == 2 || j == 5)) draw = true;
            
            // Internal texture
            if (i > 2 && i < 17 && j > 1 && j < 6) {
                if ((i + j) % 3 == 0) draw = true;
            }
            
            if (draw && x + i < DISPLAY_WIDTH && y + j < DISPLAY_HEIGHT) {
                zmk_display_buffer_draw_pixel(buffer, x + i, y + j, true);
            }
        }
    }
}

static void draw_lightning(struct zmk_display_buffer *buffer, uint8_t x, uint8_t y, uint8_t length) {
    int cx = x;
    int cy = y;
    
    for (int i = 0; i < length && cy < DISPLAY_HEIGHT - 1; i++) {
        if (cx >= 0 && cx < DISPLAY_WIDTH) {
            zmk_display_buffer_draw_pixel(buffer, cx, cy, true);
            zmk_display_buffer_draw_pixel(buffer, cx, cy + 1, true);
        }
        
        if (i % 2 == 0) {
            cx += (i % 4 == 0) ? 1 : -1;
        }
        cy += 2;
    }
}

// Custom render function that extends the default status screen
static int lightning_status_render(struct zmk_display_buffer *buffer) {
    uint32_t current_time = k_uptime_get();
    
    // Draw cloud in top-right corner
    draw_cloud(buffer, DISPLAY_WIDTH - 25, 2);
    
    // Handle lightning animation
    if (lightning_active) {
        uint32_t elapsed = current_time - lightning_start_time;
        
        if (elapsed < 150) { // 150ms animation duration
            lightning_frame = (elapsed * 8) / 150;
            uint8_t lightning_length = 25 - (lightning_frame * 2);
            
            if (lightning_length > 0) {
                draw_lightning(buffer, DISPLAY_WIDTH - 15, 10, lightning_length);
            }
        } else {
            lightning_active = false;
        }
    }
    
    return 0;
}

// WPM event handler
static int lightning_wpm_listener(const zmk_event_t *eh) {
    struct zmk_wpm_state_changed *ev = as_zmk_wpm_state_changed(eh);
    
    if (ev != NULL && ev->state >= wpm_threshold && !lightning_active) {
        lightning_active = true;
        lightning_start_time = k_uptime_get();
        lightning_frame = 0;
        
        // Adjust threshold for high WPM
        if (ev->state > 80) {
            wpm_threshold = ev->state - 10;
        } else {
            wpm_threshold = 30;
        }
        
        LOG_INF("Lightning triggered! WPM: %d", ev->state);
    }
    
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(lightning_display, lightning_wpm_listener)
ZMK_SUBSCRIPTION(lightning_display, zmk_wpm_state_changed);

// Register custom render function
static const struct zmk_status_screen lightning_status_screen = {
    .render = lightning_status_render,
};

DEVICE_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_APPLICATION_INIT_PRIORITY, &lightning_status_screen); 
