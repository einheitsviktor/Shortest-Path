#include "grid.h"

Grid::Grid() {
    initGrid(Visualizer::floor);
}

std::array<Coordinates, 4> Grid::DELTA = {
    Coordinates{1, 0},  // East
    Coordinates{-1, 0}, // West
    Coordinates{0, -1}, // North
    Coordinates{0, 1}   // South
};

bool Grid::inBounds(Coordinates id) const {
    return 0 <= id.x && id.x < WIDTH && 0 <= id.y && id.y < HEIGHT;
}
bool Grid::passable(Coordinates id) const {
    return this->obstacles.find(id) == this->obstacles.end();
}

std::vector<Coordinates> Grid::neighbors(Coordinates id) const {
    std::vector<Coordinates> ret;

    for (const auto& dir : this->DELTA) {
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
            if (tile->state == State::obstacle)
                this->obstacles.insert(Coordinates{tile->y, tile->x});
        }
    }
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
                Visualizer::setTile(next, State::visited);
            }
        }
    }
    this->cameFrom.clear();
    this->obstacles.clear();
    // Visualizer::setTile(Visualizer::startCoordinates, State::start);
    Visualizer::setTile(Visualizer::goalCoordinates, State::goal);
}

double WeightedGrid::cost(Coordinates fromNode, Coordinates toNode) const {
    return 1;
}

void WeightedGrid::dijkstraSearch() {
    PrioriyQueue<Coordinates, double> frontier;
    frontier.put(Visualizer::startCoordinates, 0);

    this->cameFrom[Visualizer::startCoordinates] = Visualizer::startCoordinates;
    this->costSoFar[Visualizer::startCoordinates] = 0;

    while (!frontier.empty()) {
        Coordinates current = frontier.get();

        if (current == Visualizer::goalCoordinates) {
            printPath();
            break;
        }

        for (Coordinates next : this->neighbors(current)) {
            double newCost = this->costSoFar[current]  +  this->cost(current, next);
            if (this->costSoFar.find(next) == this->costSoFar.end() || newCost < this->costSoFar[next]) {
                this->costSoFar[next] = newCost;
                this->cameFrom[next] = current;
                frontier.put(next, newCost);
                Visualizer::setTile(next, State::visited);
            }
        }
    }
    this->cameFrom.clear();
    this->obstacles.clear();
    this->costSoFar.clear();
    Visualizer::setTile(Visualizer::goalCoordinates, State::goal);
}

// Helper function
inline double heuristic(const Coordinates& a, const Coordinates& b) {
    return std::abs(b.x - a.x) + abs(b.y - a.y);
}

void WeightedGrid::aStarSearch() {
    PrioriyQueue<Coordinates, double> frontier;
    frontier.put(Visualizer::startCoordinates, 0);

    this->cameFrom[Visualizer::startCoordinates] = Visualizer::startCoordinates;
    this->costSoFar[Visualizer::startCoordinates] = 0;

    while (!frontier.empty()) {
        Coordinates current = frontier.get();

        if (current == Visualizer::goalCoordinates) {
            printPath();
            break;
        }

        for (Coordinates next : this->neighbors(current)) {
            double newCost = this->costSoFar[current] + this->cost(current, next);
            if (this->costSoFar.find(next) == this->costSoFar.end() || newCost < this->costSoFar[next]) {
                this->costSoFar[next] = newCost;
                double priority = newCost + heuristic(next, Visualizer::goalCoordinates);
                frontier.put(next, priority);
                this->cameFrom[next] = current;
                Visualizer::setTile(next, State::visited);
            }
        }
    }
    this->cameFrom.clear();
    this->obstacles.clear();
    this->costSoFar.clear();
    Visualizer::setTile(Visualizer::goalCoordinates, State::goal);
}
