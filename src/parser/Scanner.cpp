#include "Scanner.h"

using namespace std;

bool isIdentifier(string str) {
	for (auto& s : str)
	{
		if (!isalpha(s))
		{
			return false;
		}
	}
	return true;
}

bool isNumber(string str) {
	for (auto& s : str)
	{
		if (!isdigit(s))
		{
			return false;
		}
	}
	return true;
}

vector<vector<string>> scannerFunction(string allStreamInString) {

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

		if (currentChar == '{')
		{
			currentChar = allStreamInString[++i];
			if (!currentString.empty())
			{
				endFlag = true;
			}
			while (currentChar != '}')
			{
				currentChar = allStreamInString[i++];
			}
		}
		else if (currentChar == ' ' || currentChar == '\n' || currentChar == '\r')
		{
			i++;
			if (!currentString.empty())
			{
				endFlag = true;
			}
		}
		else if (search == specialChars.end())
		{
			currentString += currentChar;
			i++;
			mustBeToken = false;
		}
		else if (mustBeToken)
		{
			currentString += currentChar;
			i++;
			mustBeToken = false;

			if (currentChar != ':')
			{
				endFlag = true;
			}
			if (currentChar == ':')
			{
				mustBeToken = true;
			}
		}
		else
		{
			endFlag = true;
		}

		if (endFlag && !currentString.empty())
		{
			stringVector.push_back(currentString);
			endFlag = false;
			currentString.clear();
			mustBeToken = true;
		}
	}
	if (!currentString.empty())
	{
		stringVector.push_back(currentString);
	}

	vector<vector<string>> stringReturn{};
	for (i = 0; i < stringVector.size(); i++)
	{
		auto token = tokenValueVsType.find(stringVector.at(i));
		if (!stringVector.at(i).empty())
		{
			if (token != tokenValueVsType.end())
			{
				vector<string> temp{ tokenValueVsType.at(stringVector.at(i)), stringVector.at(i) };
				stringReturn.push_back(temp);
			}
			else if (isNumber(stringVector.at(i))) {
				vector<string> temp{ "NUMBER", stringVector.at(i) };
				stringReturn.push_back(temp);
			}
			else if (isIdentifier(stringVector.at(i))) {
				vector<string> temp{ "IDENTIFIER", stringVector.at(i) };
				stringReturn.push_back(temp);
			}
			else
			{
				vector<string> temp{ "ERROR" , stringVector.at(i) };
				stringReturn.push_back(temp);
			}
		}
	}

	return stringReturn;
}
