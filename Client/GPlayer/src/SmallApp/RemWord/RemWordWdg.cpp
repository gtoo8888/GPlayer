#include "RemWordWdg.h"

RemWordWdg::RemWordWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordWdg) {
    ui->setupUi(this);
    vlayout = new QVBoxLayout(this);
    ui->funWdg->setLayout(vlayout);
    this->setMinimumSize(QSize(1600, 600));

    mspRemWordListWdg = nullptr;
    mspRemWordRemWdg = nullptr;
    mspRemWordToolWdg = nullptr;
    mspRemWordExWdg = nullptr;

    mMapBtn.insert(std::make_pair(ui->btnWordList, false));
    mMapBtn.insert(std::make_pair(ui->btnWordRember, false));
    mMapBtn.insert(std::make_pair(ui->btnTool, false));
    mMapBtn.insert(std::make_pair(ui->btnWordExample, false));

    connect(ui->btnWordList, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenWordListWdg);
    connect(ui->btnWordRember, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenWordRemWdg);
    connect(ui->btnTool, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenToolWdg);
    connect(ui->btnWordExample, &QPushButton::clicked, this, &RemWordWdg::slotBtnOpenWordExWdg);

    slotBtnOpenWordRemWdg();
}

void RemWordWdg::slotBtnOpenWordListWdg(void) {
    updateBtnStatus(ui->btnWordList);
    mspRemWordListWdg = std::make_shared<RemWordListWdg>(ui->funWdg);
    vlayout->addWidget(mspRemWordListWdg.get());
    managePages(mspRemWordListWdg);
}

void RemWordWdg::slotBtnOpenWordRemWdg(void) {
    updateBtnStatus(ui->btnWordRember);
    mspRemWordRemWdg = std::make_shared<RemWordRemWdg>(ui->funWdg);
    vlayout->addWidget(mspRemWordRemWdg.get());
    managePages(mspRemWordRemWdg);
}

void RemWordWdg::slotBtnOpenToolWdg(void) {
    updateBtnStatus(ui->btnTool);
    mspRemWordToolWdg = std::make_shared<RemWordToolWdg>(ui->funWdg);
    vlayout->addWidget(mspRemWordToolWdg.get());
    managePages(mspRemWordToolWdg);
}

void RemWordWdg::slotBtnOpenWordExWdg(void) {
    updateBtnStatus(ui->btnWordExample);
    mspRemWordExWdg = std::make_shared<RemWordExWdg>(ui->funWdg);
    vlayout->addWidget(mspRemWordExWdg.get());
    managePages(mspRemWordExWdg);
}

void RemWordWdg::managePages(std::shared_ptr<QWidget> currentPage) {
    if (mspRemWordListWdg && mspRemWordListWdg != currentPage) {
        mspRemWordListWdg->close();
        mspRemWordListWdg.reset();
    }
    if (mspRemWordRemWdg && mspRemWordRemWdg != currentPage) {
        mspRemWordRemWdg->close();
        mspRemWordRemWdg.reset();
    }
    if (mspRemWordToolWdg && mspRemWordToolWdg != currentPage) {
        mspRemWordToolWdg->close();
        mspRemWordToolWdg.reset();
    }
    if (mspRemWordExWdg && mspRemWordExWdg != currentPage) {
        mspRemWordExWdg->close();
        mspRemWordExWdg.reset();
    }

    if (currentPage) {
        currentPage->show();
    }
}

void RemWordWdg::updateBtnStatus(QPushButton* btn) {
    for (auto& it : mMapBtn) {
        it.second = (it.first == btn);
        it.first->setDisabled(it.second);
    }
}

RemWordWdg::~RemWordWdg() {
}
