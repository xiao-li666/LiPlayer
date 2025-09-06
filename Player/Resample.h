#pragma once

extern "C" {
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}
#include <mutex>
class Resample
{
public:
	Resample();
	~Resample();

	virtual bool Open(AVCodecParameters* param, bool isFreeParam);
	virtual int StartResample(AVFrame* frame, unsigned char *d);
	virtual void Close();

private:

	SwrContext *sctx = NULL;

	std::mutex mux;
};

