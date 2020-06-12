#pragma once
#include <mutex>
#include "XDecodeThread.h"
struct AVCodecParameters;
class XAudioPlay;
class XResample;

class XAudioThread : public XDecodeThread
{
public:
	// ��ǰ��Ƶ���ŵ� pts
	long long pts = 0;
	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters* para, int sampleRate, int channels);
	void run() override;
	XAudioThread();
	virtual ~XAudioThread();
protected:
	std::mutex amux;
	XAudioPlay* ap = nullptr;
	XResample* res = nullptr;
};
