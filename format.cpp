#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

void processcpp(char* filename);
bool fexists(char *filename);
void clear(FILE* in);
void formatfiles(int argc, char *argv[]);
void revertfiles(int argc, char *argv[]);
void stripwhite(string* line);

int main(int argc, char *argv[])
{
	char c;
	if (strcmp(argv[1],"-f") == 0)
	{
		formatfiles(argc,argv);
	}
	else if (strcmp(argv[1],"-r") == 0)
	{
		fprintf(stderr,"Revert files? This cannot be undone. (y/n) ");
			c = getchar();
			clear(stdin);
			if (c == 'y' or c == 'Y')
			{
				revertfiles(argc,argv);
			}
	}
	else
	{
		fprintf(stderr,"Usage: ./format [-r] [-f] files...\n\n -r : use to revert your files to the backed up version\n -f : use to format your files\n\n");
	}
	return 0;
}
///////////////////////////////////////////
///////////////////////////////////////////
void revertfiles(int argc, char *argv[])
{
	char backupfilename[100];
	char oldfilename[100];
	for (int i = 2; i < argc; i++)
	{
		strcpy(backupfilename,argv[i]);
		strcat(backupfilename,".backup");
		strcpy(oldfilename,argv[i]);
		strcat(oldfilename,".old");
		
		fprintf(stderr,"Reverting file: %s\n",argv[i]);
		if (fexists(argv[i]))
		{
			fprintf(stderr,"	Processing file\n");
			
			if( rename(argv[i],oldfilename) != 0 )
			    perror( "Error reverting file - original could not be removed" );
			else
			{
				if ( rename(backupfilename,argv[i]) == 0)
				{	fprintf(stderr, "	Successfully reverted file\n" );
					remove(oldfilename);
				}
				else
				{
					perror( "Error reverting file - backup could not be reverted" );
					rename(oldfilename,argv[i]);
					}
			}			
		}
		else
		{
			fprintf(stderr,"	Backup does not exist. Skipping file.\n");
		}
	}
}
///////////////////////////////////////////
///////////////////////////////////////////
void formatfiles(int argc, char *argv[])
{
	int j;
	char* extension;
	char c;
	bool overwrite = true;
	
	for (int i = 2; i < argc; i ++)
	{
		j = strlen(argv[i]);
		while (j > 0 and argv[i][j] != '.')
		{
			j = j - 1;
		}
	
		extension = &argv[i][j];
		
		fprintf(stderr,"Formatting file: %s, as a %s filetype\n",argv[i],extension);
		
		if (fexists(argv[i]))
		{
			fprintf(stderr,"	Back up detected. Overwrite the current backup? (y/n) ");
			c = getchar();
			clear(stdin);
			if (c == 'y' or c == 'Y')
			{
				overwrite = true;
			}
			else if (c == 'n' or c == 'N')
			{
				overwrite = false;
			}
			else
			{
				
				fprintf(stderr,"\n	Invalid response. Not overwriting. c = %c\n",c);
				overwrite = false;
			}
		}
		if (overwrite)
		{
			fprintf(stderr,"	Processing file\n");
		//if (strcmp(extension,".cpp") == 0)
		//{
			processcpp(argv[i]);
		//}
		//else
		//{
			//fprintf(stderr,"\n	Error: File is an unknown type.\n");			
		//}
		}
	}
}
///////////////////////////////////////////
///////////////////////////////////////////
bool fexists(char* filename)
{
	char file[strlen(filename)];
	strcpy(file,filename);
    struct stat buf;
    if (stat(strcat(file,".backup"), &buf) != -1)
    {
        return true;
    }
    return false;
}
///////////////////////////////////////////
///////////////////////////////////////////
void clear(FILE* in)
{
char ch = getc(in);
while (ch != '\n') ch = getc(in);
}
///////////////////////////////////////////
///////////////////////////////////////////
void stripwhite(string* line, int i)
{
try
{
	//cout << "stripping white from: " << *line << endl;
	while(line->at(i) == 9 || line->at(i) == 32)
	{
		//fprintf(stderr,"stripped\n");
		line->erase(i,1);
	}
	}
	catch (exception& e)
  {
    //cerr << "exception caught: " << e.what() << endl;
  }
}
///////////////////////////////////////////
///////////////////////////////////////////
void processcpp(char* filename)
{
	ifstream myfile;
	char newfilename[100];	
	char backupfilename[100];	
	int result;
	ofstream newfile;
	int numtab = 0;
	string line = "";
	bool quote = false;
	bool tick = false;
	bool lastnewline = false;
	bool linecom = false;
	bool bloccom = false;
	char newchar[2];
	newchar[0] = ' ';
	newchar[1] = 0;
	
	strcpy(newfilename,filename);
	strcat(newfilename,"new");
	strcpy(backupfilename,filename);
	strcat(backupfilename,".backup");
	string newline = "";
	
	myfile.open(filename);
	newfile.open(newfilename);
	
	while (myfile.good())
	{
		newline = "";
		lastnewline = false;
		linecom = false;
		getline(myfile,line);
		stripwhite(&line,0);
		//cout << "current line is :" << line << endl;
		if (line[0] != '}') newline.append(numtab,9);
		for (int i = 0; i < line.length(); i++)
		{
			if ((line[i] == '{') and !quote and !tick and !linecom and !bloccom)
			{
				if (i != 0 and not lastnewline)
				{
					newline.append("\n");
					lastnewline = true;
					//fprintf(stderr,"at %i, before char '%c', adding newline\n",i,line[i]);	
					newline.append(numtab,9);
				}
				newline.append("{");
				stripwhite(&line,i+1);
				numtab++;
				//fprintf(stderr,"opening: changing tabs to %i\n",numtab);
				if (i != line.length() - 1)
				{
					newline.append("\n");
					lastnewline = true;
					//fprintf(stderr,"at %i, after char '%c', adding newline\n",i,line[i]);	
					newline.append(numtab,9);
				}
			}
			else if(line[i] == '}' and !quote and !tick and !linecom and !bloccom)
			{
				if (numtab > 0)
				{
					
					numtab--;
					//fprintf(stderr,"closing: changing tabs to %i\n",numtab);		
				}
				if (i == 0)
				{
					newline.append(numtab,9);
					//fprintf(stderr,"at %i, before char '%c', appending '%i' tabs\n",i,line[i],numtab);	
				}
				if (i != 0 and not lastnewline)
				{
					newline.append("\n");
					lastnewline = true;
					//fprintf(stderr,"at %i, before char '%c', adding newline and appending '%i' tabs\n",i,line[i],numtab);	
					newline.append(numtab,9);
				}
				else if (newline[newline.length()-1] == '	' and lastnewline) newline.erase(newline.length()-1);
				newline.append("}");
				stripwhite(&line,i+1);
				if ((i != line.length()-1))
				{
					newline.append("\n");
					lastnewline = true;	
					//fprintf(stderr,"at %i, after char '%c', adding newline and appending '%i' tabs\n",i,line[i],numtab);	
					newline.append(numtab,9);
				}	
			}
			else if( line[i] == ';' and !quote and !tick and !linecom and !bloccom)
			{
				newline.append(";");
				stripwhite(&line,i+1);
				if ((i != line.length()-1))
				{
					newline.append("\n");
					lastnewline = true;	
					//fprintf(stderr,"at %i, after char '%c', adding newline and appending '%i' tabs\n",i,line[i],numtab);	
					newline.append(numtab,9);
				}	
			}
			else if(i > 0 and line[i] == '/' and line[i-1] == '/' and !tick and !quote and !bloccom)
			{
				linecom = true;
				//fprintf(stderr,"found line comment\n");
				newchar[0] = line[i];
				newline.append(newchar);
				lastnewline = false;
			}
			else if (i > 0 and line[i] == '*' and line[i-1] == '/' and !tick and !quote and !bloccom and !linecom)
			{
				bloccom = true;
				//fprintf(stderr,"found start of block comment\n");
				newchar[0] = line[i];
				newline.append(newchar);
				lastnewline = false;
			}
			else if (i > 0 and line[i] == '/' and line[i-1] == '*' and !tick and !quote and bloccom and !linecom)
			{
				bloccom = false;
				//fprintf(stderr,"found end of block comment\n");
				newchar[0] = line[i];
				newline.append(newchar);
				lastnewline = false;
			}
			else if((line[i] == '"') and !tick)
			{
				if (i > 0)
				{
					if (line[i-1] != 92)
					{
						quote = !quote;
					}
				}
				else
				{
					quote = !quote;
				}
				newline.append("\"");
				lastnewline = false;
			}
			else if((line[i] == 39/*tick*/) and !quote)
			{
				if (i > 0)
				{
					if (line[i-1] != 92)
					{
						tick = !tick;
					}
				}
				else
				{
					tick = !tick;
				}
				newline.append("'");
				lastnewline = false;
			}
			else
			{
				newchar[0] = line[i];
				newline.append(newchar);
				lastnewline = false;
			}
		}

		if(myfile.good())
		{
			newline.append("\n");
			//fprintf(stderr,"adding newline after line\n");
		}
		newfile << newline;
	}
	
	myfile.close();
	newfile.close();
	
	/*result = rename(filename,backupfilename);
	if ( result == 0 )	puts ( "	Backup successfully created" );
	else	perror( "Error making backup file" );
	
	result = rename(newfilename,filename);
	if ( result == 0 )	puts ( "	Original successfully replaced" );
	else	perror( "Error replacing original file" );*/
}
