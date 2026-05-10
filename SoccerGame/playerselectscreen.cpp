#include "playerselectscreen.h"
#include "constants.h"
#include "player.h"

#include <QKeyEvent>
#include <QPainter>

PlayerSelectScreen::PlayerSelectScreen(QWidget* parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    background = QPixmap(":/images/image/background.png");
    headshots = {
        QPixmap(":/SoccerGame/image/player_1.png"),
        QPixmap(":/SoccerGame/image/player_2.png"),
        QPixmap(":/SoccerGame/image/player_3.png"),
        QPixmap(":/SoccerGame/image/player_4.png"),
        QPixmap(":/SoccerGame/image/player_5.png"),
        QPixmap(":/SoccerGame/image/player_6.png")
    };

    bodies = {
        QPixmap(":/SoccerGame/image/CHE.png"),
        QPixmap(":/SoccerGame/image/ATM.png"),
        QPixmap(":/SoccerGame/image/ARS.png"),
        QPixmap(":/SoccerGame/image/MCI.png"),
        QPixmap(":/SoccerGame/image/PSG.png"),
        QPixmap(":/SoccerGame/image/FCB.png")
    };
}

void PlayerSelectScreen::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (!background.isNull()) {
        painter.drawPixmap(rect(), background);
    } else {
        painter.fillRect(rect(), QColor(135, 206, 235));
    }

    const int count = headshots.size();
    const double spacing = 24.0;
    const double totalWidth = Constants::WindowWidth - spacing * 2.0;
    const double slotWidth = totalWidth / count;
    const double headSize = Constants::PlayerRadius * 2.0;
    const double groundY = Constants::GroundLevel;
    const double headCenterY = groundY - Constants::PlayerHeight;
    const double headY = headCenterY - Constants::PlayerRadius;

   const QString names[] = {
        QStringLiteral("恩佐"),
        QStringLiteral("阿尔瓦雷斯"),
        QStringLiteral("赖斯"),
        QStringLiteral("哈兰德"),
        QStringLiteral("凯恩"),
        QStringLiteral("登贝莱")
    };

  QFont nameFont(QStringLiteral("Microsoft YaHei"), 18, QFont::Bold);
    painter.setFont(nameFont);
    painter.setPen(Qt::white);
    for (int i = 0; i < count; ++i) {
        const double centerX = spacing + slotWidth * (i + 0.5);

        Player preview;
        preview.pos = QPointF(centerX, headCenterY);
        preview.playerface = 1;
        preview.setHeadshot(headshots[i]);
        preview.setBody(bodies[i]);
        preview.DrawPlayer(&painter);

        painter.drawText(QRectF(centerX - slotWidth * 0.5, groundY + 8, slotWidth, 40), Qt::AlignHCenter | Qt::AlignTop, names[i]);
    }

    const double arrowSize = 18.0;
    const double topArrowY = headY - 55.0;
    const double bottomArrowY = groundY + 45.0;

    auto drawTopArrow = [&](int index, const QColor& color) {
        const double centerX = spacing + slotWidth * (index + 0.5);
        QPolygonF topArrow;
        topArrow << QPointF(centerX, topArrowY + arrowSize)
                 << QPointF(centerX - arrowSize * 0.6, topArrowY)
                 << QPointF(centerX + arrowSize * 0.6, topArrowY);

        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(topArrow);
    };

    auto drawBottomArrow = [&](int index, const QColor& color) {
        const double centerX = spacing + slotWidth * (index + 0.5);
        QPolygonF bottomArrow;
        bottomArrow << QPointF(centerX, bottomArrowY)
                    << QPointF(centerX - arrowSize * 0.6, bottomArrowY + arrowSize)
                    << QPointF(centerX + arrowSize * 0.6, bottomArrowY + arrowSize);

        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(bottomArrow);
    };

   drawTopArrow(player1Index, QColor(255, 215, 0));
    drawBottomArrow(player2Index, QColor(0, 200, 255));
}

void PlayerSelectScreen::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) {
        QWidget::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_A) {
        player1Index = (player1Index + headshots.size() - 1) % headshots.size();
        update();
        return;
    }

    if (event->key() == Qt::Key_D) {
        player1Index = (player1Index + 1) % headshots.size();
        update();
        return;
    }

    if (event->key() == Qt::Key_Left) {
        player2Index = (player2Index + headshots.size() - 1) % headshots.size();
        update();
        return;
    }

    if (event->key() == Qt::Key_Right) {
        player2Index = (player2Index + 1) % headshots.size();
        update();
        return;
    }

    if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        emit playersSelected(player1Index, player2Index);
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        emit backToMenu();
        return;
    }

    QWidget::keyPressEvent(event);
}
