#include "prot.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <regex>

using namespace std;

/*
 * Type: constructor
 * Name: Prot
 * Description: Create an empty Prot object
 * Parameters: null
 * Return: null
*/
 Prot::Prot(){
	m_file = "";
	
	//Generate the m_ref table (aa weights) only once
	if(Prot::m_ref_todo){
		Prot::init_ref();
		Prot::m_ref_todo=false;
	}
}

/*
 * Type: constructor
 * Name: Prot
 * Description: Create a Prot object
 * Parameters: string path_to_protein_file
 * Return: null
*/
 Prot::Prot(const string& file){
	m_file = file;
	
	//Generate the m_ref table (aa weights) only once
	if(Prot::m_ref_todo){
		Prot::init_ref();
		Prot::m_ref_todo=false;
	}
}

/*
 * Type: method (Prot)
 * Name: get_file
 * Description: Return the file path of the protein
 * Parameters: null
 * Return: string protein_file_path
*/
string Prot::get_file(){return m_file;}

/*
 * Type: method (Prot)
 * Name: cut_misscleavage_0
 * Description: get all peptides position of the protein without cleavage and calculate for each peptide its weight without H2O
 * Parameters: null
 * Return: vector<pair<u_int peptide_position, float peptide_weight> >
*/
vector<pair<unsigned int, float> > Prot::cut_misscleavage_0(){
	string seq=m_file; //TODO: read the file of the protein, not directly a sequence
	
	
	vector<pair<unsigned int, float> > table(1, make_pair(seq.size(),0)); //Create the output table and initialised it with the last_pos+ of the sequence => To get the last peptide, of the sequence, size
	
	
	
	regex e (Prot::m_motif);
	smatch sm;
	
	while(regex_match(seq, sm, e)){
		regex_match(seq, sm, e);//Search motif (from the right)
		table.insert(table.begin(),make_pair(sm.position(2), calc_weight(sm.str(2))));//Add pair<peptide_position, peptide_weight> at the beginning of table
		
		seq = sm.str(1);//remove the peptide from the read sequence
	}
	
	table.insert(table.begin(),make_pair(0, calc_weight(seq)));//Add the last peptide (the first in the sequence in fact)

	return table;	
}

//statics methods and attributs
map<char, float> Prot::m_ref={}; //Initialisation of the has_table of aa weights
bool Prot::m_ref_todo=true;//Does the m_ref must be generated ?

string  Prot::m_motif="(.*[KR])([^P].*)"; //motif to cut (1)(2)

/*
 * Type: static method (Prot)
 * Name: calc_weight
 * Description: calculate the weight of a peptide using de m_ref hash table
 * Parameters: string peptide_sequence
 * Return: float peptide_weight
*/
float Prot::calc_weight(const string& pep){
	float weight=0;
	const unsigned int s=pep.size();
	
	for(unsigned int i=0; i < s; i++){//Read all aa of the peptide sequence
		weight += m_ref.at(pep.at(i));//Add the weight of the current aa
	}
	return weight;
}

/*
 * Type: static method (Prot)
 * Name: init_ref
 * Description: generate the m_ref hash table containing aa weights
 * Parameters: string csv_file_of_aa_weights
 * Return: null
*/
void Prot::init_ref(const std::string& file){
	ifstream myfile (file);
	if (myfile.is_open()){
		m_ref.clear();
		string line;
		while ( getline (myfile,line) ){
			m_ref[line[0]]=stof(line.substr(2));
    		}
    		myfile.close();
  	}
	else cerr << "Unable to open file" << endl;
}
