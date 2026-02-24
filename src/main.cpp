#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_display.h>
#include <bn_random.h>
#include <bn_vector.h>
#include <bn_log.h>

#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_avg_sprite.h"

static constexpr int HALF_SCREEN_WIDTH = bn::display::width() / 2;
static constexpr bn::fixed MIN_X = -HALF_SCREEN_WIDTH;
static constexpr bn::fixed MAX_X = HALF_SCREEN_WIDTH;
static constexpr int HALF_SCREEN_HEIGHT = bn::display::height() / 2;
static constexpr bn::fixed MAX_Y = HALF_SCREEN_HEIGHT;
static constexpr bn::fixed MIN_Y = -HALF_SCREEN_HEIGHT;

// Speed range: random value between 1 and BASE_SPEED_MAX
static constexpr bn::fixed BASE_SPEED_MAX = 4;

// Maximum number of bouncers on screen at once
static constexpr int MAX_BOUNCERS = 20;

bn::fixed random_speed(bn::random &rng)
{
    bn::fixed speed = bn::fixed(1) + bn::fixed(rng.get_int(bn::fixed(BASE_SPEED_MAX - 1).integer()));
    if (rng.get_int(2) == 0)
    {
        speed *= -1;
    }
    return speed;
}

class Bouncer
{
public:
   bn::sprite_ptr avg_sprite = bn::sprite_items::avg_sprite.create_sprite(0, 0);
    bn::fixed x_speed;
    bn::fixed y_speed;

    Bouncer(bn::random &rng) : x_speed(random_speed(rng)),
                               y_speed(random_speed(rng))
    {
    }

    void update()
    {
        bn::fixed x = sprite.x() + x_speed;
        bn::fixed y = sprite.y() + y_speed;

        if (x > MAX_X)
        {
            x = MAX_X;
            x_speed *= -1;
        }
        else if (x < MIN_X)
        {
            x = MIN_X;
            x_speed *= -1;
        }

        if (y > MAX_Y)
        {
            y = MAX_Y;
            y_speed *= -1;
        }
        else if (y < MIN_Y)
        {
            y = MIN_Y;
            y_speed *= -1;
        }

        sprite.set_x(x);
        sprite.set_y(y);
    }
};

bn::fixed average_x(const bn::vector<Bouncer, MAX_BOUNCERS> &bouncers)
{
    if (bouncers.empty())
        return 0;

    bn::fixed x_sum = 0;
    for (const Bouncer &bouncer : bouncers)
    {
        x_sum += bouncer.sprite.x();
    }
    return x_sum / bouncers.size();
}

// Returns the average y position across all bouncers.
bn::fixed average_y(const bn::vector<Bouncer, MAX_BOUNCERS> &bouncers)
{
    if (bouncers.empty())
        return 0;

    bn::fixed y_sum = 0;
    for (const Bouncer &bouncer : bouncers)
    {
        y_sum += bouncer.sprite.y();
    }
    return y_sum / bouncers.size();
}

void add_bouncer(bn::vector<Bouncer, MAX_BOUNCERS> &bouncers, bn::random &rng)
{
    if (bouncers.size() < bouncers.max_size())
    {
        bouncers.push_back(Bouncer(rng));
    }
}

int main()
{
    bn::core::init();

    // Single shared random instance
    bn::random rng;

    bn::vector<Bouncer, MAX_BOUNCERS> bouncers;

    // Extension: a separate sprite that sits at the average position
    bn::sprite_ptr avg_sprite = bn::sprite_items::dot.create_sprite(0, 0);
    avg_sprite.set_visible(false); // hidden until at least one biuer exists

    while (true)
    {
        if (bn::keypad::a_pressed())
        {
            add_bouncer(bouncers, rng);
        }

        if (bn::keypad::b_pressed())
        {
            BN_LOG("Average x: ", average_x(bouncers));
            BN_LOG("Average y: ", average_y(bouncers));
        }

        for (Bouncer &bouncer : bouncers)
        {
            bouncer.update();
        }

        // Update the average-position dot
        if (!bouncers.empty())
        {
            avg_sprite.set_visible(true);
            avg_sprite.set_x(average_x(bouncers));
            avg_sprite.set_y(average_y(bouncers));
        }
        else
        {
            avg_sprite.set_visible(false);
        }

        bn::core::update();
    }
}