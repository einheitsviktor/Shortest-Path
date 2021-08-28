#include "grid.h"

// Grid::Grid(int width, int height, QVector<QVector<Tile*>>& floor)
    // : width(width)
    // , height(height)
// {
    // initGrid(floor);
// }

// TEST constructor
Grid::Grid() : width(WIDTH), height(HEIGHT) {
    initGrid(Visualizer::floor);
}

std::array<Coordinates, 4> Grid::DIRS = {
    Coordinates{1, 0},  // East
    Coordinates{-1, 0}, // West
    Coordinates{0, -1}, // North
    Coordinates{0, 1}
};

bool Grid::inBounds(Coordinates id) const {
    return 0 <= id.x && id.x < width && 0 < id.y && id.y < height;
}
bool Grid::passable(Coordinates id) const {
    return this->obstacles.find(id) == this->obstacles.end();
}

std::vector<Coordinates> Grid::neighbors(Coordinates id) const {
    std::vector<Coordinates> ret;

    for (const auto& dir : this->DIRS) {
        Coordinates next{id.x + dir.x, id.y + dir.y}; // Create delta coordinates
        if (inBounds(next) && passable(next))
            ret.push_back(next);
    }
    // Nudge directions for "prettier" paths
    if ((id.x + id.y) % 2 == 0) { std::reverse(ret.begin(), ret.end()); }
    return ret;
}

void Grid::initGrid(QVector<QVector<Tile*>> floor) {
    for (const auto& row : floor) {
        for (const auto& tile : row) {
            switch (tile->state) {
            case State::obstacle: this->obstacles.insert(Coordinates{tile->y, tile->x}); break;
            case State::origin: this->start = {tile->y, tile->x}; break;
            case State::destination: this->goal = {tile->y, tile->x}; break;
            default: break;
            }
        }
    }
}
