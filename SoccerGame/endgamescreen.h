#pragma once

#include <QtWidgets/QWidget>
#include <QPointer>

class QLabel;
class QPushButton;

class EndGameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit EndGameScreen(int score1, int score2, QWidget* parent = nullptr);
    ~EndGameScreen() override = default;

    void setResult(int score1, int score2);
    void playCelebrationVideo(const QString& videoPath);

protected:
    void resizeEvent(QResizeEvent* event) override;

signals:
    void playAgain();
    void backToMenu();

private:
    QLabel* resultLabel;
    QPushButton* playAgainButton;
    QPushButton* backToMenuButton;
    QPointer<QWidget> videoWidget;
    QPointer<class QMediaPlayer> videoPlayer;
    QPointer<class QAudioOutput> audioOutput;
};
