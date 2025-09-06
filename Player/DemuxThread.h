#pragma once
#include "D:\software\Qt\5.15.2\msvc2019\include\QtCore\qthread.h"
#include "AudioThread.h"
#include "VideoThread.h"
#include "Demux.h"
#include <mutex>
class DemuxThread :
    public QThread
{
    Q_OBJECT
public:
    DemuxThread();
    ~DemuxThread();
    virtual bool Open(const char *url, VideoCall *video);
    virtual void Start();
    virtual void Clear();
    virtual void Close();
    virtual void setPause(bool flag);
    virtual long long getDuration();
    virtual void seek(double pos);
    virtual void setVolum(float volum);
    virtual float getVolum();
    virtual void setSpeed(float speed);
    void run();

    Demux* dex = NULL;
    long long pts = 0;
signals:
    void positionChanged(long long pts, long long totalMs);
private:
    AudioThread* at = NULL;
    VideoThread* vt = NULL;
    std::mutex mux;
    std::mutex pausemux;
    bool isPause;
    bool isExit;
};

