#include "RemWordListWdg.h"

RemWordListWdg::RemWordListWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordListWdg) {
    ui->setupUi(this);
    mspWordSql = nullptr;

    // 1735136045 2024-12-25 22:14:05
    // unix2StrTimeChrono(1735136045);
    // unix2StrTimeQString(1735136045);

    initUI();
    initWordList();
    updateTableWdg();
}

RemWordListWdg::~RemWordListWdg() {
}

void RemWordListWdg::initUI(void) {
    this->setMinimumSize(QSize(1600, 600));
    ui->twWordList->verticalHeader()->setVisible(false);
    cleanTableWdg();
    ui->twWordList->verticalHeader()->setVisible(false);
    ui->twWordList->setSelectionBehavior(QAbstractItemView::SelectRows);  // 可以选中一行
    ui->twWordList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->dateEdit->setCalendarPopup(true);
}

void RemWordListWdg::cleanTableWdg(void) {
    ui->twWordList->clear();
    while (ui->twWordList->rowCount() > 0) {
        ui->twWordList->removeRow(0);
    }

    QStringList headerLabels = {"序号",   "单词",     "翻译",     "词性",     "查翻译",
                                "同义词", "遇见次数", "创建时间", "修改时间", "删除时间"};
    // 自动调整列宽
    ui->twWordList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 每一列都自动调整列宽，但是无法手动修改
    for (int i = 0; i < headerLabels.size(); i++) {
        ui->twWordList->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
    ui->twWordList->setColumnCount(headerLabels.size());
    ui->twWordList->setHorizontalHeaderLabels(headerLabels);
}

void RemWordListWdg::updateTableWdg(void) {
    int wordNum = mvWordList.size();
    ui->twWordList->setRowCount(wordNum);  // 一定要设置行数以后才能显示
    for (int idx = 0; idx < wordNum; idx++) {
        ui->twWordList->setItem(idx, 0, new QTableWidgetItem(QString::number(mvWordList[idx].id)));
        ui->twWordList->setItem(idx, 1, new QTableWidgetItem(QString::fromStdString(mvWordList[idx].Word)));
        ui->twWordList->setItem(idx, 2, new QTableWidgetItem(QString::fromStdString(mvWordList[idx].WordTranslation)));
        ui->twWordList->setItem(idx, 3, new QTableWidgetItem(QString::fromStdString(mvWordList[idx].PartOfSpeech)));
        ui->twWordList->setItem(idx, 4, new QTableWidgetItem(QString::fromStdString(mvWordList[idx].SearchMean)));
        ui->twWordList->setItem(idx, 5, new QTableWidgetItem(QString::fromStdString(mvWordList[idx].Synonym)));
        ui->twWordList->setItem(idx, 6, new QTableWidgetItem(QString::number(mvWordList[idx].MeetTime)));
        std::string createTimeStr = unix2StrTimeChrono(mvWordList[idx].CreateTime);
        ui->twWordList->setItem(idx, 7, new QTableWidgetItem(QString::fromStdString(createTimeStr)));
        std::string modifyTimeStr = unix2StrTimeChrono(mvWordList[idx].ModifyTime);
        ui->twWordList->setItem(idx, 8, new QTableWidgetItem(QString::fromStdString(modifyTimeStr)));
        std::string deleteTimeStr = unix2StrTimeChrono(mvWordList[idx].DeleteTime);
        ui->twWordList->setItem(idx, 9, new QTableWidgetItem(QString::fromStdString(deleteTimeStr)));
    }
}

void RemWordListWdg::initWordList(void) {
    mspWordSql = std::make_shared<WordSql>("word.db", "WordList");
    mspWordSql->initDB();

    mvWordList = mspWordSql->getWordTable();
}

QString RemWordListWdg::unix2StrTimeQString(int64 time) {
    QDateTime dateTime = QDateTime::fromTime_t(time);
    QString formattedDateTime = dateTime.toString("yyyy-MM-dd HH:mm:ss");
    LOG_DBG("Formatted datetime: {}", formattedDateTime.toStdString());
    return formattedDateTime;
}

std::string RemWordListWdg::unix2StrTimeChrono(int64 time) {
    if (time == 0) {
        return std::string("");
    }
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);
    std::time_t localTime = std::chrono::system_clock::to_time_t(tp);
    std::tm* tmLocal = localtime(&localTime);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmLocal);
    std::string formattedDateTime(buffer);
    LOG_DBG("Formatted datetime: {}", formattedDateTime);
    return formattedDateTime;
}
