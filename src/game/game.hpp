#pragma once
#include <vector>
#include <utility>
#include <string>

namespace game {
    extern int score;
    enum class State {
        playing, over, paused, menu
    };
    extern State state;
    extern std::vector<std::pair<int, std::string>> scores;
    void spaceshipProjectileCollisions();
    void start();
    void gameOver();
    void loadScores();
    void saveScores();
}