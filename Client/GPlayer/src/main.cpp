#include <QTextCodec>
#include <QtWidgets/QApplication>
#include "ClockWdg.h"
#include "GtooPlayer.h"
#include "RemWordWdg.h"
#include "XiaomiWatchDisplayWdg.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 如果解开注释textBrower会输出乱码
    // QTextCodec *codec = QTextCodec::codecForName("UTF-8");  // 或者"GBK",不分大小写
    // QTextCodec::setCodecForLocale(codec);

    GtooPlayer w;
    w.show();

    // ClockWdg clockwdg;
    // clockwdg.show();

    //RemWordWdg *remWordWdg = new RemWordWdg();
    //remWordWdg->show();

    // XiaomiWatchDisplayWdg* miWdg = new XiaomiWatchDisplayWdg();
    // miWdg->show();
    return a.exec();
}
