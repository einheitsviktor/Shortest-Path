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
Tile* Visualizer::originPtr = nullptr;
Tile* Visualizer::destinationPtr = nullptr;


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
            case State::visitied: {
                floor[id.y][id.x]->state = State::visitied;
                floor[id.y][id.x]->setStyleSheet(VISITED);
                // floor[x][y]->setText("");

        }
            case State::origin: {
                floor[id.y][id.x]->state = State::origin;
                floor[id.y][id.x]->setStyleSheet(ORIGIN);
                floor[id.y][id.x]->setText("O");
                originPtr = floor[id.y][id.x];
                break;
            }
            case State::destination: {
                floor[id.y][id.x]->state = State::destination;
                floor[id.y][id.x]->setStyleSheet(DESTINATION);
                floor[id.y][id.x]->setText("D");
                destinationPtr = floor[id.y][id.x];
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

    qInfo() << "{" << height << "," << width << "},";
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
    setTile({1, 1}, State::origin);
    setTile({WIDTH-2, HEIGHT-2}, State::destination);
}
void Visualizer::resetFloor() {
    for (auto& row : floor) {
        for (auto& tile : row) {
            if (tile->isVisited() || tile->isObstacle())
                setTile({tile->y, tile->x}, State::empty);
        }
    }
    this->searchExecuted = false;
}
void Visualizer::clearFloor() {
    for (auto& row : floor) {
        for (auto& tile : row) {
            if (tile->isVisited())
                setTile({tile->y, tile->x}, State::empty);
        }
    }
    this->searchExecuted = false;
}

void Visualizer::on_Reset_clicked() { resetFloor(); }
void Visualizer::on_Clear_clicked() { clearFloor(); }
void Visualizer::on_Search_clicked() {
    static Grid grid; // TODO: Change
}

// Move Start and Goal
void Visualizer::updateOrigin(Coordinates id) {
    if (inBounds(id) && !floor[id.y][id.x]->isDestination()) {
        setTile({originPtr->y, originPtr->x}, State::empty);
        setTile(id, State::origin);
    }
}
void Visualizer::updateDestination(Coordinates id) {
    if (inBounds(id) && !floor[id.y][id.x]->isOrigin()) {
        setTile({destinationPtr->y, destinationPtr->x}, State::empty);
        setTile(id, State::destination);
    }
}
void Visualizer::on_UpO_clicked() { updateOrigin({originPtr->y, originPtr->x-1}); }
void Visualizer::on_LeftO_clicked() { updateOrigin({originPtr->y-1, originPtr->x}); }
void Visualizer::on_DownO_clicked() { updateOrigin({originPtr->y, originPtr->x+1}); }
void Visualizer::on_RightO_clicked() { updateOrigin({originPtr->y+1, originPtr->x}); }
void Visualizer::on_UpD_clicked() { updateDestination({destinationPtr->y, destinationPtr->x-1}); }
void Visualizer::on_LeftD_clicked() { updateDestination({destinationPtr->y-1, destinationPtr->x}); }
void Visualizer::on_DownD_clicked() { updateDestination({destinationPtr->y, destinationPtr->x+1}); }
void Visualizer::on_RightD_clicked() { updateDestination({destinationPtr->y+1, destinationPtr->x}); }

void Visualizer::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Return:    on_Search_clicked(); break;
        case Qt::Key_Backspace: on_Clear_clicked(); break;
        case Qt::Key_R:         on_Reset_clicked(); break;
        case Qt::Key_P: printObstacles(); break;
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


// TODO: DELETE LATER
//HELPER
void Visualizer::printObstacles() {
    for (const auto& row : floor) {
        for (const auto& tile: row) {
            if (tile->isObstacle())
                qDebug() << "{" << tile->x << "," << tile->y << "},";
        }
    }
}

// Preset setter
void Visualizer::printPreset(const QVector<QVector<int>>& preset) {
    resetFloor();
    for (auto& c : preset)
        setTile({c[1], c[0]}, State::obstacle);
}

void Visualizer::on_Preset1_clicked() {
    printPreset(this->preset1);
    updateOrigin({12, 10});
    updateDestination({34, 1});
}
void Visualizer::on_Preset2_clicked() {
    printPreset(this->preset2);
}
void Visualizer::on_Preset3_clicked() {
   printPreset(this->preset3);
}
void Visualizer::on_Preset4_clicked() {
   printPreset(this->preset4);
}
void Visualizer::on_Preset5_clicked() {
   printPreset(this->preset5);
}

void Visualizer::on_BreadthSearch_toggled(bool checked) { this->algorithm = Algorithm::breadthFirst; }
void Visualizer::on_DijkstraSearch_toggled(bool checked) { this->algorithm = Algorithm::dijkstra; }
void Visualizer::on_AstarSearch_toggled(bool checked) { this->algorithm = Algorithm::astar; }
