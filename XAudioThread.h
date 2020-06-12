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
	// 当前音频播放的 pts
	long long pts = 0;
	//打开，不管成功与否都清理
	virtual bool Open(AVCodecParameters *para, int sampleRate, int channels);
	virtual void Push(AVPacket *pkt);
	void run();
	XAudioThread();
	virtual ~XAudioThread();

	//最大队列
	unsigned long long maxList = 50;
	// 线程退出标志量
	bool isExit = false;
protected:
	// 队列，存数据，相当于生产者消费者，由 push 生成数据，在 run 中消费数据
	std::list <AVPacket *> packs;
	std::mutex mux;
	XDecode *decode = 0;
	XAudioPlay *ap = 0;
	XResample *res = 0;

};

