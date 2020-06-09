#include "XAudioPlay.h"
#include <QAudioFormat>
#include <QAudioOutput>
#include <mutex>

class CXAudioPlay : public XAudioPlay
{
public:
	QAudioOutput* output = nullptr;
	QIODevice* io = nullptr;
	std::mutex mux;

	void Close() override
	{
		mux.lock();
		if (io)
		{
			io->close();
			io = nullptr;
		}
		if (output)
		{
			output->stop();
			delete output;
			output = nullptr;
		}
		mux.unlock();
	}

	bool Open() override
	{
		Close();
		QAudioFormat fmt;
		fmt.setSampleRate(sampleRate);
		fmt.setSampleSize(sampleSize);
		fmt.setChannelCount(channels);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);
		mux.lock();
		output = new QAudioOutput(fmt);
		io = output->start(); //¿ªÊ¼²¥·Å
		mux.unlock();
		if (io)
			return true;
		return false;
	}
};

XAudioPlay* XAudioPlay::Get()
{
	static CXAudioPlay play;
	return &play;
}

XAudioPlay::XAudioPlay()
{
}


XAudioPlay::~XAudioPlay()
{
}
