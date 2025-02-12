#include "PlayList.h"
#include <QDebug>
#include <QDir>
#include <QHBoxLayout>

PlayList::PlayList(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PlayList) {
    ui->setupUi(this);
    InitUi();
    InitConnect();
    setAcceptDrops(true);
}

PlayList::~PlayList() {
    QStringList strListPlayList;
    // for (int i = 0; i < ui->List->count(); i++)
    //{
    //     strListPlayList.append(ui->List->item(i)->toolTip());
    // }
    // GlobalHelper::SavePlaylist(strListPlayList);
    delete ui;
}

void PlayList::InitUi() {
    ui->listWidget->clear();
    QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);

    //-----------TODEBUG-----------------
    QFileInfo fileInfo(
        "E:/Desktop/languguetest/Cplusplustest/3-VisualStudio2017/0-GtooPlayer/test_video/"
        "1_1_autoconf�Զ�����Makefile_��9�� autoconf�Զ�����Makefile.mp4");
    pItem->setData(Qt::UserRole, QVariant(fileInfo.filePath()));  // �û����� TODO �û����浽���ģ�
    pItem->setText(fileInfo.fileName());                          // ��ʾ�ı�
    pItem->setToolTip(fileInfo.filePath());  // ������ʾ�������ͣʱ��ʾ���ı���Ϊ�ļ�������·����
    ui->listWidget->addItem(pItem);
    //-----------TODEBUG-----------------
}

void PlayList::InitConnect() {
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &PlayList::AddFile);
    connect(ui->pushButtonRemove, &QPushButton::clicked, this, &PlayList::RemoveFile);
    connect(ui->pushButtonClearList, &QPushButton::clicked, ui->listWidget, &QListWidget::clear);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayList::playClickedItem);
}

void PlayList::playClickedItem(QListWidgetItem *item) {
    QString nowItem = item->data(Qt::UserRole).toString();
    emit SigPlay(nowItem);
    mCurrentPlayListIndex = ui->listWidget->row(item);
    ui->listWidget->setCurrentRow(mCurrentPlayListIndex);
}

bool PlayList::GetPlaylistStatus() {
    if (this->isHidden()) {
        return false;
    }

    return true;
}

void PlayList::OnBackwardPlay() {
    /* if (m_nCurrentPlayListIndex == 0)
     {
         m_nCurrentPlayListIndex = ui->List->count() - 1;
         on_List_itemDoubleClicked(ui->List->item(m_nCurrentPlayListIndex));
         ui->List->setCurrentRow(m_nCurrentPlayListIndex);
     }
     else
     {
         m_nCurrentPlayListIndex--;
         on_List_itemDoubleClicked(ui->List->item(m_nCurrentPlayListIndex));
         ui->List->setCurrentRow(m_nCurrentPlayListIndex);
     }*/
}

void PlayList::OnForwardPlay() {
    // if (m_nCurrentPlayListIndex == ui->List->count() - 1)
    //{
    //     m_nCurrentPlayListIndex = 0;
    //     on_List_itemDoubleClicked(ui->List->item(m_nCurrentPlayListIndex));
    //     ui->List->setCurrentRow(m_nCurrentPlayListIndex);
    // }
    // else
    //{
    //     m_nCurrentPlayListIndex++;
    //     on_List_itemDoubleClicked(ui->List->item(m_nCurrentPlayListIndex));
    //     ui->List->setCurrentRow(m_nCurrentPlayListIndex);
    // }
}

void PlayList::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    for (QUrl url : urls) {
        QString strFileName = url.toLocalFile();

        OnAddFile(strFileName);
    }
}

void PlayList::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void PlayList::AddFile() {
    // QList<QUrl> QFileDialog::getOpenFileUrls
    QStringList listFileName = QFileDialog::getOpenFileNames(this, "Select to play video��",  // QDir::homePath(),
        "E:/Desktop/languguetest/Cplusplustest/3-VisualStudio2017/0-GtooPlayer/test_video",
        "Video file(*.mkv *.rmvb *.mp4 *.avi *.flv *.wmv *.3gp)");

    qDebug() << listFileName;

    for (QString strFileName : listFileName) {
        OnAddFile(strFileName);
    }
}

void PlayList::OnAddFile(QString strFileName) {
    bool bSupportMovie =
        strFileName.endsWith(".mkv", Qt::CaseInsensitive) || strFileName.endsWith(".rmvb", Qt::CaseInsensitive) ||
        strFileName.endsWith(".mp4", Qt::CaseInsensitive) || strFileName.endsWith(".avi", Qt::CaseInsensitive) ||
        strFileName.endsWith(".flv", Qt::CaseInsensitive) || strFileName.endsWith(".wmv", Qt::CaseInsensitive) ||
        strFileName.endsWith(".3gp", Qt::CaseInsensitive);
    if (!bSupportMovie) {
        return;
    }

    QFileInfo fileInfo(strFileName);
    // �����ļ��ظ������,����ļ��Ѿ�����˾͵����Ѿ���ӵ��ļ���λ��
    QList<QListWidgetItem *> listRepeatItem = ui->listWidget->findItems(fileInfo.fileName(), Qt::MatchExactly);
    QListWidgetItem *pItem = nullptr;
    if (listRepeatItem.isEmpty()) {
        pItem = new QListWidgetItem(ui->listWidget);
        pItem->setData(Qt::UserRole, QVariant(fileInfo.filePath()));  // �û����� TODO �û����浽���ģ�
        pItem->setText(fileInfo.fileName());                          // ��ʾ�ı�
        pItem->setToolTip(fileInfo.filePath());  // ������ʾ�������ͣʱ��ʾ���ı���Ϊ�ļ�������·����
        ui->listWidget->addItem(pItem);
    } else {
        pItem = listRepeatItem.at(0);
    }
}

void PlayList::OnAddFileAndPlay(QString strFileName) {
    // bool bSupportMovie = strFileName.endsWith(".mkv", Qt::CaseInsensitive) ||
    //     strFileName.endsWith(".rmvb", Qt::CaseInsensitive) ||
    //     strFileName.endsWith(".mp4", Qt::CaseInsensitive) ||
    //     strFileName.endsWith(".avi", Qt::CaseInsensitive) ||
    //     strFileName.endsWith(".flv", Qt::CaseInsensitive) ||
    //     strFileName.endsWith(".wmv", Qt::CaseInsensitive) ||
    //     strFileName.endsWith(".3gp", Qt::CaseInsensitive);
    // if (!bSupportMovie)
    //{
    //     return;
    // }

    // QFileInfo fileInfo(strFileName);

    // QList<QListWidgetItem *> listItem = ui->List->findItems(fileInfo.fileName(), Qt::MatchExactly);
    // QListWidgetItem *pItem = nullptr;
    // if (listItem.isEmpty())
    //{
    //     pItem = new QListWidgetItem(ui->List);
    //     pItem->setData(Qt::UserRole, QVariant(fileInfo.filePath()));  // �û�����
    //     pItem->setText(fileInfo.fileName());  // ��ʾ�ı�
    //     pItem->setToolTip(fileInfo.filePath());
    //     ui->List->addItem(pItem);
    // }
    // else
    //{
    //     pItem = listItem.at(0);
    // }
    // on_List_itemDoubleClicked(pItem);
}

void PlayList::RemoveFile() {
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}
