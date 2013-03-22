#include <QApplication>
#include <vlcwidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVlcWidget vlc;
    vlc.resize(640,480);
    vlc.show();

    return a.exec();
}
