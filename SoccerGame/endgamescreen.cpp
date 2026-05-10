#include "endgamescreen.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QString>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QUrl>
#include <QResizeEvent>

EndGameScreen::EndGameScreen(int score1, int score2, int player1Type, int player2Type, QWidget* parent)
    : QWidget(parent),
      resultLabel(new QLabel(this)),
      playAgainButton(new QPushButton("再来一局", this)),
      backToMenuButton(new QPushButton("返回主菜单", this)),
      player1Type(player1Type),
      player2Type(player2Type)
{
    setAutoFillBackground(false);
    background = QPixmap(":/images/image/background.png");

    resultLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Microsoft YaHei", 32, QFont::Bold);
    titleFont.setBold(true);
    resultLabel->setFont(titleFont);
    resultLabel->setStyleSheet("color: white;");

    playAgainButton->setMinimumSize(220, 56);
    backToMenuButton->setMinimumSize(220, 56);

    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->addStretch();
    rootLayout->addWidget(resultLabel, 0, Qt::AlignHCenter);
    rootLayout->addSpacing(12);
    rootLayout->addStretch();
    rootLayout->addWidget(playAgainButton, 0, Qt::AlignHCenter);
    rootLayout->addSpacing(14);
    rootLayout->addWidget(backToMenuButton, 0, Qt::AlignHCenter);
    rootLayout->addStretch();

    setResult(score1, score2, player1Type, player2Type);

    connect(playAgainButton, &QPushButton::clicked, this, &EndGameScreen::playAgain);
    connect(backToMenuButton, &QPushButton::clicked, this, &EndGameScreen::backToMenu);
}

void EndGameScreen::setResult(int score1, int score2, int player1TypeValue, int player2TypeValue)
{
    QString winnerText;
    const QString abbreviations[] = {
        QStringLiteral("CHE"),
        QStringLiteral("ATM"),
        QStringLiteral("ARS"),
        QStringLiteral("MCI"),
        QStringLiteral("PSG"),
        QStringLiteral("FCB")
    };

    auto abbreviationForType = [&](int type) -> QString {
        if (type >= 0 && type < 6) {
            return abbreviations[type];
        }
        return QString();
    };

    if (score1 > score2) {
        winnerText = abbreviationForType(player1TypeValue);
    }
    else if (score2 > score1) {
        winnerText = abbreviationForType(player2TypeValue);
    }
    else {
        winnerText = QStringLiteral("平局！");
    }

    const QString displayText = winnerText.isEmpty() ? QStringLiteral("平局！") : winnerText + "   WIN!";
    resultLabel->setText(QString("%1\n最终比分：%2 : %3").arg(displayText).arg(score1).arg(score2));
}

void EndGameScreen::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (videoWidget) {
        videoWidget->setGeometry(rect());
    }
}

void EndGameScreen::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (!background.isNull()) {
        painter.drawPixmap(rect(), background);
    }
    else {
        painter.fillRect(rect(), QColor(135, 206, 235));
    }

    QWidget::paintEvent(event);
}

void EndGameScreen::playCelebrationVideo(const QString& videoPath)
{
    if (videoWidget) {
        videoWidget->deleteLater();
        videoWidget = nullptr;
    }
    if (videoPlayer) {
        videoPlayer->stop();
        videoPlayer->deleteLater();
        videoPlayer = nullptr;
    }
    if (audioOutput) {
        audioOutput->deleteLater();
        audioOutput = nullptr;
    }

    if (videoPath.isEmpty()) {
        return;
    }

    auto* player = new QMediaPlayer(this);
    auto* widget = new QVideoWidget(this);
    auto* output = new QAudioOutput(this);
    videoWidget = widget;
    videoPlayer = player;
    audioOutput = output;

    widget->setAttribute(Qt::WA_DeleteOnClose, true);
    widget->setGeometry(rect());
    player->setVideoOutput(widget);
    player->setAudioOutput(output);
    player->setSource(QUrl(videoPath));

    widget->show();
    player->play();

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this, player, widget, videoPath](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            widget->close();
            videoWidget = nullptr;
            player->deleteLater();
            videoPlayer = nullptr;
            emit celebrationFinished(videoPath);
        }
    });
}
