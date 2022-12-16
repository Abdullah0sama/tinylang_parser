#include "Parser.h"

using namespace std;

Parser::Parser(vector<string> tokenT, vector<string> tokenV)
	:root{ new Node{} }, tokenTypes{ tokenT },
	tokenValues{ tokenV }, errorFlag{ false },
	currentNodeIndex{}, currentParsingIndex{}
{
	root->typeOfproc = ROOT;
	root->valueAssociated = ROOT;
	procStmtSeq(root);

	if (currentParsingIndex != tokenTypes.size() - 1)
	{
		errorFlag = true;
	}
}

Parser::~Parser()
{
	if (root->nextNode != nullptr)
	{
		traverseDelete(root->nextNode);
	}
	delete root;
	root = nullptr;
}

void Parser::traverseDelete(Node*& currentNode) {
	if (currentNode->nextNode != nullptr)
	{
		traverseDelete(currentNode->nextNode);
	}
	if (currentNode->firstChild != nullptr)
	{
		traverseDelete(currentNode->firstChild);
	}
	if (currentNode->secondChild != nullptr)
	{
		traverseDelete(currentNode->secondChild);
	}
	delete currentNode;
	currentNode = nullptr;
}

void Parser::procStmtSeq(Node*& currentNode) {

	if (currentParsingIndex == tokenTypes.size())
	{
		return;
	}

	Node* newNode{ nullptr };
	if (currentNode->typeOfproc == "")
	{
		newNode = currentNode;
	}
	else
	{
		newNode = new Node{};
		currentNode->nextNode = newNode;
	}

	if (tokenTypes.at(currentParsingIndex) == IF)
	{
		procIf(newNode);
	}
	else if (tokenTypes.at(currentParsingIndex) == REPEAT)
	{
		procRepeat(newNode);
	}
	else if (tokenTypes.at(currentParsingIndex) == READ)
	{
		procRead(newNode);
	}
	else if (tokenTypes.at(currentParsingIndex) == WRITE)
	{
		procWrite(newNode);
	}
	else if (tokenTypes.at(currentParsingIndex) == IDENTIFIER)
	{
		procAssign(newNode);
	}
	else
	{
		errorFlag = true;
		return;
	}


	while (tokenTypes.at(currentParsingIndex) == SEMICOLON)
	{
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
		procStmtSeq(newNode);
	}

}

void Parser::procIf(Node*& currentNode) {

	currentNode->typeOfproc = IF;
	if (currentParsingIndex == tokenTypes.size() - 1)
	{
		return;
	}
	currentParsingIndex++;

	Node* newNode = new Node{};
	procExp(newNode);
	currentNode->firstChild = newNode;


	if (tokenTypes.at(currentParsingIndex) == THEN) {
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
		newNode = new Node{};
		procStmtSeq(newNode);
		currentNode->secondChild = newNode;
	}
	else
	{
		errorFlag = true;
		return;
	}
	if (tokenTypes.at(currentParsingIndex) == END) {
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
	}
	else
	{
		errorFlag = true;
		return;
	}
}

void Parser::procRepeat(Node*& currentNode) {

	currentNode->typeOfproc = REPEAT;
	if (currentParsingIndex == tokenTypes.size() - 1)
	{
		return;
	}
	currentParsingIndex++;

	Node* newNode = new Node{};
	procStmtSeq(newNode);
	currentNode->firstChild = newNode;

	if (tokenTypes.at(currentParsingIndex) == UNTIL)
	{
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
		newNode = new Node{};
		procExp(newNode);
		currentNode->secondChild = newNode;
	}
	else
	{
		errorFlag = true;
		return;
	}
}

void Parser::procRead(Node*& currentNode) {

	currentNode->typeOfproc = READ;

	if (currentParsingIndex == tokenTypes.size() - 1)
	{
		return;
	}
	currentParsingIndex++;

	if (tokenTypes.at(currentParsingIndex) == IDENTIFIER)
	{
		currentNode->valueAssociated = tokenValues.at(currentParsingIndex);
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
	}
	else
	{
		errorFlag = true;
		return;
	}
}

void Parser::procWrite(Node*& currentNode) {

	currentNode->typeOfproc = WRITE;
	if (currentParsingIndex == tokenTypes.size() - 1)
	{
		return;
	}
	currentParsingIndex++;

	Node* newNode = new Node{};
	procExp(newNode);
	currentNode->secondChild = newNode;
}

void Parser::procAssign(Node*& currentNode) {

	currentNode->typeOfproc = ASSIGN;

	if (tokenTypes.at(currentParsingIndex) == IDENTIFIER)
	{
		currentNode->valueAssociated = tokenValues.at(currentParsingIndex);
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
	}
	else
	{
		errorFlag = true;
		return;
	}
	if (tokenTypes.at(currentParsingIndex) == ASSIGN)
	{
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
	}
	else
	{
		errorFlag = true;
		return;
	}

	Node* newNode = new Node{};
	procExp(newNode);
	currentNode->firstChild = newNode;
}

void Parser::procExp(Node*& currentNode) {

	Node* newNodeLeft = new Node{};
	procSimpleExp(newNodeLeft);

	if (tokenTypes.at(currentParsingIndex) == LESSTHAN || tokenTypes.at(currentParsingIndex) == EQUAL)
	{
		currentNode->firstChild = newNodeLeft;

		currentNode->typeOfproc = EXP;
		currentNode->valueAssociated = tokenValues.at(currentParsingIndex);
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;

		Node* newNodeRight = new Node{};
		procSimpleExp(newNodeRight);
		currentNode->secondChild = newNodeRight;
	}
	else
	{
		delete currentNode;
		currentNode = newNodeLeft;
	}
}

void Parser::procSimpleExp(Node*& currentNode) {

	Node* newNodeLeft = new Node{};
	procTerm(newNodeLeft);

	if (tokenTypes.at(currentParsingIndex) == PLUS
		|| tokenTypes.at(currentParsingIndex) == MINUS)
	{
		Node* newChild = new Node{};

		newChild->typeOfproc = SIMPLEEXP;
		newChild->valueAssociated = tokenValues.at(currentParsingIndex);
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;

		newChild->firstChild = newNodeLeft;

		Node* newNodeRight = new Node{};
		procTerm(newNodeRight);
		newChild->secondChild = newNodeRight;

		while (tokenTypes.at(currentParsingIndex) == PLUS
			|| tokenTypes.at(currentParsingIndex) == MINUS)
		{
			Node* currentParent = new Node{};

			currentParent->firstChild = newChild;

			currentParent->typeOfproc = SIMPLEEXP;
			currentParent->valueAssociated = tokenValues.at(currentParsingIndex);
			if (currentParsingIndex == tokenTypes.size() - 1)
			{
				return;
			}
			currentParsingIndex++;

			newNodeRight = new Node{};
			procTerm(newNodeRight);
			currentParent->secondChild = newNodeRight;

			newChild = currentParent;
		}
		currentNode = newChild;
	}
	else
	{
		delete currentNode;
		currentNode = newNodeLeft;
	}
}

void Parser::procTerm(Node*& currentNode) {

	Node* newNodeLeft = new Node{};
	procFactor(newNodeLeft);

	if (tokenTypes.at(currentParsingIndex) == MULT
		|| tokenTypes.at(currentParsingIndex) == DIV)
	{
		Node* newChild = new Node{};

		newChild->typeOfproc = TERM;
		newChild->valueAssociated = tokenValues.at(currentParsingIndex);
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;

		newChild->firstChild = newNodeLeft;

		Node* newNodeRight = new Node{};
		procFactor(newNodeRight);
		newChild->secondChild = newNodeRight;

		while (tokenTypes.at(currentParsingIndex) == MULT
			|| tokenTypes.at(currentParsingIndex) == DIV)
		{
			Node* currentParent = new Node{};

			currentParent->firstChild = newChild;

			currentParent->typeOfproc = TERM;
			currentParent->valueAssociated = tokenValues.at(currentParsingIndex);
			if (currentParsingIndex == tokenTypes.size() - 1)
			{
				return;
			}
			currentParsingIndex++;

			newNodeRight = new Node{};
			procFactor(newNodeRight);
			currentParent->secondChild = newNodeRight;

			newChild = currentParent;
		}
		currentNode = newChild;
	}
	else
	{
		delete currentNode;
		currentNode = newNodeLeft;
	}
}

void Parser::procFactor(Node*& currentNode) {
	if (tokenTypes.at(currentParsingIndex) == OPENBRACKET)
	{
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;

		procExp(currentNode);

		if (tokenTypes.at(currentParsingIndex) == CLOSEDBRACKET)
		{
			if (currentParsingIndex == tokenTypes.size() - 1)
			{
				return;
			}
			currentParsingIndex++;
		}
		else
		{
			errorFlag = true;
			return;
		}
	}
	else if (tokenTypes.at(currentParsingIndex) == NUMBER)
	{
		currentNode->typeOfproc = NUMBER;
		currentNode->valueAssociated = tokenValues.at(currentParsingIndex);
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
	}
	else if (tokenTypes.at(currentParsingIndex) == IDENTIFIER)
	{
		currentNode->typeOfproc = IDENTIFIER;
		currentNode->valueAssociated = tokenValues.at(currentParsingIndex);
		if (currentParsingIndex == tokenTypes.size() - 1)
		{
			return;
		}
		currentParsingIndex++;
	}
	else
	{
		errorFlag = true;
	}
}

string Parser::stringToShow(Node* rootNode) {

	string buffer{};
	if (rootNode != nullptr)
	{
		buffer += "graph {\nrankdir = TB \n ordering = \"out\"";
		buffer += stringToShowForNode(rootNode->nextNode, currentNodeIndex);
		buffer += "}";

	}

	return buffer;
}

string Parser::stringToShowForNode(Node* node, int nodeIndex) {

	string buffer{};
	string shape{ "oval" };
	string procedure{};

	if (node->typeOfproc == IF || node->typeOfproc == READ || node->typeOfproc == ASSIGN
		|| node->typeOfproc == WRITE || node->typeOfproc == REPEAT || node->typeOfproc == ROOT
		|| node->typeOfproc == EXPWITHBRACKETS)
	{
		shape = "box";
		procedure = node->typeOfproc;
	}
	if (node->typeOfproc == EXP || node->typeOfproc == SIMPLEEXP || node->typeOfproc == TERM)
	{
		procedure = "op";
	}
	if (node->typeOfproc == IDENTIFIER)
	{
		procedure = "id";
	}
	if (node->typeOfproc == NUMBER)
	{
		procedure = "const";
	}

	buffer += to_string(nodeIndex) + " [ shape = " + shape + " label = \""
		+ procedure + "\\n(" + node->valueAssociated + ")\" ]\n";

	if (node->nextNode != nullptr)
	{
		buffer += to_string(nodeIndex) + " -- " + to_string(++currentNodeIndex) + "\n";
		buffer += "{ rank = same; " + to_string(nodeIndex) + "; " + to_string(currentNodeIndex) + "; }\n";
		buffer += stringToShowForNode(node->nextNode, currentNodeIndex);
	}
	if (node->firstChild != nullptr)
	{
		buffer += to_string(nodeIndex) + " -- " + to_string(++currentNodeIndex) + "\n";
		buffer += stringToShowForNode(node->firstChild, currentNodeIndex);
	}
	if (node->secondChild != nullptr)
	{
		buffer += to_string(nodeIndex) + " -- " + to_string(++currentNodeIndex) + "\n";
		buffer += stringToShowForNode(node->secondChild, currentNodeIndex);
	}

	return buffer;
}
