#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <map>
#include "RemWordListWdg.h"
#include "RemWordRemWdg.h"
#include "RemWordToolWdg.h"
#include "RemWordExWdg.h"
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
    void updateBtnStatus(QPushButton* btn);
    void managePages(std::shared_ptr<QWidget> currentPage);

    Ui::RemWordWdg* ui;
    QVBoxLayout* vlayout;
    std::map<QPushButton*, bool> mMapBtn;
    std::shared_ptr<RemWordListWdg> mspRemWordListWdg;
    std::shared_ptr<RemWordToolWdg> mspRemWordToolWdg;
    std::shared_ptr<RemWordRemWdg> mspRemWordRemWdg;
    std::shared_ptr<RemWordExWdg> mspRemWordExWdg;
};
