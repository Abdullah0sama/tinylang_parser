#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cctype>

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
	')'
};

bool isIdentifier(string str);

bool isNumber(string str);

vector<vector<string>> scannerFunction(string allStreamInString);
