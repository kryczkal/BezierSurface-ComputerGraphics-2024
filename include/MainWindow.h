#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent *event) override;
    ~MainWindow();
};
#endif // MAINWINDOW_H
