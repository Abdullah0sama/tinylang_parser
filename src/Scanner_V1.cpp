#ifndef SCANNER
#define SCANNER

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <vector>
using namespace std;

const unordered_map<string, string> tokenValueVsType{
		{";", "SEMICOLON"},
		{"if", "IF"},
		{"then", "THEN"},
		{"end", "END"},
		{"repeat", "REPEAT"},
		{"until", "UNTIL"},
		{":=", "ASSIGN"},
		{"read", "READ"},
		{"write", "WRITE"},
		{"<", "LESSTHAN"},
		{"=", "EQUAL"},
		{"+", "PLUS"},
		{"-", "MINUS"},
		{"*", "MULT"},
		{"/", "DIV"},
		{"(", "OPENBRACKET"},
		{")", "CLOSEDBRACKET"}
};

const unordered_set<char> specialChars{
	';',
	':',
	'=',
	'<',
	'+',
	'-',
	'*',
	'/',
	'(',
	')',
	' ',
	'\n',
	'\r',
	'{',
	'}'
};

string scannerFunction(string allStreamInString) {

	string currentString{};
	char currentChar{};
	vector <string> stringVector{};
	bool endFlag{ false };
	bool mustBeToken{ false };
	auto search = specialChars.find(currentChar);

	size_t i = 0;
	while (i < allStreamInString.size())
	{
		currentChar = allStreamInString[i];
		search = specialChars.find(currentChar);

		// Save identifier or number if exist
		if (specialChars.find(currentChar) != specialChars.end() && !currentString.empty()) {
			stringVector.push_back(currentString);
			currentString = "";
		};

		if (currentChar == '{')
		{
			// i < allStreamInString.size() &&
			while ( allStreamInString[++i] != '}') ;
		}
		else if (search == specialChars.end())
		{
			currentString += currentChar;
		}
		else if(tokenValueVsType.find(string(1, currentChar)) != tokenValueVsType.end())
		{
			if (currentChar == ':' && i < allStreamInString.size() && allStreamInString[i + 1] == '=') stringVector.push_back(":=");
			else stringVector.push_back(string(1, currentChar));
		}
		i++;
	}
	
	if (!currentString.empty())
	{
		stringVector.push_back(currentString);
	}

	// vector<string> stringReturn{};
	string ret = "";
	for (i = 0; i < stringVector.size(); i++)
	{
		auto token = tokenValueVsType.find(stringVector.at(i));
		if (!stringVector.at(i).empty())
		{
			if (token != tokenValueVsType.end())
			{
				ret += (stringVector.at(i) + "\t" + tokenValueVsType.at(stringVector.at(i)) + "\n");
			}
			else if (stringVector.at(i).at(0) <= '9' && stringVector.at(i).at(0) >= '0') {
				ret += (stringVector.at(i) + "\t" + "NUMBER" + "\n");
			}
			else if (stringVector.at(i).at(0) >= 'A' && stringVector.at(i).at(0) <= 'z') {
				ret += (stringVector.at(i) + "\t" + "IDENTIFIER" + "\n");
			}
			else
			{
				ret += (stringVector.at(i) + "\t" + "ERROR" + "\n");
			}
		}
	}

	return ret;
}

#endif

//int main()
//{

//	string inputFile = "source.txt";
//	string outputFile = "result.txt";

//	// getting the streams open
//	ofstream output_stream(outputFile);

//	ifstream input_stream;
//	input_stream.open(inputFile);

//	// asserting there is exist files to read
//	if (output_stream.fail() || input_stream.fail())
//	{
//		cout << "error in opening input file or creating the output file";
//	}

//	// copying the input stream to a string
//	string inputString(istreambuf_iterator<char>(input_stream), {});

//	// closing the input stream
//	input_stream.close();

//	// getting the string scanned using the scannerFunction
//	vector<string> result = scannerFunction(inputString);

//	//writing the result to the output stream
//	for (size_t i = 0; i < result.size(); i++)
//	{
//		output_stream << result.at(i);
//	}
//	// closing the output stream
//	output_stream.close();

//}

