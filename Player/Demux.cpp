#include "Demux.h"
#include <iostream>
using  namespace std;
Demux::Demux()
{
}

Demux::~Demux()
{
}

bool Demux::Open(const char* url)
{
	Clear();
	Close();
	if (!url || url[0] == '\0') {
		return false;
	}

	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	av_dict_set(&opts, "max_delay", "1000000", 0);
	mux.lock();
	int ret = avformat_open_input(&ic, url, NULL, NULL);
	if (ret < 0) {
		mux.unlock();
		return false;
	}

	if ((ret = avformat_find_stream_info(ic, NULL)) < 0) {
		mux.unlock();
		return false;
	}

	totalMs = ic->duration / ((AV_TIME_BASE) / 1000);
	cout << "总时长为：" << totalMs << endl;

	av_dump_format(ic, 0, url, 0);//打印媒体信息

	audioStreamIdx = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	videoStreamIdx = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	width = ic->streams[videoStreamIdx]->codecpar->width;
	height = ic->streams[videoStreamIdx]->codecpar->height;
	mux.unlock();

	cout << "音频ID： " << audioStreamIdx << " " << "视频ID： " << videoStreamIdx << endl;

	return true;
}

AVPacket* Demux::Recv()
{
	mux.lock();
	if (!ic) {
		mux.unlock();
		return NULL;
	}
	pkt = av_packet_alloc();
	if (!pkt) {
		mux.unlock();
		cout << "内存不足" << endl;
		return NULL;
	}
	int ret = av_read_frame(ic, pkt);
	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
		mux.unlock();
		av_packet_free(&pkt);
		return NULL;
	}
	if (ret < 0) {
		mux.unlock();
		av_packet_free(&pkt);
		cout << "failed to read frame from ic" << endl;
		return NULL;
	}
	pkt->pts = pkt->pts * av_q2d(ic->streams[pkt->stream_index]->time_base) * 1000;
	mux.unlock();
	return pkt;
}

bool Demux::Seek(double pos)
{
	mux.lock();
	if (!ic)
	{
		mux.unlock();
		return false;
	}

	avformat_flush(ic);
	long long seekPos = 0;
	seekPos = ic->streams[videoStreamIdx]->duration * pos;
	int ret =  av_seek_frame(ic, videoStreamIdx, seekPos, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
	if (ret < 0) {
		mux.unlock();
		return false;
	}
	mux.unlock();
	return true;
}

bool Demux::Close()
{
	mux.lock();
	if (ic) {
		avformat_close_input(&ic);
	}
	else
	{
		mux.unlock();
		return false;
	}
	if (!opts) {
		av_dict_free(&opts);
	}
	if (!videoParam) {
		avcodec_parameters_free(&videoParam);
	}
	if (!audioParam) {
		avcodec_parameters_free(&audioParam);
	}
	mux.unlock();
	return true;
}

void Demux::Clear()
{
	mux.lock();
	if (ic) {
		avformat_flush(ic);
	}
	mux.unlock();
}

AVPacket* Demux::ReadVideo()
{
	AVPacket* pkt = NULL;
	for (int i = 0; i < 20; i++) {
		pkt = Recv();
		if (pkt->stream_index == videoStreamIdx) {
			break;
		}
		av_packet_free(&pkt);
	}
	return pkt;
}

AVCodecParameters* Demux::getVideoParam()
{
	mux.lock();
	if (ic) {
		videoParam = avcodec_parameters_alloc();//先申请空间
		int ret = avcodec_parameters_copy(videoParam, ic->streams[videoStreamIdx]->codecpar);
		if (ret < 0) {
			mux.unlock();
			return nullptr;
		}
	}
	mux.unlock();
	return videoParam;
}

AVCodecParameters* Demux::getAudioParam()
{
	mux.lock();
	if (ic) {
		audioParam = avcodec_parameters_alloc();
		int ret = avcodec_parameters_copy(audioParam, ic->streams[audioStreamIdx]->codecpar);
		if (ret < 0) {
			mux.unlock();
			return nullptr;
		}
	}
	mux.unlock();
	return audioParam;
}

int Demux::getAudioIdx()
{
	return audioStreamIdx;
}

int Demux::getVideoIdx()
{
	return videoStreamIdx;
}

long long Demux::getTotalTime()
{
	return totalMs;
}
