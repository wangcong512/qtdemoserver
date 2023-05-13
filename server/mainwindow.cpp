#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTime>
#include "datetimeutil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");

    itemModel = new QStandardItemModel(this);

//    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    // tcp server

    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::onNewConnect);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qDebug() << "click" << endl;
    ui->pushButton->setDisabled(false);
    QHostAddress host = QHostAddress("127.0.0.1");
    quint16 port = 9988;
    if(tcpServer->isListening()){
        tcpServer->close();
        ui->pushButton->setText(u8"开始");
    }else{
        bool bStart = tcpServer->listen(host, port);
        if(bStart){
            ui->pushButton->setText(u8"已启动");
            qDebug() << QString("server started: ip %1 port: %2").arg(host.toString()).arg(port);
        }
    }

}

void MainWindow::onNewConnect()
{
    QTcpSocket *s = tcpServer->nextPendingConnection();
    if(s){
       if(sockets.contains(s)) {
           sockets.removeOne(s);
           sockets.append(s);
       }else{
           sockets.append(s);
       }

      QHostAddress host =  s->peerAddress();
      quint16 port = s->peerPort();
      qDebug()<< "host:" << host.toString() << "port:" << port;
      QString tm = DateTimeUtil::getCurTimeStr();
      ui->textBrowser->append(host.toString() + tm + ":" + QString(u8"加入连接"));

      connect(s, &QTcpSocket::readyRead, this, [=](){
          QByteArray buf  = s->readAll();
          qDebug() << "revDat:" << QString(buf) << u8"字节数：" << buf.size();
          QString curTime = DateTimeUtil::getCurTimeStr();
          QString msg = host.toString() + curTime + ":" + QString(buf);
          ui->textBrowser->append(msg);
          this->broadcastMsg(msg.toUtf8());
      });
      connect(s, &QTcpSocket::connected, this, [=](){
          QHostAddress host =  s->peerAddress();
       });
      connect(s, &QTcpSocket::disconnected, this, [=](){
          sockets.removeOne(s);
          QHostAddress host =  s->peerAddress();
          QString curTime = DateTimeUtil::getCurTimeStr();
          ui->textBrowser->append(host.toString() + curTime +":" + QString(u8"离开连接"));
          this->refreshListUI();
      });

      this->refreshListUI();
    }
    qDebug() << "click" << endl;
}

void MainWindow::broadcastMsg(QByteArray msg)
{
    foreach(QTcpSocket* s, sockets){
        s->write(msg, msg.length());
        qDebug() << "broadcast:" << QString(msg);
    }

}

void MainWindow::refreshListUI()
{
    QStringList sl;
    int len = itemModel->rowCount();
    for (int i = 0; i < len; i++) {
        QStandardItem *item = itemModel->item(i);
        delete item;
    }
    itemModel->clear();
    foreach(QTcpSocket* item,sockets){
        QString name = item->peerAddress().toString();
        QStandardItem *row = new QStandardItem(name);
        itemModel->appendRow(row);
    }

    ui->listView->setModel(itemModel);

}


void MainWindow::on_pushButton_2_clicked()
{
    qApp->quit();
}

