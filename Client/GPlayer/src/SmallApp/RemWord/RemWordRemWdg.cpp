#include "RemWordRemWdg.h"
#include <QScrollArea>

RemWordRemWdg::RemWordRemWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordRemWdg) {
    ui->setupUi(this);

    vlayout = new QVBoxLayout(this);
    ui->scrollAreaWdg->setLayout(vlayout);
    ui->dateEdit->setCalendarPopup(true);

    //QScrollArea *mpScroll = new QScrollArea(ui->wordListWdg);
    //mpScroll->setWidget(ui->wordListWdg);

    connect(ui->btnTest, &QPushButton::clicked, this, &RemWordRemWdg::slotBtnTest);
}

RemWordRemWdg::~RemWordRemWdg() {
}

void RemWordRemWdg::slotBtnTest(void) {
    WrtSglWdg* wrtSglWdg = new WrtSglWdg(ui->scrollAreaWdg);
    vlayout->addWidget(wrtSglWdg);
}
