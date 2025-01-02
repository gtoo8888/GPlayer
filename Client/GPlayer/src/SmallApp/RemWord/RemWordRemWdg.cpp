#include "RemWordRemWdg.h"
#include <QMessageBox>
#include <QScrollArea>

RemWordRemWdg::RemWordRemWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordRemWdg) {
    ui->setupUi(this);

    initUI();
    // 艾宾浩斯遗忘曲线，需要复习的天数
    mvEbbinghausForgettingCurve = {1, 2, 4, 7, 15, 30, 90, 180};

    connect(ui->btnTest, &QPushButton::clicked, this, &RemWordRemWdg::slotBtnTest);
    connect(ui->btnBatchConfirm, &QPushButton::clicked, this, &RemWordRemWdg::slotBtnBatchConfirm);
    connect(ui->btnClean, &QPushButton::clicked, this, &RemWordRemWdg::slotBtnClean);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &RemWordRemWdg::slotDateChange);
}

RemWordRemWdg::~RemWordRemWdg() {
}

void RemWordRemWdg::slotBtnTest(void) {
}

void RemWordRemWdg::slotBtnBatchConfirm(void) {
    int32 wordNum = mvWordList.size();
    mvbWordListAns.resize(wordNum, false);
    std::vector<int32> errWord;
    for (int i = 0; i < wordNum; i++) {
        bool ans = mvspWrtSglWdg[i]->slotBtnConfirm();
        mvbWordListAns[i] = ans;
        if (ans == false) {
            errWord.push_back(mvspWrtSglWdg[i]->getWordIdx());
        }
    }

    processMemoryResult(errWord);
}

void RemWordRemWdg::slotDateChange(void) {
    slotBtnClean();  // 更新之前先清空
    QDate date = ui->dateEdit->date();
    std::string getDay = date.toString("yyyy-MM-dd").toStdString();
    std::string startTime = getDay + " 00:00:00";
    int64 startUnix = GUtils::str2Unix(startTime);
    std::string endTime = getDay + " 23:59:59";
    int64 endUnix = GUtils::str2Unix(endTime);
    mvWordList = mspWordSql->selectWordTableElapsedTime(startUnix, endUnix);

    int32 wordNum = mvWordList.size();
    for (int i = 0; i < wordNum; i++) {
        LOG_DBG("{} {}", mvWordList[i].WordTranslation, mvWordList[i].Word);
        std::shared_ptr<WrtSglWdg> wrtSglWdg = std::make_shared<WrtSglWdg>(mvWordList[i], ui->scrollAreaWdg);
        pGridLayout->addWidget(wrtSglWdg.get());
        mvspWrtSglWdg.push_back(wrtSglWdg);
    }
    mvbWordListAns.resize(wordNum, false);
}

void RemWordRemWdg::slotBtnClean(void) {
    // FIXME 仅仅清空了界面，还是会有内存泄露
    for (std::shared_ptr<WrtSglWdg>& it : mvspWrtSglWdg) {
        it.reset();
    }

    mvspWrtSglWdg.clear();
    mvWordList.clear();
}

void RemWordRemWdg::initUI(void) {
    pGridLayout = new QGridLayout();
    pGridLayout->setHorizontalSpacing(0);
    pGridLayout->setVerticalSpacing(0);
    pGridLayout->setSpacing(0);
    pGridLayout->setAlignment(Qt::AlignTop);  // 始终靠着顶部
    ui->scrollAreaWdg->setLayout(pGridLayout);

    initWordList();
    mvWordList = mspWordSql->selectWordTable();
    int32 wordNum = mvWordList.size();
    for (int i = 0; i < wordNum; i++) {
        LOG_DBG("{} {}", mvWordList[i].WordTranslation, mvWordList[i].Word);
        std::shared_ptr<WrtSglWdg> wrtSglWdg = std::make_shared<WrtSglWdg>(mvWordList[i], ui->scrollAreaWdg);
        pGridLayout->addWidget(wrtSglWdg.get());
        mvspWrtSglWdg.push_back(wrtSglWdg);
    }

    mvbWordListAns.resize(wordNum, false);

    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->labInfo->setText("Start Test Memory Word");
}

void RemWordRemWdg::initWordList(void) {
    mspWordSql = std::make_shared<WordSql>("word.db", "WordList");
    mspWordSql->initDB();
}

void RemWordRemWdg::processMemoryResult(std::vector<int32> errWordList) {
    if (errWordList.empty()) {
        std::string correctWordPre("All words are correct.");
        LOG_DBG("{}", correctWordPre);
        // QMessageBox::information(this, "Info", QString::fromStdString(correctWordPre));
        ui->labInfo->setText(QString::fromStdString(correctWordPre));
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::blue);
        ui->labInfo->setPalette(pe);
    } else {
        std::string errWordPre("Partial word errors. ");
        LOG_DBG("{}", errWordPre);
        std::string errWordStr("Error Word idx: ");
        int32 errWordNum = errWordList.size();
        for (int i = 0; i < errWordNum; i++) {
            std::string tNum = std::to_string(errWordList[i]);
            errWordStr += (i == errWordNum - 1) ? tNum : tNum + ",";
        }
        LOG_DBG("{}", errWordStr);
        std::string errWordTotal = errWordPre + "\n" + errWordStr;
        // QMessageBox::warning(this, "Warning", QString::fromStdString(errWordTotal));
        ui->labInfo->setText(QString::fromStdString(errWordTotal));
        QPalette pe;
        // pe.setColor(QPalette::Background, Qt::black);
        pe.setColor(QPalette::WindowText, Qt::red);
        ui->labInfo->setPalette(pe);
    }
}
