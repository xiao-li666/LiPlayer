#include "VideoThread.h"
extern "C" {
#include <libavutil/time.h>
}
#define AV_SYNC_THRESHOLD 10.0
#define AV_NOSYNC_THRESHOLD 10000.0
VideoThread::VideoThread()
{
	if (!vdecode) {
		vdecode = new Decode();
	}
}

VideoThread::~VideoThread()
{
	isExit = true;
	wait();
}

bool VideoThread::Open(AVCodecParameters *param, VideoCall *video)
{
	if (!param) {
		return false;
	}
	mux.lock();
	this->video = video;
	if (this->video) {
		video->Init(param->width, param->height);
	}
	if (vdecode) {
		vdecode->Open(param);
	}
	frame_timer = (double)av_gettime() / 1000.0;
	frame_last_delay = 40e-3;
	frame_last_pts = 0;
	isPause = false;
	mux.unlock();
	return true;
}

void VideoThread::Clear()
{
	if (vdecode) {
		vdecode->Clear();
	}
	pmux.lock();
	while (!packs.empty()) {
		av_packet_free(&packs.front());
		packs.pop_front();
	}
	pmux.unlock();
}

void VideoThread::Close()
{
	Clear();
	isExit = true;
	wait();
	mux.lock();
	if (vdecode) {
		vdecode->Close();
		delete vdecode;
		vdecode = NULL;
	}

	mux.unlock();
}

void VideoThread::Push(AVPacket* pkt)
{
	if (!pkt) {
		return;
	}
	while (!isExit) {
		pmux.lock();
		if (packs.size() < MAX_PACKSZIE) {
			packs.push_back(pkt);
			pmux.unlock();
			break;
		}
		else {
			pmux.unlock();
			msleep(5);
		}
	}
	return;
}

void VideoThread::SynchVideoAndAudio(AVFrame* frame)
{
	double actual_delay, delay, sync_threshold, diff;
	if (frame_last_pts == 0) {
		delay = 0;
	}
	else
	{
		delay = (frame->pts - frame_last_pts)/speed;
	}
	//������֡���������ʱ������һ֡��֡���
	if (delay <= 0 || delay >= 1000.0) {
		/* if incorrect delay, use previous one */
		delay = frame_last_delay;
	}
	frame_last_delay = delay;
	frame_last_pts = frame->pts;
	diff = (frame->pts - synpts)/speed;
	sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
	if (fabs(diff) < AV_NOSYNC_THRESHOLD) {//����ֵ�����ɵ�����Χ֮��ʱ����ͬ��
		if (diff <= -sync_threshold) {//����Ƶ����Ƶ����sync_threshold����ʱ��������
			delay = 0;
		}
		else if (diff >= sync_threshold) {//����Ƶ����Ƶ����sync_threshold����ʱ��֡���ӳ���������
			delay = fmin(diff,2*delay); // 2 * delay;
		}
	}
	frame_timer += delay;//is->frame_timer��ʾ��һ֡���ſ�ʼ��ʱ�䣬�ټ�����һ֡�뵱ǰ֮֡���delay��ʾ��ǰ֡���ſ�ʼ��ʱ��
	/* computer the REAL delay */
	actual_delay = frame_timer - (av_gettime() / 1000.0);//��ǰ֡���ſ�ʼ��ʱ�� - ��ǰ��ϵͳʱ�� = ��ǰ֡����ȴ���ʱ��
	//if (actual_delay < 10) {
	//	/* Really it should skip the picture instead */
	//	actual_delay = 10;
	//}
	if (actual_delay < 0) {
		actual_delay = 0;
	}
	msleep(actual_delay);
}

void VideoThread::setPause(bool flag)
{
	pausemux.lock();
	isPause = flag;
	pausemux.unlock();
}

bool VideoThread::RepaintPts(long long pts, AVPacket* pkt)
{
	mux.lock();
	if (vdecode) {
		if (!vdecode->Send(pkt)) {
			mux.unlock();
			return true;
		}
	}
	AVFrame* frame = vdecode->Recv();
	if (!frame) {
		mux.unlock();
		return false;
	}
	if (frame->pts >= pts) {
		if (video) {
			video->Repaint(frame);
			mux.unlock();
			return true;
		}
	}
	av_frame_free(&frame);
	mux.unlock();
	return false;
}

void VideoThread::setSpeed(float speed)
{
	mux.lock();
	this->speed = speed;
	mux.unlock();
}

void VideoThread::run()
{
	while (!isExit) {
		pausemux.lock();
		if (isPause) {
			pausemux.unlock();
			msleep(10);
			continue;
		}
		pausemux.unlock();
		pmux.lock();
		if (packs.empty() || !vdecode || !video) {
			pmux.unlock();
			msleep(1);
			continue;
		}
		/*if (synpts > 0 && synpts < (vdecode->pts)) {
			pmux.unlock();
			msleep(1);
			continue;
		}*/
		vdecode->Send(packs.front());
		packs.pop_front();
		pmux.unlock();
		mux.lock();
		while (!isExit) {
			AVFrame* frame = vdecode->Recv();
			if (!frame) {
				break;
			}
			else {
				SynchVideoAndAudio(frame);//����Ƶͬ��
				video->Repaint(frame);
				//msleep(40);
			}
		}
		mux.unlock();
	}
}

