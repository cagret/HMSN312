#include <iostream>
#include <iomanip> //setprecision
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <locale>

#include "parseXml.hpp"

using namespace std;

double  extractPeaks(string line){
	size_t i=0;
	while(i < line.size() && (isdigit(line[i]) || line[i]=='.')){i++;}
	
	return 	stod(line.substr(0, i));
}

void parseXML(string& file){
	string idProt, seq, charge;
	vector<float> peaks;

	//open file
	ifstream ifs;

	ifs.open("identifications.xml", ifstream::in);

	if(!ifs.is_open()){
		cerr << "Error: impossible to open " << file << endl;
		exit(1);

	}
	string line;
	string actualSeq="";
	bool inPeaks=false;

	smatch m;
	regex seq_regex ("<idi:sequence>(.+)</idi:sequence>");
	regex peaks_start_regex("<ple:peaks>.*");
	regex peaks_end_regex(".*</ple:peaks>");

	while(getline(ifs, line).good()){
		if(inPeaks){
			//TODO
			//Test if end of peaks => write data on vector then inPeaks=false; 
			if(regex_match(line, regex(".*(</ple:peaks>).*"))){
				inPeaks=false;
			}
			//Else read peak
			else{
				double peak = extractPeaks(line);
				cout << "\t" << setprecision(10) << peak << endl;
			}
		}
		else{
			//TODO
			//Test if seq line
			if(regex_search(line, m, seq_regex)){
				actualSeq = m[1];
				cout << "actual seq: " << actualSeq << endl;
			}
			if(regex_match(line, regex(".*(<ple:peaks>).*"))){
				inPeaks=true;
			}
			//Test if start peaks line => inPeaks=true;
		}
		//cout << line << endl;	
	}

	ifs.close();

}

