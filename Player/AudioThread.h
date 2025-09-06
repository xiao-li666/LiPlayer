#pragma once

#include <QThread>
#include <mutex>
#include <list>
#include "Decode.h"
#include "Resample.h"
#include "AudioPlay.h"
#include "sonic.h"
#define MIX_PACKETSZIE 25
struct AVPacket;
struct AVCodecParameters;

class AudioThread  : public QThread
{
public:
	AudioThread();
	~AudioThread();

	virtual bool Open(AVCodecParameters *param);
	void run();
	virtual void Push(AVPacket* pkt);

	virtual void Clear();
	virtual void SeekClear();
	virtual void Close();
	virtual void setPause(bool flag);
	virtual void setVolum(float volum);
	virtual float getVolum();
	virtual void setSpeed(float speed);

	bool isExit = false;
	long long pts = 0;
private:
	std::list<AVPacket*> packs;
	std::mutex mux;
	std::mutex pmux;
	std::mutex pausemux;
	Decode* adecode = NULL;
	Resample* res = NULL;
	AudioPlay* ap = NULL;
	bool isPause;

	float speed = 1.0f;

	sonicStream sonic; // sonicÁ÷¶ÔÏó
};
