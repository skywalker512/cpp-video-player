#include <iostream>
#include <QtWidgets/QApplication>


#include "XDecode.h"
#include "Xplay.h"
#include "XDemux.h"

using namespace std;

int main(int argc, char *argv[])
{
	XDemux demux;

	char *url = "small.mp4";
	cout << "demux.Open = " << demux.Open(url);
	demux.Read();
	demux.Clear();
	demux.Close();
	cout << "demux.Open = " << demux.Open(url);
	cout << "CopyVPara = " << demux.CopyVPara() << endl;
	cout << "CopyAPara = " << demux.CopyAPara() << endl;
	cout << "seek=" << demux.Seek(0.95) << endl;

	XDecode vdecode;
	cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
	vdecode.Clear();
	vdecode.Close();
	XDecode adecode;
	cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
	
	for (;;)
	{
		AVPacket *pkt = demux.Read();
		if (!pkt)break;
	}

    QApplication a(argc, argv);
    Xplay w;
    w.show();
    return a.exec();
}
