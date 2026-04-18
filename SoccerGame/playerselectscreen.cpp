#include "playerselectscreen.h"

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

PlayerSelectScreen::PlayerSelectScreen(QWidget* parent)
    : QWidget(parent),
      player1ComboBox(new QComboBox(this)),
      player2ComboBox(new QComboBox(this))
{
    player1ComboBox->addItems({"前锋", "中场", "后卫"});
    player2ComboBox->addItems({"前锋", "中场", "后卫"});

    auto* player1Group = new QGroupBox("玩家1", this);
    auto* player2Group = new QGroupBox("玩家2", this);

    auto* player1Layout = new QVBoxLayout(player1Group);
    auto* player2Layout = new QVBoxLayout(player2Group);
    player1Layout->addWidget(player1ComboBox);
    player2Layout->addWidget(player2ComboBox);

    auto* playersLayout = new QHBoxLayout();
    playersLayout->addWidget(player1Group);
    playersLayout->addSpacing(24);
    playersLayout->addWidget(player2Group);

    auto* startButton = new QPushButton("开始比赛", this);
    auto* backButton = new QPushButton("返回", this);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);
    buttonLayout->addSpacing(16);
    buttonLayout->addWidget(startButton);
    buttonLayout->addStretch();

    auto* centerLayout = new QVBoxLayout();
    centerLayout->addLayout(playersLayout);
    centerLayout->addSpacing(28);
    centerLayout->addLayout(buttonLayout);

    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->addStretch();
    rootLayout->addLayout(centerLayout);
    rootLayout->addStretch();

    connect(startButton, &QPushButton::clicked, this, [this] {
        emit playersSelected(player1ComboBox->currentIndex(), player2ComboBox->currentIndex());
    });

    connect(backButton, &QPushButton::clicked, this, [this] {
        emit backToMenu();
    });
}
