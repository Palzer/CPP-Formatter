#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

void process(char* filename);
bool validfile(char* filename);

int main(int argc, char *argv[])
{
	
	for (int i = 1; i < argc; i ++)
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
	return 0;
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
	bool finline = false;
	bool binline = false;
	
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
		finline = false;
		binline = false;
		getline(myfile,line);
		//cout << line;
		while(line[0] == 9 || line[0] == 32){
			line.erase(0,1);
		}
		for (int i = 0; i < line.length(); i++)
		{
			if ((line[i] == '{') and !quote)
				{
					finline = true;
					numtab++;
				}
				else if(line[i] == '}' and !quote)
			{
				if (i != 0) binline = true;
				if (numtab > 0)
				{
					numtab--;		
				}				
			}
			else if((line[i] == '"') and !tick)
			{
				quote = !quote;
			}
			else if((line[i] == 39/*tick*/) and !quote)
			{
				tick = !tick;
			}
		}
		
		if (numtab < 0) numtab = 0;
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
		newline.append(line);
		if(myfile.good())
		{
			newline.append("\n");
		}
		newfile << newline;
	}
	
	myfile.close();
	newfile.close();
	
	result = rename(filename,backupfilename);
	if ( result == 0 )	puts ( "	Backup successfully created" );
	else	perror( "Error making backup file" );
	
	result = rename(newfilename,filename);
	if ( result == 0 )	puts ( "	Original successfully replaced" );
	else	perror( "Error replacing original file" );
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
