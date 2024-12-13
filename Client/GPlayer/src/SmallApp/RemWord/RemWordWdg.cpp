#include "RemWordWdg.h"

RemWordWdg::RemWordWdg(QWidget *parent)
    : QWidget(parent)
{
    ui->setupUi(this);
    vlayout = new QVBoxLayout(this);
    ui->widget_2->setLayout(vlayout);
    
}

void RemWordWdg::slotOpenToolWdg(void) {
    RemWordToolWdg* remWordToolWdg = new RemWordToolWdg();


    vlayout->addWidget(remWordToolWdg);
}


RemWordWdg::~RemWordWdg()
{}
