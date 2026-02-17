#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_display.h>

#include "bn_sprite_items_dot.h"

static constexpr int HALF_SCREEN_WIDTH = bn::display::width() / 2;
static constexpr int HALF_SCREEN_HEIGHT = bn::display::height() / 2; 

int main() {
    bn::core::init();

    bn::sprite_ptr bouncer = bn::sprite_items::dot.create_sprite(20, 50);

    bn::fixed dx = -2.5;
    bn::fixed dy = 3.1;

    while(true) {

        bn::fixed x = bouncer.x();
        bn::fixed y = bouncer.y();

        x += dx;
        y += dy;

        if(x > HALF_SCREEN_WIDTH) {
            x = HALF_SCREEN_WIDTH;
            dx *=-1;
        }
        if(x < -HALF_SCREEN_WIDTH) {
            x = -HALF_SCREEN_WIDTH;
            dx *= -1;
        }

        if(y > HALF_SCREEN_HEIGHT) {
            y = HALF_SCREEN_HEIGHT;
            dy *= -1;
        }
        if(y < -HALF_SCREEN_HEIGHT) {
            y = -HALF_SCREEN_HEIGHT;
            dy *= -1;
        }

        bouncer.set_x(x);
        bouncer.set_y(y);

        bn::core::update();
    }
}