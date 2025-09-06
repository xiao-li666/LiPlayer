#include "Resample.h"

Resample::Resample()
{
}

Resample::~Resample()
{
}

bool Resample::Open(AVCodecParameters* param, bool isFreeParam)
{
	if (!param) return false;
	mux.lock();
	sctx = swr_alloc_set_opts(sctx, param->channel_layout, AV_SAMPLE_FMT_S16, param->sample_rate,
		param->channel_layout, (AVSampleFormat)param->format, param->sample_rate,
		0, NULL);
	if (!sctx) {
		mux.unlock();
		avcodec_parameters_free(&param);
		return false;
	}
	if (isFreeParam) {
		avcodec_parameters_free(&param);
	}
	swr_init(sctx);
	mux.unlock();
	return true;
}

int Resample::StartResample(AVFrame* frame, unsigned char* d)
{
	if (!frame) return false;
	uint8_t* data[2] = { 0 };
	data[0] = d;
	mux.lock();
	int ret = swr_convert(sctx, data, frame->nb_samples, (const uint8_t **)frame->extended_data, frame->nb_samples);
	if (ret < 0) {
		mux.unlock();
		av_frame_free(&frame);
		return 0;
	}
	int outSize = frame->channels * ret * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
	av_frame_free(&frame);

	mux.unlock();
	return outSize;
}

void Resample::Close()
{
	mux.lock();
	if (sctx) {
		swr_free(&sctx);
	}
	mux.unlock();
}
