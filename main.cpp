#include "Xplay.h"
#include "XDemuxThread.h"

using namespace std;

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Xplay w;
	w.show();

	XDemuxThread dt;
	dt.Open("2.mp4", w.ui.video);
	dt.Start();

	return a.exec();
}
