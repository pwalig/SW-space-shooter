#pragma once

namespace game {
    extern int score;
    enum class State {
        playing, over, paused, menu
    };
    extern State state;
    void spaceshipProjectileCollisions();
}