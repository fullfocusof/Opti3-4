#pragma once
#include "RFraction.h"

#include <iomanip>

#include <fstream>

#include <Windows.h>
#include <conio.h>

#include <vector>
#include <string>

class SymplexInterraction
{
	int vars, limits;
	vector<Fraction> objFunc;
	Fraction funcConst;
	bool funcTarget; // 0 - min; 1 - max

	vector<pair<vector<Fraction>, string>> limitsCoefs;
	vector<Fraction> limitsConstants;

public:

	void readFromFile(string filename);
	void printData();
	void clearData();

	void isContradiction();

	void castToStandard(string filename);
	void standardToFile(string filename);
	void standardToFile(string filename, vector<int> baseID, vector<int> rowBaseID);
	void symplex2ToFile(string filename, pair<vector<Fraction>, Fraction>& subFunc);

	Fraction getObjFuncValue(vector<Fraction>& solution);

	vector<Fraction> isCanonical(string filename);
	int findEnteringVar();
	int findLeadRow(int leadColID);
	void symplexIteration();
	bool isEndSymplex();

	int findEnteringVar(vector<Fraction>& subFunc);
	void symplex2Iteration(pair<vector<Fraction>, Fraction>& subFunc);
	bool isEndSymplex2(vector<Fraction>& subFunc);
	bool isSolvedSubFunc(vector<Fraction>& subFunc);
	
	void symplexMethod(string filename);
	void symplex2Method(string filename);
	void universalSymplex(string filename);
};

void printQuit();