#pragma once

#include <QtWidgets/QWidget>

class QLabel;
class QPushButton;

class EndGameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit EndGameScreen(int score1, int score2, QWidget* parent = nullptr);
    ~EndGameScreen() override = default;

    void setResult(int score1, int score2);

signals:
    void playAgain();
    void backToMenu();

private:
    QLabel* resultLabel;
    QPushButton* playAgainButton;
    QPushButton* backToMenuButton;
};
