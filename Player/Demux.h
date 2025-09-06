#pragma once

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavutil/time.h>
}
#include <mutex>

class Demux
{
public:
	Demux();
	~Demux();

	virtual bool Open(const char *url);
	virtual AVPacket* Recv();
	virtual bool Seek(double pos);
	virtual bool Close();
	virtual void Clear();
	virtual AVPacket* ReadVideo();

	AVCodecParameters* getVideoParam();
	AVCodecParameters* getAudioParam();

	virtual int getAudioIdx();
	virtual int getVideoIdx();
	virtual long long getTotalTime();

private:
	std::mutex mux;

	AVFormatContext* ic = NULL;

	AVDictionary* opts = NULL;
	AVCodecParameters* videoParam = NULL;
	AVCodecParameters* audioParam = NULL;
	AVPacket* pkt = NULL;

	int videoStreamIdx = 0;
	int audioStreamIdx = 0;

	long long totalMs = 0;

public:
	int width = 0;
	int height = 0;

};

