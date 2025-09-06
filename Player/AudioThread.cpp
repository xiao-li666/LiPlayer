#include "AudioThread.h"
#include <QDebug>
AudioThread::AudioThread()
{
	if (!adecode) {
		adecode = new Decode();
	}
	if (!res) {
		res = new Resample();
	}
	if (!ap) {
		ap = AudioPlay::Get();
	}
}

AudioThread::~AudioThread()
{
	isExit = true;
	wait();
}

bool AudioThread::Open(AVCodecParameters * param)
{
	if (!param) {
		return false;
	}
	Clear();
	pts = 0;
	bool ret = true;
	mux.lock();

	// ��ʼ��sonic
	if (sonic) {
		sonicDestroyStream(sonic);
	}
	sonic = sonicCreateStream(param->sample_rate, param->channels);
	sonicSetSpeed(sonic, speed); // Ĭ��1����
	sonicSetPitch(sonic, 1.0f);    // ������������
	sonicSetRate(sonic, 1.0f);     // ���ֲ����ʲ���
	sonicSetVolume(sonic, 1.0f);   // ������Ϊ1.0
	sonicSetQuality(sonic, 0);     // ������ģʽ
	if (res) {
		if (!res->Open(param, false)) {
			ret = false;
		}
	}
	ap->channels = param->channels;
	ap->sampleRate = param->sample_rate;
	ap->sampleSize = 16;
	if (adecode) {
		if (!adecode->Open(param)) {
			ret = false;
		}
	}
	if (ap) {
		if (!ap->Open()) {
			ret = false;
		}
	}
	isPause = false;

	mux.unlock();
	return ret;
}

void AudioThread::run()
{
	unsigned char* pcm = new unsigned char[1024 * 1024 * 10];
	short* sonicInput = new short[1024 * 1024];  // sonic���뻺����
	short* sonicOutput = new short[1024 * 1024]; // sonic���������
	while (!isExit) {
		//mux.lock();
		pausemux.lock();
		if (isPause) {
			pausemux.unlock();
			msleep(10);
			continue;
		}
		pausemux.unlock();
		pmux.lock();
		if (packs.empty() || !adecode || !res) {
			pmux.unlock();
			msleep(1);
			continue;
		}

		AVPacket* pkt = packs.front();
		packs.pop_front();
		pmux.unlock();
		mux.lock();
		bool ret = adecode->Send(pkt);
		if (!ret) {
			mux.unlock();
			msleep(1);
			continue;
		}
		while (!isExit) {
			AVFrame  *frame =adecode->Recv();
			if (!frame) {
				break;
			}
			pts = adecode->pts - ap->GetNoPlayMs();
			//�ز���
			int size = res->StartResample(frame, pcm);
			if (size <= 0) {
				//av_frame_free(&frame);
				break;
			}
			// ��PCM����ת��Ϊshort���ͣ�sonic��Ҫ16λPCM��
			int numSamples = size / (2*ap->channels); // ÿ������2�ֽ�
			memcpy(sonicInput, pcm, size);

			// д��sonic���д���
			sonicWriteShortToStream(sonic, sonicInput, numSamples);

			// ��sonic��ȡ��������Ƶ
			int outSamples = sonicReadShortFromStream(sonic, sonicOutput, 1024 * 512);
			int outSize = outSamples * 2*ap->channels; // ת���ֽ���

			// ���Ŵ�������Ƶ
			while (!isExit && outSize > 0) {
				if (outSize > ap->GetFree()) {
					msleep(1);
					continue;
				}
				if (ap) {
					ap->Write((unsigned char*)sonicOutput, outSize);
				}
				break;
			}
		}

		mux.unlock();
	}
	delete[]pcm;
	delete[] sonicInput;
	delete[] sonicOutput;
}

void AudioThread::Push(AVPacket* pkt)
{
	if (!pkt) {
		return;
	}
	while (!isExit) {
		pmux.lock();
		if (packs.size() < MIX_PACKETSZIE) {
			packs.push_back(pkt);
			pmux.unlock();
			break;
		}
		else {
			pmux.unlock();
			msleep(1);
		}
	}
	return;
}

void AudioThread::Clear()
{
	if (adecode) {
		adecode->Clear();
	}
	pmux.lock();
	while (!packs.empty()) {
		av_packet_free(&packs.front());
		packs.pop_front();
	}
	pmux.unlock();
}

void AudioThread::SeekClear()
{
	Clear();
	mux.lock();
	if (ap) {
		ap->Clear();
	}
	mux.unlock();
}

void AudioThread::Close()
{
	Clear();
	isExit = true;
	wait();
	mux.lock();
	if (adecode) {
		adecode->Close();
		delete adecode;
		adecode = NULL;
	}
	if (res) {
		res->Close();
		delete res;
		res = NULL;
	}
	if (ap) {
		ap->Close();
		//delete ap;
		ap = NULL;
	}
	if (sonic) {
		sonicDestroyStream(sonic);
		sonic = nullptr;
	}
	mux.unlock();
}

void AudioThread::setPause(bool flag)
{
	pausemux.lock();
	//if (ap)ap->setPause(flag);
	isPause = flag;
	pausemux.unlock();
}

void AudioThread::setVolum(float volum)
{
	if (ap) {
		ap->setVolum(volum);
	}
}

float AudioThread::getVolum()
{
	return ap->getVolum();
}

void AudioThread::setSpeed(float speed)
{
	// ȷ���ٶ�����Ч��Χ��
	if (speed < SONIC_MIN_SPEED) speed = SONIC_MIN_SPEED;
	if (speed > SONIC_MAX_SPEED) speed = SONIC_MAX_SPEED;

	mux.lock();
	this->speed = speed;
	if (sonic) {
		sonicSetSpeed(sonic, speed);
	}
	mux.unlock();
}
