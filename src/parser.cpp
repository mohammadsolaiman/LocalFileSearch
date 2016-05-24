/*
 * parser.cpp
 *
 *  Created on: May 20, 2015
 *      Author: Mohammad
 */
#include "parser.h"

using namespace std;

Parser::Parser(string dir_name, string file_files, string dataCont,
		string index, string list) {
	_path_name = dir_name;
	_dataContaner_file = dataCont;
	_index_file = index;
	_list_file = list;
	_fileOfFiles = file_files;
}

void Parser::creatLocalParse() {
	ofstream data(_dataContaner_file.c_str());
	ifstream addresses(_fileOfFiles.c_str());
	if (!data || !addresses) {
		cerr << "data container can't open Or addresses not ready! " << endl;
		exit(1);
	}
	parse(data, addresses);
	indexing();
	sortSecondary();
}

void Parser::creatFilesAndParse() {
	ofstream add(_fileOfFiles.c_str(), ios::trunc);
	add.close();
	scanDir(_path_name.c_str());
	ofstream data(_dataContaner_file.c_str());
	ifstream addresses(_fileOfFiles.c_str());
	if (!data || !addresses) {
		cerr << "data container can't open Or addresses not ready! " << endl;
		exit(1);
	}
	parse(data, addresses);
	indexing();
	sortSecondary();
}

int Parser::scanDir(string _dirName) {
	cout << "SCANING DIRECTORY...\n";
	cout << _dirName << endl;
	ofstream fileOfFiles(_fileOfFiles.c_str(), ios::app);
	if (!fileOfFiles) {
		cerr << "addresses File error" << endl;
		exit(1);
	}
	struct stat st;
	DIR *dir;
	struct dirent *ent;

	stringstream dirpath;
	dirpath << "\"" << _dirName << "\"";
	cout << "path is : " << dirpath.str() << endl;
	if ((dir = opendir(_dirName.c_str())) != NULL) {
		// print all the files and directories within directory
		while ((ent = readdir(dir)) != NULL) {
			// cout<< ent->d_name<<"    ";

			stringstream ss;
			ss << _dirName << "\\" << ent->d_name;
			// string s;
			// ss>>s;
			if (ss.str().compare(ss.str().length() - 1, 1, ".") == 0
					|| ss.str().compare(ss.str().length() - 2, 2, "..") == 0)
				continue;
			//cout<<"S : "<<s<<endl;
			if (stat(ss.str().c_str(), &st) == 0) {
				if (st.st_mode & S_IFDIR) {
					//cout<<s<<" is dir"<<endl;     //this is directory
					scanDir(ss.str().c_str());
				} else {
					//here is a file <check type>
					if (ss.str().compare(ss.str().length() - 4, 4, ".txt") == 0
							|| ss.str().compare(ss.str().length() - 5, 5,
									".text") == 0
							|| ss.str().compare(ss.str().length() - 5, 5,
									".html") == 0
							|| ss.str().compare(ss.str().length() - 4, 4,
									".htm") == 0) {

						fileOfFiles.seekp(0, fileOfFiles.end);
						fileOfFiles << ss.str() << endl;

						cout<<"FILE : "<<ss.str() <<"\tADDED"<< endl;
					}
				}
			}
		}
		fileOfFiles.clear();
		fileOfFiles.close();
		closedir(dir);
	} else {
		//  could not open directory
		perror("could not open directory!");
		return EXIT_FAILURE;
	}
	cout << "\nDIRECTORY SCAN DONE!." << endl;
	return 0;
}

void Parser::parse(ofstream &data, ifstream &addresses) {
	cout << "\nPARCING PLEAS WAIT..." << endl;
	vector<string> adds;
	//to check if the file is empty
	addresses.seekg(0, addresses.end);
	if ((int) addresses.tellg() == 0) {
		cerr << "\nTHERE IS NO FILES IN THIS DIRECTORY\n" << endl;
		exit(0);
	}
	addresses.seekg(0);
	string line;
	//ofstream byte_offset("byte_offset.txt");
	while (!addresses.eof()) {
		getline(addresses, line);
		string result = line;
		if (line == "")
			continue;
		if (line.substr(line.length() - 1, line.length()) == "\r") {
			result = line.substr(0, line.length() - 1);
		}
		adds.push_back(result);

	}

	string word;
	Prime_record rec;
	for (unsigned i = 0; i < adds.size(); i++) {

		ifstream txtfile;
		txtfile.open(adds[i].c_str(), ios::in);
		if (!txtfile) {
			cout << "";
			cerr << "file " << adds[i].c_str() << " can't open" << endl;
			continue;
		}
		string loc = adds[i];
		while (!txtfile.eof()) {
			int _byteOffset = txtfile.tellg();
			txtfile >> word;
			word.erase(std::remove(word.begin(), word.end(), '\n'), word.end());

			if (word != "\r" && word != "" && word.size() < 99) {

				rec.byte_offset = _byteOffset;
				rec.sorce_file = loc;
				transform(word.begin(), word.end(), word.begin(), ::tolower);
				rec.word = word;
				//cout<<rec.word<<"     B_O : "<<rec.byte_offset<<"    in :  "<<rec.sorce_file<<endl;//line="";
				stringstream ss, BoFst;
				BoFst << rec.byte_offset;
				string byteOF;
				BoFst >> byteOF;
				ss << setw(03) << setfill('0') << rec.word.length() << word
						<< setw(03) << setfill('0') << byteOF.length() << byteOF
						<< setw(03) << setfill('0') << rec.sorce_file.length()
						<< rec.sorce_file << "|$|";
				//ss<<"|word="<<word<<"|byte="<<rec.byte_offset<<"|sorce="<<rec.sorce_file;
				string record = "";
				data.write(ss.str().c_str(), ss.str().size());
				while (!ss.eof()) {
					ss >> record;
					//byte_offset<<data.tellp()<<'\n';
				}

			}

		}
		txtfile.close();
	}

	cout << "PARCING DONE!" << endl;
	data.clear();
	data.close();
	addresses.clear();
	addresses.close();
	// byte_offset.clear();
	// byte_offset.close();
}

void Parser::indexing() {
	cout << "\nINDEXING PLEAS WAIT..." << endl;
	fstream indexFile(_index_file.c_str(),
			ios::in | ios::out | ios::binary | ios::trunc);	//fixed length record files
	fstream indexlist(_list_file.c_str(),
			ios::in | ios::out | ios::binary | ios::trunc);	//--//--//--//---//--//--//
	ifstream data(_dataContaner_file.c_str());
	if (!indexFile || !indexlist) {
		ofstream indexFile1(_index_file.c_str(), ios::binary);//fixed length record files
		ofstream indexlist1(_list_file.c_str(), ios::binary);

		indexFile1.close();
		indexlist1.close();
		indexFile.open(_index_file.c_str(), ios::in | ios::out | ios::binary);
		indexlist.open(_list_file.c_str(), ios::in | ios::out | ios::binary);
		if (!indexFile || !indexlist) {
			cerr << "can't creat index files" << endl;
			exit(1);
		}
	}
	string byteOffset;
	//data.clear();

	data.seekg(0);
	int _indCounter = 0;
	int byte_offset;
	while (!data.eof() && byte_offset != -1) {
		int seek_size = 0;
		List list_rec;
		Index index_rec;
		string num(3, '\0');
		stringstream ss;

		byte_offset = data.tellg();	//cout<<"byte_offset :: "<<byte_offset<<endl;

		//cout<<"Here the byte offset :: "<<byte_offset<<endl;
		data.read(reinterpret_cast<char *>(&num[0]), 3);
		ss << num;
		ss >> seek_size;
		//cout<<"Seek Size ::   "<<num<<"   "<<seek_size<<endl;
		string dat(seek_size, '\0');
		data.read(reinterpret_cast<char *>(&dat[0]), seek_size);

		if (byte_offset == -1 || seek_size == 0)
			continue;

		//cout<<"Word: "<<dat<<endl;
		dat.copy(list_rec.word, dat.length());
		dat.copy(index_rec.word, dat.length());
		index_rec.word[dat.length()] = '\0';

		list_rec.word[dat.length()] = '\0';
		list_rec.byte_offset = byte_offset;

		//cout<<list_rec.word<<endl;
		int tmp = checkExist(dat, indexFile);

		if (tmp == -2) {

			indexlist.seekp(0, indexlist.end);
			index_rec.ptr = indexlist.tellp();
			list_rec.ptr = -1;
			indexlist.write(reinterpret_cast<char *>(&list_rec), sizeof(List));
			//cout<<"INDEXING In this Line...!"<<endl;
			indexFile.seekp(0, indexFile.end);
			indexFile.write(reinterpret_cast<char *>(&index_rec),
					sizeof(Index));
		} else {

			//now the tmp contain the index file seek number
			indexFile.seekg(tmp);
			Index _secondarytmp;
			indexFile.read(reinterpret_cast<char *>(&_secondarytmp),
					sizeof(Index));
			//cout<<"_secondarytmp.pointer :: "<<_secondarytmp.pointer<<endl;

			//cout<<"secondary PTR : "<<_secondary.pointer<<endl;
			indexlist.seekg(_secondarytmp.ptr);
			List _SILtmp;
			indexlist.read(reinterpret_cast<char *>(&_SILtmp), sizeof(List));

			while (_SILtmp.ptr != -1) {
				//cout<<"_SILtmp.ptr :: "<<_SILtmp.ptr<<endl;
				indexlist.seekg(_SILtmp.ptr);
				indexlist.read(reinterpret_cast<char *>(&_SILtmp),
						sizeof(List));
			}
			//cout<<"secondary ptr is > -1"<<endl;
			//cout<<"_SILtmp.ptr : "<<_SILtmp.ptr<<" ACCN :  "<<_SILtmp.accountNumber<<endl;

			int pos = indexlist.tellg();
			indexlist.seekp(pos - sizeof(List));
			_SILtmp.ptr = _indCounter * sizeof(List);
			list_rec.ptr = -1;
			indexlist.write(reinterpret_cast<char *>(&_SILtmp), sizeof(List));

			indexlist.seekp(0, std::ios::end);
			//cout<<"Befor writing :: "<<_secondaryList.accountNumber<<endl;
			indexlist.write(reinterpret_cast<char *>(&list_rec), sizeof(List));

		}

		for (int i = 0; i < 2; i++) {
			data.read(reinterpret_cast<char *>(&num[0]), 3);
			stringstream ss2;
			ss2 << num;
			ss2 >> seek_size;
			//cout<<"Seek Size ::   "<<num<<"       "<<seek_size<<endl;
			int pos = data.tellg();
			data.seekg(pos + seek_size);
			//string str(seek_size,'\0');
			//data.read(reinterpret_cast<char *>(&str[0]),seek_size);
		}
		string delemeter(3, '\0');
		data.read(reinterpret_cast<char *>(&delemeter[0]), 3);

		//cout<<"delemeter :: "<<delemeter<<endl;
		_indCounter++;

	}
	cout << "INDEXING DONE!" << endl;
}

//return -2 if the record not exist and seek size if exist
int Parser::checkExist(string name, fstream &file) {
	file.clear();
	file.seekg(0);
	Index word;
	int seekSize = 0;
	file.read(reinterpret_cast<char *>(&word), sizeof(Index));
	while (!file.eof()) {
		int carrlength = 0;
		for (int i = 0; i < 15; i++) {
			if (word.word[i] == '\0')
				break;
			else
				carrlength++;
		}
		string CName(word.word, carrlength);

		//cout<<"compare : "<<name<<"  ::and::  "<<CName<<"   ::  "<<CName.compare(name)<<endl;
		if (CName.compare(name) == 0) {
			file.clear();
			return seekSize;
		}
		seekSize += sizeof(Index);
		file.read(reinterpret_cast<char *>(&word), sizeof(Index));
	}
	file.clear();
	return -2;
}

void Parser::sortSecondary() {
	cout << "\nSORTING PLEAS WAIT..." << endl;
	fstream secFile(_index_file.c_str(), ios::in | ios::out | ios::binary);
	if (!secFile) {
		cerr << "sort secondary index cann't open" << endl;
		return;
	}
	secFile.clear();
	secFile.seekg(0);
	int count = 0;
	Index tmp;

	secFile.read(reinterpret_cast<char *>(&tmp), sizeof(Index));
	while (!secFile.eof()) {
		count++;
		// cout<<"LAst ::: "<<tmp.LName<<endl;
		secFile.read(reinterpret_cast<char *>(&tmp), sizeof(Index));

	}
	vector<Index> arr(count);
	secFile.clear();
	secFile.seekg(0);
	//SIndex ttmp;
	for (int i = 0; i < count; i++) {
		secFile.read(reinterpret_cast<char *>(&arr[i]), sizeof(Index));
	}

	/* for(int i=0 ; i<count ; i++)
	 cout<<"Not Sorted Arr :: "<<arr[i].LName<<endl;*/
	Index key;
	for (int i = 0; i < count - 1; i++) {
		for (int j = i; j >= 0; j--) {

			string s2 = tostring(arr[j + 1].word);
			string s1 = tostring(arr[j].word);
			// cout<<"S2 :: "<<s2<<"\n"<<"S1 :: "<<s1<<" \n :: compare ::"<<s1.compare(s2)<<endl;
			if (s1.compare(s2) > 0) {
				key = arr[j + 1];
				arr[j + 1] = arr[j];
				arr[j] = key;
			}
		}
	}
//for(int i=0 ; i<count ; i++)
//	cout<<" Sorted Arr :: "<<arr[i].LName<<endl;

//fstream secondary("secondaryIndex.dat" ,ios::in | ios::out | ios::binary);

	secFile.seekp(0);
	for (int i = 0; i < count; i++) {
		secFile.write(reinterpret_cast<char*>(&arr[i]), sizeof(Index));
	}

	cout << "SORTING DONE!" << endl;

}

int* Parser::binarySearch_by_Name(string &l_name) {
	fstream secFile(_index_file.c_str(), ios::in | ios::out | ios::binary);
	fstream secList(_list_file.c_str(), ios::in | ios::out | ios::binary);
	fstream sorce(_dataContaner_file.c_str(), ios::in | ios::out | ios::binary);
	if (!secFile || !secList) {
		cerr << "search secondary index cann't open" << endl;
		return 0;
	}

	secFile.clear();
	secFile.seekg(0);
	secList.clear();
	secList.seekg(0);
	sorce.clear();
	sorce.seekg(0);
	int count = 0;
	Index tmp;

	secFile.read(reinterpret_cast<char *>(&tmp), sizeof(Index));
	while (!secFile.eof()) {
		count++;

		secFile.read(reinterpret_cast<char *>(&tmp), sizeof(Index));
	}

	Index *arr = new Index[count];
	secFile.clear();
	secFile.seekg(0);
	//SIndex ttmp;
	for (int i = 0; i < count; i++) {
		secFile.read(reinterpret_cast<char *>(&arr[i]), sizeof(Index));
	}

	//binary search..
	int main_index = binary_search(arr, l_name, 0, count - 1);
	if (main_index == -1) {
		cerr << "\n No results of word : " << l_name << endl;
		secFile.close();
		secList.close();
		sorce.close();
		return 0;
	} else {
		int recCount = 0;
		if (arr[main_index].ptr == -1) {
			cerr << "\n No results of word : " << l_name << endl;
			secFile.close();
			secList.close();
			sorce.close();
			return 0;
		} else {
			int result[100];
			recCount++;
			List tmp;
			secList.seekg(arr[main_index].ptr);
			secList.read(reinterpret_cast<char *>(&tmp), sizeof(List));
			result[recCount - 1] = tmp.byte_offset;
			while (tmp.ptr != -1) {
				recCount++;
				secList.seekg(tmp.ptr);
				secList.read(reinterpret_cast<char *>(&tmp), sizeof(List));
				result[recCount - 1] = tmp.byte_offset;
			}
			//cout<<"recCount :: "<<recCount<<endl;
			vector<int> finalResult(recCount);
			Prime_record founds;
			/*cout<<"\n"<<left<<setw(10)<<"ID"
			 <<setw(16)<<"Last Name"
			 <<setw(11)<<"FirstName"
			 <<setw(10)<<setprecision(2)<<right<<fixed
			 <<"Balance"<<setw(13)
			 <<"BranchID \n"<<endl;*/
			cout << "    " << recCount << "  results :\n" << endl;
			for (int i = 0; i < recCount; i++) {
				finalResult[i] = result[i];
				read_record(sorce, founds, finalResult[i]);
				cout << i + 1 << " - IN FILE : " << founds.sorce_file
						<< "   the word (" << founds.word << " ) in offset : "
						<< founds.byte_offset << "\n" << endl;
				cout << "        " << getWordFromSorce(founds) << "\n\n"
						<< endl;
				//sorce.read(reinterpret_cast<char *>(&client),sizeof(ClientData));
				//outputLine(cout , client);
				//cout<<"rec Pos :: "<<finalResult[i]<<endl;
			}
			secFile.close();
			secList.close();
			sorce.close();
			return 0;
		}
	}
}

int Parser::binary_search(Index A[], string &key, int min, int max) {
	if (max < min)
		return -1;
	else {
		int mid = (min + max) / 2;
		string s = tostring(A[mid].word);
		if (s.compare(key) < 0)
			return binary_search(A, key, mid + 1, max);
		else if (s.compare(key) > 0)
			return binary_search(A, key, min, mid - 1);
		else
			return mid;
	}
}

string Parser::tostring(char a[]) {
	int carrlength = 0;
	for (int i = 0; i < 15; i++) {
		if (a[i] == '\0')
			break;
		else
			carrlength++;
	}
	string CName(a, carrlength);
	return CName;

}

void Parser::read_record(fstream &sorce, Prime_record &record,
		int byte_offset) {

	sorce.clear();
	sorce.seekg(byte_offset);
	int seek_size = 0;
	string num(3, '\0');
	stringstream ss, ss2, ss3, ss4, ss5, ss6;
	sorce.read(reinterpret_cast<char *>(&num[0]), 3);
	ss << num;
	ss >> seek_size;
	string word(seek_size, '\0');
	sorce.read(reinterpret_cast<char *>(&word[0]), seek_size);
	ss2 << word;
	ss2 >> record.word;
	sorce.read(reinterpret_cast<char *>(&num[0]), 3);
	ss3 << num;
	ss3 >> seek_size;
	string byte(seek_size, '\0');
	sorce.read(reinterpret_cast<char *>(&byte[0]), seek_size);
	ss4 << byte;
	ss4 >> record.byte_offset;

	sorce.read(reinterpret_cast<char *>(&num[0]), 3);
	ss5 << num;
	ss5 >> seek_size;
	string sorce_file(seek_size, '\0');
	sorce.read(reinterpret_cast<char *>(&sorce_file[0]), seek_size);
	ss6 << sorce_file;
	record.sorce_file = ss6.str();
	int pos = sorce.tellg();
	sorce.seekg(pos + 3);
//cout<<"in method:: "<<word<<"  "<<byte<<"  "<<sorce_file<<endl;
}

string Parser::getWordFromSorce(Prime_record &wordData) {
	ifstream wordfile(wordData.sorce_file.c_str());
	if (!wordfile) {
		cerr << wordData.sorce_file << "   file not found! " << endl;
		return "EROOR";
	}

	if (wordData.byte_offset >= 30) {
		wordfile.seekg(wordData.byte_offset - 30);
	} else {
		wordfile.seekg(0);
	}
	string sentence(70, '\0');
	wordfile.read(reinterpret_cast<char *>(&sentence[0]), 70);
	sentence.erase(std::remove(sentence.begin(), sentence.end(), '\n'),
			sentence.end());
	return sentence;

}

void Parser::setDataContanerName(string name) {
	_dataContaner_file = name;
}
void Parser::setFileOfFilsName(string name) {
	_fileOfFiles = name;
}
void Parser::setIndexName(string name) {
	_index_file = name;
}
void Parser::setListName(string name) {
	_list_file = name;
}
void Parser::setPathName(string name) {
	_path_name = name;
}

string Parser::getDataContanerName() const {
	return _dataContaner_file;
}
string Parser::getFileOfFilsName() const {
	return _fileOfFiles;
}
string Parser::getIndexName() const {
	return _index_file;
}
string Parser::getListName() const {
	return _list_file;
}
string Parser::getPathName() const {
	return _path_name;
}
