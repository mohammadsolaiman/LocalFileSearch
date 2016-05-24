/*
 * parser.h
 *
 *  Created on: May 20, 2015
 *      Author: Mohammad
 */

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <stdlib.h>
#include "dirent.h"
#include <sys/stat.h>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
using namespace std;

struct Prime_record {
	string word;
	int byte_offset;
	string sorce_file;
};
struct Index {
	char word[99];
	int ptr;
};
struct List {
	char word[99];
	int byte_offset;
	int ptr;
};

class Parser {
public:
	Parser(string, string, string, string, string);
	Parser();
	int scanDir(string); //scan directory and save the list of all files in file named _fileOfFiles

	void creatLocalParse();
	void creatFilesAndParse();
	void parse(ofstream &, ifstream &);
	void indexing();
	int checkExist(string, fstream &);
	void sortSecondary();

	int* binarySearch_by_Name(string &);
	int binary_search(Index[], string &, int, int);
	void read_record(fstream &, Prime_record &, int);
	string tostring(char[]);
	string getWordFromSorce(Prime_record &);

	void setPathName(string);
	void setDataContanerName(string);
	void setIndexName(string);
	void setListName(string);
	void setFileOfFilsName(string);

	string getPathName() const;
	string getDataContanerName() const;
	string getIndexName() const;
	string getListName() const;
	string getFileOfFilsName() const;

private:
	string _path_name, _dataContaner_file, _index_file, _list_file,
			_fileOfFiles;

};

#endif /* SRC_PARSER_H_ */
