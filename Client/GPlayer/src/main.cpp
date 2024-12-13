#include <QtWidgets/QApplication>
#include "GtooPlayer.h"
#include "XiaomiWatchDisplayWdg.h"
#include "RemWordWdg.h"
#include "gtest/gtest.h"
// #include <QTest>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //testing::InitGoogleTest(&argc, argv);
    //RUN_ALL_TESTS();

    //GtooPlayer w;
    //w.show();

    //RemWordWdg* remWordWdg = new RemWordWdg();
    //remWordWdg->show();

    XiaomiWatchDisplayWdg* miWdg = new XiaomiWatchDisplayWdg();
    miWdg->show();
    return a.exec();
}
