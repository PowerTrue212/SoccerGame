#include "mainwindow.h"

#include "constants.h"
#include "menuscreen.h"

#include <QDebug>
#include <QPushButton>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      menuScreen(new MenuScreen(this))
{
    setWindowTitle("SoccerGame");
    setFixedSize(Constants::WindowWidth, Constants::WindowHeight);

    setCentralWidget(stackedWidget);
    stackedWidget->addWidget(menuScreen);
    stackedWidget->setCurrentWidget(menuScreen);

    connect(menuScreen->startGameButton(), &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
}

void MainWindow::onStartGameClicked()
{
    qDebug() << "Start game button clicked";
}
