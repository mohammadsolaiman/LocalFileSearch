/*
 * main.cpp
 *
 *  Created on: May 20, 2015
 *      Author: Mohammad
 */
#include "parser.h"

int enterChoice();

enum Choices {PARSE_LOCAL = 1 ,PARSE_PATH , SEARCH ,PARSE_SEARCH, END};
int main(){

	string local_Path = ".";
	Parser parserObj("","File_Files.txt","data.dat" , "index.dat" , "list.dat");
	string word , path;
	int choice;
	while((choice = enterChoice()) != END)
		{
			switch(choice)
			{
			case PARSE_LOCAL :
				cout<<"\nENTER THE PATH OF ADRESSES FILE (FILE CONTAINING ALL PATHS YOU NEED)!\n"<<endl;
				getline(cin,path);
				getline(cin,path);
				path.erase(std::remove(path.begin(), path.end(), '\r'), path.end());
				parserObj.setFileOfFilsName(path);
				parserObj.creatLocalParse();

				break;
			case PARSE_PATH:
				cout<<"\nENTER THE DIRECTORY PATH YOU WANT TO SCAN!\n"<<endl;
				getline(cin,path);
				getline(cin,path);
				path.erase(std::remove(path.begin(), path.end(), '\r'), path.end());
				//cin>>path;
				parserObj.setPathName(path);
				parserObj.creatFilesAndParse();
				break;
			case SEARCH :

				cout<<"\nENTER THE WORD TO SEARSH FOR\n";
				cin>>word;
				parserObj.binarySearch_by_Name(word);
				break;
			case PARSE_SEARCH:
				cout<<"\nENTER THE DIRECTORY PATH YOU WANT TO SCAN!\n"<<endl;
				getline(cin,path);
				getline(cin,path);
				path.erase(std::remove(path.begin(), path.end(), '\r'), path.end());
				//cin>>path;
				cout<<"\nENTER THE WORD TO SEARSH FOR\n";
				cin>>word;
				parserObj.setPathName(path);
				parserObj.creatFilesAndParse();
				parserObj.binarySearch_by_Name(word);

				break;
			default:
				cerr<<"\n NO MATCH! \n"<<endl;
				break;

			}
		}
}


int enterChoice(){
	//description..
	cout<<"\n/**************************************************************/"
			"\n/**************************************************************/"
		  "\n/************   © 2015 MOHAMAD SULAIMAN AL-ASHKAR    **********/"
		  "\n/**************************************************************/"
			"\n/**************************************************************/"<<endl;
	//disply available
	cout <<"\nEnter Your choice "<<endl
			<<"1 - PARCE THE SET OF FILES " <<endl
			<<"2 - PARCE SPESEFIC DIRECTURY FILES"<<endl
			<<"3 - SEARCH FOR WORD"<<endl
			<<"4 - PARSE AND SEARCH"<<endl
			<<"5 - END PROGRAM \n? ";


	int menuChoice ;
	cin>>menuChoice ;
	return menuChoice ;

}

