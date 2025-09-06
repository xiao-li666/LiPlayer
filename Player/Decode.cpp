#include "Decode.h"
extern "C" {
#include <libavutil/log.h>
#include <libavutil/time.h>
}
#include<iostream>

Decode::Decode()
{
}

Decode::~Decode()
{
}

bool Decode::Open(AVCodecParameters* param)
{
	//Clear();
	Close();
	decodec = avcodec_find_decoder(param->codec_id);
	if (!decodec) {
		avcodec_parameters_free(&param);
		return false;
	}
	mux.lock();
	ctx = avcodec_alloc_context3(decodec);
	if (!ctx) {
		mux.unlock();
		avcodec_parameters_free(&param);
		return false;
	}
	int ret = avcodec_parameters_to_context(ctx, param);
	if (ret < 0) {
		mux.unlock();
		avcodec_parameters_free(&param);
		return false;
	}
	//
	ctx->thread_count = 16;

	avcodec_parameters_free(&param);

	if ((ret = avcodec_open2(ctx, decodec, NULL)) < 0) {
		mux.unlock();
		return false;
	}
	mux.unlock();
	return true;
}

void Decode::Close()
{
	mux.lock();
	if (ctx) {
		avcodec_close(ctx);
		avcodec_free_context(&ctx);
	}
	pts = 0;
	mux.unlock();
}

void Decode::Clear()
{
	mux.lock();
	if (ctx) {
		avcodec_flush_buffers(ctx);
	}
	mux.unlock();
}

bool Decode::Send(AVPacket* pkt)
{
	mux.lock();
	if (!pkt || pkt->size <= 0 || !pkt->data) {
		mux.unlock();
		av_packet_free(&pkt);
		return false;
	}
	int ret = avcodec_send_packet(ctx, pkt);
	av_packet_free(&pkt);
	if (ret != 0) {
		mux.unlock();
		return false;
	}
	mux.unlock();
	return true;
}

AVFrame* Decode::Recv()
{
	mux.lock();
	AVFrame* frame = av_frame_alloc();
	if (!ctx) {
		mux.unlock();
		av_frame_free(&frame);
		return NULL;
	}
	int ret = avcodec_receive_frame(ctx, frame);
	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
		av_frame_free(&frame);
		mux.unlock();
		return NULL;
	}
	if (ret != 0) {
		av_frame_free(&frame);
		std::cout << "failed to receive frame from decoder!" << std::endl;
	}
	//pts = frame->pts;
	if (frame->format == AV_PIX_FMT_NONE) {
		//ÒôÆµ
		pts = frame->pts;
	}
	else {
		//ÊÓÆµ
		frame->pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts;
		if (frame->pts != 0) {
			video_clock = frame->pts;
		}
		else
		{
			frame->pts = video_clock;
		}
		double frame_delay;
		frame_delay = av_q2d(this->ctx->time_base) * 1000;
		frame_delay += frame->repeat_pict * (frame_delay * 0.5);
		video_clock += frame_delay;
		pts = frame->pts;
	}
	mux.unlock();
	return frame;
}
