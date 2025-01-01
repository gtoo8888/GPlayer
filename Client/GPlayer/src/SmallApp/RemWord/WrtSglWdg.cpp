#include "WrtSglWdg.h"
#include <QMessageBox>

WrtSglWdg::WrtSglWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::WrtSglWdg) {
    ui->setupUi(this);

    // TODO 警告，没有传递参数
    LOG_ERR("No input parameters!!");
}

std::string convertIdxToStr(int32 idx) {
    char* res = static_cast<char*>(calloc(5, sizeof(char)));
    snprintf(res, 5, "%03d. ", idx);
    return std::string(res);
}

WrtSglWdg::WrtSglWdg(WordSqlInfo wordSqlInfo, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::WrtSglWdg) {
    ui->setupUi(this);
    mWordSqlInfo = wordSqlInfo;
    mWordIdx = mWordSqlInfo.id;
    msChinesWord = mWordSqlInfo.WordTranslation;
    msEnglishWord = mWordSqlInfo.Word;

    ui->labIdx->setText(QString::fromStdString(convertIdxToStr(mWordIdx)));
    ui->leChinese->setText(QString::fromStdString(msChinesWord));
    // ui->leEnglish->setText(QString::fromStdString(msEnglishWord));

    connect(ui->btnconfirm, &QPushButton::clicked, this, &WrtSglWdg::slotBtnConfirm);
}

WrtSglWdg::~WrtSglWdg() {
}

bool WrtSglWdg::slotBtnConfirm(void) {
    std::string inputMemoryEnglish = ui->leEnglish->text().toStdString();
    if (checkMemeryEnglish(inputMemoryEnglish)) {
        ui->tbStatus->setText(QString("对"));
        return true;
    } else {
        ui->tbStatus->setText(QString("错"));
        return false;
    }
}

bool WrtSglWdg::checkMemeryEnglish(std::string word) {
    if (word.empty()) {
        LOG_WRN("Input memory english is empty");
        return false;
    }
    if (!word.compare(msEnglishWord)) {
        return true;
    } else {
        LOG_WRN("Input word is different from the correct word");
        return false;
    }
    return false;
}

int32 WrtSglWdg::getWordIdx(void) {
    return mWordIdx;
}
