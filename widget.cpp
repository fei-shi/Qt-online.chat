#include "widget.h"
#include "ui_widget.h"
#include "QDebug"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Client");
    this->cli=nullptr;
    connect(ui->pushButton,&QPushButton::clicked,[&](){
        if(cli==nullptr){ui->textEdit_2->append("error,maybe not connected");return;}
        QString ttem=ui->textEdit->toPlainText();
        if(QTcpSocket::ConnectedState==cli->state())
        {
            if(ttem.size()!=0)
            {
                cli->write(ttem.toUtf8().data());
                ui->textEdit_2->append("client: \r"+ttem);ui->textEdit->clear();
            }
        }
        else ui->textEdit_2->append("error,maybe not connected");
    });
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        if(!cli)return;
       if(QTcpSocket::ConnectedState==cli->state())
       {
           cli->disconnectFromHost();
           ui->textEdit_2->append("had dicsonnected from server.");
       }
       else return;
    });
    connect(ui->pushButton_3,&QPushButton::clicked,[&](){
        this->cli=new QTcpSocket(this);
        cli->connectToHost(QHostAddress(ui->lineEdit->text()),ui->lineEdit_2->text().toInt());
        connect(cli,&QTcpSocket::connected,[&](){
          if(QTcpSocket::ConnectedState==cli->state())ui->textEdit_2->append("server has been connected");
          else ui->textEdit_2->append("error");
       });
        //connect(cli,&QTcpSocket::error)
        connect(cli,&QTcpSocket::readyRead,[&](){
           ui->textEdit_2->append(cli->readAll().data());
        });
    });
}

Widget::~Widget()
{
    delete ui;
}
