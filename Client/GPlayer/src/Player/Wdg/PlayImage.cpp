#include "PlayImage.h"

PlayImage::PlayImage(QWidget *parent)
    : QWidget(parent) {
    // 适用调色板设置背景色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::black);  // 设置背景黑色
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

// 传入Qimage图片显示
void PlayImage::updateImage(const QImage &image) {
    m_mutex.lock();
    m_pixmap = QPixmap::fromImage(image);
    m_mutex.unlock();
    update();
}

// 使用Qpainter显示图片
void PlayImage::paintEvent(QPaintEvent *event) {
    if (!m_pixmap.isNull()) {
        QPainter painter(this);

        // 经过粗略测试，QImage先缩放后转为QPixmap的方式在图像比较小时耗时少，图片越大耗时越大
        // QPixmap pixmap = QPixmap::fromImage(m_image.scaled(this->size(), Qt::KeepAspectRatio));
        // 先将QImage转换为QPixmap再进行缩放则耗时比较少，并且稳定，不会因为缩放图片大小而产生太大影响
        // QPixmap pixmap1 = QPixmap::fromImage(m_image).scaled(this->size(), Qt::KeepAspectRatio);

        m_mutex.lock();
        QPixmap pixmap = m_pixmap.scaled(this->size(), Qt::KeepAspectRatio);
        m_mutex.unlock();
        int x = (this->width() - pixmap.width()) / 2;
        int y = (this->height() - pixmap.height()) / 2;
        painter.drawPixmap(x, y, pixmap);
    }
    QWidget::paintEvent(event);
}
