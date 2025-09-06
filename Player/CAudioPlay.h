#pragma once
#include "AudioPlay.h"
#include <qaudioformat.h>
#include <qaudiooutput.h>
#include <mutex>
class CAudioPlay :
    public AudioPlay
{
public:
    CAudioPlay();
    ~CAudioPlay();

    virtual bool Open();
    virtual bool Write(const unsigned char* data, int datasize);
    virtual int GetFree();
    virtual void Close();
    virtual void Clear();
    virtual long long GetNoPlayMs();
    virtual void setPause(bool isPause);
    virtual void setVolum(float volum);
    virtual float getVolum();

private:
    QIODevice* io = NULL;
    QAudioOutput* out = NULL;

    std::mutex mux;
    float currentVolum = 0.8f;
};

