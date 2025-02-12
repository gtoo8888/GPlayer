#include "About.h"

About::About(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::About) {
    ui->setupUi(this);

    connect(ui->ClosePushButton, &QPushButton::clicked, this, &About::closeCurrentWindow);
}

void About::closeCurrentWindow(void) {
    this->close();
}

About::~About() {
    delete ui;
}
