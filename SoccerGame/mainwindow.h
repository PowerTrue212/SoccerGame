#pragma once

#include <QtWidgets/QMainWindow>

class QStackedWidget;
class MenuScreen;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private:
    void onStartGameClicked();

    QStackedWidget* stackedWidget;
    MenuScreen* menuScreen;
};
