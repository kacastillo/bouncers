#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_display.h>
#include <bn_random.h>
#include <bn_vector.h>

#include "bn_sprite_items_dot.h"

static constexpr int HALF_SCREEN_WIDTH = bn::display::width() / 2;
static constexpr int HALF_SCREEN_HEIGHT = bn::display::height() / 2; 

static constexpr bn::fixed MIN_X = -HALF_SCREEN_WIDTH;
static constexpr bn::fixed MAX_X = HALF_SCREEN_WIDTH;
static constexpr bn::fixed MIN_Y = -HALF_SCREEN_HEIGHT;
static constexpr bn::fixed MAX_Y = HALF_SCREEN_HEIGHT;

static constexpr bn::fixed MIN_DX = -2;
static constexpr bn::fixed MAX_DX = 2;
static constexpr bn::fixed MIN_DY = -2;
static constexpr bn::fixed MAX_DY = 2;


int main() {
    bn::core::init();

    bn::vector<bn::sprite_ptr, 20> sprites = {};
    bn::vector<bn::fixed, 20> speeds = {};

    while(true) {

        if(bn::keypad::a_pressed()) {
            sprites.push_back(bn::sprite_items::dot.create_sprite());
            speeds.push_back(3);
        }

        for(int i = 0; i < sprites.size(); i++) {
            bn::sprite_ptr sprite = sprites[i];

            bn::fixed x = sprite.x();

            x += speeds[i];

            if(x > MAX_X) {
                x = MAX_X;
                speeds[i] *=-1;
            }
            if(x < MIN_X) {
                x = MIN_X;
                speeds[i] *= -1;
            }

            sprite.set_x(x);

        }

        bn::core::update();
    }
}