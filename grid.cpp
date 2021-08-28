#include "grid.h"

Grid::Grid() : width(WIDTH), height(HEIGHT) {
    initGrid(Visualizer::floor);
}

std::array<Coordinates, 4> Grid::DIRS = {
    Coordinates{1, 0},  // East
    Coordinates{-1, 0}, // West
    Coordinates{0, -1}, // North
    Coordinates{0, 1}   // South
};

bool Grid::inBounds(Coordinates id) const {
    return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
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
                // case State::start: this->start = Visualizer::startCoordinates; break;
                // case State::goal: this->goal = Visualizer::goalCoordinates; break;
                default: break;
            }
        }
    }
    qDebug() << "OBSTACLES: " << this->obstacles.size();
    // qDebug() << "Start:" << this->start.y << "," << this->start.x;
    // qDebug() << "Goal:" << this->goal.y << "," << this->goal.x;
}

void Grid::printPath() {
    QVector<Coordinates> path;
    Coordinates current = Visualizer::goalCoordinates;
    while (current != Visualizer::startCoordinates) {
        path.push_back(current);
        current = this->cameFrom[current];
    }
    auto rit = path.rbegin();
    for (; rit != path.rend(); ++rit) {
        Visualizer::setTile(*rit, State::path);
    }
}

void Grid::breadthFirstSearch() {
    std::queue<Coordinates> frontier;
    frontier.push(Visualizer::startCoordinates);

    this->cameFrom[Visualizer::startCoordinates] = Visualizer::startCoordinates;

    while (!frontier.empty()) {
        Coordinates current = frontier.front();
        frontier.pop();

        if (current == Visualizer::goalCoordinates) {
            // TODO: PRINT PATH
            printPath();
            break;
        }

        for (Coordinates next : this->neighbors(current)) {
            if (this->cameFrom.find(next) == this->cameFrom.end()) {
                frontier.push(next);
                this->cameFrom[next] = current;
                // if (next != this->start && next != this->goal)
                    Visualizer::setTile(next, State::visited);
            }
        }
    }
    this->cameFrom.clear();
    this->obstacles.clear();
    // Visualizer::setTile(Visualizer::startCoordinates, State::start);
    Visualizer::setTile(Visualizer::goalCoordinates, State::goal);
}
