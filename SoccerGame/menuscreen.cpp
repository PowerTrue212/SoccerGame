#include "menuscreen.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsDropShadowEffect>   // 用于按钮阴影
#include <QLabel>

MenuScreen::MenuScreen(QWidget* parent)
    : QWidget(parent),
    startButton(new QPushButton("开始游戏", this)),
    exitButton(new QPushButton("退出", this))
{
    setObjectName("menuScreen");

    auto* titleLabel = new QLabel("SoccerGame", this);
    QFont titleFont("Microsoft YaHei", 48, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    titleLabel->setStyleSheet("color: white;");

    // ---------- 大小 ----------
    startButton->setFixedSize(250, 120);
    exitButton->setFixedSize(250, 120);

    // ---------- 字体 ----------
    QFont btnFont("Microsoft YaHei", 24, QFont::Bold);
    startButton->setFont(btnFont);
    exitButton->setFont(btnFont);

    // ---------- 给两个按钮添加立体投影 ----------
    auto* startShadow = new QGraphicsDropShadowEffect;
    startShadow->setBlurRadius(20);               // 阴影模糊度
    startShadow->setColor(QColor(0, 0, 0, 100));  // 黑色半透明
    startShadow->setOffset(0, 6);                 // 向下偏移 6 像素
    startButton->setGraphicsEffect(startShadow);

    auto* exitShadow = new QGraphicsDropShadowEffect;
    exitShadow->setBlurRadius(20);
    exitShadow->setColor(QColor(0, 0, 0, 100));
    exitShadow->setOffset(0, 6);
    exitButton->setGraphicsEffect(exitShadow);

    // ---------- 立体样式表 ----------
    // 退出按钮 – 灰色立体
    exitButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #c0c5c8, stop:1 #8a9196);
            color: white;
            border: 1px solid #6c7276;
            border-radius: 12px;
            padding: 10px 20px;          /* 预留按下位移的空间 */
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #d1d6d9, stop:1 #9ba1a6);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #8a9196, stop:1 #6c7276);
            padding-top: 14px;           /* 按下时文字下移，产生凹陷感 */
            padding-bottom: 6px;
        }
    )");

    // 开始按钮 – 橙色立体
    startButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f39c12, stop:1 #d35400);
            color: white;
            border: 1px solid #a04000;
            border-radius: 12px;
            padding: 10px 20px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f5b041, stop:1 #e67e22);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #d35400, stop:1 #a04000);
            padding-top: 14px;           /* 按下位移 */
            padding-bottom: 6px;
        }
    )");

    setStyleSheet(
        styleSheet() +   // 注意：如果之前已经给 this 设置过样式，需要叠加
        "QWidget#menuScreen {"
        "   border-image: url(:/images/image/background.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    // ---------- 布局 ----------
    auto* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addSpacing(80);
    layout->addWidget(startButton, 0, Qt::AlignHCenter);
    layout->addSpacing(150);
    layout->addWidget(exitButton, 0, Qt::AlignHCenter);
    layout->addStretch();

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}


QPushButton* MenuScreen::startGameButton() const
{
    return startButton;
}
