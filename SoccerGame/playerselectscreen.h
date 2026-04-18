#pragma once

#include <QtWidgets/QWidget>

class QComboBox;

class PlayerSelectScreen : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerSelectScreen(QWidget* parent = nullptr);
    ~PlayerSelectScreen() override = default;

signals:
    void playersSelected(int p1Type, int p2Type);
    void backToMenu();

private:
    QComboBox* player1ComboBox;
    QComboBox* player2ComboBox;
};
