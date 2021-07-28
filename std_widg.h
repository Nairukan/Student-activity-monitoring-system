#ifndef STD_WIDG_H
#define STD_WIDG_H

#include <QWidget>
#include <vector>

namespace Ui {
class STD_WIDG;
}


class STD_WIDG : public QWidget
{
    Q_OBJECT

public:
    explicit STD_WIDG(QWidget *parent = nullptr);
    ~STD_WIDG();
    void paintLines(std::vector<QLine> l);
    void SetF(QFont F);
    void paintEvent(QPaintEvent* event);
    std::vector<QLine> LIN;
    QFont F;
private:
    Ui::STD_WIDG *ui;
};

#endif // STD_WIDG_H
