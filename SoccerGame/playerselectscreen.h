#pragma once

#include <QtWidgets/QWidget>
#include <QPixmap>

class PlayerSelectScreen : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerSelectScreen(QWidget* parent = nullptr);
    ~PlayerSelectScreen() override = default;

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void playersSelected(int p1Type, int p2Type);
    void backToMenu();

private:
    QPixmap background;
    QVector<QPixmap> headshots;
    int player1Index = 0;
    int player2Index = 0;
};
