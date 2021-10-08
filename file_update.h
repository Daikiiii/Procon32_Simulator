#pragma once

#include<stdio.h>


//USBからパソコンにデータを取り込む
void readfunc() {

	char data[500];
	FILE* outfile;
	FILE* infile;

	outfile = fopen("output.txt", "a"); //データを取り込む先(適宜変更が必要です)
	infile = fopen("D:/電工部/contest_output.txt", "r"); //データの参照元(適宜変更が必要です)

	while (fgets(data,128,infile ) != NULL){
		fprintf(outfile,"%s",data);
	}

	//fprintf(outfile, "%s ", "-1");	
	//fprintf(infile, "%s ", "successed program");

	//printf("end\n");

	fclose(outfile);
	fclose(infile);
}

//パソコンからUSBにデータを書き込む
void writefunc() {

	char data[500];
	FILE* outfile;
	FILE* infile;

	outfile = fopen("D:/電工部/contest_output.txt", "a"); //データを書き込む先
	infile = fopen("output.txt", "r"); //データの参照元

	while (fgets(data, 128, infile) != NULL) {
		fprintf(outfile, "%s", data);
	}

	//fprintf(outfile, "%s ", "-1");	
	//fprintf(infile, "%s ", "successed program");

	//printf("end\n");

	fclose(outfile);
	fclose(infile);
}
