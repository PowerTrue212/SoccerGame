#include "endgamescreen.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>

EndGameScreen::EndGameScreen(int score1, int score2, QWidget* parent)
    : QWidget(parent),
      resultLabel(new QLabel(this)),
      playAgainButton(new QPushButton("再来一局", this)),
      backToMenuButton(new QPushButton("返回主菜单", this))
{
    resultLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Arial", 28);
    titleFont.setBold(true);
    resultLabel->setFont(titleFont);

    playAgainButton->setMinimumSize(220, 56);
    backToMenuButton->setMinimumSize(220, 56);

    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->addStretch();
    rootLayout->addWidget(resultLabel, 0, Qt::AlignHCenter);
    rootLayout->addSpacing(36);
    rootLayout->addWidget(playAgainButton, 0, Qt::AlignHCenter);
    rootLayout->addSpacing(14);
    rootLayout->addWidget(backToMenuButton, 0, Qt::AlignHCenter);
    rootLayout->addStretch();

    setResult(score1, score2);

    connect(playAgainButton, &QPushButton::clicked, this, &EndGameScreen::playAgain);
    connect(backToMenuButton, &QPushButton::clicked, this, &EndGameScreen::backToMenu);
}

void EndGameScreen::setResult(int score1, int score2)
{
    QString winnerText;
    if (score1 > score2) {
        winnerText = "玩家1 获胜！";
    }
    else if (score2 > score1) {
        winnerText = "玩家2 获胜！";
    }
    else {
        winnerText = "平局！";
    }

    resultLabel->setText(QString("%1\n最终比分：%2 : %3").arg(winnerText).arg(score1).arg(score2));
}
