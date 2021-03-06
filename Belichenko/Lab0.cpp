// Lab0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include "math.h"

// Символы представляются в виде числа в десятичной
int ctoi(char c){ 
	if ((c >= '0') && (c <= '9'))
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else return c - 'A' + 10;
}

// Число в десятичной представляется символом
char itoc(int i) { 
	if ((i >= 0) && (i <= 9))
		return i + '0';
	else
		return i + 'A' - 10;
}

//Разворот массива символов (используется в переводе дробной части)
void Reverse(char* str, int N) {
	char c;
	for (int i = 0; i < (N+1)/2; i++) {
		c = str[i];
		str[i] = str[N - i];
		str[N - i] = c;
	}
}

// Перевод числа в десятичную
double ToDec(int* num, double* frac, int N, int point, int basein) { 
	double integ;
	integ = 0;
	*frac = 0;
	int deg = point;
	for (int i = 0; i < point; i++) {
		deg--;
		integ += num[i] * pow((double)basein, (double)deg);
	}
	for (int i = point; i < N; i++) {
		deg--;
		*frac += num[i] * pow((double)basein, (double)deg);
	}
	return integ;
}

// Перевод числа из десятичной в заданную
void FromDec(double integ, double frac, char *str, int baseout) { 
	long long s = (long long)integ;
	int i = 0;
	str[0] = '0';
	while (s > 0) {
		str[i] = itoc(s % baseout); 
		s /= baseout;
		i++;
	}
	Reverse(str, i-1);
	if (frac == 0)
		return;
	if (i == 0)
		i++;
	str[i] = '.';
	i++;
	int k = i + 12;
	double l = 0;
	while ((i < k) && (frac!=0)) {
		frac = modf(frac*baseout, &l);
		str[i] = itoc((int)l);
		i++;
	}
}



int main(){
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "wt");
	if (fin == NULL) {
		fprintf(fout, "failed to open in.txt");
		fclose(fout);
		return 0;
	}
	int basein, baseout;
	fscanf(fin, "%d %d", &basein, &baseout);
	// Проверка корректности ввода
	if ((basein < 2) || (basein > 16) || (baseout < 2) || (baseout > 16)){
		fprintf(fout, "bad input");
		fclose(fin);
		fclose(fout);
		return 0;
	}

	//Считывание числа и позиции запятой
	char *str = (char*)calloc(12,sizeof(char));
	char c;
	int N = 0;
	int point = 0;
	c = fgetc(fin);
	c = fgetc(fin);
	while (c != EOF && c!='\n') {
		str[N] = c;
		N++;
		c = fgetc(fin);
		if (c == '.') {
			point = N;
			c = fgetc(fin);
		}
	}
	fclose(fin);
	if (point == N) {
		fprintf(fout, "bad input");
		free(str);
		fclose(fout);
		return 0;
	}
	if (point == 0)
		point = N;

	//Перевод символьной записи в массив чисел
	int *num = (int*)calloc(12, sizeof(int));
	for (int i = 0; i < N; i++) {
 		num[i] = ctoi(str[i]);
		if (num[i] >= basein || num[i] < 0) {
			fprintf(fout, "bad input");
			free(num);
			free(str);
			fclose(fout);
			return 0;
		}
	}
	free(str);
	double *frac = (double*)calloc(1, sizeof(double));
	double integ = ToDec(num, frac, N, point, basein);
	free(num);
	str = (char*)calloc(49, sizeof(char));
	FromDec(integ, *frac, str, baseout);
	fprintf(fout, str);
	free(str);
	free(frac);
	fclose(fout);
    return 0;
}

