#include <iostream>
#include <vector>
#include <utility>

#include "prot.hpp"

using namespace std;

//Informations of a peptide
struct Peptide{
	Prot* protein;//The protein of this peptide
	unsigned int posB;//The position of the beginning of the peptide in the sequence of the protein
	unsigned int posE;//The poistion of the end
};

int main(int argc, char* argv[]){
	const unsigned int mc = 4; //misscleavage
	const float water=18.01056; //water weight
	
	vector<string> protFiles;
	protFiles.push_back("IIPAGIHNERAIKPKRGAPGHAIPRAGARPPNGRRARGIKAHVAE"); //test
	protFiles.push_back("IGARAKPNREIIKPGHAKKGIRAVWY");
	
	//get db proteins
	vector<Prot> proteins;//vector of proteins
	for(unsigned int i=0; i < protFiles.size(); i++){
		proteins.emplace_back(Prot(protFiles[i]));
	}

	map<float, vector<Peptide> > peptides; //hash table of all possibles peptides sort by keys: [float:weight] => {Prot*:protein, u_int:posB, u_int:posE}

	//generate for each db proteins their peptides
	for(unsigned int i=0; i < proteins.size(); i++){
		//Get peptide with a misscleavage of 0
		vector<pair<unsigned int, float> > peps = proteins[i].cut_misscleavage_0();
		
		//Calculate of peptides (with misscleavage to)
		const unsigned int nb_peps0=peps.size()-1;//- because the last value is not a peptide 
		for(unsigned int p=0; p < nb_peps0; p++){
			Prot* thisProt=&proteins[i];
			//Get value of peptide with a misscleavage of 0
			float weight = peps[p].second+water;
			unsigned int posB = peps[p].first;
			
			//Add peptide with a misscleavage of 0
			peptides[weight].emplace_back(Peptide{thisProt, posB, peps[p+1].first-1});

			//For each miss cleavage value
			for(unsigned int m=1; m <= mc; m++){
				//Test if the peptide is possible
				if(p+m < nb_peps0){
					//Calculate the peptide
					weight += peps[p+m].second; //Add the cleaved peptide weight to the previous peptide weight
					peptides[weight].emplace_back(Peptide{thisProt, posB, peps[p+m+1].first-1}); //Add this new cleaved peptide
				}
			}
		}

		//tests
		cout << "Position\tWeight" << endl;
		string seqCutPos;
		for(unsigned int i=0; i < peps.size(); i++){
			while(peps[i].first > seqCutPos.size()){seqCutPos+=' ';}
			seqCutPos+='^';
			cout << peps[i].first << "\t\t" << peps[i].second << endl;
		}

		cout << endl;
		cout << "protein: " << &proteins[i] << endl;
		cout << "sequence: " << proteins[i].get_file() << endl;
		cout << "Cut pos:  " << seqCutPos << endl;
		//tests-end
	}

	

	//Test
	cout << endl << "Peptides:" << endl << "Weight\t\tPositionB\tPositionE\tProtein\t\tSequence" << endl;
	for(map<float,vector<Peptide> >::const_iterator it=peptides.cbegin(); it != peptides.cend(); it++){
		for(unsigned int j=0; j<it->second.size(); j++){
			unsigned int posB=it->second[j].posB;
			unsigned int posE=it->second[j].posE;
			string seq = it->second[j].protein->get_file();
			cout << it->first << "\t\t" << posB << "\t\t" << posE << "\t\t" << it->second[j].protein << "\t" << seq.substr(posB, posE-posB+1)  << endl;
		}
	}
	//Test-end
	
	return 0;
}
