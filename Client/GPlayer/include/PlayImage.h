#ifndef PLAYIMAGE_H
#define PLAYIMAGE_H

#include <qmutex.h>
#include <QWidget>

class PlayImage : public QWidget {
    Q_OBJECT
public:
    explicit PlayImage(QWidget *parent = nullptr);

    void updateImage(const QImage &image);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pixmap;
    QMutex m_mutex;
};

#endif  // PLAYIMAGE_H
