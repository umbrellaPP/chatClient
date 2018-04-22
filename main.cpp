#include <QApplication>
#include <QDebug>
#include "util/dialogmgr.h"
#include "util/datamamager.h"
#include "util/chattingrecordutil.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Net->init();
    DataMgr->init();
    ChattingRecord->init();

    DialogMgr->showLogin();
    return a.exec();
}
