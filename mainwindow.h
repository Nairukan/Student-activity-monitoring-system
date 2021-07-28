#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <vector>
#include <QLabel>
#include <QPushButton>
#include <QCursor>
#include <QMouseEvent>
#include <QWindow>
#include <QLineEdit>
#include <QPainter>
#include <vector>
#include <QMessageBox>
#include <QTextStream>
#include <QTextDecoder>
#include <QFileDialog>
#include "std_widg.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct student{
    QLabel* Info; //Указатель на объект "Надпись" где хранится ФИО
    QLabel* Buttons[3]; //Указатели на объекты "надпись" которые используются как кнопки для
  //Пополнения баллов в соответсвующие активности
    QLineEdit* Balls[3]; //Указатели на объекты "Поле для ввода" которые используются
  //для отображения баллов в соответствующих активностях
    student(QWidget *parent = nullptr){ //Конструктор, необязательный параметр - родительское окно,
  //К которому будут привзяваться элементы структуры
        Info=new QLabel(parent); //Выделяем пямять под компонент типа "Надпись" и привязываем его к окну
        Info->setText("Name1 name2 name3"); //Устанавливаем его текст
        Info->setAlignment(Qt::AlignVCenter); //Устанавливаем выравнивание текста
        for (int i=0; i<3; i++){ //Проходимся по видам активности
            Buttons[i]=new QLabel(parent); //Выделяем пямять под компонент типа "Надпись" и привязываем его к окну
            Buttons[i]->setStyleSheet("QLabel { background-color : lightGray; color : black; }");
            Balls[i]=new QLineEdit(parent);
            Balls[i]->setText("0");
            Balls[i]->setEnabled(false); //Устанавливаем показатель доступности "нельзя изменить пользователю"

            Buttons[i]->setAlignment(Qt::AlignCenter);
            Balls[i]->setAlignment(Qt::AlignCenter);
        }
        Buttons[0]->setText("Добавить посещение");
        Buttons[1]->setText("Отметить работу");
        Buttons[2]->setText("Отметить \"творчество\"");
    }
    ~student(){
        delete Info; //Освобождаем выделенную память для избежания утечки
        for (int i=0; i<3; i++){
            delete Buttons[i];
            delete Balls[i];
        }
    }

    void show(){
        Info->show(); //Проявляем компонент на родительском окне
        for (int i=0; i<3; i++){
            Buttons[i]->show();
            Balls[i]->show();
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, QRect screen, QString role, MainWindow* connected);
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);
    void paintEvent(QPaintEvent* event);
    void UPDATE_POS();
    void GetFont();
    ~MainWindow();
    std::vector <student*> ST;
    int t[3];
    QRect screen;
    QWidget* ST_WIDG=nullptr;
    QFont F;
    STD_WIDG* ST_TABLE=nullptr;
    QString Role_window, openAdress="";
    MainWindow *Main, *Edit_weight, *Edit_stud;
    int Hind;
    std::vector<QPushButton*> B_Save;
    std::vector<QLineEdit*> L_Save;
    std::vector<QLabel*> Lab_Save;
    int ST_ind;
    QPushButton* ButtonAdd, *OpenGroup, *CreateGroup;
private slots:
    void Save_click();
    void Delete_click();
    void Add_click();
    void OpenGroup_click();
    void CreateGroup_click();
    void on_action_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
