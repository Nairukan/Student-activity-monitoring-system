#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <QFontMetrics>

int str_to_int(QString x){
    int ans=0;
    bool Neg=false;
    int i=0;
    if (x[0]=='-'){ i++; Neg=true;}
    for (; i<x.length(); i++){
        ans*=10;
        ans+=(x[i].digitValue());
    }
    if (Neg) ans*=-1;
    return ans;
}

void Func(int &PS, QString Text, int wid, int hei){ //Функция уменьшающая шрифт если он не влазит в некоторые размеры
    for (;; ){ //OpenGroup and CreateGroup button
        QFont f("Arial", PS, 10);
        QFontMetrics fm(f);
        if (wid-3>fm.width("Открыть список группы") && wid-3>fm.width("Создать список группы") && hei>=fm.height()) break;
        PS--;
    }
}

void MainWindow::GetFont(){ //Функуция которая прогоняет основные текста элементов для получения оптимального шрифта
    int PS=30;
    Func(PS, "Открыть список группы", 0.2*this->width(), 0.05*this->height()); //OpenGroup check textSize
    Func(PS, "Создать список группы", 0.2*this->width(), 0.05*this->height()); //CreateGroup check textSize
    Func(PS, "Добавить посещение", 0.166*this->width(), 0.051*this->height()); //Butt[0] check textSize
    Func(PS, "Отметить работу", 0.166*this->width(), 0.051*this->height()); //Butt[0] check textSize
    Func(PS, "Отметить \"творчество\"", 0.166*this->width(), 0.051*this->height()); //Butt[0] check textSize
    this->F=QFont("Arial", PS, 1);
}

MainWindow::MainWindow(QWidget *parent, QRect screen, QString flag, MainWindow* connected) //Конструктор класса главного окна,
//принемает указатель на родителя, Прямоугольник экрана, Роль Окна, Экземпляр класса MainWindow, с которым связан текущий экземпляр
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->screen=screen;
    this->Role_window=flag;
    //...
    if (this->Role_window=="Main"){
        this->resize(0.41875*screen.width(), 0.564*screen.height());
        this->GetFont();
        //QMessageBox::critical(this, "", this->styleSheet());
        //this->setStyleSheet("background-color: rgb(250, 250, 250)");
        OpenGroup=new QPushButton(this);
        CreateGroup=new QPushButton(this);
        OpenGroup->setFont(F);
        OpenGroup->setText("Открыть список группы");
        CreateGroup->setFont(F);
        CreateGroup->setText("Создать список группы");
        connect(OpenGroup, SIGNAL(clicked()), this, SLOT(OpenGroup_click()));
        connect(CreateGroup, SIGNAL(clicked()), this, SLOT(CreateGroup_click()));
        ui->scrollArea->show();
        ui->scrollArea->setGeometry(0, this->height()*0.1, this->width(), this->height()*0.9);
        ST_WIDG=ui->scrollAreaWidgetContents;
        ST_TABLE=new STD_WIDG(ST_WIDG);
        ST_TABLE->SetF(F);
        ST_WIDG->setFixedSize(this->width(), this->height()*0.89);
        ST_TABLE->resize(ST_WIDG->size());
        //ST_WIDG->setStyleSheet("background-color: rgb(240, 240, 240)");
        QString path = QCoreApplication::applicationDirPath();
        QFile FileW;
        QTextCodec* defaultTextCodec = QTextCodec::codecForName("UTF-8");
        QTextDecoder *decoder = new QTextDecoder(defaultTextCodec);
        FileW.setFileName(path+"/Res.weight");
        if (FileW.open(QIODevice::ReadWrite)){
            QString buffer=defaultTextCodec->toUnicode(FileW.readAll());
            if (buffer.length()!=0){
                QStringList BALlist =buffer.split(QRegExp("\\s+"));
                t[0]=str_to_int(BALlist[0]);
                t[1]=str_to_int(BALlist[1]);
                t[2]=str_to_int(BALlist[2]);
            }
            FileW.close();
        }
        //QMessageBox::critical(this, "", QString::number(blist.size()));
        ButtonAdd=new QPushButton(ST_TABLE);
        ButtonAdd->setText("Добавить студента");
        ButtonAdd->hide();
        connect(ButtonAdd, SIGNAL(clicked()), this, SLOT(Add_click()));
        UPDATE_POS();
    }else if (Role_window=="Edit_weight"){
        Main=connected;
        this->setFixedSize(0.239*screen.width(), 0.139*screen.height());
        QPushButton * SaveButton=new QPushButton(this);
        B_Save.push_back(SaveButton);
        this->ui->scrollArea->hide();
        SaveButton->setGeometry(0.25*this->width(), this->height()*0.6, this->width()*0.5, this->height()*0.33);
        SaveButton->setText("Сохранить изменения");
        connect(SaveButton, SIGNAL(clicked()), this, SLOT(Save_click()));
        L_Save.push_back(new QLineEdit(this));
        L_Save.push_back(new QLineEdit(this));
        L_Save.push_back(new QLineEdit(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save.push_back(new QLabel(this));
        for (int i=0; i<3; i++){
            L_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.3*this->height(), 0.23*this->width(), 0.15*this->height());
            L_Save[i]->setText(QString::number(Main->t[i]));
            Lab_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.15*this->height(), 0.23*this->width(), 0.09*this->height());
        }
        Lab_Save[0]->setText("Баллы посещения");
        Lab_Save[1]->setText("Быллы практики");
        Lab_Save[2]->setText("Баллы творчества");
        Lab_Save[3]->setText("Изменение значения баллов за нажатие кнопок");
        Lab_Save[3]->setAlignment(Qt::AlignCenter);
    }
    else if (Role_window=="Edit_stud"){
        Main=connected;
        this->setFixedSize(0.239*screen.width(), 0.17*screen.height());
        QPushButton * SaveButton=new QPushButton(this);
        QPushButton * DeleteButton=new QPushButton(this);
        B_Save.push_back(SaveButton);
        B_Save.push_back(DeleteButton);
        this->ui->scrollArea->hide();
        SaveButton->setGeometry(0.1*this->width(), this->height()*0.7, this->width()*0.35, this->height()*0.25);
        SaveButton->setText("Сохранить изменения");
        DeleteButton->setGeometry(0.55*this->width(), this->height()*0.7, this->width()*0.35, this->height()*0.25);
        DeleteButton->setText("Удалить студента");
        DeleteButton->setStyleSheet("QPushButton { background-color : red; color : black; }");
        connect(SaveButton, SIGNAL(clicked()), this, SLOT(Save_click()));
        connect(DeleteButton, SIGNAL(clicked()), this, SLOT(Delete_click()));
        L_Save.push_back(new QLineEdit(this));
        L_Save.push_back(new QLineEdit(this));
        L_Save.push_back(new QLineEdit(this));
        L_Save.push_back(new QLineEdit(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save.push_back(new QLabel(this));
        Lab_Save[0]->setText("Баллы посещения");
        Lab_Save[1]->setText("Быллы практики");
        Lab_Save[2]->setText("Баллы творчества");
        Lab_Save[3]->setText("Изменение данных о студенте");
        Lab_Save[4]->setText("Фамилия Имя Отчество");
        Lab_Save[3]->setAlignment(Qt::AlignCenter);
    }
}

MainWindow::~MainWindow()
{
    for (int i=0; i<ST.size(); i++){
        delete ST[i];
    }
    for (int i=0; i<B_Save.size(); i++)
        delete B_Save[i];
    for (int i=0; i<L_Save.size(); i++)
        delete L_Save[i];
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event){//Функция которая вызывается при нажатии кнопки мыши в пределах окна,
    //Нужна для определения индекса студента, на элемент которого было нажатие
    if (this->Role_window=="Main"){
        char flag='N';
        int Fl_Ind=-1, H_ind;
        QPoint p=event->localPos().toPoint();
        p.setY(p.y()-ui->menuBar->height()-ST_WIDG->y()-ST_TABLE->y()-ui->scrollArea->y());
        if (ST.size()!=0 && ST[0]->Info!=nullptr){
            if (p.x()>ST[0]->Info->x() && p.x()<ST[0]->Info->x()+ST[0]->Info->width()){
                for (int i=0; i<ST.size(); i++){
                    if (p.y()>ST[i]->Info->y() && p.y()<ST[i]->Info->y()+ST[i]->Info->height()){
                        flag='I';
                        Fl_Ind=i;
                        break;
                    }
                }
            }else
            if (p.x()>ST[0]->Buttons[0]->x() && p.x()<ST[0]->Buttons[0]->x()+ST[0]->Buttons[0]->width()){
                for (int i=0; i<ST.size(); i++){
                    if (p.y()>ST[i]->Buttons[0]->y() && p.y()<ST[i]->Buttons[0]->y()+ST[i]->Buttons[0]->height()){
                        flag='B';
                        Fl_Ind=i;
                        H_ind=0;
                        break;
                    }else if (p.y()>ST[i]->Balls[0]->y() && p.y()<ST[i]->Balls[0]->y()+ST[i]->Balls[0]->height()){
                        flag='M';
                        Fl_Ind=i;
                        H_ind=0;
                        break;
                    }
                }
            }else
            if (p.x()>ST[0]->Buttons[1]->x() && p.x()<ST[0]->Buttons[1]->x()+ST[0]->Buttons[1]->width()){
                for (int i=0; i<ST.size(); i++){
                    if (p.y()>ST[i]->Buttons[1]->y() && p.y()<ST[i]->Buttons[1]->y()+ST[i]->Buttons[1]->height()){
                        flag='B';
                        Fl_Ind=i;
                        H_ind=1;
                        break;
                    }else if (p.y()>ST[i]->Balls[1]->y() && p.y()<ST[i]->Balls[1]->y()+ST[i]->Balls[1]->height()){
                        flag='M';
                        Fl_Ind=i;
                        H_ind=1;
                        break;
                    }
                }
            }else
            if (p.x()>ST[0]->Buttons[2]->x() && p.x()<ST[0]->Buttons[2]->x()+ST[0]->Buttons[2]->width()){
                for (int i=0; i<ST.size(); i++){
                    if (p.y()>ST[i]->Buttons[2]->y() && p.y()<ST[i]->Buttons[2]->y()+ST[i]->Buttons[2]->height()){
                        flag='B';
                        Fl_Ind=i;
                        H_ind=2;
                        break;
                    }else if (p.y()>ST[i]->Balls[2]->y() && p.y()<ST[i]->Balls[2]->y()+ST[i]->Balls[2]->height()){
                        flag='M';
                        Fl_Ind=i;
                        H_ind=2;
                        break;
                    }
                }
            }
        }
        if (flag=='I'){
            Edit_stud->ST_ind=Fl_Ind;
            Edit_stud->show();
            this->hide();
        }else if (flag=='B'){
            if (event->button()==Qt::LeftButton){
                QString temp=ST[Fl_Ind]->Balls[H_ind]->text();
                int x=str_to_int(temp);
                ST[Fl_Ind]->Balls[H_ind]->setText(QString::number(x+t[H_ind]));
            }else{
                Edit_weight->Hind=Fl_Ind;
                Edit_weight->show();
                this->hide();
            }
        }else if(flag=='M'){

        }
    }else if (Role_window=="Edit_weight"){

    }
}

short comp(QString a, QString b){ //Для сравнения двух строк в лексикографическом порядке
    int n=std::min(a.length(), b.length());
    for (int i=0; i<n; i++){
        if (a[i].toUpper()>b[i].toUpper()) return 1;
        if (a[i].toUpper()<b[i].toUpper()) return -1;
    }
    if (a.length()==b.length()) return 0;
    if (a.length()==n) return -1;
    return 1;
}

bool sortRule(student* a, student* b){ //правило для сортировки студентов по алфавиту
    QString as=a->Info->text(), bs=b->Info->text();
    QStringList asl =as.split(QRegExp("\\s+"));
    QStringList bsl =bs.split(QRegExp("\\s+"));
    if (asl.size()<bsl.size()) return false;
    if (bsl.size()<asl.size()) return true;
    for (int i=0; i<asl.size(); i++){
        short tj=comp(asl[i], bsl[i]);
        if (tj!=0)
            return tj-1;
    }
    return false;
}

void MainWindow::UPDATE_POS(){ //Разметка
    int topInfo=0.06*this->height(), topBut=0.05*this->height(), topBall=0.105*this->height(), tempH=0.105*this->height(), tempBut=0.01*this->width(), tempBall=0.01*this->width();
    int leftInfo=0.076*this->width(), leftBut=0.44*this->width(), leftBall=0.44*this->width(), widthInfo=0.364*this->width(), heightInfo=0.068*this->height(), widthBut=0.17*this->width(), heightBut=0.057*this->height(), widthBall=0.17*this->width(), heightBall=0.046*this->height();
    std::sort(ST.begin(), ST.end(), sortRule);
    for (int i=0; i<ST.size(); i++){
        ST[i]->Info->setFont(F);
        ST[i]->Info->setGeometry(leftInfo, topInfo, widthInfo, heightInfo);
        topInfo+=tempH;
        ST[i]->Buttons[0]->setFont(F);
        ST[i]->Balls[0]->setFont(F);
        ST[i]->Buttons[0]->setGeometry(leftBut, topBut, widthBut, heightBut);
        ST[i]->Balls[0]->setGeometry(leftBut, topBall, widthBut, heightBall);

        ST[i]->Buttons[1]->setFont(F);
        ST[i]->Balls[1]->setFont(F);
        ST[i]->Buttons[1]->setGeometry(leftBut+tempBut+widthBut, topBut, widthBut, heightBut);
        ST[i]->Balls[1]->setGeometry(leftBut+tempBut+widthBall, topBall, widthBut, heightBall);

        ST[i]->Buttons[2]->setFont(F);
        ST[i]->Balls[2]->setFont(F);
        ST[i]->Buttons[2]->setGeometry(leftBut+tempBut*2+widthBut*2, topBut, widthBut, heightBut);
        ST[i]->Balls[2]->setGeometry(leftBut+tempBut*2+widthBall*2, topBall, widthBut, heightBall);
        topBall+=tempH;
        topBut+=tempH;
    }
    ButtonAdd->setFont(F);
    if (ST.size()>0)
        ButtonAdd->setGeometry(ST[ST.size()-1]->Info->x(), ST[ST.size()-1]->Info->y()+0.1*this->height(), (leftBut+tempBut*2+widthBall*3-leftInfo), ST[ST.size()-1]->Info->height());
    else
        ButtonAdd->setGeometry(leftInfo, topInfo, (leftBut+tempBut*2+widthBall*3-leftInfo), heightInfo);
    ST_WIDG->setFixedSize(ui->scrollArea->width()-20, std::max(ButtonAdd->y()+ButtonAdd->height()+0.04*this->height(), this->height()*0.9-ui->menuBar->height()-3));
    ST_TABLE->resize(ST_WIDG->size());
    repaint();
    //INF_WIDG->setGeometry(0,0, ui->scrollAreaWidgetContents_2->width(), ui->scrollAreaWidgetContents_2->height());

    //ST[0]->Info->move(0,0);
    //ST[0]->Info->setText(QString::number(ST_WIDG->height()));
}

void MainWindow::resizeEvent(QResizeEvent *event){ //Разметка
    if (this->Role_window=="Main"){
        GetFont();
        if (ST_TABLE!=nullptr){
            ST_TABLE->SetF(F);
        }
        OpenGroup->setFont(F);
        OpenGroup->setGeometry(0.07*this->width(), 0.06*this->height(), 0.2*this->width(), 0.05*this->height());
        CreateGroup->setFont(F);
        CreateGroup->setGeometry(0.3*this->width(), 0.06*this->height(), 0.2*this->width(), 0.05*this->height());
        ui->scrollArea->setGeometry(0,this->height()*0.1, this->width(), this->height()*0.9-ui->menuBar->height());
        ST_TABLE->resize(ST_WIDG->size());
        UPDATE_POS();
    }else if (this->Role_window=="Edit_weight"){
        if (B_Save.size()!=0){
            B_Save[0]->setGeometry(0.25*this->width(), this->height()*0.6, this->width()*0.5, this->height()*0.33);
            for (int i=0; i<3; i++){
                L_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.4*this->height(), 0.23*this->width(), 0.15*this->height());
                Lab_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.25*this->height(), 0.23*this->width(), 0.09*this->height());
            }
            Lab_Save[3]->setGeometry(0.2*this->width(), 0.03*this->height(), 0.6*this->width(), 0.2*this->height());
        }
    }else if (this->Role_window=="Edit_stud"){
        if (B_Save.size()!=0){
            B_Save[0]->setGeometry(0.1*this->width(), this->height()*0.7, this->width()*0.35, this->height()*0.25);
            B_Save[1]->setGeometry(0.55*this->width(), this->height()*0.7, this->width()*0.35, this->height()*0.25);
            for (int i=0; i<3; i++){
                L_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.5*this->height(), 0.23*this->width(), 0.15*this->height());
                if (ST_ind>=0 && ST_ind<Main->ST.size()) L_Save[i]->setText(Main->ST[ST_ind]->Balls[i]->text());
                Lab_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.40*this->height(), 0.23*this->width(), 0.09*this->height());
            }
            Lab_Save[3]->setGeometry(0.2*this->width(), 0.01*this->height(), 0.6*this->width(), 0.15*this->height());
            L_Save[3]->setGeometry(0.07*this->width(), 0.17*this->height(), 0.55*this->width(), 0.15*this->height());
            L_Save[3]->setText(Main->ST[ST_ind]->Info->text());
            Lab_Save[4]->setGeometry(0.65*this->width(), 0.17*this->height(), 0.3*this->width(), 0.15*this->height());
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event){ //Рисование Линий
    //QPainter p(this)
    if (Role_window=="Main" && ST.size()>0){
        std::vector <QLine> l;
        l.clear();
        for (int i=0; i<=ST.size(); i++){
            l.push_back(QLine(5, 0.047*this->height()+(int)(0.105*this->height())*i, this->width()-5, 0.047*this->height()+(int)(0.105*this->height())*i));
        }
        l.push_back(QLine(0.047*this->width(), 5, 0.047*this->width(), ButtonAdd->y()-0.011*this->height()));
        l.push_back(QLine(0.435*this->width(), 5, 0.435*this->width(), ButtonAdd->y()-0.011*this->height()));
        ST_TABLE->paintLines(l);
    }
}

void MainWindow::closeEvent(QCloseEvent* event){ //Сохранение данных перед выходом
    if (Role_window=="Main" && openAdress!=""){
        QFile FileS;
        FileS.setFileName(openAdress);
        FileS.open(QIODevice::WriteOnly);
        FileS.write(QString::number(ST.size()).toUtf8());
        FileS.write("\n");
        for (int i=0; i<ST.size(); i++){
            FileS.write(ST[i]->Info->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[0]->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[1]->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[2]->text().toUtf8());
            FileS.write("\n");
        }
        FileS.close();
    }
    else if (Role_window=="Edit_weight" || Role_window=="Edit_stud"){
        Main->show();
        Main->UPDATE_POS();
    }
}

bool isNumber(QString a){
    int i=0;
    if (a[0]=='-') i++;
    for (; i<a.length(); i++){
        if (!(a[i].isNumber())) return false;
    }
    return true;
}

void MainWindow::Save_click(){ //Обновление информации при изменении весов или студента
    bool contin=true;
    for (int i=0; i<3; i++){
        if (!isNumber(L_Save[i]->text())){
            contin=false;
            L_Save[i]->setText("Wrong type data");
        }
    }
    if (Role_window=="Edit_weight"){
        if (contin){
            QFile FileW;
            FileW.setFileName(QCoreApplication::applicationDirPath()+"/Res.weight");
            FileW.open(QIODevice::WriteOnly);
            FileW.write(L_Save[0]->text().toUtf8());
            Main->t[0]=str_to_int(L_Save[0]->text());
            FileW.write(" ");
            FileW.write(L_Save[1]->text().toUtf8());
            Main->t[1]=str_to_int(L_Save[1]->text());
            FileW.write(" ");
            FileW.write(L_Save[2]->text().toUtf8());
            Main->t[2]=str_to_int(L_Save[2]->text());
            FileW.close();
            Main->show();
            this->hide();
        }
    }else
        if (Role_window=="Edit_stud"){
            QString tenp=L_Save[3]->text();
            QStringList tenpL=tenp.split(QRegExp("\\s+"));
            if (tenpL.size()==3){
                if (contin){

                    Main->ST[ST_ind]->Info->setText(tenp);
                    //QMessageBox::critical(this, "", "");
                    Main->ST[ST_ind]->Balls[0]->setText(L_Save[0]->text());
                    Main->ST[ST_ind]->Balls[1]->setText(L_Save[1]->text());
                    Main->ST[ST_ind]->Balls[2]->setText(L_Save[2]->text());
                    Main->show();
                    Main->UPDATE_POS();
                    this->hide();
                }
            }else{
                L_Save[3]->setText("Wrong type data");
            }
        }
}

void MainWindow::Delete_click(){ //Удаление некоторого студента из вектора
    std::vector<student*>::iterator it=Main->ST.begin()+ST_ind;
    student* delstud=Main->ST[ST_ind];
    delete delstud;
    Main->ST.erase(it);
    Main->show();
    Main->UPDATE_POS();
    this->hide();
}

void MainWindow::Add_click(){ //Добавление студента
    ST.push_back(new student(this->ST_TABLE));
    ST[ST.size()-1]->show();
    Edit_stud->ST_ind=ST.size()-1;
    Edit_stud->show();
    this->hide();
}

void MainWindow::showEvent(QShowEvent *event){ //Разметка
    if (Role_window=="Edit_stud"){
        B_Save[0]->setGeometry(0.1*this->width(), this->height()*0.7, this->width()*0.35, this->height()*0.25);
        B_Save[1]->setGeometry(0.55*this->width(), this->height()*0.7, this->width()*0.35, this->height()*0.25);
        for (int i=0; i<3; i++){
            L_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.5*this->height(), 0.23*this->width(), 0.15*this->height());
            L_Save[i]->setText(Main->ST[ST_ind]->Balls[i]->text());
            Lab_Save[i]->setGeometry((0.10+0.23*i+0.05*i)*this->width(), 0.40*this->height(), 0.23*this->width(), 0.09*this->height());
        }
        Lab_Save[3]->setGeometry(0.2*this->width(), 0.01*this->height(), 0.6*this->width(), 0.15*this->height());
        L_Save[3]->setGeometry(0.07*this->width(), 0.17*this->height(), 0.55*this->width(), 0.15*this->height());
        L_Save[3]->setText(Main->ST[ST_ind]->Info->text());
        Lab_Save[4]->setGeometry(0.65*this->width(), 0.17*this->height(), 0.3*this->width(), 0.15*this->height());
    }
}

void MainWindow::OpenGroup_click(){
    ButtonAdd->hide();
    if (openAdress!=""){ //Если есть необходимость сначала сохранить - сохраняем
        QFile FileS;
        FileS.setFileName(openAdress);
        FileS.open(QIODevice::WriteOnly);
        FileS.write(QString::number(ST.size()).toUtf8());
        FileS.write("\n");
        for (int i=0; i<ST.size(); i++){
            FileS.write(ST[i]->Info->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[0]->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[1]->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[2]->text().toUtf8());
            FileS.write("\n");
        }
        FileS.close();
        for (int i=0; i<ST.size(); i++){
            delete ST[i];
        }
        ST.clear();
        ST_TABLE->LIN.clear();
    }
    this->openAdress=QFileDialog::getOpenFileName(this, "Open Dialog", "", "*.stud"); //Считываение студентов из выбранного файла
    if (openAdress!=""){
        this->ButtonAdd->show();
        QFile FileS;
        FileS.setFileName(openAdress);
        FileS.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextCodec* defaultTextCodec = QTextCodec::codecForName("UTF-8");
        QTextDecoder *decoder = new QTextDecoder(defaultTextCodec);
        QString buffer=defaultTextCodec->toUnicode(FileS.readAll());
        QStringList blist =buffer.split(QRegExp("\\s+"));
        FileS.close();
        int i=0;
        for (; i<str_to_int(blist[0]); i++){
            student* s = new student(ST_TABLE);
            QString inf=blist[i*6+1]+" "+blist[i*6+2]+" "+blist[i*6+3];
            s->Info->setText(inf);
            s->Balls[0]->setText(blist[i*6+4]);
            s->Balls[1]->setText(blist[i*6+5]);
            s->Balls[2]->setText(blist[i*6+6]);
            s->show();
            ST.push_back(s);
        }

    }else
        QMessageBox::critical(this, "Ошибка", "Указан некорректный путь к файлу");
    UPDATE_POS();
}

void MainWindow::CreateGroup_click(){
    ButtonAdd->hide();
    if (openAdress!=""){ //Если есть необходимость сначала сохранить - сохраняем
        QFile FileS;
        FileS.setFileName(openAdress);
        FileS.open(QIODevice::WriteOnly);
        FileS.write(QString::number(ST.size()).toUtf8());
        FileS.write("\n");
        for (int i=0; i<ST.size(); i++){
            FileS.write(ST[i]->Info->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[0]->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[1]->text().toUtf8());
            FileS.write(" ");
            FileS.write(ST[i]->Balls[2]->text().toUtf8());
            FileS.write("\n");
        }
        FileS.close();
        for (int i=0; i<ST.size(); i++){
            delete ST[i];
        }
        ST.clear();
        ST_TABLE->LIN.clear();
    }
    this->openAdress=QFileDialog::getSaveFileName(this, "Save", "", "*.stud");
    if (openAdress!=""){//Создание новой группы
        this->ButtonAdd->show();
    }else
        QMessageBox::critical(this, "Ошибка", "Указан некорректный путь к файлу");
    UPDATE_POS();
}

void MainWindow::on_action_triggered()
{
    QString Text;
    Text+="Данная программа реализует возможность управления и просмотра активности студентов."; Text+="\n\n";
    Text+="Для начала работы необхожимо создать или открыть списки студентов, соответствующими кнопками в верхней части окна."; Text+="\n\n";
    Text+="Для добавление к списку нового студента - нажмите на кнопку \"добавить\""; Text+="\n\n";
    Text+="Для редактирования(удаление) информации о студенте - нажмите на ФИО студента"; Text+="\n\n";
    Text+="Для изменения активности студента - нажмите левой кнопкой мыши на соответвующую кнопку на одном уровне с ФИО студента"; Text+="\n\n";
    Text+="Для изменения кол-ва баллов за нажатие кнопок - нажмите правой кнопкой мыши на любую кнопку добавления баллов активности"; Text+="\n\n";
    QMessageBox::information(this, "Руководство пользователя", Text);
}
