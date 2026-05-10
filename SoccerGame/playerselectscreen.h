#pragma once

#include <QtWidgets/QWidget>
#include <QPixmap>

// 选人界面，展示球员卡并处理选择输入。
class PlayerSelectScreen : public QWidget
{
    Q_OBJECT

public:
    // 初始化选人界面与资源贴图。
    explicit PlayerSelectScreen(QWidget* parent = nullptr);
    ~PlayerSelectScreen() override = default;

protected:
    // 绘制背景、球员预览以及选择指示。
    void paintEvent(QPaintEvent* event) override;
    // 处理玩家按键选择逻辑。
    void keyPressEvent(QKeyEvent* event) override;

signals:
    // 当两名玩家选择确认时触发。
    void playersSelected(int p1Type, int p2Type);
    // 请求返回主菜单。
    void backToMenu();

private:
    QPixmap background;
    QVector<QPixmap> headshots;
    QVector<QPixmap> bodies;
    int player1Index = 0;
    int player2Index = 0;
};
