#include "endgamescreen.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsDropShadowEffect>
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
    resultLabel->setAttribute(Qt::WA_StyledBackground, true);
    resultLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 160); padding: 16px 28px; border: 2px solid rgba(255, 255, 255, 120);");

    playAgainButton->setMinimumSize(220, 56);
    backToMenuButton->setMinimumSize(220, 56);

    QFont buttonFont("Microsoft YaHei", 22, QFont::Bold);
    playAgainButton->setFont(buttonFont);
    backToMenuButton->setFont(buttonFont);

    auto* playAgainShadow = new QGraphicsDropShadowEffect;
    playAgainShadow->setBlurRadius(20);
    playAgainShadow->setColor(QColor(0, 0, 0, 100));
    playAgainShadow->setOffset(0, 6);
    playAgainButton->setGraphicsEffect(playAgainShadow);

    auto* backShadow = new QGraphicsDropShadowEffect;
    backShadow->setBlurRadius(20);
    backShadow->setColor(QColor(0, 0, 0, 100));
    backShadow->setOffset(0, 6);
    backToMenuButton->setGraphicsEffect(backShadow);

    playAgainButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f39c12, stop:1 #d35400);
            color: white;
            border: 1px solid #a04000;
            border-radius: 12px;
            padding: 8px 18px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f5b041, stop:1 #e67e22);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #d35400, stop:1 #a04000);
            padding-top: 12px;
            padding-bottom: 4px;
        }
    )");

    backToMenuButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #c0c5c8, stop:1 #8a9196);
            color: white;
            border: 1px solid #6c7276;
            border-radius: 12px;
            padding: 8px 18px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #d1d6d9, stop:1 #9ba1a6);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #8a9196, stop:1 #6c7276);
            padding-top: 12px;
            padding-bottom: 4px;
        }
    )");

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
    QColor winnerColor = Qt::white;
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

    auto teamColorForType = [&](int type) -> QColor {
        switch (type) {
        case 0:
            return QColor("#034694");
        case 1:
            return QColor("#CB3524");
        case 2:
            return QColor("#EF0107");
        case 3:
            return QColor("#6CABDD");
        case 4:
            return QColor("#004170");
        case 5:
            return QColor("#DC052D");
        default:
            return Qt::white;
        }
    };

    if (score1 > score2) {
        winnerText = abbreviationForType(player1TypeValue);
        winnerColor = teamColorForType(player1TypeValue);
    }
    else if (score2 > score1) {
        winnerText = abbreviationForType(player2TypeValue);
        winnerColor = teamColorForType(player2TypeValue);
    }
    else {
        winnerText = QStringLiteral("平局！");
        winnerColor = Qt::white;
    }

    const QString displayText = winnerText.isEmpty() ? QStringLiteral("平局！") : winnerText + "   WIN!";
    resultLabel->setText(QString("%1\n最终比分：%2 : %3").arg(displayText).arg(score1).arg(score2));
    resultLabel->setStyleSheet(QString(
        "color: %1; background-color: rgba(0, 0, 0, 160); padding: 16px 28px; border: 2px solid rgba(255, 255, 255, 120);")
        .arg(winnerColor.name()));
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
