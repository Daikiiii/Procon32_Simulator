#pragma once

#include<stdio.h>


//USB����p�\�R���Ƀf�[�^����荞��
void readfunc() {

	char data[500];
	FILE* outfile;
	FILE* infile;

	outfile = fopen("output.txt", "a"); //�f�[�^����荞�ސ�(�K�X�ύX���K�v�ł�)
	infile = fopen("D:/�d�H��/contest_output.txt", "r"); //�f�[�^�̎Q�ƌ�(�K�X�ύX���K�v�ł�)

	while (fgets(data,128,infile ) != NULL){
		fprintf(outfile,"%s",data);
	}

	//fprintf(outfile, "%s ", "-1");	
	//fprintf(infile, "%s ", "successed program");

	//printf("end\n");

	fclose(outfile);
	fclose(infile);
}

//�p�\�R������USB�Ƀf�[�^����������
void writefunc() {

	char data[500];
	FILE* outfile;
	FILE* infile;

	outfile = fopen("D:/�d�H��/contest_output.txt", "a"); //�f�[�^���������ސ�
	infile = fopen("output.txt", "r"); //�f�[�^�̎Q�ƌ�

	while (fgets(data, 128, infile) != NULL) {
		fprintf(outfile, "%s", data);
	}

	//fprintf(outfile, "%s ", "-1");	
	//fprintf(infile, "%s ", "successed program");

	//printf("end\n");

	fclose(outfile);
	fclose(infile);
}
