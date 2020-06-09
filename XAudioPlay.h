#pragma once
class XAudioPlay
{
public:
	int sampleRate = 44100;
	int sampleSize = 16;
	int channels = 2;

	//´ò¿ªÒôÆµ²¥·Å
	virtual bool Open() = 0;
	virtual void Close() = 0;

	static XAudioPlay* Get();
	XAudioPlay();
	virtual ~XAudioPlay();
};
