#include "RemWordRemWdg.h"
#include <QMessageBox>
#include <QScrollArea>

RemWordRemWdg::RemWordRemWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordRemWdg) {
    ui->setupUi(this);

    initUI();

    connect(ui->btnTest, &QPushButton::clicked, this, &RemWordRemWdg::slotBtnTest);
    connect(ui->btnBatchConfirm, &QPushButton::clicked, this, &RemWordRemWdg::slotBtnBatchConfirm);
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

void RemWordRemWdg::initUI(void) {
    pGridLayout = new QGridLayout();
    pGridLayout->setHorizontalSpacing(0);
    pGridLayout->setVerticalSpacing(0);
    pGridLayout->setSpacing(0);
    pGridLayout->setAlignment(Qt::AlignTop);  // Ê¼ÖÕ¿¿×Å¶¥²¿
    ui->scrollAreaWdg->setLayout(pGridLayout);

    initWordList();
    int32 wordNum = mvWordList.size();
    for (int i = 0; i < wordNum; i++) {
        LOG_DBG("{} {}", mvWordList[i].WordTranslation, mvWordList[i].Word);
        std::shared_ptr<WrtSglWdg> wrtSglWdg = std::make_shared<WrtSglWdg>(mvWordList[i], ui->scrollAreaWdg);
        pGridLayout->addWidget(wrtSglWdg.get());
        mvspWrtSglWdg.push_back(wrtSglWdg);
    }

    mvbWordListAns.resize(wordNum, false);

    ui->dateEdit->setCalendarPopup(true);
    ui->labInfo->setText("Start Test Memory Word");
}

void RemWordRemWdg::initWordList(void) {
    mspWordSql = std::make_shared<WordSql>("word.db", "WordList");
    mspWordSql->initDB();

    mvWordList = mspWordSql->getWordTable();
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
