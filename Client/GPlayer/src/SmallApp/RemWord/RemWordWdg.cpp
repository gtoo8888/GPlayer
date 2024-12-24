#include "RemWordWdg.h"

RemWordWdg::RemWordWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordWdg) {
    ui->setupUi(this);
    vlayout = new QVBoxLayout(this);
    ui->funWdg->setLayout(vlayout);

    //std::map<QPushButton, bool> mmBtnList;
    //std::shared_ptr<RemWordListWdg> mspRemWordListWdg;
    //std::shared_ptr<RemWordToolWdg> mspRemWordToolWdg;

    mspRemWordListWdg = nullptr;
    mspRemWordToolWdg = nullptr;


    mMapBtn.insert(std::make_pair(ui->btnWordList, false));

    connect(ui->btnWordList, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenWordListWdg);
    connect(ui->btnWordRember, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenWordRemWdg);
    connect(ui->btnTool, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenToolWdg);
    connect(ui->btnWordExample, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenWordExWdg);
}

void RemWordWdg::slotBtnOpenWordListWdg(void) {
    ui->btnWordList->setDisabled(true);
    mspRemWordListWdg = std::make_shared<RemWordListWdg>(ui->funWdg);
    vlayout->addWidget(mspRemWordListWdg.get());
    mspRemWordListWdg->close();
    mspRemWordListWdg->show();
    
}

void RemWordWdg::slotBtnOpenWordRemWdg(void) {
}

void RemWordWdg::slotBtnOpenToolWdg(void) {
    ui->btnTool->setDisabled(true);
    mspRemWordListWdg = std::make_shared<RemWordToolWdg>(ui->funWdg);
    vlayout->addWidget(mspRemWordListWdg.get());
    mspRemWordListWdg->show();
}

void RemWordWdg::slotBtnOpenWordExWdg(void) {
}

RemWordWdg::~RemWordWdg() {
}
