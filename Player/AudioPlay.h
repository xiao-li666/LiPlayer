#pragma once
class AudioPlay
{
public:
	AudioPlay();
	virtual ~AudioPlay();

	static AudioPlay* Get();

	virtual bool Open() = 0;
	virtual bool Write(const unsigned char *data, int datasize) = 0;
	virtual int GetFree() = 0;
	virtual void Close() = 0;
	virtual void Clear() = 0;
	virtual void setPause(bool isPause) = 0;
	virtual void setVolum(float volum) = 0;
	virtual float getVolum() = 0;
	virtual long long GetNoPlayMs() = 0;

public:

	int sampleRate = 0;
	int sampleSize = 0;
	int channels = 0;
};

