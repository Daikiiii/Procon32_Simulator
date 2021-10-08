#include<stdio.h>
//#include"ÉwÉbÉ_Å[.h"
//#include"file_update.h"
//#include"file_input.h"

#include <iostream>
#include<vector>
#include<queue>

using namespace std;


int width, height;
int selectable;
int s_cost, c_cost;


int main() {
	//infunc(&width, &height, &selectable, &s_cost, &c_cost, map);
	//func(0, 0);
	//readfunc();
	
	vector<int> map;
	
	//Ç±Ç±Ç©ÇÁcinÇÃë„ë÷
	FILE* infile;
	int temp;

	infile = fopen("input.txt", "r");
	//cout << "ok" << endl;
	fscanf(infile, "%d %d", &width, &height);
	fscanf(infile, "%d", &selectable);
	fscanf(infile, "%d %d", &s_cost, &c_cost);
	map.resize(width * height);
	for (int i = 0; i < width * height; i++) {
		fscanf(infile, "%d", &temp);
		map.at(i) = temp;

	}
	fclose(infile);
	//Ç±Ç±Ç‹Ç≈cinÇÃë„ë÷

	printf("%d %d\n", width, height);
	printf("%d\n", selectable);
	printf("%d %d\n", s_cost, c_cost);
	for (int i = 0; i < height * width; i++) {
		temp = map.at(i);
		printf("%02d ",temp);
		if ((i+1) % width == 0) printf("\n");
	}

	queue<int> que; que.push(2);

	//Ç±Ç±Ç©ÇÁcoutÇÃë„ë÷
	FILE* outfile;

	outfile=fopen("output.txt","w");
	fprintf(outfile, "%d\n", s_cost); // cout<<s_cost<<endl  ÇÃÇÊÇ§Ç»èàóù
	fprintf(outfile, "%d\n", que.front());  //cout<<que.front()<<endl  ÇÃÇÊÇ§Ç»èàóù
	fprintf(outfile, "%c", 'U');  //  cout<<'U'  ÇÃÇÊÇ§Ç»èàóù
	fprintf(outfile, "\n");  // cout<<endl   ÇÃÇÊÇ§Ç»èàóù
	fclose(outfile);
	//Ç±Ç±Ç‹Ç≈coutÇÃë„ë÷

	return 0;
}