#include "XAudioThread.h"
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"
#include <iostream>
extern "C" {
#include <libavutil/frame.h>
}
using namespace std;

void XAudioThread::Push(AVPacket* pkt)
{
	if (!pkt) return;
	//����
	while (!isExit)
	{
		mux.lock();
		// ��󻺳����
		if (packs.size() < maxList)
		{
			packs.push_back(pkt);
			mux.unlock();
			break;
		}
		mux.unlock();
	}
}

bool XAudioThread::Open(AVCodecParameters* para, int sampleRate, int channels)
{
	if (!para)return false;
	mux.lock();
	// �����ϴ�Ӱ��
	pts = 0;
	if (!decode) decode = new XDecode();
	if (!res) res = new XResample();
	if (!ap) ap = XAudioPlay::Get();
	// ��־���Ƿ��������̳ɹ�
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
	mux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XAudioThread::run()
{
	auto* pcm = new unsigned char[1024 * 1024 * 10];
	while (!isExit)
	{
		mux.lock();

		//û������
		if (packs.empty() || !decode || !res || !ap)
		{
			mux.unlock();
			msleep(1);
			continue;
		}
		// �Ӷ�ͷȡ����
		AVPacket* pkt = packs.front();
		// ��ջ
		packs.pop_front();
		bool re = decode->Send(pkt);
		if (!re)
		{
			mux.unlock();
			msleep(1);
			continue;
		}
		//һ��send ���recv
		while (!isExit)
		{
			AVFrame* frame = decode->Recv();
			if (!frame) break;

			//��ȥ������δ���ŵ�ʱ��
			pts = decode->pts - ap->GetNoPlayMs();

			//�ز��� 
			int size = res->Resample(frame, pcm);
			av_frame_free(&frame);
			//������Ƶ
			while (!isExit)
			{
				if (size <= 0)break;
				//����δ���꣬�ռ䲻��
				if (ap->GetFree() < size)
				{
					msleep(1);
					continue;
				}
				ap->Write(pcm, size);
				break;
			}
		}
		mux.unlock();
		msleep(1);
	}
	delete[] pcm;
}

XAudioThread::XAudioThread()
{
}


XAudioThread::~XAudioThread()
{
	//�ȴ��߳��˳�
	isExit = true;
	wait();
}
