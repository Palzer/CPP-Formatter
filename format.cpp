#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

void processcpp(char* filename);
bool validcppfile(char* filename);
void formatfiles(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	if (strcmp(argv[1],"-f") == 0)
	{
		formatfiles(argc,argv);
	}
	else if (strcmp(argv[1],"-r") == 0)
	{
		fprintf(stderr,"You want to revert your files. This is unimplemented\n");
	}
	else
	{
		fprintf(stderr,"Usage: ./format [-r] [-f]\n\n -r : use to revert your files to the backed up version\n -f : use to format your files\n\n");
	}
	return 0;
}
///////////////////////////////////////////
///////////////////////////////////////////
void formatfiles(int argc, char *argv[])
{
	int j;
	char* extension;
	
	for (int i = 2; i < argc; i ++)
	{
		fprintf(stderr,"Formatting file: %s",argv[i]);
		
		j = strlen(argv[i]);
		while (j > 0 and argv[i][j] != '.')
		{
			j = j - 1;
		}
	
		extension = &argv[i][j];
		//if (strcmp(extension,".cpp") == 0)
		//{
			fprintf(stderr," as a .cpp file\n");
			processcpp(argv[i]);
		//}
		//else
		//{
			//fprintf(stderr,"\n	Error: File is not a known type.\n");			
		//}
	}
}
///////////////////////////////////////////
///////////////////////////////////////////
bool validcppfile(char* filename)
{
	int i = strlen(filename);
	char* extension;
	while (i > 0 and filename[i] != '.')
	{
		i = i - 1;
	}
	
	extension = &filename[i];
	return (strcmp(extension,".cpp") == 0);
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
		getline(myfile,line);
		while(line[0] == 9 || line[0] == 32)
		{
			line.erase(0,1);
		}
		//cout << "current line is :" << line << endl;
		if (line[0] != '{' && line[0] != '}') newline.append(numtab,9);
		for (int i = 0; i < line.length(); i++)
		{
			if ((line[i] == '{') and !quote and !tick)
			{
				if (i == 0)
				{
					newline.append(numtab,9);	
				}
				if (i != 0 and not lastnewline)
				{
					newline.append("\n");
					lastnewline = true;
					//fprintf(stderr,"at %i, before char '%c', adding newline\n",i,line[i]);	
					newline.append(numtab,9);
				}
				newline.append("{");
				
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
			else if(line[i] == '}' and !quote and !tick)
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
				if ((i != line.length()-1))
				{
					newline.append("\n");
					lastnewline = true;	
					//fprintf(stderr,"at %i, after char '%c', adding newline and appending '%i' tabs\n",i,line[i],numtab);	
					newline.append(numtab,9);
				}	
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
