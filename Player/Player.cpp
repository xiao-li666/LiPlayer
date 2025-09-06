#include "Player.h"
#include <qfiledialog.h>
#include <iostream>
#include <QMenu>
#include <QActionGroup>
#include <QMessageBox>
#include <QInputDialog>

Player::Player(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->setStyleSheet("background-color: #2c2c2c;");
    styleSheet();
    setSpeedButton();

    ui.audioSlider->setValue(80);

    connect(ui.btnFile, &QPushButton::clicked, this, &Player::btnOpenFile);
    connect(ui.isPlay, &QPushButton::clicked, this, &Player::btnIsPlay);
    // 连接DemuxThread的信号到Player的槽
    connect(&dt, &DemuxThread::positionChanged, this, &Player::updatePosition);
    connect(ui.playPos, &QSlider::sliderPressed, this, &Player::sliderPressed);
    connect(ui.playPos, &QSlider::sliderReleased, this, &Player::sliderReleased);
    connect(ui.audioSlider, &QSlider::valueChanged, this, &Player::videoSlider);
    connect(ui.btnAudio, &QPushButton::clicked, this, &Player::btnAudio);
    connect(ui.btnDeleteFile, &QPushButton::clicked, this, &Player::btnPlayListHint);
    connect(ui.playList, &QListWidget::itemDoubleClicked, this, &Player::playFromPlayListWidget);
    connect(ui.btnPre, &QPushButton::clicked, this, &Player::btnPreSlot);
    connect(ui.btnNext, &QPushButton::clicked, this, &Player::btnNextSlot);
    connect(ui.btnNet, &QPushButton::clicked, this, &Player::OnBtnNet);
    connect(ui.btnMaxMin, &QPushButton::clicked, this, &Player::OnBtnMaxOrMin);
    

    dt.Start();
    //startTimer(200);
}

Player::~Player()
{
    dt.Close();
}

void Player::timerEvent(QTimerEvent* e)
{
    long long duration = dt.getDuration();
    if (duration > 0) {
        double pos = (double)dt.pts / (double)duration;
        int value = ui.playPos->maximum() * pos;
        ui.playPos->setValue(value);
        ui.totalTime->setText(formatTime(duration));
    }
}

QString Player::formatTime(long long ms)
{
    int totalSeconds = ms / 1000;  // 转换为秒

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    return QString("%1:%2:%3")
        .arg(hours, 2, 10, QLatin1Char('0'))    // 2位数小时，不足补零
        .arg(minutes, 2, 10, QLatin1Char('0'))  // 2位数分钟，不足补零
        .arg(seconds, 2, 10, QLatin1Char('0')); // 2位数秒，不足补零
}

void Player::resizeEvent(QResizeEvent* e)
{
    ui.playPos->move(100, this->height() - 160);
    ui.currentTime->move(5, this->height() - 160);
    ui.totalTime->move(this->width() - 240, this->height() - 160);
    ui.playPos->resize(this->width() - 340, ui.playPos->height());
    ui.currentTime->resize(ui.currentTime->width(), ui.currentTime->height());
    ui.totalTime->resize(ui.totalTime->width(), ui.totalTime->height());
    ui.video->resize(QSize(this->width()-220, this->height() - 180));
    ui.playList->move(this->width() - 220, 0);
    ui.playList->resize(this->width() - 1280, this->height() - 180);

    //按钮
    ui.btnNet->move(50, this->height() - 100);
    ui.btnPre->move(300, this->height() - 100);
    ui.isPlay->move(370, this->height() - 100);
    ui.btnNext->move(440, this->height() - 100);
    ui.btnAudio->move(640, this->height() - 100);
    ui.audioSlider->move(692, this->height() - 80);
    ui.tbSpeed->move(900, this->height() - 90);
    ui.btnDeleteFile->move(this->width() - 150, this->height() - 100);
    ui.btnFile->move(this->width() - 220, this->height() - 100);
    ui.btnMaxMin->move(this->width() - 80, this->height() - 100);
}

void Player::styleSheet()
{
    ui.btnPre->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/pre.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"
        "   border-image:url(:/Player/resource/pre1.svg);"// 半透明白色遮罩
        "}"
    );
    ui.btnNext->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/next.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/next1.svg);"
        "}"
    );
    ui.isPlay->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/start.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/start1.svg);"
        "}"
    );
    ui.btnNet->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/net.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/net1.svg);"
        "}"
    );
    ui.btnAudio->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/audio.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/audio1.svg);"
        "}"
    );
    ui.btnDeleteFile->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/list.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/list1.svg);"
        "}"
    );
    ui.btnFile->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/file.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/file1.svg);"
        "}"
    );
    ui.totalTime->setStyleSheet(
        "QLabel {"
        "   color: rgb(255,255,255);"
        "}"
    );
    ui.currentTime->setStyleSheet(
        "QLabel {"
        "   color: rgb(255,255,255);"
        "}"
    );
    ui.btnMaxMin->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/max.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/max1.svg);"
        "}"
    );
    
}

void Player::setSpeedButton()
{
    ui.tbSpeed->setText("1.0x");
    ui.tbSpeed->setPopupMode(QToolButton::InstantPopup);
    // 包含各种状态的完整样式
    ui.tbSpeed->setStyleSheet(
        "QToolButton {"
        "    background-color: #2c2c2c;"           // 正常状态背景色
        "    color: white;"                        // 正常状态文字颜色
        "    border-radius: 8px;"                  // 圆角
        "    border: 2px solid #e6e6e6;"           // 边框
        "    font-family: 'Segoe UI';"             // 字体
        "    font-size: 18px;"                     // 字体大小
        "    font-weight: bold;"                   // 字体粗细
        "    padding: 8px 8px;"                   // 内边距
        "}"
        "QToolButton:hover {"
        //"    background-color: #388E3C;"           // 悬停状态背景色
        "    border: 2px solid #1296db;"           // 悬停状态边框
        "}"
        "QToolButton:pressed {"
        //"    background-color: #2E7D32;"           // 按下状态背景色
        "    padding: 9px 14px 7px 16px;"          // 按下时微调位置，产生按下效果
        "}"
        "QToolButton::menu-indicator {"
        "    image: none;"  // 隐藏箭头
        "}"
    );
    // 创建倍速菜单
    QMenu* speedMenu = new QMenu(this);
    speedMenu->setStyleSheet(
        "QMenu { background-color: #333; color: white; }"
        "QMenu::item:selected { background-color: #555; }"
        "QMenu { menu-scrollable: 1; }"
    );

    // 添加倍速选项
    QActionGroup* speedGroup = new QActionGroup(this);
    float speeds[] = { 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 2.0f, 2.5f, 3.0f };

    for (float speed : speeds) {
        QAction* action = new QAction(QString("%1x").arg(speed), this);
        action->setData(speed);
        action->setCheckable(true);
        if (qFuzzyCompare(speed, 1.0f)) {
            action->setChecked(true);
        }
        speedMenu->addAction(action);
        speedGroup->addAction(action);
    }

    ui.tbSpeed->setMenu(speedMenu);

    // 连接信号槽
    connect(speedMenu, &QMenu::triggered, this, &Player::onSpeedChanged);
}

bool Player::PlayVideo(QString url)
{
    if (url.isEmpty()) {
        return false;
    }
    this->setWindowTitle(url);//设置窗口标题

    if (!dt.Open(url.toStdString().c_str(), ui.video)) {
        QMessageBox::information(nullptr, "error", "Failed to open file!");
        return false;
    }
    isPause = false;
    ui.isPlay->setStyleSheet(
        "QPushButton {"
        "   border-image:url(:/Player/resource/stop.svg);"
        "}"
        "QPushButton:hover {"
        //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
        "   border-image:url(:/Player/resource/stop1.svg);"
        "}"
    );
    return true;
}

void Player::btnIsPlay()
{
    if (isPause) {
        isPause = false;
        dt.setPause(isPause);
        ui.isPlay->setStyleSheet(
            "QPushButton {"
            "   border-image:url(:/Player/resource/stop.svg);"
            "}"
            "QPushButton:hover {"
            //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
            "   border-image:url(:/Player/resource/stop1.svg);"
            "}"
        );
    }
    else {
        isPause = true;
        dt.setPause(isPause);
        ui.isPlay->setStyleSheet(
            "QPushButton {"
            "   border-image:url(:/Player/resource/start.svg);"
            "}"
            "QPushButton:hover {"
            //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
            "   border-image:url(:/Player/resource/start1.svg);"
            "}"
        );
    }
}

void Player::updatePosition(long long pts, long long totalMs)
{
    if (isPressSlider) return;
    if (totalMs > 0) {
        double pos = (double)pts / (double)totalMs;
        int value = ui.playPos->maximum() * pos;
        // 避免在信号频繁时过度刷新UI，可以加判断
        if (value != ui.playPos->value()) {
            ui.playPos->setValue(value);
        }
        ui.totalTime->setText(formatTime(totalMs));
        ui.currentTime->setText(formatTime(pts));
    }
}

void Player::sliderPressed()
{
    isPressSlider = true;
}

void Player::sliderReleased()
{
    isPressSlider = false;
    double pos = 0.0;
    pos = (double)ui.playPos->value() / (double)ui.playPos->maximum();
    dt.seek(pos);
}

void Player::videoSlider(int value)
{
    value = qBound(0, value, 100);
    if (value == 0) {
        dt.setVolum(0.0f);
        return;
    }
    // 线性映射：0~100 → 0.0~1.0（取消过度分段和压缩）
    float volume = value / 100.0f;
    // 可选：轻微对数映射（增强低音量敏感度，避免压缩过度）
    // volume = pow(volume, 1.2); // 指数>1时，低音量更敏感
    dt.setVolum(volume);
}

void Player::btnAudio()
{
    if (isVolum) {
        //如果是静音，恢复
        int value = lastVolum * 100;
        dt.setVolum(lastVolum);
        ui.audioSlider->setValue(value);
        ui.btnAudio->setStyleSheet(
            "QPushButton {"
            "   border-image:url(:/Player/resource/audio.svg);"
            "}"
            "QPushButton:hover {"
            //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
            "   border-image:url(:/Player/resource/audio1.svg);"
            "}"
        );
        isVolum = false;
    }
    else
    {
        //如果不是静音，记录当前音量，随后设置静音
        lastVolum = dt.getVolum();
        ui.audioSlider->setValue(0);
        dt.setVolum(0.0f);
        ui.btnAudio->setStyleSheet(
            "QPushButton {"
            "   border-image:url(:/Player/resource/noaudio.svg);"
            "}"
            "QPushButton:hover {"
            //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
            "   border-image:url(:/Player/resource/noaudio1.svg);"
            "}"
        );
        isVolum = true;
    }
}

void Player::onSpeedChanged(QAction* action)
{
    float speed = action->data().toFloat();
    dt.setSpeed(speed); // 假设 dt 是你的 DemuxThread

    // 更新按钮文本
    
    ui.tbSpeed->setText(QString("%1x").arg(speed));
}

void Player::btnPlayListHint()
{
    if (ui.playList->isHidden()) {
        ui.playList->show();
        ui.video->resize(QSize(this->width() - 220, this->height() - 180));
    }
    else
    {
        ui.playList->hide();
        ui.video->resize(QSize(this->width(), this->height() - 180));
    }
}

void Player::playFromPlayListWidget(QListWidgetItem *item)
{
    if (isPause) {
        dt.setPause(false);
    }
    if (!item) {
        return;
    }
    QUrl fileUrl = item->data(Qt::UserRole).toUrl();
    QString filePath = fileUrl.toLocalFile();
    bool ret = PlayVideo(filePath);
    if (ret) {
        currentplayindex = ui.playList->row(item);
        ui.playList->setCurrentItem(item);
    }
}

void Player::btnPreSlot()
{
    if (ui.playList->count() == 0) {
        return;
    }
    if (currentplayindex == -1) {
        //当前没有播放默认播放第一个
        currentplayindex = 0;
    }
    else if (currentplayindex == 0) {
        //正在播放第一个
        currentplayindex = ui.playList->count() - 1;
    }
    else {
        currentplayindex -= 1;
    }

    QListWidgetItem* item = ui.playList->item(currentplayindex);
    playFromPlayListWidget(item);
}

void Player::btnNextSlot()
{
    if (ui.playList->count() == 0) {
        return;
    }
    if (currentplayindex == -1) {
        //当前没有播放默认播放第一个
        currentplayindex = 0;
    }
    else if (currentplayindex == (ui.playList->count() - 1)) {
        currentplayindex = 0;
    }
    else {
        currentplayindex += 1;
    }
    QListWidgetItem* item = ui.playList->item(currentplayindex);
    playFromPlayListWidget(item);
}

void Player::OnBtnNet()
{
    QInputDialog dialog(this);
    dialog.setWindowTitle(QString::fromLocal8Bit("输入URL"));
    dialog.setLabelText(QString::fromLocal8Bit("输入视频URL"));
    dialog.setInputMode(QInputDialog::TextInput);
    QLineEdit* lineEdit = dialog.findChild<QLineEdit*>();
    if (lineEdit) {
        connect(lineEdit, &QLineEdit::returnPressed, &dialog, &QInputDialog::accept);
    }
    if (dialog.exec() == QInputDialog::Accepted) {
        QString url = dialog.textValue();

        PlayVideo(url);
    }
}

void Player::OnBtnMaxOrMin()
{
    if (isFullScreen()) {
        this->showNormal();
        ui.btnMaxMin->setStyleSheet(
            "QPushButton {"
            "   border-image:url(:/Player/resource/max.svg);"
            "}"
            "QPushButton:hover {"
            //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
            "   border-image:url(:/Player/resource/max1.svg);"
            "}"
        );
    }
    else {
        this->showFullScreen();
        ui.btnMaxMin->setStyleSheet(
            "QPushButton {"
            "   border-image:url(:/Player/resource/min.svg);"
            "}"
            "QPushButton:hover {"
            //"   background-color: rgba(255, 255, 255, 30);"               // 半透明白色遮罩
            "   border-image:url(:/Player/resource/min1.svg);"
            "}"
        );
    }
}

void Player::btnOpenFile()
{
    /*QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));
    std::cout << "video name is " << name.toStdString() << std::endl;
    if (dt.Open(name.toStdString().c_str(), ui.video)) {
        isPause = false;
        ui.isPlay->setStyleSheet(
            "QPushButton {"
            "   border-image:url(:/Player/resource/stop.svg);"
            "}"
        );
    }*/
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择媒体文件",
        QDir::homePath(),
        "媒体文件 (*.mp3 *.mp4 *.wav *.avi *.mkv)"
    );

    if (!files.isEmpty()) {
        foreach(const QString & file, files) {
            QListWidgetItem* item = new QListWidgetItem(QFileInfo(file).fileName());
            item->setData(Qt::UserRole, QUrl::fromLocalFile(file));
            ui.playList->addItem(item);
        }
    }
}