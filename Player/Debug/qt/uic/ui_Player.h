/********************************************************************************
** Form generated from reading UI file 'Player.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYER_H
#define UI_PLAYER_H

#include <PlayPosSlider.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <VideoWidget.h>

QT_BEGIN_NAMESPACE

class Ui_PlayerClass
{
public:
    VideoWidget *video;
    QPushButton *btnFile;
    PlayPosSlider *playPos;
    QLabel *currentTime;
    QLabel *totalTime;
    QPushButton *isPlay;
    QPushButton *btnNet;
    QPushButton *btnPre;
    QPushButton *btnNext;
    QPushButton *btnAudio;
    QPushButton *btnDeleteFile;
    QSlider *audioSlider;
    QToolButton *tbSpeed;
    QListWidget *playList;
    QPushButton *btnMaxMin;

    void setupUi(QWidget *PlayerClass)
    {
        if (PlayerClass->objectName().isEmpty())
            PlayerClass->setObjectName(QString::fromUtf8("PlayerClass"));
        PlayerClass->resize(1500, 980);
        PlayerClass->setMinimumSize(QSize(1500, 980));
        video = new VideoWidget(PlayerClass);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(0, 0, 1280, 720));
        video->setMinimumSize(QSize(1280, 720));
        btnFile = new QPushButton(PlayerClass);
        btnFile->setObjectName(QString::fromUtf8("btnFile"));
        btnFile->setGeometry(QRect(1360, 850, 50, 50));
        btnFile->setMinimumSize(QSize(50, 50));
        playPos = new PlayPosSlider(PlayerClass);
        playPos->setObjectName(QString::fromUtf8("playPos"));
        playPos->setGeometry(QRect(80, 760, 1091, 20));
        playPos->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"    border: none;\n"
"    height: 8px;\n"
"    background: #3a3a3a;              /* \345\272\225\350\275\250\357\274\214\346\267\261\347\201\260\347\225\245\344\272\256 */\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"    background: qlineargradient(\n"
"        x1: 0, y1: 0, x2: 1, y2: 0,\n"
"        stop: 0 #1e90ff,              /* \350\265\267\345\247\213\357\274\232\344\272\256\350\223\235 (DodgerBlue) */\n"
"        stop: 1 #00c3ff               /* \347\273\223\346\235\237\357\274\232\351\235\222\350\223\235 (Cyan) */\n"
"    );\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal {\n"
"    background: #666666;              /* \346\234\252\346\222\255\346\224\276\351\203\250\345\210\206\357\274\232\346\265\205\347\201\260\345\201\217\344\272\256\357\274\214\345\222\214\350\203\214\346\231\257\345\214\272\345\210\206 */\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: qradialgradie"
                        "nt(\n"
"        cx: 0.5, cy: 0.5, radius: 0.6,\n"
"        fx: 0.5, fy: 0.5,\n"
"        stop: 0 #00c3ff,              /* \345\206\205\345\234\210\344\272\256\351\235\222\350\223\235 */\n"
"        stop: 1 #0066ff               /* \345\244\226\345\234\210\346\267\261\350\223\235 */\n"
"    );\n"
"    border: 2px solid #66ccff;        /* \345\244\226\347\216\257\346\267\241\350\223\235\357\274\214\345\210\266\351\200\240\345\217\221\345\205\211\346\204\237 */\n"
"    width: 16px;\n"
"    height: 16px;\n"
"    margin: -5px 0;\n"
"    border-radius: 8px;               /* \345\234\206\345\275\242\346\273\221\345\235\227 */\n"
"    box-shadow: 0px 0px 8px #00c3ff;  /* \345\217\221\345\205\211\346\225\210\346\236\234\357\274\210\351\203\250\345\210\206 Qt \346\224\257\346\214\201\357\274\211 */\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"    background: qradialgradient(\n"
"        cx: 0.5, cy: 0.5, radius: 0.6,\n"
"        fx: 0.5, fy: 0.5,\n"
"        stop: 0 #66e0ff,\n"
"        stop: 1 #0099ff\n"
"    "
                        ");\n"
"    border: 2px solid #aee7ff;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:pressed {\n"
"    background: #0099ff;              /* \346\214\211\344\270\213\346\227\266\345\241\253\345\205\205\344\272\256\350\223\235 */\n"
"    border: 2px solid #66ccff;\n"
"}\n"
""));
        playPos->setMaximum(1000);
        playPos->setOrientation(Qt::Orientation::Horizontal);
        currentTime = new QLabel(PlayerClass);
        currentTime->setObjectName(QString::fromUtf8("currentTime"));
        currentTime->setGeometry(QRect(20, 770, 71, 19));
        currentTime->setMinimumSize(QSize(71, 19));
        totalTime = new QLabel(PlayerClass);
        totalTime->setObjectName(QString::fromUtf8("totalTime"));
        totalTime->setGeometry(QRect(1190, 760, 71, 19));
        totalTime->setMinimumSize(QSize(71, 19));
        isPlay = new QPushButton(PlayerClass);
        isPlay->setObjectName(QString::fromUtf8("isPlay"));
        isPlay->setGeometry(QRect(230, 870, 50, 50));
        isPlay->setMinimumSize(QSize(50, 50));
        btnNet = new QPushButton(PlayerClass);
        btnNet->setObjectName(QString::fromUtf8("btnNet"));
        btnNet->setGeometry(QRect(30, 870, 50, 50));
        btnPre = new QPushButton(PlayerClass);
        btnPre->setObjectName(QString::fromUtf8("btnPre"));
        btnPre->setGeometry(QRect(130, 870, 50, 50));
        btnNext = new QPushButton(PlayerClass);
        btnNext->setObjectName(QString::fromUtf8("btnNext"));
        btnNext->setGeometry(QRect(330, 870, 50, 50));
        btnAudio = new QPushButton(PlayerClass);
        btnAudio->setObjectName(QString::fromUtf8("btnAudio"));
        btnAudio->setGeometry(QRect(430, 870, 50, 50));
        btnDeleteFile = new QPushButton(PlayerClass);
        btnDeleteFile->setObjectName(QString::fromUtf8("btnDeleteFile"));
        btnDeleteFile->setGeometry(QRect(1420, 850, 50, 50));
        audioSlider = new QSlider(PlayerClass);
        audioSlider->setObjectName(QString::fromUtf8("audioSlider"));
        audioSlider->setGeometry(QRect(510, 890, 131, 16));
        audioSlider->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"    height: 6px;\n"
"    background: #343434;\n"
"    border: 1px solid #424242;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #9370DB, stop:0.5 #8A2BE2, stop:1 #6A5ACD);\n"
"    height: 6px;\n"
"    border-radius: 3px;\n"
"    box-shadow: 0px 0px 4px rgba(138, 43, 226, 0.7);\n"
"}\n"
"\n"
"QSlider::add-page:horizontal {\n"
"    background: #3A3A3A;\n"
"    height: 6px;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5,\n"
"        stop:0 #FFFFFF, stop:0.6 #F0E8FF, stop:1 #E6E6FA);\n"
"    width: 16px;\n"
"    height: 16px;\n"
"    margin: -5px 0;\n"
"    border: 2px solid #8A2BE2;\n"
"    border-radius: 8px;\n"
"    box-shadow: 0px 0px 6px rgba(138, 43, 226, 0.9);\n"
"}"));
        audioSlider->setOrientation(Qt::Orientation::Horizontal);
        tbSpeed = new QToolButton(PlayerClass);
        tbSpeed->setObjectName(QString::fromUtf8("tbSpeed"));
        tbSpeed->setGeometry(QRect(1150, 880, 100, 40));
        playList = new QListWidget(PlayerClass);
        playList->setObjectName(QString::fromUtf8("playList"));
        playList->setGeometry(QRect(1280, 0, 221, 721));
        playList->setMinimumSize(QSize(221, 721));
        playList->setStyleSheet(QString::fromUtf8("/* \344\270\273\345\210\227\350\241\250\346\216\247\344\273\266\346\240\267\345\274\217 */\n"
"QListWidget {\n"
"    background-color: #2D2D2D;         /* \350\203\214\346\231\257\350\211\262 */\n"
"    border: 1px solid #3A3A3A;        /* \350\276\271\346\241\206 */\n"
"    border-radius: 4px;               /* \345\234\206\350\247\222 */\n"
"    padding: 2px;                     /* \345\206\205\350\276\271\350\267\235 */\n"
"    outline: 0;                       /* \347\247\273\351\231\244\347\204\246\347\202\271\350\231\232\347\272\277\346\241\206 */\n"
"    font-family: \"Microsoft YaHei\";   /* \345\255\227\344\275\223 */\n"
"    font-size: 12px;                  /* \345\255\227\344\275\223\345\244\247\345\260\217 */\n"
"    color: #CCCCCC;                   /* \351\273\230\350\256\244\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"/* \345\210\227\350\241\250\351\241\271\346\240\267\345\274\217 */\n"
"QListWidget::item {\n"
"    height: 28px;                     /* \351\241\271\351\253\230"
                        "\345\272\246 */\n"
"    padding-left: 8px;                /* \345\267\246\344\276\247\345\206\205\350\276\271\350\267\235 */\n"
"    background-color: transparent;    /* \351\273\230\350\256\244\351\200\217\346\230\216\350\203\214\346\231\257 */\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234\346\225\210\346\236\234 */\n"
"QListWidget::item:hover {\n"
"    background-color: #3A3A3A;        /* \346\202\254\345\201\234\350\203\214\346\231\257\350\211\262 */\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"/* \351\200\211\344\270\255\351\241\271\346\225\210\346\236\234 */\n"
"QListWidget::item:selected {\n"
"    background-color: #4A4A4A;        /* \351\200\211\344\270\255\350\203\214\346\231\257\350\211\262 */\n"
"    color: #FFFFFF;                  /* \351\200\211\344\270\255\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"    border-left: 3px solid #1E90FF;   /* \345\267\246\344\276\247\351\253\230\344\272\256\346\235\241 */\n"
"}\n"
"\n"
"/* \346\273\232\345\212\250\346\235\241\346\240\267"
                        "\345\274\217 */\n"
"QScrollBar:vertical {\n"
"    width: 10px;                      /* \346\273\232\345\212\250\346\235\241\345\256\275\345\272\246 */\n"
"    background: #2D2D2D;              /* \350\203\214\346\231\257\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background: #4A4A4A;              /* \346\273\221\345\235\227\351\242\234\350\211\262 */\n"
"    min-height: 20px;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #5A5A5A;              /* \346\202\254\345\201\234\346\273\221\345\235\227\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical,\n"
"QScrollBar::sub-line:vertical {\n"
"    height: 0px;                      /* \351\232\220\350\227\217\344\270\212\344\270\213\347\256\255\345\244\264 */\n"
"}"));
        btnMaxMin = new QPushButton(PlayerClass);
        btnMaxMin->setObjectName(QString::fromUtf8("btnMaxMin"));
        btnMaxMin->setGeometry(QRect(1270, 880, 50, 50));

        retranslateUi(PlayerClass);

        QMetaObject::connectSlotsByName(PlayerClass);
    } // setupUi

    void retranslateUi(QWidget *PlayerClass)
    {
        PlayerClass->setWindowTitle(QCoreApplication::translate("PlayerClass", "Player", nullptr));
        btnFile->setText(QString());
        currentTime->setText(QCoreApplication::translate("PlayerClass", "00:00:00", nullptr));
        totalTime->setText(QCoreApplication::translate("PlayerClass", "00:00:00", nullptr));
        isPlay->setText(QString());
        btnNet->setText(QString());
        btnPre->setText(QString());
        btnNext->setText(QString());
        btnAudio->setText(QString());
        btnDeleteFile->setText(QString());
        tbSpeed->setText(QCoreApplication::translate("PlayerClass", "...", nullptr));
        btnMaxMin->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PlayerClass: public Ui_PlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYER_H
