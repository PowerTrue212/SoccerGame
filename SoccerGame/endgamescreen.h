#pragma once

#include <QtWidgets/QWidget>
#include <QPointer>

class QLabel;
class QPushButton;

class EndGameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit EndGameScreen(int score1, int score2, int player1Type, int player2Type, QWidget* parent = nullptr);
    ~EndGameScreen() override = default;

    void setResult(int score1, int score2, int player1Type, int player2Type);
    void playCelebrationVideo(const QString& videoPath);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

signals:
    void playAgain();
    void backToMenu();
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
