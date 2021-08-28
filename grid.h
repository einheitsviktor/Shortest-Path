#ifndef GRID_H
#define GRID_H

#include "helper.h"
#include "visualizer.h"


class Grid {
    // int width, height;
    // Coordinates start, goal;
public:

    // Container
    static std::array<Coordinates, 4> DELTA;
    std::unordered_set<Coordinates> obstacles;
    std::unordered_map<Coordinates, Coordinates> cameFrom;

    // Getter
    bool inBounds(Coordinates id) const;
    bool passable(Coordinates id) const;
    std::vector<Coordinates> neighbors(Coordinates id) const;

    // Grid(int width, int height, QVector<QVector<Tile*>>& tiles);
    Grid();

    // Methods
    void initGrid(QVector<QVector<Tile*>> tiles);
    void printPath();

    // Search algorithm
    void breadthFirstSearch();
};

class WeightedGrid : public  Grid {
public:
    std::unordered_map<Coordinates, double> costSoFar;
    double cost(Coordinates fromNode, Coordinates toNode) const;
    void dijkstraSearch();
    void aStarSearch();
};

#endif // GRID_H
