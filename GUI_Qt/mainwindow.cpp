#include <QDebug>
#include <QMessageBox>
#include <QPixmap>


#include <iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<map>
#include<time.h>
#include<cassert>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

int gCount = 0;
multimap<int, pair<int, int>> node;
#define be_size 30

char rot[257];//回転情報の保存

class set {
public:
    queue<int> moved;
    vector<int> board;
    int cost;
    int val;
    int selectable;
    int n_select;
    int recent;

    set& operator =(const set& temp) {
        try {
            moved = temp.moved;
            board = temp.board;
            cost = temp.cost;
            val = temp.val;
            selectable = temp.selectable;
            n_select = temp.n_select;
            recent = temp.recent;
            return *this;
        }
        catch (...) {
            //assert(false);
        }
    }
};

struct Beam {
    int width; //横
    int height;  //縦
    int s_rate, c_rate;
    vector<vector<int>> relate; //それぞれの座標に対する隣接リスト

    set predata[be_size];
    set nextdata[be_size];



    void be_search(vector<int> board, queue<int>moved, int cost, int selectable, int n_select, int recent, int n_val, int pre_root) {    //n_selectは現在選択しているピースの座標

        int valu;
        int c;  //交換対象の座標
        int i, j;

        /*if (n_val == 0&&gCount==0) {
            gCount++;
            be_finished(moved);
            //cout << "cost=" << cost << endl;
        }*/
        //else {
        queue<int>n_moved;
        /*選択操作を行う*/
        if (selectable > 1) {
            for (i = 0; i < width * height; i++) {
                if (i != board.at(i) && i != n_select) {
                    for (j = 0; j < 4; j++) {
                        if (relate.at(i).at(j) != -1) {
                            c = relate.at(i).at(j);
                            valu = n_val - (piece_val(c, board.at(c)) + piece_val(i, board.at(i))) + (piece_val(c, board.at(i)) + piece_val(i, board.at(c)));
                            node.emplace(valu, make_pair((i + 1) * 1000 + j, pre_root));
                        }
                    }
                }
            }
        }
        if (recent != -1) {
            for (i = 0; i < 4; i++) {
                if (abs(i - recent) == 2 || relate.at(n_select).at(i) == -1) continue;
                c = relate.at(n_select).at(i);
                valu = n_val - (piece_val(c, board.at(c)) + piece_val(n_select, board.at(n_select))) + (piece_val(c, board.at(n_select)) + piece_val(n_select, board.at(c)));
                node.emplace(valu, make_pair(i, pre_root));
            }
        }
        //}
    }
    void be_finished(queue<int> moved) {    //探索成功
        int s_count = 0; //選択回数のカウンター
        queue<int> s_rec;    //選択座標の記録
        int count;  //交換回数のカウンター
        queue<int> c_count; //交換回数のカウントの記録
        queue<int> c_rec;  //交換方向の記録

        FILE* outfile;
        outfile = fopen("output.txt", "w");
        fprintf(outfile, "%s\n", rot);


        while (moved.empty() == false) {
            s_count++;
            s_rec.push(((moved.front() - 1000) / width) + ((moved.front() - 1000) % width) * 16);
            moved.pop();
            count = 0;
            while (moved.empty() == false) {
                if (moved.front() > 4) break;   //選択操作ならbreak
                count++;
                c_rec.push(moved.front());
                moved.pop();
            }
            c_count.push(count);
        }
        fprintf(outfile, "%d\n", s_count); //cout << s_count << endl;
        while (c_rec.empty() == false) {
            if (c_count.front() == 0) {
                s_rec.pop();
                c_count.pop();
                continue;
            }
            fprintf(outfile, "%02x\n", s_rec.front()); //printf("%02x\n", s_rec.front());
            s_rec.pop();
            fprintf(outfile, "%d\n", c_count.front()); //cout << c_count.front() << endl;
            for (int i = 0; i < c_count.front(); i++) {
                switch (c_rec.front()) {
                case 0:
                    fprintf(outfile, "U"); //cout << 'U';
                    break;
                case 1:
                    fprintf(outfile, "R"); //cout << 'R';
                    break;
                case 2:
                    fprintf(outfile, "D"); //cout << 'D';
                    break;
                case 3:
                    fprintf(outfile, "L"); //cout << 'L';
                    break;
                }
                c_rec.pop();
            }
            fprintf(outfile, "\n"); //cout << endl;
            c_count.pop();
        }
        fclose(outfile);
    }
    int piece_val(int np, int cp) {  //np->現在位置cp->正解位置
        int cx = cp % width;
        int cy = cp / width;
        int nx = np % width;
        int ny = np / width;
        int val = min(abs(nx - cx), width - abs(nx - cx)) + min(abs(ny - cy), height - abs(ny - cy));
        val *= c_rate;
        return val;
    }
    Beam(vector<vector<int>> rel, int w, int h, int sr, int cr) {
        relate = rel;
        width = w;
        height = h;
        s_rate = sr;
        c_rate = cr;
    }
};

void inputdata() {
    int width, height;
    int selectable;
    int s_rate, c_rate;

    FILE* datafile;

    datafile = fopen("C:/Procon32_Simulator/puzzle_solver/puzzle_text/puzzle_solution.txt", "r");

    (void)fscanf(datafile, "%d %d", &width, &height);

    vector<int> board(width * height);

    (void)fscanf(datafile, "%d", &selectable);
    (void)fscanf(datafile, "%d %d", &s_rate, &c_rate);
    for (int i = 0; i < width * height; i++) {
        (void)fscanf(datafile, "%x", &board[i]);
    }
    (void)fscanf(datafile, "%s", rot);

    fclose(datafile);


    FILE* infile;
    infile = fopen("input.txt", "w");
    fprintf(infile, "%d %d\n", width, height);
    fprintf(infile, "%d\n", selectable);
    fprintf(infile, "%d %d\n", s_rate, c_rate);
    for (int i = 0; i < width * height; i++) {
        fprintf(infile, "%x ", board[i]);
    }
    fprintf(infile, "\n");
    fclose(infile);
}

void outputdata() {
    FILE* outfile;
    FILE* answerfile;
    int temp;

    outfile = fopen("output.txt", "r");
    answerfile = fopen("C:/Procon32_Simulator/puzzle_solver/puzzle_text/solution.txt", "w");

    while ((temp = fgetc(outfile)) != EOF) {
        fprintf(answerfile, "%c", temp);
    }

    fclose(outfile);
    fclose(answerfile);
}

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
    cout<<"pusshedbutton"<<endl;
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
    QString pythonCodePath = "C:/Procon32_Simulator/procon32_download.py";	// 注：自分の環境に合わせて書き換える
    QStringList args;
    args << pythonCodePath;
    m_proc.start("python", args);
    cout<<"dawnload button pusshed"<<endl;
}


void MainWindow::on_pushButton_5_clicked()
{

    // pythonを別プロセスで実行
    QString pythonCodePath = "C:/Procon32_Simulator/procon32_submit.py";	// 注：自分の環境に合わせて書き換える
    QStringList args;
    args << pythonCodePath;
    m_proc.start("python", args);
        cout<<"submit button pusshed"<<endl;

}


void MainWindow::on_pushButton_3_clicked()
{
    inputdata();

        //clock_t start = clock();
        int width, height;
        int selectable;
        int s_rate, c_rate; //s_rateは選択コスト,c_rateは交換コスト

        FILE* infile;
        infile = fopen("input.txt", "r"); //パスを適宜変更してください
        (void)fscanf(infile, "%d %d", &width, &height); //cin >> width >> height;
        vector<int> board(width * height);
        (void)fscanf(infile, "%d", &selectable); //cin >> selectable;
        (void)fscanf(infile, "%d %d", &s_rate, &c_rate); //cin >> s_rate >> c_rate;
        int i, j, k;
        for (i = 0; i < width * height; i++) {
            (void)fscanf(infile, "%x", &board[i]); //(void)scanf("%x", &board[i]);
            board.at(i) = (int)(board.at(i) / 16) + (board.at(i) % 16) * width;

        }
        fclose(infile);

        vector<vector<int>> relate(width * height, vector<int>(4));

        //relateの書き込み
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                for (k = 0; k <= 3; k++) {
                    switch (k) {
                    case 0: //上方向の隣接
                        if (i != 0)
                            relate.at(i * width + j).at(0) = (i - 1) * width + j;
                        else
                            relate.at(j).at(0) = (height - 1) * width + j;
                        break;
                    case 1: //右方向
                        if (j != width - 1)
                            relate.at(i * width + j).at(1) = i * width + j + 1;
                        else
                            relate.at(i * width + j).at(1) = i * width;
                        break;
                    case 2: //下方向
                        if (height == 2) {   //ダブり防止
                            relate.at(i * width + j).at(2) = -1;
                            break;
                        }
                        if (i != height - 1)
                            relate.at(i * width + j).at(2) = (i + 1) * width + j;
                        else
                            relate.at(i * width + j).at(2) = j;
                        break;
                    case 3: //左方向
                        if (width == 2) {
                            relate.at(i * width + j).at(2) = -1;
                            break;
                        }
                        if (j != 0)
                            relate.at(i * width + j).at(3) = i * width + j - 1;
                        else
                            relate.at(i * width + j).at(3) = (i + 1) * width - 1;
                        break;
                    }
                }
            }
        }
        //relateの書き込み終了


        Beam problem(relate, width, height, s_rate, c_rate);



        int val = 0;
        for (i = 0; i < width * height; i++) {
            val += problem.piece_val(i, board.at(i));
        }
        //cout << "val=" << val << endl;
        int cost = 0;
        queue<int> moved;//駒の操作を管理するqueue

        problem.predata[0].moved = moved;
        problem.predata[0].board = board;
        problem.predata[0].cost = 0;
        problem.predata[0].val = val;
        problem.predata[0].selectable = selectable;
        problem.predata[0].n_select = -1;
        problem.predata[0].recent = -1;


        problem.be_search(board, moved, cost, selectable, -1, -1, val, 0);



        int node_count;
        int depth = 0;

        while (gCount == 0) {
            int dir, sel, root, c;
            node_count = 0;
            for (auto itr = node.begin(); itr != node.end(); ++itr) {
                root = itr->second.second;  //  遷移元の指定
                if (itr->second.first >= 1000 || depth == 0) {      //選択+交換操作
                    dir = itr->second.first % 1000;
                    sel = itr->second.first;


                    sel -= dir;
                    sel = (sel / 1000) - 1;


                    c = relate.at(sel).at(dir);
                    problem.nextdata[node_count].moved = problem.predata[root].moved;
                    problem.nextdata[node_count].moved.push(sel + 1000);
                    problem.nextdata[node_count].moved.push(dir);
                    swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));
                    problem.nextdata[node_count].board = problem.predata[root].board;
                    swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));

                    problem.nextdata[node_count].cost = problem.predata[root].cost + s_rate + c_rate;
                    problem.nextdata[node_count].val = itr->first;
                    problem.nextdata[node_count].selectable = problem.predata[root].selectable - 1;
                    problem.nextdata[node_count].n_select = c;
                    problem.nextdata[node_count].recent = dir;
                }
                else {                                          //交換操作
                    dir = itr->second.first;
                    sel = problem.predata[root].n_select;

                    c = relate.at(sel).at(dir);
                    problem.nextdata[node_count].moved = problem.predata[root].moved;
                    problem.nextdata[node_count].moved.push(dir);
                    swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));
                    problem.nextdata[node_count].board = problem.predata[root].board;
                    swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));   //戻す

                    problem.nextdata[node_count].cost = problem.predata[root].cost + c_rate;
                    problem.nextdata[node_count].val = itr->first;
                    problem.nextdata[node_count].selectable = problem.predata[root].selectable;
                    problem.nextdata[node_count].n_select = c;
                    problem.nextdata[node_count].recent = dir;
                }
                node_count++;
                if (node_count == be_size) break;
            }
            depth++;

            for (i = 0; i < node_count; i++) {
                problem.predata[i] = problem.nextdata[i];
            }

            for (i = 0; i < node_count; i++) {
                if (gCount == 0 && problem.predata[i].val == 0) {
                    problem.be_finished(problem.predata[i].moved);
                    gCount++;
                    break;
                }
                problem.be_search(problem.predata[i].board, problem.predata[i].moved, problem.predata[i].cost, problem.predata[i].selectable, problem.predata[i].n_select, problem.predata[i].recent, problem.predata[i].val, i);
            }
        }
        outputdata();
        //cout <<"depth="<< depth<<endl;
        //clock_t end = clock();
        //cout << "time=" << ((double)end - start) / 1000 << endl;

        cout<<"sorted puzzle"<<endl;

}
