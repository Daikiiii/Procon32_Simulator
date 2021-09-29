#include <QDebug>
#include <QMessageBox>
#include <QPixmap>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_proc(this)
{
    ui->setupUi(this);


    // スロット設定
        connect(&m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(ProcOutput()));
        connect(&m_proc, SIGNAL(readyReadStandardError()), this, SLOT(ProcError()));
        connect(&m_proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(ProcFinished(int,QProcess::ExitStatus)));

        // pythonを別プロセスで実行
        QString pythonCodePath = "C:/Procon32_Simulator/create_puzzle";	// 注：自分の環境に合わせて書き換える
        QStringList args;
        args << pythonCodePath;
        m_proc.start("python", args);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ProcOutput()
{
    QByteArray output = m_proc.readAllStandardOutput();
    QString str = QString::fromLocal8Bit(output);

    ui->textBrowser->setText(str);
}

void MainWindow::ProcError()
{
    QByteArray output = m_proc.readAllStandardError();
    QString str = QString::fromLocal8Bit(output);

    ui->textBrowser->setText(str);
}

void MainWindow::ProcFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if( exitStatus == QProcess::CrashExit )
    {
        QMessageBox::warning(this, "Error", "Crashed");
    }
    else if( exitCode != 0 )
    {
        QMessageBox::warning(this, "Error", "Failed");
    }
    else
    {
        // 成功時の処理はここに入れる
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->setText(QString::fromLocal8Bit("test"));

        // pythonを別プロセスで実行
        QString pythonCodePath = "C:/Procon32_Simulator/create_puzzle";	// 注：自分の環境に合わせて書き換える
        QStringList args;
        args << pythonCodePath;
        m_proc.start("python", args);


    //表示する画像のアドレスをsに代入
    QString s="C:/Procon32_Simulator/puzzle_image/puzzle.jpg";
    //Qpixmapのaにいれる
    QPixmap a(s);
    //縦横調整
    int w = ui->label->width();
    int h = ui->label->height();
    //labelに表示
    ui->label->setPixmap(a.scaled(w,h,Qt::KeepAspectRatio));
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser->setText(QString::fromLocal8Bit("test message"));


    // pythonを別プロセスで実行
    QString pythonCodePath = "C:/Procon32_Simulator/puzzle_solver/puzzle_solver.py";	// 注：自分の環境に合わせて書き換える
    QStringList args;
    args << pythonCodePath;
    m_proc.start("python", args);

    ui->textBrowser->setText(QString::fromLocal8Bit("fin"));
}


void MainWindow::on_pushButton_4_clicked()
{

    // pythonを別プロセスで実行
    QString pythonCodePath = "C:/Procon32_Simulator/puzzle_solver/puzzle_solver.py";	// 注：自分の環境に合わせて書き換える
    QStringList args;
    args << pythonCodePath;
    m_proc.start("python", args);
}

