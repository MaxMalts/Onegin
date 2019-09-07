#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/**
*	Определяет размер текстового файла с учетом символа '\r'
*
*	@param[in] f Файл
*
*	@return Размер
*/

int GetFileSize(FILE* f) {
	assert(fseek(f, 0, SEEK_END) == 0);

	int fileSize = ftell(f);
	assert(fileSize != -1);

	assert(fseek(f, 0, SEEK_SET) == 0);
	return fileSize;
}


/**
*	Определяет количество строк в тексте
*
*	@param[in] text Текст
*	@param[in] binSize Размер текста с учетом символа '\r'
*
*	@return Количество строк
*/

int CountLines(char* text, int binSize) {
	int NLines = 1;
	for (int i = 0; i < binSize; i++) {
		if (text[i] == '\r') {
			assert(text[i + 1] == '\n');
			NLines++;
		}
	}
	return NLines;
}


/**
*	Сопоставляет строки текста и указатели на них, а также обнуляет концы строк
*
*	@param[in] text Текст
*	@param[in] index Указатели
*	@param[in] binSize Размер текста с учетом символа '\r'
*/

void MatchAndNull(char* text, char** index, int binSize) {
	index[0] = &text[0];
	int indexNum = 1;
	for (int i = 0; i < binSize; i++) {
		if (text[i] == '\r') {
			assert(text[i + 1] == '\n');
			index[indexNum] = &text[i + 2];
			indexNum++;
			text[i] = '\0';
			text[i + 1] = '\0';
		}
	}
	assert(text[binSize] == '\0');
}


/**
*	Сортирует строки (пузырьком)
*
*	@param[in] index Указатели на строки
*	@param[in] NLines Количество строк
*/

void SortStrings(char** index, int NLines) {
	for (int i = NLines - 1; i > 0; i--) {
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
*	@param[in] NLines Количество строк
*	@param[in] f Файл
*/

void StringsOutput(char** index, int NLines, FILE* f) {
	for (int i = 0; i < NLines; i++) {
		fprintf(f, "%s\n", index[i]);
	}
}

int main() {

	FILE* fin = fopen("Input.txt", "rb");
	assert(fin != NULL);
	int binSize = GetFileSize(fin);
	char* text = (char*)calloc(binSize + 1, 1);
	assert(fread(text, sizeof(char), binSize, fin)==binSize);
	fclose(fin);

	int NLines = CountLines(text, binSize);

	char** index = (char**)calloc(NLines, sizeof(int));
	MatchAndNull(text, index, binSize);

	SortStrings(index, NLines);

	FILE* fout = fopen("Output.txt", "w");
	StringsOutput(index, NLines, fout);
	fclose(fout);

	return 0;
}

/*
/**
*	Читает строку из файла, не добавляя \n
*
*	@param[in] str Строка
*	@param[in] f Файл
*
*	@return Полученная строка
*

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
*

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
*

void MatchAndNull(char* text, char** index) {
	for (int i = 0; i < NLines; i++) {
		index[i] = text[i];
	}
}


/**
*	Сортирует строки (пузырьком)
*
*	@param[in] index Указатели на строки
*

void SortStrings(char** index, int NLines) {
	for (int i = NLines - 1; i > 0; i--) {
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
*

void StringsOutput(char** index, int NLines, FILE* f) {
	for (int i = 0; i < NLines; i++) {
		fprintf(f, "%s\n", index[i]);
	}
}*/