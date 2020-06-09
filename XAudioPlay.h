#pragma once
class XAudioPlay
{
public:
	int sampleRate = 44100;
	int sampleSize = 16;
	int channels = 2;

	//����Ƶ����
	virtual bool Open() = 0;
	virtual void Close() = 0;

	//������Ƶ
	virtual bool Write(const unsigned char *data, int datasize) = 0;
	virtual int GetFree() = 0;
	
	static XAudioPlay* Get();
	XAudioPlay();
	virtual ~XAudioPlay();
};
