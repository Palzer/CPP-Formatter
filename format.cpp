#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

void process(char* filename);
bool validfile(char* filename);
void formatfiles(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	if (strcmp(argv[1],"-f") == 0)
	{
		formatfiles(argc,argv);
	}
	else if (strcmp(argv[1],"-r") == 0)
	{
		fprintf(stderr,"You want to revert your files\n");
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
	for (int i = 2; i < argc; i ++)
	{
		fprintf(stderr,"Formatting file: %s\n",argv[i]);
		if (validfile(argv[i]))
		{
			process(argv[i]);
		}
		else
		{
			fprintf(stderr,"	Error: File is not .cpp\n");			
		}
	}
}
///////////////////////////////////////////
///////////////////////////////////////////
void process(char* filename)
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
	bool lastbrace = false;
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
		lastbrace = false;
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
				if (i != 0 and not lastbrace)
				{
					newline.append("\n");
					//fprintf(stderr,"at %i, before char '%c', adding newline\n",i,line[i]);	
					newline.append(numtab,9);
				}
				newline.append("{");
				
				numtab++;
				//fprintf(stderr,"opening: changing tabs to %i\n",numtab);
				if (i != line.length() - 1)
				{
					newline.append("\n");
					//fprintf(stderr,"at %i, after char '%c', adding newline\n",i,line[i]);	
					newline.append(numtab,9);
				}
				lastbrace = true;
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
				if (i != 0 and not lastbrace)
				{
					newline.append("\n");
					//fprintf(stderr,"at %i, before char '%c', adding newline and appending '%i' tabs\n",i,line[i],numtab);	
					newline.append(numtab,9);
				}
				else if (newline[newline.length()-1] == '	' and lastbrace) newline.erase(newline.length()-1);
				newline.append("}");	
				if ((i != line.length()-1))
				{
					newline.append("\n");	
					//fprintf(stderr,"at %i, after char '%c', adding newline and appending '%i' tabs\n",i,line[i],numtab);	
					newline.append(numtab,9);
				}
				lastbrace = true;	
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
				lastbrace = false;
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
				lastbrace = false;
			}
			else
			{
				newchar[0] = line[i];
				newline.append(newchar);
				lastbrace = false;
			}
		}
		
		/*if (numtab < 0) numtab = 0;
		if ((finline) and (!binline) and (numtab > 0))
		{
			newline.append(numtab-1,9);
			//cout << numtab-1 << " indents on line '" << line << "'" << "	/withopen" << endl;
		}
		else if ((!finline) and (binline)) 
		{
			newline.append(numtab+1,9);
			//cout << numtab+1 << " indents on line '" << line << "'" << "	/withclose" << endl;
		}
		else
		{
			newline.append(numtab,9);
			//cout << numtab << " indents on line '" << line << "'" << endl;
		}
		newline.append(line);*/
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
///////////////////////////////////////////
///////////////////////////////////////////
bool validfile(char* filename)
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
