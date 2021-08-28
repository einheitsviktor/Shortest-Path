#ifndef GRID_H
#define GRID_H

#include "helper.h"
#include "visualizer.h"


class Grid {



private:
    int width, height;
    Coordinates start, goal;

    // Container
    static std::array<Coordinates, 4> DIRS;
    std::unordered_set<Coordinates> obstacles;
    std::unordered_map<Coordinates, Coordinates> cameFrom;

    // Getter
    bool inBounds(Coordinates id) const;
    bool passable(Coordinates id) const;
    std::vector<Coordinates> neighbors(Coordinates id) const;

public:
    // Grid(int width, int height, QVector<QVector<Tile*>>& tiles);
    Grid();

    // Methods
    void initGrid(QVector<QVector<Tile*>> tiles);
    void printPath();

    // Search algorithm
    void breadthFirstSearch();
};

#endif // GRID_H
