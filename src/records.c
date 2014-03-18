#include "records.h"

void getRecords(int* score, int* level, int* lines, char pseudo[20])
{
	char fileName[15] = "records.ini";
	FILE* file = fopen(fileName, "r+");
	if(file == NULL)
	{
		file = fopen(fileName, "a");
		fprintf(file, "[Records] \n \nscore=%d \nlevel=%d \nlines=%d \npseudo=%s", 0, 1, 0, "Unknow");
	}

	dictionary* parser = iniparser_load(fileName);

	if(score != NULL)
		*score = iniparser_getint(parser, "Records:score", 0);
	if(level != NULL)
		*level = iniparser_getint(parser, "Records:level", 1);
	if(lines != NULL)
		*lines = iniparser_getint(parser, "Records:lines", 0);

	if(pseudo != NULL)
	{
		char *getPseudo = iniparser_getstring(parser, "Records:pseudo", "Unknow");
		int i;
		for(i=0; getPseudo[i] != '\0'; i++)
			pseudo[i] = getPseudo[i];
		pseudo[i] = '\0';
	}

	iniparser_freedict(parser);
	fclose(file);
}

void setRecords(int score, int level, int lines, const char* pseudo)
{
	char fileName[15] = "records.ini";
	FILE* file = fopen(fileName, "r+");

	if(file == NULL)
		file = fopen(fileName, "a");

	fprintf(file, "[Records] \nscore=%d \nlevel=%d \nlines=%d \npseudo=%s", score, level, lines, pseudo);
	fclose(file);
	return;
}
