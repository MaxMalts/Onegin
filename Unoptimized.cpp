#include <stdio.h>
#include <string.h>
#include <assert.h>

const int NLines = 14;   ///<Количество строк
const int NChars = 200;  ///<Количество символов


/**
*	Читает строку из файла, не добавляя \n
*
*	@param[in] str Строка
*	@param[in] f Файл
*
*	@return Полученная строка
*/

char* FgetsNoSlashN(char* str, FILE* f) {
	int i = 0;
	char curch = getc(f);
	while ((curch != EOF) && (curch != '\n') && (i < NChars)) {
		str[i] = curch;
		curch = getc(f);
		i++;
	}
	return str;
}


/**
*	Считывает все строки из файла
*
*	@param[in] text Массив строк
*	@param[in] f Файл
*/

void StringsInput(char text[NLines][NChars], FILE* f) {
	for (int i = 0; i < NLines; i++) {
		char curstr[NChars] = {};
		FgetsNoSlashN(curstr, f);
		strcpy(text[i], curstr);
	}
}


/**
*	Сопоставляет указатели со строками
*
*	@param[in] text Массив строк
*	@param[in] index Указатели на строки
*/

void Match(char text[NLines][NChars], char* index[]) {
	for (int i = 0; i < NLines; i++) {
		index[i] = text[i];
	}
}


/**
*	Сортирует строки (пузырьком)
*
*	@param[in] index Указатели на строки
*/

void SortStrings(char* index[]) {
	for (int i = NLines-1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (strcmp(index[j], index[j + 1]) > 0) {
				char* t = index[j];
				index[j] = index[j + 1];
				index[j + 1] = t;
			}
		}
	}
}


/**
*	Записывает строки в файл
*
*	@param[in] index Указатели на строки
*	@param[in] f Файл
*/

void StringsOutput(char* index[], FILE* f) {
	for (int i = 0; i < NLines; i++) {
		fprintf(f, "%s\n", index[i]);
	}
}

int main() {
	FILE* fin = fopen("Input.txt", "r");
	assert(fin != NULL);
	FILE* fout = fopen("Output.txt", "w");
	char* index[NLines] = {};
	char text[NLines][NChars] = {};
	StringsInput(text, fin);
	Match(text, index);
	SortStrings(index);
	StringsOutput(index, fout);
	return 0;
}