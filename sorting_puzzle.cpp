#include <iostream>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;

int gCount = 0;

struct IDA {
    int width; //横
    int height;  //縦
    int s_rate, c_rate;
    vector<vector<int>> relate; //それぞれの座標に対する隣接リスト
    void id_search(vector<int> board,queue<int>moved,int limit, int cost, int selectable,int n_select, int val,int recent) {    //n_selectは現在選択しているピースの座標
        int n_val,n_cost;
        int c;  //交換対象の座標
        int i;
        if (cost <= limit&&val==0) {
            
                gCount++;
                id_finished(moved);
                cout <<"cost="<< cost << endl;
            
        }
        else if (cost >= limit) {

        }
        else {
            queue<int>n_moved;
            /*もし直前の操作が選択
            残り選択可能回数が0
            現在選択している座標のピースが正解位置ではない
            のいずれかを満たすならなら交換操作を行う*/
            if (recent >= 1000 || selectable == 0||n_select!=board.at(n_select)) {
                for (i = 0; i < 4; i++) {
                    if (abs(i-recent) == 2||relate.at(n_select).at(i)==-1) continue;
                    n_moved = moved;
                    c = relate.at(n_select).at(i);
                    n_val = val - (piece_val(c, board.at(c)) + piece_val(n_select, board.at(n_select))) + (piece_val(c, board.at(n_select)) + piece_val(n_select, board.at(c)));
                    //printf("%d %d %d %d %d %d\n", c, board.at(c), n_select, board.at(n_select), val, n_val);
                    n_cost = cost + c_rate;
                    swap(board.at(n_select), board.at(c));
                    n_moved.push(i);
                    if (n_val + n_cost <= limit)
                        id_search(board,n_moved, limit, n_cost,selectable, c, n_val,i);
                    swap(board.at(n_select), board.at(c));  //元に戻す
                }
            }
            else {
                for (i = 0; i < width * height;i++) {
                    if (i == board.at(i)) continue;
                    n_moved = moved;
                    n_moved.push(i + 1000);
                    n_cost = cost + s_rate;
                    if (val + n_cost <= limit)
                        id_search(board,n_moved, limit, n_cost,selectable-1, i, val,i+1000);
                }
            }
        }
    }
    void id_finished(queue<int> moved) {    //探索成功時の最終処理
       int s_count = 0; //選択回数のカウンター
        queue<int> s_rec;    //選択座標の記録
        int count;  //交換回数のカウンター
        queue<int> c_count; //交換回数のカウントの記録
        queue<int> c_rec;  //交換方向の記録
        while (moved.empty() == false) {
            s_count++;
            s_rec.push(((moved.front() - 1000) / width)+ ((moved.front() - 1000) % width)*16);
            moved.pop();
            count = 0;
            while (moved.empty()==false) {
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
    int piece_val(int np,int cp) {  //np->現在位置cp->正解位置
        int cx = cp % width;
        int cy = cp / width;
        int nx = np % width;
        int ny = np / width;
        int val = min(abs(nx - cx), width - abs(nx - cx)) + min(abs(ny - cy), height - abs(ny - cy));
        val *= c_rate;
        return val;
    }
    IDA(vector<vector<int>> rel,int w, int h,int sr,int cr) {
        width = w;
        height = h;
        s_rate = sr;
        c_rate = cr;
        relate = rel;
    }
};

int gcd(int a, int b);

int main() {
    int width, height;
    int selectable;
    int s_rate, c_rate; //s_rateは選択コスト,c_rateは交換コスト
    cin >> width >> height;
    vector<int> board(width*height);
    cin >> selectable;
    cin >> s_rate >> c_rate;

    int i,j,k;
    for (i = 0; i < width*height; i++) {
        (void)scanf("%x", &board[i]);
        board.at(i) = (int)(board.at(i) / 16)  + (board.at(i) % 16)*width;
    }

    vector<int> board_copy(width * height);
    board_copy = board;

    vector<vector<int>> relate(width*height, vector<int>(4));

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
                        relate.at(i * width + j).at(3) = (i+1)*width - 1;
                    break;
                }
            }
        }
    }
    //relateの書き込み終了

    
    IDA problem(relate,width,height,s_rate,c_rate);

    int val=0;
    for (i = 0; i < width * height; i++) {
        val+=problem.piece_val(i, board.at(i));
    }
    cout << val << endl;
    int cost = s_rate;
    int limit = s_rate + val / 2;
    int n_select;
    queue<int> moved;//駒の操作を管理するqueue

    int rate_gcd = gcd(s_rate, c_rate);

    while (gCount==0&&limit<200) {
        for (i = 0; i < width * height; i++) {
            if (problem.piece_val(i,board.at(i)) != 0) {
                n_select = i;
                moved.push(i + 1000);
                problem.id_search(board,moved,limit,cost,selectable,n_select,val,i+1000);
                while (moved.empty()== false) moved.pop();
            }
            board = board_copy;
        }
        limit += rate_gcd;
    }
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
