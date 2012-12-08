#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

void process(char* filename);

int main(int argc, char *argv[])
{
	
	for (int i = 1; i < argc; i ++)
	{
		fprintf(stderr,"Formatting file: %s\n",argv[i]);
		process(argv[i]);
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
	bool isinline = false;
	
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
		isinline = false;
		getline(myfile,line);
		//cout << line;
		while(line[0] == 9 || line[0] == 32){
			line.erase(0,1);
		}
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '{')
				{
					isinline = true;
					numtab++;
				}
			else if(line[i] == '}')
			{
				if (numtab > 0)
				{
					numtab--;		
				}				
			}
		}
		if (isinline) newline.append(numtab-1,9);
		else newline.append(numtab,9);
		newline.append(line);
		newline.append("\n");
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
