#include "visualizer.h"
#include "./ui_visualizer.h"
#include "helper.h"

Visualizer::Visualizer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Visualizer)
    , algorithm(Algorithm::breadthFirst)
{
    ui->setupUi(this);
    setupFloor();
}

Visualizer::~Visualizer() { delete ui; }

QVector<QVector<Tile*>> Visualizer::floor;
Coordinates Visualizer::startCoordinates;
Coordinates Visualizer::goalCoordinates;


// Helper function
bool inBounds(Coordinates id) {
    return  0 <= id.x && id.x < WIDTH && 0 <= id.y && id.y < HEIGHT;
}
void Visualizer::setTile(Coordinates id, State state) {
    if (inBounds(id)) {
        switch (state) {
            case State::empty: {
                floor[id.y][id.x]->state = State::empty;
                floor[id.y][id.x]->setStyleSheet(EMPTY);
                floor[id.y][id.x]->setText("");
                break;
            }
            case State::obstacle: {
                floor[id.y][id.x]->state = State::obstacle;
                floor[id.y][id.x]->setStyleSheet(OBSTACLE);
                floor[id.y][id.x]->setText("");
                break;
            }
            case State::visited: {
                floor[id.y][id.x]->state = State::visited;
                floor[id.y][id.x]->setStyleSheet(VISITED);
                break;
            }
            case State::path: {
                floor[id.y][id.x]->state = State::path;
                floor[id.y][id.x]->setStyleSheet(PATH);
                break;
            }
            case State::start: {
                floor[id.y][id.x]->state = State::start;
                floor[id.y][id.x]->setStyleSheet(START);
                floor[id.y][id.x]->setText("S");
                startCoordinates = id;
                break;
            }
            case State::goal: {
                floor[id.y][id.x]->state = State::goal;
                floor[id.y][id.x]->setStyleSheet(GOAL);
                floor[id.y][id.x]->setText("G");
                goalCoordinates = id;
                break;
            }
            default: break;
        }
    }

}
void Visualizer::handleObstacleClick(int index) {
    if (searchExecuted) clearFloor();

    int height = index / WIDTH;
    int width = index % WIDTH;

    if (floor[height][width]->isEmpty())
        setTile({width, height}, State::obstacle);
    else if (floor[height][width]->isObstacle())
        setTile({width, height}, State::empty);
}
void Visualizer::setupFloor() {
    QGridLayout* gridLayout = ui->gridLayout;
    QSignalMapper* mapper = new QSignalMapper(this);

    // i = height; j = width
    for (int height = 0; height < HEIGHT; ++height) {
        QVector<Tile*> row;
        for (int width = 0; width < WIDTH; ++width) {
            Tile* tile = new Tile(height, width, State::empty);
            row.push_back(tile);
            tile->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            tile->setText("");
            tile->setMinimumSize(QSize(20, 20));
            QFont font = tile->font();
            font.setPointSize(17);
            tile->setFont(font);
            gridLayout->addWidget(tile, height, width);
            mapper->setMapping(tile, height * WIDTH + width);
            connect(tile, SIGNAL(clicked()), mapper, SLOT(map()));
        }
        floor.push_back(row);
    }
    gridLayout->setSpacing(0);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(handleObstacleClick(int)));
    setTile({1, 1}, State::start);
    setTile({WIDTH-2, HEIGHT-2}, State::goal);
}
void Visualizer::resetFloor() {
    for (auto& row : floor) {
        for (auto& tile : row) {
            if (tile->isVisited() || tile->isObstacle() || tile->isPath())
                setTile({tile->y, tile->x}, State::empty);
        }
    }
    this->searchExecuted = false;
}
void Visualizer::clearFloor() {
    for (auto& row : floor) {
        for (auto& tile : row) {
            if (tile->isVisited() || tile->isPath())
                setTile({tile->y, tile->x}, State::empty);
        }
    }
    this->searchExecuted = false;
}

void Visualizer::on_Reset_clicked() { resetFloor(); }
void Visualizer::on_Clear_clicked() { clearFloor(); }
void Visualizer::on_Search_clicked() {
    if (this->searchExecuted) clearFloor();
    if (this->algorithm == Algorithm::breadthFirst) {
        static Grid grid; // TODO: Change
        grid.initGrid(floor);
        grid.breadthFirstSearch();
    }
    else if (this->algorithm == Algorithm::dijkstra) {
        static WeightedGrid grid;
        grid.initGrid(floor);
        grid.dijkstraSearch();
    }
    else if (this->algorithm == Algorithm::astar) {
        static WeightedGrid grid;
        grid.initGrid((floor));
        grid.aStarSearch();
    }
    this->searchExecuted = true;
}

// Move Start and Goal
void Visualizer::updateStart(Coordinates id) {
    if (this->searchExecuted) clearFloor();
    if (inBounds(id) && !floor[id.y][id.x]->isGoal()) {
        setTile(startCoordinates, State::empty);
        setTile(id, State::start);
    }
}
void Visualizer::updateGoal(Coordinates id) {
    if (this->searchExecuted) clearFloor();
    if (inBounds(id) && !floor[id.y][id.x]->isStart()) {
        setTile(goalCoordinates, State::empty);
        setTile(id, State::goal);
    }
}
void Visualizer::on_UpO_clicked() { updateStart({startCoordinates.x, startCoordinates.y-1}); }
void Visualizer::on_LeftO_clicked() { updateStart({startCoordinates.x-1, startCoordinates.y}); }
void Visualizer::on_DownO_clicked() { updateStart({startCoordinates.x, startCoordinates.y+1}); }
void Visualizer::on_RightO_clicked() { updateStart({startCoordinates.x+1, startCoordinates.y}); }

void Visualizer::on_UpD_clicked() { updateGoal({goalCoordinates.x, goalCoordinates.y-1}); }
void Visualizer::on_LeftD_clicked() { updateGoal({goalCoordinates.x-1, goalCoordinates.y}); }
void Visualizer::on_DownD_clicked() { updateGoal({goalCoordinates.x, goalCoordinates.y+1}); }
void Visualizer::on_RightD_clicked() { updateGoal({goalCoordinates.x+1, goalCoordinates.y}); }

void Visualizer::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Return:    on_Search_clicked(); break;
        case Qt::Key_Backspace: on_Clear_clicked(); break;
        case Qt::Key_R:         on_Reset_clicked(); break;
        // // WASD
        case Qt::Key_W: on_UpO_clicked(); break;
        case Qt::Key_A: on_LeftO_clicked(); break;
        case Qt::Key_S: on_DownO_clicked(); break;
        case Qt::Key_D: on_RightO_clicked(); break;
        // // IJKL
        case Qt::Key_I: on_UpD_clicked(); break;
        case Qt::Key_J: on_LeftD_clicked(); break;
        case Qt::Key_K: on_DownD_clicked(); break;
        case Qt::Key_L: on_RightD_clicked(); break;
        default: break;
    }
}

// Preset printer
void Visualizer::printPreset(const QVector<QVector<int>>& preset) {
    resetFloor();
    for (auto& c : preset)
        setTile({c[1], c[0]}, State::obstacle);
}

void Visualizer::on_Preset1_clicked() {
    printPreset(this->preset1);
    updateStart({12, 10});
    updateGoal({34, 1});
}
void Visualizer::on_Preset2_clicked() {
    printPreset(this->preset2);
    updateStart({0, 1});
    updateGoal({38, 5});
}
void Visualizer::on_Preset3_clicked() {
   printPreset(this->preset3);
    updateStart({0, 1});
    updateGoal({35, 2});
}
void Visualizer::on_Preset4_clicked() {
   printPreset(this->preset4);
    updateStart({11, 15});
    updateGoal({22, 4});
}
void Visualizer::on_Preset5_clicked() {
   printPreset(this->preset5);
    updateStart({11, 9});
    updateGoal({26, 9});
}

void Visualizer::on_BreadthSearch_toggled(bool checked) { this->algorithm = Algorithm::breadthFirst; }
void Visualizer::on_DijkstraSearch_toggled(bool checked) { this->algorithm = Algorithm::dijkstra; }
void Visualizer::on_AstarSearch_toggled(bool checked) { this->algorithm = Algorithm::astar; }
