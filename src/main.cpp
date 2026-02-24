#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_display.h>
#include <bn_random.h>
#include <bn_vector.h>
#include <bn_log.h>

#include "bn_sprite_items_dot.h"

static constexpr int HALF_SCREEN_WIDTH = bn::display::width() / 2;
static constexpr bn::fixed MIN_X = -HALF_SCREEN_WIDTH;
static constexpr bn::fixed MAX_X = HALF_SCREEN_WIDTH;
static constexpr int HALF_SCREEN_HEIGHT = bn::display::height() / 2;
static constexpr bn::fixed MAX_Y = HALF_SCREEN_HEIGHT;
static constexpr bn::fixed MIN_Y = -HALF_SCREEN_HEIGHT;

// Starting speed of a bouncer
static constexpr bn::fixed BASE_SPEED = 2;

// Maximum number of bouncers on screen at once
static constexpr int MAX_BOUNCERS = 20;

class Bouncer {
    public:
        bn::sprite_ptr sprite = bn::sprite_items::dot.create_sprite();
        bn::fixed x_speed = BASE_SPEED;
        bn::fixed y_speed = BASE_SPEED;

        void update() {
            bn::fixed x = sprite.x();
            bn::fixed y = sprite.y();

    
            x += x_speed;
            y += y_speed;
            if(x > MAX_X) {
                x = MAX_X;
                x_speed *=-1;
            }

            if(x < MIN_X) { //reverse
                x = MIN_X;
                x_speed *= -1;
            }

            if (y > MAX_Y) {
                y = MAX_Y;
                y_speed *=-1;
            }

            if (y < MIN_Y) {
                y = MIN_Y;
                y_speed *=-1;
            }

            sprite.set_x(x);
            sprite.set_y(y);
        }
    };

bn::fixed average_x( bn::vector<Bouncer, MAX_BOUNCERS> bouncers ) {
    bn::fixed x_sum = 0;
    for(Bouncer& bouncer : bouncers) {
       x_sum += bouncer.sprite.x();
    }

    bn::fixed x_average= x_sum;
    if(bouncers.size() > 0) {
        x_average /= bouncers.size();
    }

    return x_average;

}

void add_bouncer(bn::vector<Bouncer, MAX_BOUNCERS>& bouncers)   {
    if(bouncers.size() < bouncers.max_size()) {
        bouncers.push_back(Bouncer());
    }
}

int main() {
    bn::core::init();
    bn::vector<Bouncer, MAX_BOUNCERS> bouncers = {};

    while(true) {
        if(bn::keypad::a_pressed()) {
           add_bouncer(bouncers);
        }

        if(bn::keypad::b_pressed()) {
          BN_LOG("Average x: ", average_x(bouncers));
        }
        
       for(Bouncer& bouncer : bouncers) {
        bouncer.update();
       }

        bn::core::update();
    }
}