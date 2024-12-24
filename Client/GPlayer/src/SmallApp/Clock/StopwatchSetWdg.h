#ifndef STOPWATCHSETWDG_H
#define STOPWATCHSETWDG_H

#include <QVBoxLayout>
#include <QWidget>
#include "ClockConfig.h"

namespace Ui {
class StopwatchSetWdg;
}

class StopwatchSetWdg : public QWidget {
    Q_OBJECT

public:
    static StopwatchSetWdg* getInstance() {
        if (instanceSetWdg == nullptr) {
            instanceSetWdg = new StopwatchSetWdg();
        }
        return instanceSetWdg;
    }

private:
    StopwatchSetWdg(QWidget* parent = nullptr);
    ~StopwatchSetWdg();

    Ui::StopwatchSetWdg* ui;

    void initUi(void);
    static StopwatchSetWdg* instanceSetWdg;
    ClockConfig* clockConfig;
};

#endif  // STOPWATCHSETWDG_H
