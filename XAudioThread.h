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
	void Close() override;
	void Clear() override;
	void run() override;
	XAudioThread();
	virtual ~XAudioThread();
	void SetPause(bool isPause);
	bool isPause = false;
protected:
	std::mutex amux;
	XAudioPlay* ap = nullptr;
	XResample* res = nullptr;
};
