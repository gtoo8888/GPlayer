#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <map>
#include "RemWordListWdg.h"
#include "RemWordToolWdg.h"
#include "ui_RemWordWdg.h"

class RemWordWdg : public QWidget {
    Q_OBJECT

public:
    RemWordWdg(QWidget* parent = nullptr);
    ~RemWordWdg();

public slots:
    void slotBtnOpenWordListWdg(void);
    void slotBtnOpenWordRemWdg(void);
    void slotBtnOpenToolWdg(void);
    void slotBtnOpenWordExWdg(void);

private:
    Ui::RemWordWdg* ui;
    QVBoxLayout* vlayout;
    std::map<QPushButton, bool> mMapBtn;
    std::shared_ptr<RemWordListWdg> mspRemWordListWdg;
    std::shared_ptr<RemWordToolWdg> mspRemWordToolWdg;
};
