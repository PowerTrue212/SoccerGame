#pragma once

#include <QtWidgets/QWidget>

#include <QPushButton>
#include <QVBoxLayout>

class QPushButton;

// 主菜单界面，提供开始与退出入口。
class MenuScreen : public QWidget
{
public:
    // 构建菜单界面并初始化按钮样式。
    explicit MenuScreen(QWidget* parent = nullptr);
    ~MenuScreen() override = default;

    // 获取开始游戏按钮指针。
    QPushButton* startGameButton() const;

private:
    QPushButton* startButton;
    QPushButton* exitButton;
};
