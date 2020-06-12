#pragma once
#include <QThread>
#include <mutex>
#include <list>
struct AVCodecParameters;
class XDecode;
class XAudioPlay;
class XResample;
class AVPacket;
class XAudioThread :public QThread
{
public:
	// ��ǰ��Ƶ���ŵ� pts
	long long pts = 0;
	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters *para, int sampleRate, int channels);
	virtual void Push(AVPacket *pkt);
	void run();
	XAudioThread();
	virtual ~XAudioThread();

	//������
	unsigned long long maxList = 50;
	// �߳��˳���־��
	bool isExit = false;
protected:
	// ���У������ݣ��൱�������������ߣ��� push �������ݣ��� run ����������
	std::list <AVPacket *> packs;
	std::mutex mux;
	XDecode *decode = 0;
	XAudioPlay *ap = 0;
	XResample *res = 0;

};

