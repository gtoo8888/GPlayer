#ifndef STOPWATCHSETSHORTKEY_H
#define STOPWATCHSETSHORTKEY_H

#include <QWidget>

namespace Ui {
class StopwatchSetShortKey;
}

class StopwatchSetShortKey : public QWidget
{
    Q_OBJECT

public:
    explicit StopwatchSetShortKey(QWidget *parent = nullptr);
    ~StopwatchSetShortKey();

private:
    Ui::StopwatchSetShortKey *ui;
};

#endif // STOPWATCHSETSHORTKEY_H
