#pragma once
#include "D:\software\Qt\5.15.2\msvc2019\include\QtCore\qthread.h"
#include <mutex>
#include <list>
#include "Decode.h"
#include "VideoWidget.h"

#define MAX_PACKSZIE 200
struct AVPacket;
struct AVCodecParameters;
class VideoThread :
    public QThread
{
public:
    VideoThread();
    ~VideoThread();

    virtual bool Open(AVCodecParameters*param, VideoCall *video);
    virtual void Clear();
    virtual void Close();
    virtual void Push(AVPacket* pkt);
    virtual void SynchVideoAndAudio(AVFrame* frame);
    virtual void setPause(bool flag);
    virtual bool RepaintPts(long long pts, AVPacket* pkt);
    virtual void setSpeed(float speed);
    void run();

    bool isExit = false;
    long long synpts = 0;

private:
    std::list<AVPacket*>packs;
    std::mutex mux;
    std::mutex pmux;
    std::mutex pausemux;

    Decode* vdecode = NULL;
    VideoCall* video = NULL;
    double frame_timer;
    long long frame_last_pts;
    double frame_last_delay;
    bool isPause;

    float speed = 1.0f; // ≤•∑≈ÀŸ∂»
};

