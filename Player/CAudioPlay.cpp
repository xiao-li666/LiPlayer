#include "CAudioPlay.h"

CAudioPlay::CAudioPlay()
{
}

CAudioPlay::~CAudioPlay()
{
}

bool CAudioPlay::Open()
{
	mux.lock();
	QAudioFormat fmt;
	fmt.setSampleRate(sampleRate);
	fmt.setSampleSize(sampleSize);
	fmt.setChannelCount(channels);
	fmt.setCodec("audio/pcm");
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	fmt.setSampleType(QAudioFormat::SignedInt);//重采样中音频输出格式为AV_SAMPLE_FMT_S16有符号，这里也设置为有符号，不然音量调节出现杂音

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if (!info.isFormatSupported(fmt)) {
		fmt = info.nearestFormat(fmt);  // 若不支持，使用最接近的格式
	}

	out = new QAudioOutput(fmt);
	out->setVolume(currentVolum);//设置初始音量

	io = out->start();
	if (!io) {
		mux.unlock();
		return false;
	}
	mux.unlock();
	return true;
}

bool CAudioPlay::Write(const unsigned char* data, int datasize)
{
	if (!data || datasize < 0) {
		return false;
	}
	if (!out || !io) {
		return false;
	}
	mux.lock();
	int size = io->write((char *)data, datasize);
	if (size != datasize) {
		mux.unlock();
		return false;
	}
	mux.unlock();
	return true;
}

int CAudioPlay::GetFree()
{
	mux.lock();
	if (!out) {
		mux.unlock();
		return 0;
	}
	int size = out->bytesFree();
	mux.unlock();
	return size;
}

void CAudioPlay::Close()
{
	mux.lock();
	if (io) {
		io->close();
		delete io;
		io = NULL;
	}
	if (out) {
		//out->stop();
		delete out;
		out = NULL;
	}
	mux.unlock();
}

void CAudioPlay::Clear()
{
	mux.lock();
	if (io) {
		io->reset();
	}
	mux.unlock();
}

long long CAudioPlay::GetNoPlayMs()
{
	if (!out) {
		return 0;
	}
	double size = out->bufferSize() - out->bytesFree();
	double secondSize = (sampleSize / 8) * sampleRate * channels;
	if (secondSize <= 0) {
		return 0;
	}
	return (size/secondSize)*1000;
}

void CAudioPlay::setPause(bool isPause)
{
	mux.lock();
	if (!out) {
		mux.unlock();
		return;
	}
	if (isPause) {
		out->suspend();
	}
	else
	{
		out->resume();
	}
	mux.unlock();
}

void CAudioPlay::setVolum(float volum)
{
	if (volum > 1.0f) volum = 1.0f;
	if (volum < 0.0f) volum = 0.0f;
	currentVolum = volum;
	if (out) {
		out->setVolume(volum);
	}
}

float CAudioPlay::getVolum()
{
	return currentVolum;
}
