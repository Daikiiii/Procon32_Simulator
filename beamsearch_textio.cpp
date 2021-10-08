#include <iostream>

#include<algorithm>

#include<vector>

#include<queue>

#include<map>

#include<time.h>



#include<cassert>





using namespace std;



int gCount = 0;

multimap<int, pair<int, int>> node;

#define be_size 10



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

    /*

    set(vector<int> board1,int cost1,int val1,int selectable1,int n_select1,int recent1){

        board = board1;

        cost = cost1;

        val = val1;

        selectable = selectable1;

        n_select = n_select1;

        recent = recent1;

    }

    */

};



struct Beam {

    int width; //��

    int height;  //�c

    int s_rate, c_rate;

    vector<vector<int>> relate; //���ꂼ��̍��W�ɑ΂���אڃ��X�g



    set predata[be_size];

    set nextdata[be_size];







    void be_search(vector<int> board, queue<int>moved, int cost, int selectable, int n_select, int recent, int n_val, int pre_root) {    //n_select�͌��ݑI�����Ă���s�[�X�̍��W



        int valu;

        int c;  //�����Ώۂ̍��W

        int i, j;



        if (n_val == 0 && gCount == 0) {



            gCount++;

            be_finished(moved);


            //cout << "cost=" << cost << endl;



        }

        else {

            queue<int>n_moved;

            /*�����A�c��I���\�񐔂�0�ł͂Ȃ�

            �Ȃ�I�𑀍���s��*/

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

        }

    }

    void be_finished(queue<int> moved) {    //�T������

        int s_count = 0; //�I���񐔂̃J�E���^�[

        queue<int> s_rec;    //�I�����W�̋L�^

        int count;  //�����񐔂̃J�E���^�[

        queue<int> c_count; //�����񐔂̃J�E���g�̋L�^

        queue<int> c_rec;  //���������̋L�^

        FILE* outfile;

        outfile = fopen("output.tet", "w");

        while (moved.empty() == false) {

            s_count++;

            s_rec.push(((moved.front() - 1000) / width) + ((moved.front() - 1000) % width) * 16);

            moved.pop();

            count = 0;

            while (moved.empty() == false) {

                if (moved.front() > 4) break;   //�I�𑀍�Ȃ�break

                count++;

                c_rec.push(moved.front());

                moved.pop();

            }

            c_count.push(count);

        }

        fprintf(outfile,"%d\n",s_count); //cout << s_count << endl;

        while (c_rec.empty() == false) {

            if (c_count.front() == 0) {

                s_rec.pop();

                c_count.pop();

                continue;

            }

            fprintf(outfile,"%02x\n",s_rec.front()); //printf("%02x\n", s_rec.front());

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

                fprintf(outfile, "\n"); //cout << endl;

            }

            c_count.pop();

        }

        fclose(outfile);

    }

    int piece_val(int np, int cp) {  //np->���݈ʒucp->�����ʒu

        int cx = cp % width;

        int cy = cp / width;

        int nx = np % width;

        int ny = np / width;

        int val = min(abs(nx - cx), width - abs(nx - cx)) + min(abs(ny - cy), height - abs(ny - cy));

        val *= c_rate;

        return val;

    }

    Beam(vector<vector<int>> rel, int w, int h, int sr, int cr) {

        width = w;

        height = h;

        s_rate = sr;

        c_rate = cr;

        relate = rel;

    }

};



int main() {

    //clock_t start = clock();

    int width, height;

    int selectable;

    int s_rate, c_rate; //s_rate�͑I���R�X�g,c_rate�͌����R�X�g

    FILE* infile;

    infile = fopen("input.txt", "r"); //�p�X��K�X�ύX���Ă�������

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



    //relate�̏�������

    for (i = 0; i < height; i++) {

        for (j = 0; j < width; j++) {

            for (k = 0; k <= 3; k++) {

                switch (k) {

                case 0: //������̗א�

                    if (i != 0)

                        relate.at(i * width + j).at(0) = (i - 1) * width + j;

                    else

                        relate.at(j).at(0) = (height - 1) * width + j;

                    break;

                case 1: //�E����

                    if (j != width - 1)

                        relate.at(i * width + j).at(1) = i * width + j + 1;

                    else

                        relate.at(i * width + j).at(1) = i * width;

                    break;

                case 2: //������

                    if (height == 2) {   //�_�u��h�~

                        relate.at(i * width + j).at(2) = -1;

                        break;

                    }

                    if (i != height - 1)

                        relate.at(i * width + j).at(2) = (i + 1) * width + j;

                    else

                        relate.at(i * width + j).at(2) = j;

                    break;

                case 3: //������

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

    //relate�̏������ݏI��





    Beam problem(relate, width, height, s_rate, c_rate);







    int val = 0;

    for (i = 0; i < width * height; i++) {

        val += problem.piece_val(i, board.at(i));

    }

    int cost = 0;

    queue<int> moved;//��̑�����Ǘ�����queue



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

            root = itr->second.second;  //  �J�ڌ��̎w��

            if (itr->second.first >= 1000 || depth == 0) {      //�I��+��������

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

                problem.nextdata[node_count].n_select = sel;

                problem.nextdata[node_count].recent = dir;

            }

            else {                                          //��������

                dir = itr->second.first;

                sel = problem.predata[root].n_select;



                c = relate.at(sel).at(dir);

                problem.nextdata[node_count].moved = problem.predata[root].moved;

                problem.nextdata[node_count].moved.push(dir);

                swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));

                problem.nextdata[node_count].board = problem.predata[root].board;

                swap(problem.predata[root].board.at(sel), problem.predata[root].board.at(c));   //�߂�



                problem.nextdata[node_count].cost = problem.predata[root].cost + c_rate;

                problem.nextdata[node_count].val = itr->first;

                problem.nextdata[node_count].selectable = problem.predata[root].selectable;

                problem.nextdata[node_count].n_select = sel;

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

            problem.be_search(problem.predata[i].board, problem.predata[i].moved, problem.predata[i].cost, problem.predata[i].selectable, problem.predata[i].n_select, problem.predata[i].recent, problem.predata[i].val, i);

        }

    }

    //clock_t end = clock();

    //cout << "time=" << ((double)end - start) / 1000 << endl;

    return 0;

}