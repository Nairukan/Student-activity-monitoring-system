#include "std_widg.h"
#include "ui_std_widg.h"
#include <QPainter>

STD_WIDG::STD_WIDG(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::STD_WIDG)
{
    ui->setupUi(this);
}

void STD_WIDG::paintLines(std::vector<QLine> l){
    LIN.clear();
    LIN=l;

}
void STD_WIDG::SetF(QFont F){
    this->F=F;
}

void STD_WIDG::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.setPen(Qt::darkGray);
    for (int i=0; i<LIN.size(); i++)
        p.drawLine(LIN[i].x1(), LIN[i].y1(), LIN[i].x2(), LIN[i].y2());
    p.setPen(Qt::black);
    if (LIN.size()>0){
        p.setFont(F);
        p.drawText(QPoint(0.015*this->width(), 0.037*this->parentWidget()->parentWidget()->height()), "№");
        p.drawText(QPoint(0.062*this->width(), 0.037*this->parentWidget()->parentWidget()->height()), "ФИО студентов");
        p.drawText(QPoint(0.45*this->width(), 0.037*this->parentWidget()->parentWidget()->height()), "Кнопочные меню");
    }
    for (int i=3; i<LIN.size(); i++){
        p.drawText(QPoint(0.015*this->width(), LIN[i-3].y1()+0.052*this->parentWidget()->parentWidget()->height()), QString::number(i-2)+".");
    }
}

STD_WIDG::~STD_WIDG()
{
    delete ui;
}
