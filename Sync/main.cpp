#include "obsidiansync.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ObsidianSync w;
    if(!w.getStart()) w.show();
    return a.exec();
}
