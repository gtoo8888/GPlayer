#include "RemWordWdg.h"

RemWordWdg::RemWordWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordWdg) {
    ui->setupUi(this);
    vlayout = new QVBoxLayout(this);
    ui->funWdg->setLayout(vlayout);

    connect(ui->btnTool, &QPushButton::clicked, this, &RemWordWdg::slotOpenToolWdg);
}

void RemWordWdg::slotOpenToolWdg(void) {
    ui->btnTool->setDisabled(true);
    RemWordToolWdg* remWordToolWdg = new RemWordToolWdg(ui->funWdg);
    vlayout->addWidget(remWordToolWdg);
    remWordToolWdg->show();
}

RemWordWdg::~RemWordWdg() {
}
