#include "widget.h"
#include "ui_widget.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>
#include "datetimeutil.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("client");

    client = new QTcpSocket(this);
    this->initClient();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initClient()
{
    connect(client, &QTcpSocket::readyRead, this, [&](){
          QByteArray data =  client->readAll();
          qDebug() << data;
          QHostAddress host = client->peerAddress();
          QString tm = DateTimeUtil::getCurTimeStr();
          ui->textBrowser->append(QString(host.toString()) + tm + ":" + QString(data));
    });
    connect(client, &QTcpSocket::connected, this, [&](){
          qDebug() << u8"连接成功";
          ui->pushButton->setText(QString(u8"已连接"));
          QHostAddress host = client->peerAddress();
          QString tm = DateTimeUtil::getCurTimeStr();
          ui->textBrowser->append(QString(host.toString()) + tm + ":" + QString(u8"已连接"));



    });
    connect(client, &QTcpSocket::disconnected, this, [&](){
          qDebug() << u8"断开";
          ui->pushButton->setText(QString(u8"连接"));
          QHostAddress host = client->peerAddress();
          QString tm = DateTimeUtil::getCurTimeStr();
          ui->textBrowser->append(QString(host.toString()) + tm + ":" + u8"断开连接");

    });
}

void Widget::on_pushButton_clicked()
{

    if(client->state() == QTcpSocket::ConnectedState){
           client->close();
    }else{
        QString host = "127.0.0.1";
        quint16 port = 9988;
        client->connectToHost(host, port);
    }

}

void Widget::on_pushButton_3_clicked()
{
    if(client->state() != QTcpSocket::ConnectedState) return;
    QString str = ui->textEdit->toPlainText();
    client->write(str.toUtf8(), str.toUtf8().size());
    ui->textEdit->clear();
}


void Widget::on_pushButton_2_clicked()
{
    client->close();
}

