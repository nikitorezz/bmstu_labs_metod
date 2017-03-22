#include <fstream>;
#include <iostream>;
#include <string>;
#include <vector>;


unsigned int Hash(const char* str) {
	unsigned int hash = 0;
	for (; *str; str++) {
		hash = hash * 1664525 + (unsigned int)(*str) + 1013904223;
	}
	return hash;
}

class TTree {

protected:

	TTree * Right;
	TTree * Left;
	std::string Str;
	uint32_t Hash;

public:

	uint32_t Num;

	TTree() 
		: Right(nullptr)
		, Left(nullptr)
		, Hash(0)
		, Num(0){
	}

	TTree(const uint32_t& hash, std::string str, uint32_t num, TTree * right = nullptr, TTree * left = nullptr)
		: Hash(hash)
		, Right(right)
		, Left(left)
		, Str(str)
		, Num(num){
	}

	~TTree() {
		delete Right;
		delete Left;
	}

	bool Add(uint32_t hash, std::string str, uint32_t num) {
		TTree * temp = this;
		while (true) {
			if (Num == 0 && Hash == 0) {
				temp->Hash = hash;
				temp->Str = str;
				temp->Num = num;
				return 0;
			}
			if (temp->Hash == hash) {
				return num;
			}
			if (hash < temp->Hash) {
				if (temp->Left == nullptr) {
					temp->Left = new TTree(hash, str, num);
					return 0;
				}
				else {
					temp = temp->Left;
				}
			} else {
				if (temp->Right == nullptr) {
					temp->Right = new TTree(hash, str, num);
					return 0;
				}
				else {
					temp = temp->Right;
				}
			}
		}
		temp = nullptr;
	}

	TTree * Find(uint32_t hash) {
		TTree * temp = this;
		while (true) {
			if ((hash < temp->Hash && temp->Left == nullptr) || (hash > temp->Hash && temp->Right == nullptr)) {
				return nullptr;
			}
			if (hash == temp->Hash) {
				return temp;
			}
			if (hash < temp->Hash) {
				temp = temp->Left;
			}
			else {
				temp = temp->Right;
			}
		}
		temp == nullptr;
		return nullptr;
	}

	friend class File;
};


class File {
	
	uint32_t Ammount;
	TTree *Root = nullptr;
	
public:

	File() 
		:Ammount(0){
			Root = new TTree();
	}

	~File() {
		delete Root;
	}

	bool Add(const uint32_t& hash, std::string str) {
		Ammount++;
		return Root->Add(hash,str,Ammount);
	}

	TTree * Find(uint32_t hash) {
		return Root->Find(hash);
	}
};

int main() {
	std::string filename1("laba1.txt"); // name of the file 1
	setlocale(LC_ALL, "rus");
	std::ifstream f_in1(filename1);
	unsigned int i1(0);
	std::vector<uint32_t> vec;
	File * f1 = new File();
	if (!f_in1.is_open()) {
		std::cout << "error" << std::endl;
	}
	else {
		int i = 1;
		bool temp;
		for (std::string line; std::getline(f_in1, line); i++){
			i1 = Hash(line.c_str());
			temp = f1->Add(i1, line);
			if (!temp) {
				vec.push_back(i);
			}
		}
	}


	std::string filename2("laba2.txt"); // name of the file 2
	setlocale(LC_ALL, "rus");
	std::ifstream f_in2(filename2);
	unsigned int i2(0);
	if (!f_in2.is_open()) {
		std::cout << "error" << std::endl;
	}
	else {
		int j = 1;
		TTree * temp = nullptr;
		for (std::string line; std::getline(f_in2, line);) {
			temp = f1->Find(Hash(line.c_str()));
			if (temp == nullptr) {
				std::cout << "file: 2, line: " << j << ", : " << line << std::endl;
			}
			else {
				vec[temp->Num-1] = 0;
			}
		}
	}

	f_in1.clear();
	f_in1.seekg(0, std::ios_base::beg);
	if (!f_in1.is_open()) {
		std::cout << "error" << std::endl;
	}
	else {
		int k = 0;
		for (std::string line; std::getline(f_in1, line);k++) {
			if (vec[k] != 0) {
				std::cout << "file: 1, line: " << k + 1 << ", : " << line << std::endl;
			}
		}
	}
	system("pause");
}
