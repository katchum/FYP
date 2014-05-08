#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

//get a alignment record, ignore the header, add 1 to then end
class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str,line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();

            	while(std::getline(lineStream,cell,'\t'))
            	{
                m_data.push_back(cell);
            	}
            	m_data.push_back("0");
        }
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str,CSVRow& data)
{
    data.readNextRow(str);
    return str;
}
//clear
double getGCratio(CSVRow read){
	double count = 0;
	int i = 0;
	while(i<read[9].length()){
		if(read[9][i] == 'G' ||read[9][i] == 'C')
			count++;
		i++;
	}
	std::cout << read[9].length() << "\t";
	std::cout << count << "\t";
	return count/read[9].length();
}
//clear
std::string getMAPQ(CSVRow read){
	return read[4];
}

int getFlag(CSVRow read){
	int flag = atoi(read[1].c_str());
	return flag;
}

//Only works on wgsim files
bool isReversed(CSVRow read){
	int flag = atoi(read[1].c_str());
	std::bitset<12> foo(flag);
	if (foo[4] == 1)
		return true;
	else
		return false;
}
bool isFirst(CSVRow read){
	int flag = atoi(read[1].c_str());
	std::bitset<12> foo(flag);
	if (foo[6] == 1)
		return true;
	else
		return false;
}

bool isUnmapped(CSVRow read){
	int flag = atoi(read[1].c_str());
		std::bitset<12> foo(flag);
		if (foo[3] == 1)
			return true;
		else
			return false;
}


//Only works on wgsim files, possibly unmapped
bool isCorrect(CSVRow read, int gap){
    std::size_t f0 = read[0].find("_");
   	std::size_t f1 = read[0].find("_", f0+1);
 	std::size_t f2 = read[0].find("_", f1+1);
   	int start = atoi(read[0].substr (f0+1, f1-f0-1).c_str());
	int end = atoi(read[0].substr(f1+1, f2-f1-1).c_str());
	if(read[0].substr(0, f0) == read[2]){
		if(isReversed(read)){
			if(gap > abs(atoi(read[3].c_str())-end))
				return true;
		}

		if(!isReversed(read))
			if(gap > abs(atoi(read[7].c_str())-start))
				return true;

	}
	return false;
}

int main()
{
    std::ifstream       file("chr1sim.sam");
    CSVRow              read;
    int gap = 20;
	while (file.peek() == '@'){
		file >> read;
	}
	int count = 0;
    while(file >> read)
    {
    //	EvaluateRead(read);
    /*//print
    	int i = 0;
    	while(i < read.size()){
        std::cout << i << ":" << read[i] << " ";
    	i++;
    	}
    	std::cout << "\n";
*/
    	if (!isUnmapped(read)){
    		if (!isCorrect(read, gap)){
    	    	count++;
        	}
    	}
    }
    std::cout << count << "\n";
    return 1;
}
