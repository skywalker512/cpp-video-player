#include "XAudioThread.h"
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"
#include <iostream>
#include <QDebug>

extern "C" {
#include <libavutil/frame.h>
}

using namespace std;

bool XAudioThread::Open(AVCodecParameters* para, int sampleRate, int channels)
{
	if (!para)return false;
	Clear();
	amux.lock();
	// 不受上次影响
	pts = 0;

	// 标志量是否整个过程成功
	bool re = true;
	if (!res->Open(para, false))
	{
		cout << "XResample open failed!" << endl;
		re = false;
	}
	ap->sampleRate = sampleRate;
	ap->channels = channels;
	if (!ap->Open())
	{
		re = false;
		cout << "XAudioPlay open failed!" << endl;
	}
	if (!decode->Open(para))
	{
		cout << "audio XDecode open failed!" << endl;
		re = false;
	}
	amux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XAudioThread::Close()
{
	XDecodeThread::Close();
	if (res)
	{
		res->Close();
		amux.lock();
		delete res;
		res = nullptr;
		amux.unlock();
	}
	if (ap)
	{
		ap->Close();
		amux.lock();
		ap = nullptr;
		amux.unlock();
	}
}

void XAudioThread::run()
{
	auto* pcm = new unsigned char[1024 * 1024 * 10];
	while (!isExit)
	{
		amux.lock();
		AVPacket* pkt = Pop();
		bool re = decode->Send(pkt);
		if (!re)
		{
			amux.unlock();
			msleep(1);
			continue;
		}
		//一次send 多次recv
		while (!isExit)
		{
			AVFrame* frame = decode->Recv();
			if (!frame) break;

			//减去缓冲中未播放的时间
			pts = decode->pts - ap->GetNoPlayMs();

			//重采样 
			int size = res->Resample(frame, pcm);
			//播放音频
			while (!isExit)
			{
				if (size <= 0)break;
				//缓冲未播完，空间不够
				if (ap->GetFree() < size)
				{
					msleep(1);
					continue;
				}

				ap->Write(pcm, size);
				break;
			}
		}
		amux.unlock();
	}
	delete[] pcm;
}

XAudioThread::XAudioThread()
{
	if (!res) res = new XResample();
	if (!ap) ap = XAudioPlay::Get();
}


XAudioThread::~XAudioThread()
{
}
