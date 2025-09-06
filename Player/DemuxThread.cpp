#include "DemuxThread.h"
#include <QTime>

DemuxThread::DemuxThread()
{
}

DemuxThread::~DemuxThread()
{
	isExit = true;
	wait();
}

bool DemuxThread::Open(const char *url, VideoCall *video)
{
	if (url==0 || url[0] == '\0' || !video) {
		return false;
	}
	mux.lock();
	if (dex) {
		if (!dex->Open(url)) {
			mux.unlock();
			return false;
		}
	}
	if (vt) {
		if (!vt->Open(dex->getVideoParam(), video)) {
			mux.unlock();
			return false;
		}
	}
	if (at) {
		if (!at->Open(dex->getAudioParam())) {
			mux.unlock();
			return false;
		}
	}
	isPause = false;
	mux.unlock();
	return true;
}

void DemuxThread::Start()
{
	mux.lock();
	if (!dex) {
		dex = new Demux();
	}
	if (!at) {
		at = new AudioThread();
	}
	if (!vt) {
		vt = new VideoThread();
	}
	isExit = false;
	if (dex) {
		QThread::start();
	}
	if (vt) {
		vt->start();
	}
	if (at) {
		at->start();
	}
	mux.unlock();
}

void DemuxThread::Clear()
{
	mux.lock();
	if (dex) {
		dex->Clear();
	}
	if (vt) {
		vt->Clear();
	}
	if (at) {
		at->SeekClear();
	}
	mux.unlock();
}

void DemuxThread::Close()
{
	isExit = true;
	wait();
	mux.lock();
	if (vt) {
		vt->Close();
		delete vt;
		vt = NULL;
	}
	if (at) {
		at->Close();
		delete at;
		at = NULL;
	}
	mux.unlock();
}

void DemuxThread::setPause(bool flag)
{
	pausemux.lock();
	isPause = flag;
	if (at) {
		at->setPause(flag);
	}
	if (vt) {
		vt->setPause(flag);
	}
	pausemux.unlock();
}

long long DemuxThread::getDuration()
{
	long long duration;
	mux.lock();
	if (dex) {
		duration = dex->getTotalTime();
	}
	mux.unlock();
	return duration;
}

void DemuxThread::seek(double pos)
{
	Clear();
	mux.lock();
	bool state = isPause;
	mux.unlock();
	setPause(true);
	mux.lock();
	if (dex) {
		dex->Seek(pos);
	}
	long long seekPos = pos * dex->getTotalTime();
	while (!isExit) {
		AVPacket* pkt = dex->ReadVideo();
		if (!pkt)break;
		if (vt->RepaintPts(seekPos, pkt)) {
			this->pts = seekPos;
			emit positionChanged(seekPos, dex->getTotalTime());
			break;
		}
	}
	mux.unlock();
	if (!state) {
		setPause(false);
	}
	else
	{
		emit positionChanged(seekPos, dex->getTotalTime());
	}
}

void DemuxThread::setVolum(float volum)
{
	if (at) {
		at->setVolum(volum);
	}
}

float DemuxThread::getVolum()
{
	return at->getVolum();
}

void DemuxThread::setSpeed(float speed)
{
	mux.lock();
	if (at) {
		at->setSpeed(speed);
	}
	if (vt) {
		vt->setSpeed(speed);
	}
	mux.unlock();
}

void DemuxThread::run()
{
	// ���������ڿ��Ʒ���Ƶ�ʵı���
	QTime lastEmitTime = QTime::currentTime();
	int emitIntervalMs = 40; // ÿ��40���루Լ25fps������һ�Σ��㹻�������½�����
	while (!isExit) {
		pausemux.lock();
		if (isPause) {
			pausemux.unlock();
			msleep(10);
			continue;
		}
		pausemux.unlock();
		mux.lock();
		if (!at || !vt || !dex) {
			mux.unlock();
			msleep(5);
			continue;
		}
		if (at && vt) {
			pts = at->pts;
			vt->synpts = at->pts;
			// �����źţ�֪ͨUI�̸߳��½���
			/*if (pts > 0 && pts < dex->getTotalTime()) {
				emit positionChanged(pts, dex->getTotalTime());
			}*/
			// --- ������Ƶ�ʿ����߼� ---
			QTime currentTime = QTime::currentTime();
			if (lastEmitTime.msecsTo(currentTime) > emitIntervalMs) {
				emit positionChanged(pts, dex->getTotalTime());
				lastEmitTime = currentTime; // �����ϴη���ʱ��
			}
		}
		AVPacket* pkt = dex->Recv();
		if (!pkt) {
			mux.unlock();
			continue;
		}
		if (pkt->stream_index == dex->getAudioIdx()) {
			mux.unlock();
			//Ϊʲô��ִ����Ӳ���ǰҪ�ͷ�������Ϊ��ִ��Seek()����ʱ������ִ��Clear()��������Clear()������Ҫmux��
			//������ﲻ�ͷ�mux���ͻ���ֵ�����ִ�е�����ʱisPause������Ϊtrue��ʱ���ﻹû���ͷ�mux����	Push����������
			//������ͣ״̬���ҵ�������ʱ��Push�л�һֱѭ����ȥ���³��������￨ס�����ͷ�mux����ʱ����Clear()�ͻ�����
			at->Push(pkt);
		}
		else if(pkt->stream_index == dex->getVideoIdx())
		{
			mux.unlock();
			vt->Push(pkt);
		}
		else
		{
			mux.unlock();
		}
	}
}
