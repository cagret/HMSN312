#ifndef prot_hpp
#define prot_hpp

#include <string>
#include <map>
#include <vector>
#include <utility>

class Prot
{
	public:
		Prot();
		Prot(const std::string& file);

		void set_file(std::string& file);
		std::string get_file();

		std::vector<std::pair<unsigned int, float> > cut_misscleavage_0(); //peptides_with_miss_cleavage_0 (pos_start, weight);

	private:
		static std::map<char, float> m_ref;
		static std::string m_motif;
		static void init_ref(const std::string& file="aaWeight.csv");
		static bool m_ref_todo;

		std::string m_file;
		static float calc_weight(const std::string& pep);
		
};

#endif
