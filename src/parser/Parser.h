#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Scanner.h"

using namespace std;

const string SEMICOLON = "SEMICOLON";
const string IF = "IF";
const string THEN = "THEN";
const string END = "END";
const string REPEAT = "REPEAT";
const string UNTIL = "UNTIL";
const string ASSIGN = "ASSIGN";
const string READ = "READ";
const string WRITE = "WRITE";
const string LESSTHAN = "LESSTHAN";
const string EQUAL = "EQUAL";
const string PLUS = "PLUS";
const string MINUS = "MINUS";
const string MULT = "MULT";
const string DIV = "DIV";
const string OPENBRACKET = "OPENBRACKET";
const string CLOSEDBRACKET = "CLOSEDBRACKET";
const string IDENTIFIER = "IDENTIFIER";
const string NUMBER = "NUMBER";
const string CONST = "CONST";
const string ERROR = "ERROR";
const string ROOT = "ROOT";

const string EXP = "EXP";
const string EXPWITHBRACKETS = "(EXP)";
const string SIMPLEEXP = "SIMPLEEXP";
const string TERM = "TERM";

class Parser
{
private:

	class Node
	{
	public:
		string typeOfproc{};
		string valueAssociated{};

		Node* nextNode{ nullptr };

		Node* firstChild{ nullptr };
		Node* secondChild{ nullptr };
	};

	vector<string> tokenTypes;
	vector<string> tokenValues;
	int currentNodeIndex;


	Node* root;
	int currentParsingIndex;
	bool errorFlag;

	void traverseDelete(Node*& currentNode);

	void procStmtSeq(Node*& currentNode);

	void procIf(Node*& currentNode);

	void procRepeat(Node*& currentNode);

	void procRead(Node*& currentNode);

	void procWrite(Node*& currentNode);

	void procAssign(Node*& currentNode);

	void procExp(Node*& currentNode);

	void procSimpleExp(Node*& currentNode);

	void procTerm(Node*& currentNode);

	void procFactor(Node*& currentNode);

	string stringToShowForNode(Node* node, int nodeIndex);

public:
	Parser(vector<string> tokenT, vector<string> tokenV);
	~Parser();

	Node* getRoot() {
		if (!errorFlag)
		{
			return root;
		}
		else
		{
			return nullptr;
		}
	};

	string stringToShow(Node* rootNode);

};
