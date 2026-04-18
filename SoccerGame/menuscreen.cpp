#include "menuscreen.h"

#include <QApplication>
#include <QDebug>

MenuScreen::MenuScreen(QWidget* parent)
    : QWidget(parent),
      startButton(new QPushButton("开始游戏", this)),
      exitButton(new QPushButton("退出", this))
{
    auto* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(startButton, 0, Qt::AlignHCenter);
    layout->addSpacing(20);
    layout->addWidget(exitButton, 0, Qt::AlignHCenter);
    layout->addStretch();

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}

QPushButton* MenuScreen::startGameButton() const
{
    return startButton;
}
