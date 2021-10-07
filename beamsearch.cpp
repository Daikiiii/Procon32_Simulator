#include <iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<map>
#include<time.h>

using namespace std;

int gCount = 0;
multimap<int, pair<int,int>> node;
#define be_size 5

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
        moved = temp.moved;
        board = temp.board;
        cost = temp.cost;
        val = temp.val;
        selectable = temp.selectable;
        n_select = temp.n_select;
        recent = temp.recent;
        return *this;
    }
};

struct Beam {
    int width; //横
    int height;  //縦
    int s_rate, c_rate;
    vector<vector<int>> relate; //それぞれの座標に対する隣接リスト
    int be_wid;

    set predata[100];
    set nextdata[100];



    void be_search(vector<int> board, queue<int>moved, int cost, int selectable, int n_select, int recent,int n_val,int pre_root) {    //n_selectは現在選択しているピースの座標
        int c;  //交換対象の座標
        int i,j;

        if (n_val == 0) {

            gCount++;
            be_finished(moved);
            //cout << "cost=" << cost << endl;

        }
        else {
            queue<int>n_moved;
            /*もし直前の操作が選択ではなく、
            残り選択可能回数が0ではない
            なら選択操作を行う*/
            if (recent < 1000 && selectable != 0) {
                for (i = 0; i < width * height; i++) {
                    if (i == board.at(i)||i==n_select) continue;
                    for (j = 0; j < 4; j++) {
                        if (relate.at(i).at(j) == -1) continue;
                        c = relate.at(i).at(j);
                        n_val = n_val - (piece_val(c, board.at(c)) + piece_val(i, board.at(i))) + (piece_val(c, board.at(i)) + piece_val(i, board.at(c)));
                        node.emplace(n_val,make_pair( (i + 1) * 1000 + j,pre_root));
                    }
                }
            }
            if(recent!=-1){
                for (i = 0; i < 4; i++) {
                    if (abs(i - recent) == 2 || relate.at(n_select).at(i) == -1) continue;
                    c = relate.at(n_select).at(i);
                    n_val = n_val - (piece_val(c, board.at(c)) + piece_val(n_select, board.at(n_select))) + (piece_val(c, board.at(n_select)) + piece_val(n_select, board.at(c)));
                    node.emplace(n_val, make_pair(i, pre_root));
                }
            }
            /*for (auto itr = node.begin(); itr != node.end(); ++itr) {
                if (itr->second.first >= 1000) {
                    dir = itr->second.first % 1000;
                    sel = itr->second.first;
                    sel -= dir;
                    sel = (sel / 1000) - 1;
                    n_cost = cost + s_rate + c_rate;
                    c = relate.at(sel).at(dir);
                    n_moved = moved;
                    n_moved.push(sel + 1000);
                    n_moved.push(dir);
                    swap(board.at(sel), board.at(c));
                    //be_search(board, n_moved,n_cost, selectable-1, c, n_val, dir);
                }
            }*/

            /*if (recent >= 1000 || selectable == 0 || n_select != board.at(n_select)) {
                n_cost = cost + c_rate;
                for (i = 0; i < 4; i++) {

                    if (abs(i - recent) == 2 || relate.at(n_select).at(i) == -1) continue;
                    c = relate.at(n_select).at(i);
                    n_val = val - (piece_val(c, board.at(c)) + piece_val(n_select, board.at(n_select))) + (piece_val(c, board.at(n_select)) + piece_val(n_select, board.at(c)));
                    node.emplace(n_val, i);
                    n_moved = moved;
                    swap(board.at(n_select), board.at(c));
                    n_moved.push(i);
                    if (n_val + n_cost <= limit)
                        be_search(board, n_moved, limit, n_cost, selectable, c, n_val, i);
                    swap(board.at(n_select), board.at(c));  //元に戻す
                }
            }
            

                }
            }*/
        }
    }
    void be_finished(queue<int> moved) {    //探索成功時の最終処理
        int s_count = 0; //選択回数のカウンター
        queue<int> s_rec;    //選択座標の記録
        int count;  //交換回数のカウンター
        queue<int> c_count; //交換回数のカウントの記録
        queue<int> c_rec;  //交換方向の記録
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
        cout << s_count << endl;
        while (c_rec.empty() == false) {
            if (c_count.front() == 0) {
                s_rec.pop();
                c_count.pop();
                continue;
            }
            printf("%02x\n", s_rec.front());
            s_rec.pop();
            cout << c_count.front() << endl;
            for (int i = 0; i < c_count.front(); i++) {
                switch (c_rec.front()) {
                case 0:
                    cout << 'U';
                    break;
                case 1:
                    cout << 'R';
                    break;
                case 2:
                    cout << 'D';
                    break;
                case 3:
                    cout << 'L';
                    break;
                }
                c_rec.pop();
                cout << endl;
            }
            c_count.pop();
        }
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
    Beam(vector<vector<int>> rel, int w, int h, int sr, int cr,int bw) {
        width = w;
        height = h;
        s_rate = sr;
        c_rate = cr;
        relate = rel;
        be_wid = bw;
    }
};

int gcd(int a, int b);

int main() {
    clock_t start = clock();
    int width, height;
    int selectable;
    int s_rate, c_rate; //s_rateは選択コスト,c_rateは交換コスト
    cin >> width >> height;
    vector<int> board(width * height);
    cin >> selectable;
    cin >> s_rate >> c_rate;
    int be_wid;
    cin >> be_wid;

    int i, j, k;
    for (i = 0; i < width * height; i++) {
        (void)scanf("%x", &board[i]);
        board.at(i) = (int)(board.at(i) / 16) + (board.at(i) % 16) * width;
    }

    /*vector<int> board_copy(width * height);
    board_copy = board;
    */
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


    Beam problem(relate, width, height, s_rate, c_rate,be_wid);



    int val = 0;
    for (i = 0; i < width * height; i++) {
        val += problem.piece_val(i, board.at(i));
    }
    int cost = 0;
    queue<int> moved;//駒の操作を管理するqueue

    int rate_gcd = gcd(s_rate, c_rate);


    /*while (gCount == 0) {
        for (i = 0; i < width * height; i++) {
            if (problem.piece_val(i, board.at(i)) != 0) {
                n_select = i;
                moved.push(i + 1000);
                problem.be_search(board, moved, cost, selectable, n_select, val, i + 1000);
                while (moved.empty() == false) moved.pop();
            }
            board = board_copy;
        }
    }
    */

    problem.predata[0].board = board;

    problem.be_search(board, moved, cost, selectable, -1, -1,val,0);
        //(vector<int> board, queue<int>moved, int cost, int selectable, int n_select, int recent,int pre_root)



    int node_count;
    bool depth = false;

    while (gCount == 0) {
        int dir, sel,root,c;
        node_count = 0;
        for (auto itr = node.begin(); itr != node.end(); ++itr) {
            root = itr->second.second;  //  遷移元の指定
            if (itr->second.first >= 1000||depth==false) {
                dir = itr->second.first % 1000;
                sel = itr->second.first;
                

                sel -= dir;
                sel = (sel / 1000) - 1;

                
                c = relate.at(sel).at(dir);
                problem.nextdata[root].moved= moved;
                problem.nextdata[root].moved.push(sel + 1000);
                problem.nextdata[root].moved.push(dir);
                swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));
                problem.nextdata[root].board = board;

                problem.nextdata[root].cost = cost + s_rate + c_rate;
                problem.nextdata[root].val = itr->first;
                problem.nextdata[root].selectable = selectable - 1;
                problem.nextdata[root].n_select = sel;
                problem.nextdata[root].recent = 1000;
                node_count++;
                if (node_count >= be_wid) break;
            }
            else {
                dir = itr->second.first;
                sel = problem.predata[root].n_select;

                c = relate.at(sel).at(dir);   
                problem.nextdata[root].moved = moved;
                problem.nextdata[root].moved.push(dir);
                swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));
                problem.nextdata[root].board = board;

                problem.nextdata[root].cost = cost + c_rate;
                problem.nextdata[root].val = itr->first;
                problem.nextdata[root].selectable = selectable;
                problem.nextdata[root].n_select = sel;
                problem.nextdata[root].recent = dir;
            }
        }
        depth = true;

        for (i = 0; i < node_count; i++) {
            problem.predata[i] = problem.nextdata[i];
        }

        for (i = 0; i < node_count; i++) {
            problem.be_search(problem.predata[i].board, problem.predata[i].moved, problem.predata[i].cost, problem.predata[i].selectable, problem.predata[i].n_select, problem.predata[i].recent, problem.predata[i].val,root);
        }
    }
    clock_t end = clock();
    cout << "time=" << ((double)end - start) / 1000 << endl;
    return 0;
}

int gcd(int a, int b)
{
    if (a % b == 0)
    {
        return(b);
    }
    else
    {
        return(gcd(b, a % b));
    }
}
