#ifndef CLOCKSETWDG_H
#define CLOCKSETWDG_H

#include <QWidget>

namespace Ui {
class ClockSetWdg;
}

class ClockSetWdg : public QWidget {
    Q_OBJECT

public:
    explicit ClockSetWdg(QWidget *parent = nullptr);
    ~ClockSetWdg();

private:
    Ui::ClockSetWdg *ui;
};

#endif  // CLOCKSETWDG_H
