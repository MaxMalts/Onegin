#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct indexstruct {
	char* stringp = 0;
	int length = 0;
};

/**
*	Определяет размер текстового файла с учетом символа '\r'
*
*	@param[in] f Файл
*
*	@return Размер
*/

int GetFileSize(FILE* f) {
	assert(f != NULL);
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
	assert(text != NULL);
	assert(binSize != NULL);
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
*	Сопоставляет строки текста, указатели на них и их длины, а также обнуляет концы строк
*
*	@param[in] text Текст
*	@param[in] index Структура с информацией о стрках
*	@param[in] binSize Размер текста с учетом символа '\r'
*/

void MatchAndNull(char* text, struct indexstruct* index, int binSize) {
	assert(text != NULL);
	assert(index != NULL);
	assert(binSize != NULL);
	index[0].stringp = &text[0];
	int indexNum = 1;
	int prev = 0;
	for (int i = 0; i < binSize; i++) {
		if (text[i] == '\r') {
			assert(text[i + 1] == '\n');
			index[indexNum].stringp = &text[i + 2];
			index[indexNum - 1].length = i - prev;
			prev = i + 2;
			indexNum++;
			text[i] = '\0';
			text[i + 1] = '\0';
		}
	}
	index[indexNum-1].length = binSize - prev;
	assert(text[binSize] == '\0');
}


/**
*	Сортирует строки
*
*	@param[in] index Массив структур с информацией о строках
*	@param[in] NLines Количество строк
*	@param[in] (*Compare)(char*, char*) Функция сравнения строк
*/

void SortStrings(struct indexstruct* index, int NLines, int (*Compare)(char*, char*, int, int)) {
	assert(index != NULL);
	assert(Compare != NULL);
	for (int i = NLines - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (Compare(index[j].stringp, index[j + 1].stringp, index[j].length, index[j + 1].length) > 0) {
				char* tc = index[j].stringp;
				index[j].stringp = index[j + 1].stringp;
				index[j + 1].stringp = tc;
				int ti = index[j].length;
				index[j].length = index[j + 1].length;
				index[j + 1].length = ti;
			}
		}
	}
}


/**
*	Записывает строки в файл
*
*	@param[in] index Массив с информацией о строках
*	@param[in] NLines Количество строк
*	@param[in] f Файл
*/

void StringsOutput(struct indexstruct* index, int NLines, FILE* f) {
	assert(index != NULL);
	assert(f != NULL);
	for (int i = 0; i < NLines; i++) {
		fprintf(f, "%s\n", index[i].stringp);
	}
}



/**
*	Сравнивает две строки, игнорируя все символы, кроме букв
*
*	@param[in] str1 Первая строка
*	@param[in] str2 Вторая строка
*	@param[in] len1 Длина первой строки
*	@param[in] len2 Длина второй строки
*
*	@return 1, если первая строка больше второй; -1, если первая строка меньше второй; v0, если строки равны
*/

int CompareDir(char* str1, char* str2, int len1, int len2) {
	assert(str1 != NULL);
	assert(str2 != NULL);
	assert(len1 != NULL);
	assert(len2 != NULL);

	int i1 = 0, i2 = 0;
	while (i1 < len1 || i2 < len2) {
		if ((str1[i1]<'A' || str1[i1]>'Z' && str1[i1]<'a' || str1[i1]>'z') && i1 < len1) {
			i1++;
			continue;
		}
		else if ((str2[i2]<'A' || str2[i2]>'Z' && str2[i2]<'a' || str2[i2]>'z') && i2 < len2) {
			i2++;
			continue;
		}
		else if (i1 < len1 && i2 < len2 ) {
			if (str1[i1] < str2[i2]) {
				return -1;
			}
			else if (str1[i1] > str2[i2]) {
				return 1;
			}
			else {
				i1++;
				i2++;
			}
		}
		else if (i1 == len1 && i2 < len2 ) {
			return -1;
		}
		else if (i1 < len1 && i2 == len2 ) {
			return 1;
		}
	}
	return 0;
}

/**
*	Сравнивает 2 строки наоборот, игнорируя все символы, кроме букв.
*
*	@param[in] str1 Первая строка
*	@param[in] str2 Вторая строка
*	@param[in] len1 Длина первой строки
*	@param[in] len2 Длина второй строки
*
*	@return 1, если первая строка больше второй;
*	        -1, если первая строка меньше второй;
*	        0, если строки равны
*/

int CompareRev(char* str1, char* str2, int len1, int len2) {
	assert(str1 != NULL);
	assert(str2 != NULL);
	assert(len1 != NULL);
	assert(len2 != NULL);
	
	int i1 = len1 - 1, i2 = len2-1;
	while (i1 >= 0 || i2 >= 0) {
		if ((str1[i1]<'A' || str1[i1]>'Z' && str1[i1]<'a' || str1[i1]>'z') && i1 >= 0) {
			i1--;
			continue;
		}
		else if ((str2[i2]<'A' || str2[i2]>'Z' && str2[i2]<'a' || str2[i2]>'z') && i2 >= 0) {
			i2--;
			continue;
		}
		else if (i1 >= 0 && i2 >= 0) {
			if (str1[i1] < str2[i2]) {
				return -1;
			}
			else if (str1[i1] > str2[i2]) {
				return 1;
			}
			else {
				i1--;
				i2--;
			}
		}
		else if (i1 < 0 && i2 >= 0) {
			return -1;
		}
		else if (i1 >= 0 && i2 < 0) {
			return 1;
		}
	}
	return 0;
}


/**
*	Копирует массив структур indexstruct
*
*	@param[in] structcopy Массив, в который осуществляется копия
*	@param[in] sourcestruct Первоначальный массив
*	@param[in] Count Размер массива
*/

void CopyIndexstruct(struct indexstruct* structcopy, struct indexstruct* sourcestruct, int Count) {
	assert(structcopy != NULL);
	assert(sourcestruct != NULL);
	for (int i = 0; i < Count; i++) {
		structcopy[i].stringp = sourcestruct[i].stringp;
		structcopy[i].length = sourcestruct[i].length;
	}
}

int main() {

	FILE* fin = fopen("Input.txt", "rb");
	assert(fin != NULL);
	int binSize = GetFileSize(fin);
	char* text = (char*)calloc(binSize + 1, 1);
	assert(fread(text, sizeof(char), binSize, fin) == binSize);
	fclose(fin);

	int NLines = CountLines(text, binSize);

	struct indexstruct* index = (struct indexstruct*)calloc(NLines, sizeof(struct indexstruct));
	struct indexstruct* indexRev = (struct indexstruct*)calloc(NLines, sizeof(struct indexstruct));
	struct indexstruct* indexCopy = (struct indexstruct*)calloc(NLines, sizeof(struct indexstruct));
	MatchAndNull(text, index, binSize);
	CopyIndexstruct(indexRev, index, NLines);
	CopyIndexstruct(indexCopy, index, NLines);

	SortStrings(index, NLines, CompareDir);
	SortStrings(indexRev, NLines, CompareRev);

	FILE* fout = fopen("Output.txt", "wb");
	StringsOutput(index, NLines, fout);
	fprintf(fout, "\r\n\r\n");
	StringsOutput(indexRev, NLines, fout);
	fprintf(fout, "\r\n\r\n");
	StringsOutput(indexCopy, NLines, fout);
	fclose(fout);

	free(text);
	free(index);
	free(indexRev);
	free(indexCopy);

	return 0;
}