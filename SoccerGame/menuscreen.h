#pragma once

#include <QtWidgets/QWidget>

#include <QPushButton>
#include <QVBoxLayout>

class QPushButton;

class MenuScreen : public QWidget
{
public:
    explicit MenuScreen(QWidget* parent = nullptr);
    ~MenuScreen() override = default;

    QPushButton* startGameButton() const;

private:
    QPushButton* startButton;
    QPushButton* exitButton;
};
