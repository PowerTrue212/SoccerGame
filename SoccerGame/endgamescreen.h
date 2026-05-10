#pragma once

#include <QtWidgets/QWidget>
#include <QPointer>

class QLabel;
class QPushButton;

// 结束界面，展示胜负信息与操作按钮。
class EndGameScreen : public QWidget
{
    Q_OBJECT

public:
    // 构建结束界面并初始化比分与队伍信息。
    explicit EndGameScreen(int score1, int score2, int player1Type, int player2Type, QWidget* parent = nullptr);
    ~EndGameScreen() override = default;

    // 更新结束界面显示的比分与胜方信息。
    void setResult(int score1, int score2, int player1Type, int player2Type);
    // 播放胜利视频。
    void playCelebrationVideo(const QString& videoPath);

protected:
    // 处理窗口尺寸变化以适配视频层。
    void resizeEvent(QResizeEvent* event) override;
    // 绘制结束界面背景。
    void paintEvent(QPaintEvent* event) override;

signals:
    // 重新开始比赛。
    void playAgain();
    // 返回主菜单。
    void backToMenu();
    // 胜利视频播放完毕。
    void celebrationFinished(const QString& videoPath);

private:
    QLabel* resultLabel;
    QPushButton* playAgainButton;
    QPushButton* backToMenuButton;
    QPointer<QWidget> videoWidget;
    QPointer<class QMediaPlayer> videoPlayer;
    QPointer<class QAudioOutput> audioOutput;
    int player1Type = -1;
    int player2Type = -1;
    QPixmap background;
};
