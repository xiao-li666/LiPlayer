#pragma once

#include <QtWidgets/QWidget>
#include "ui_Player.h"
#include "DemuxThread.h"

class Player : public QWidget
{
    Q_OBJECT

public:
    Player(QWidget *parent = nullptr);
    ~Player();

    void timerEvent(QTimerEvent* e);
    QString formatTime(long long ms);
    void resizeEvent(QResizeEvent* e);
    void styleSheet();
    void setSpeedButton();
    bool PlayVideo(QString url);

private slots:
    void btnOpenFile();
    void btnIsPlay();
    void updatePosition(long long pts, long long totalMs);
    void sliderPressed();
    void sliderReleased();
    void videoSlider(int value);
    void btnAudio();
    void onSpeedChanged(QAction* action);
    void btnPlayListHint();
    void playFromPlayListWidget(QListWidgetItem* item);
    void btnPreSlot();
    void btnNextSlot();
    void OnBtnNet();
    void OnBtnMaxOrMin();

private:
    Ui::PlayerClass ui;
    DemuxThread dt;
    bool isPause = true;

    bool isPressSlider = false;
    float lastVolum = 0.0f;//记录静音前音量，用于恢复
    bool isVolum = false;//是否静音，初始为false
    int currentplayindex = -1;
};
