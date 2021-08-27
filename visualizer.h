#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Visualizer; }
QT_END_NAMESPACE

class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    Visualizer(QWidget *parent = nullptr);
    ~Visualizer();

private:
    Ui::Visualizer *ui;
};
#endif // VISUALIZER_H
