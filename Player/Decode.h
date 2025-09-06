#pragma once

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <mutex>

class Decode
{
public:
	Decode();
	virtual ~Decode();

	//不论成功与否，释放param
	virtual bool Open(AVCodecParameters *param);
	virtual void Close();
	virtual void Clear();

	//无论成功与否释放pkt
	virtual bool Send(AVPacket* pkt);
	virtual AVFrame* Recv();

	long long pts = 0;
	long long video_clock = 0;

private:
	std::mutex mux;
	AVCodecContext* ctx = NULL;
	AVCodec* decodec = NULL;
};

