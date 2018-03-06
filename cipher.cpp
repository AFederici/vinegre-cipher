#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include <algorithm>
#include<map>
#include <numeric>
#include <functional>
#include <deque>

using namespace std;

//this function is very inefficient
string fileToString(string file){ //.txt file
	ifstream ifs(file);
	string str( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	string str2;
	std::remove_copy_if(str.begin(), str.end(),
                        std::back_inserter(str2), //Store output
                        std::ptr_fun<int, int>(&std::ispunct)
                       );

	string::iterator end_pos = remove(str2.begin(), str2.end(), ' ');
	str2.erase(std::remove(str2.begin(), str2.end(), '\n'), str2.end());
	str2.erase(end_pos, str2.end());
	for(auto elem : str2){
		tolower(elem);
	}
	return str2;

}

string encrypt(string str, string key)
{
    string cipher_text;

    for (int i = 0; i < str.size(); i++)
    {
        // converting in range 0-25
        int x = (((str[i] - 'a') + (key[i%key.length()] - 'a')) % 26)  + 'a';
        // convert into alphabets(ASCII)

        cipher_text.push_back(char(x));
    }
    return cipher_text;
}

string decrypt(string cipher_text, string key)
{
    string orig_text;

    for (int i = 0 ; i < cipher_text.size(); i++)
    {
        // converting in range 0-25
        int x = (cipher_text[i] - key[i%key.length()] + 26) % 26 + 'a';
        orig_text.push_back(x);
    }
    return orig_text;
}

int gcd(int a,int b) {
    int temp;
    while(b > 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int findSpacing(int ind, int sub){
	int index = ind; //should start as 0
	int sub_size = sub;
	vector<size_t> arr; // holds all the positions that sub occurs within str
	ifstream ifs("example2.txt");
	if (ifs)
	{
		string content( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
		string firstTwo = content.substr(index,sub_size);


		size_t pos = content.find(firstTwo, ind);
		while(pos != string::npos)
		{
			arr.push_back(pos);
			pos = content.find(firstTwo,pos+1);
		}

		return accumulate(arr.begin(), arr.end(), arr[0], gcd);

	}

	else cout << "Unable to open file";
	return -1;
}


int findSpacingPtTwo(){
	vector<int> composition;
	int s_list[2] = {2, 3}; //bigram analysis
	for(int a = 0; a < sizeof(s_list)/sizeof(s_list[0]); ++a ){
		int s = s_list[a];
		for (int i = 0; i < 100;  ++i)
		{
			int x = findSpacing(i, s);
			if (x > 1){ composition.push_back(x);}

		}
	}

	int max = 0;
	int mostvalue = composition[0];
    for(int i=0; i<composition.size(); i++)
    {
        int co = (int)count(composition.begin(), composition.end(), composition[i]);
        if(co > max)
        {       max = co;
                mostvalue = composition[i];
        }
    }

	return mostvalue;
}

vector<vector<string>> findKeyWord(vector<int> len){

	ifstream ifs("example2.txt");
	vector<vector<string>> keys;
	string content( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	int totalLetters = content.length();

	for (int a = 0; a < sizeof(len)/sizeof(len[0]); ++a ) //goes thorugh the different possible key lengths (runs 3 times max)
	{
		int increment = len[a];
		cout << increment << endl;
		map<char, double> my_map = {
			{'e', 12.02},{'t', 9.10},{'a', 8.12 },{'o', 7.68},{'i', 7.31 },{'n', 6.95 },{'s', 6.327 },
			{'r',6.02 },{'h', 6.094 },{'d', 4.32 },{'l', 3.98},{'u', 2.88},{'c', 2.71},{'m', 2.61},
			{'f', 2.30 },{'y', 2.11},{'w', 2.09 },{'g', 2.03 },{'p', 1.82 },{'b', 1.49},{'v', 1.11 },{'k', 0.69 },
			{'x', 0.17},{'j', 0.11 },{'q', 0.10},{'z', .07}
			};
		for(auto& p : my_map) {
			p.second = p.second * (totalLetters * 1.0 / increment) / 100;
		}
		vector<string> columnKey;
		map<char, int> usedUp;
		for (int x = 0; x < increment; ++x){ //goes through the columns
			map<char, int> mapOfWords;			//this is for one column (there are 5 if the key is 5 long)
			for (char b = 'a'; b < 'z'; b++){
				mapOfWords[b] = 0;
			}
			int columnMax = 0;
			string columnString = "";
			for (int i=x; i< totalLetters; i+=increment){
				char word = content[i];
				mapOfWords[word] += 1;
				if (mapOfWords[word] > columnMax){
					columnMax = mapOfWords[word];
					columnString = word;
				  }
			}
			deque<double> best_chi = {100000, 1000000};
			deque<char> best_shift = {'a', 'a'};
			for (int shift = 0; shift < 25; shift++){
				double chi = 0;
				for(auto& p : my_map){
					chi += (my_map[p.first] - mapOfWords[char(p.first + (shift % 26))]) * (my_map[p.first] - mapOfWords[char(p.first + (shift % 26))]);
				}
				if (chi < best_chi[0] && chi < best_chi[1]){

					best_chi.push_front(chi);
					best_shift.push_front(char('a'+shift));
					best_chi.pop_back();
					best_shift.pop_back();
					//if ( usedUp.find(char('a' + shift)) == usedUp.end()) {
					//best_chi = chi;
					//best_shift = char('a'+shift);
					//}
				}
				else if (chi < best_chi[1]){
					best_chi.pop_back();
					best_shift.pop_back();
					best_chi.push_back(chi);
					best_shift.push_back(char('a'+shift));
				}
			}

			//usedUp[best_shift] = 1;
			cout << best_shift[0] << " , " << best_shift[1] << endl;
			string send = best_shift[0] + " ; " + best_shift[1];





			columnKey.push_back(send);

		}

		keys.push_back(columnKey);


	}
	return keys;

}

void solver(){
	vector<int> possibleLens;
	int multiple = findSpacingPtTwo();	//the answer is this number or a multiple of it (ex: 3 means it could be a 3 or 6 keyword most likely)
	cout << "Multiple of: " << multiple << endl << endl;
	switch (multiple){
		case 2: possibleLens.push_back(2); possibleLens.push_back(4); possibleLens.push_back(6); possibleLens.push_back(8); break; //key wont be two
		case 3: possibleLens.push_back(3); possibleLens.push_back(6); break;
		case 4: possibleLens.push_back(4); possibleLens.push_back(8); break;
		default: possibleLens.push_back(multiple);
	}

	vector<vector<string>> answer = findKeyWord(possibleLens);
}



int main(){
	string sentence = fileToString("example.txt");
	//cout << "text" << endl << sentence << endl << endl;
	string encryption = encrypt(sentence, "secret");
	//cout << "encryption" << endl << encryption << endl << endl;

	//solver();
	cout << "decryption " << endl << decrypt(encryption, "secret");
	return 0;
}
