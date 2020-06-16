#pragma once
#include <mutex>
#include "XDecodeThread.h"
struct AVCodecParameters;
class XAudioPlay;
class XResample;

class XAudioThread : public XDecodeThread
{
public:
	// 当前音频播放的 pts
	long long pts = 0;
	//打开，不管成功与否都清理
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
