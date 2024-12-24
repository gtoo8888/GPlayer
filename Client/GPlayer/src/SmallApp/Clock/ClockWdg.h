#ifndef CLOCKWDG_H
#define CLOCKWDG_H

#include <QWidget>

namespace Ui {
class ClockWdg;
}

class ClockWdg : public QWidget {
    Q_OBJECT

public:
    explicit ClockWdg(QWidget *parent = nullptr);
    ~ClockWdg();

private slots:
    void on_btnStopwatch_clicked();

private:
    Ui::ClockWdg *ui;
};

#endif  // CLOCKWDG_H
