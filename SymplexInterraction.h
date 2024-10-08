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

	void castToStandard(string filename);
	void standardToFile(string filename);

	vector<Fraction> isCanonical(string filename);

	int findEnteringVar();
	int findLeadRow(int leadColID);
	void symplexIteration(string filename);

	Fraction getObjFuncValue(vector<Fraction>& solution);
};

void printQuit();