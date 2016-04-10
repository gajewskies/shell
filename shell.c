#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

typedef enum 
{ 
	false, 
	true 
} bool;


void intHandler(int sig)
{
	signal(SIGINT, intHandler);
	printf("\nIf you would like to exit type exit()\n");
}

void error(const char * err)
{
	printf("[ERR] %s\n", err);
}

void help()
{
	printf("Welcome to the c interactive shell\n");
	printf("Enter commands in the command line to run code\n");
}

int countChars()
{
    int count = 0;  /* number of characters seen */
    FILE *fp;    /* input file */
	int ch;

    fp = fopen("./c_code_temp.c", "r");

	    if (fp == NULL) 
	    {
	        printf("Cannot open %s\n", "./c_code_temp.c");
	        exit(8);
	    }

	    while (1) 
	    {
	        ch = fgetc(fp);
	        if (ch == EOF)
	        {
	        	break;
	        }
	        ++count;
	    }
	return count;    
}

void endBrace()
{
    FILE *fp;
    char *buffer;

   	buffer = "}";

    fp = fopen("./c_code_temp.c", "a+");

    printf("asd %d\n", countChars());
    ftruncate(fileno(fp), countChars() - 5);

   	fprintf(fp, "%s\n", buffer);
    fclose(fp);
}

void readCode()
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int lines = 0;

	fp = fopen("./c_code_temp.c", "r");
	if (fp == NULL)
	{
		exit(1);
	}

	while ((read = getline(&line, &len, fp)) != -1)
	{
		printf("%d : %s", lines++, line);
	}

	fclose(fp);
	if (line)
	{
		free(line);
	}
}

void writeCode(char *command)
{
	FILE *fp;
	static bool firstWrite = true;
	char *buffer;

	fp = fopen("./c_code_temp.c", "a+");

    ftruncate(fileno(fp), countChars() - 2);

	if (firstWrite == true)
	{
		buffer = "int main(void) {";
	}

	if (firstWrite == true)
	{
		fprintf(fp, "%s\n", buffer);
		firstWrite = false;
	}
	fprintf(fp, "%s\n", command);
	fprintf(fp, "}\n");

	fclose(fp);
}

void prompt(char *command)
{
	char buffer[100] = {'\0'};
	int i = 0;
	static bool blockCode = false;
	static int blocks = 0;

	if (blockCode == false && blocks == 0)
	{
		printf(">>> ");
	}
	else
	{
		printf("... ");
	}

	fgets (buffer, 100, stdin);

	if (strncmp(buffer, "exit()", 6) == 0)
	{
		int status;
		status = remove("./c_code_temp.c");
		if (status == 1)
		{
			error("Failure to remove file");
		}
		exit(1);
	}

	if (strncmp(buffer, "read()", 6) == 0)
	{
		readCode();
	}

	if (strncmp(buffer, "help()", 6) == 0)
	{
		help();
	}

	if (strchr(buffer, '{') != NULL)
	{
		blockCode = true;
		blocks++;
	}
	else if(strchr(buffer, '}') != NULL)
	{
		blockCode = false;
		blocks--;
	}

	for (i = 0; i < 100; i++)
	{
		command[i] = buffer[i];
	}
}

int main(void)
{
	char command[100] = {'\0'};
	help();

	signal(SIGINT, intHandler);
	while (true)
	{
		prompt(command);
		writeCode(command);
	}

	return 0;
}