#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QListWidgetItem>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileDialog>
#include "ui_PlayList.h"


class PlayList : public QWidget
{
    Q_OBJECT

public:
    explicit PlayList(QWidget *parent = 0);
    ~PlayList();

    //获取播放列表状态
    bool GetPlaylistStatus();
public:
    // 添加文件
    void OnAddFile(QString strFileName);
    // 添加文件并播放
    void OnAddFileAndPlay(QString strFileName);

    void OnBackwardPlay();
    void OnForwardPlay();

    // 在这里定义dock的初始大小
    QSize sizeHint() const
    {
        return QSize(150, 900);
    }
protected:
    // 放下事件
    void dropEvent(QDropEvent *event);
    // 拖动事件
    void dragEnterEvent(QDragEnterEvent *event);

signals:
    void sigUpdateUi();	// 界面排布更新
    void sigPlay(QString strFile); // 播放文件

private:
    void InitUi();
    void InitConnect();

public slots:
    void playClickedItem(QListWidgetItem *item);
    void AddFile();
    void RemoveFile();

private:
    Ui::PlayList *ui;

    int mCurrentPlayListIndex;
};

#endif // PLAYLIST_H
