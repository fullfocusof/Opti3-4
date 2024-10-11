#include "SymplexInterraction.h"

void printQuit()
{
	cout << endl << endl << "Backspace - возврат в меню";

	int answ = _getch();
	bool press = false;

	while (!press)
	{
		if (answ == 8)
		{
			press = true;
			break;
		}
		else
		{
			answ = _getch();
		}
	}

	system("cls");
}

void SymplexInterraction::readFromFile(string filename)
{
	ifstream ifs(filename);

	if (ifs.is_open())
	{
		ifs >> vars >> limits;

		Fraction value;
		for (int i = 0; i < vars; i++)
		{
			ifs >> value;
			objFunc.push_back(value);
		}

		string sign;
		ifs >> sign;
		funcTarget = sign == ">" ? true : false;

		limitsCoefs.resize(limits);
		
		for (int i = 0; i < limits; i++)
		{
			for (int j = 0; j < vars; j++)
			{
				ifs >> value;
				limitsCoefs[i].first.push_back(value);
			}
			ifs >> limitsCoefs[i].second;
			ifs >> value;
			limitsConstants.push_back(value);
		}

		ifs.close();
	}
	else throw exception("Не удалось открыть файл ввода");
}

void SymplexInterraction::printData()
{
	if (objFunc.empty() || limitsCoefs.empty() || limitsConstants.empty()) throw exception("Данные неполные или отсутствуют");
	else
	{
		cout << "Количество ограничений - " << limits << endl << "Количество переменных - " << vars << endl << endl;

		cout << "Целевая функция: F(X) = ";
		for (int i = 0; i < vars; i++)
		{
			if (i == vars - 1) cout << objFunc[i] << "*x" << i + 1;
			else cout << objFunc[i] << "*x" << i + 1 << " + ";
		}
		if (funcConst != 0) cout << " + " << funcConst;
		string sign = (funcTarget == false) ? " --> min" : " --> max";
		cout << sign << endl << endl;

		cout << "Ограничения:" << endl;
		for (int i = 0; i < limits; i++)
		{
			for (int j = 0; j < limitsCoefs[i].first.size(); j++)
			{
				if (j == limitsCoefs[i].first.size() - 1) cout << limitsCoefs[i].first[j] << "*x" << j + 1;
				else cout << limitsCoefs[i].first[j] << "*x" << j + 1 << " + ";
			}
			cout << " " << limitsCoefs[i].second << " " << limitsConstants[i] << endl;
		}
	}
}

void SymplexInterraction::clearData()
{
	objFunc.clear();
	limitsCoefs.clear();
	limitsConstants.clear();
	funcConst = 0;
	vars = 0;
	limits = 0;
}

void SymplexInterraction::isContradiction()
{
	if (limitsCoefs.size() != limitsConstants.size()) throw exception("Размеры вектора ограничений и правых частей должны совпадать");

	for (int i = 0; i < limits - vars; i++)
	{
		for (int j = i + 1; j < limits - vars; j++)
		{
			const auto& coeffs1 = limitsCoefs[i].first;
			const auto& coeffs2 = limitsCoefs[j].first;
			const auto& sign1 = limitsCoefs[i].second;
			const auto& sign2 = limitsCoefs[j].second;

			if (coeffs1.size() != coeffs2.size())
			{
				cerr << endl << "Размеры " << i + 1 << " и " << j + 1 << " ограничений не совпадают" << endl << endl;
				continue;
			}

			Fraction dotProduct1 = 0, dotProduct2 = 0;
			for (int k = 0; k < coeffs1.size(); ++k) 
			{
				dotProduct1 += coeffs1[k] * (sign1 == "<=" ? 1 : -1);
				dotProduct2 += coeffs2[k] * (sign2 == "<=" ? 1 : -1);
			}

			if ((sign1 == "<=" && sign2 == "<=" && dotProduct1 > dotProduct2) ||
				(sign1 == ">=" && sign2 == ">=" && dotProduct1 < dotProduct2)) throw exception("Ограничения задачи противоречивы");
		}
	}
}

void SymplexInterraction::castToStandard(string filename)
{
	if (funcTarget)
	{
		for (int i = 0; i < objFunc.size(); i++)
		{
			objFunc[i] *= -1;
		}
		funcTarget = false;
	}

	for (int i = 0; i < limits - vars; i++)
	{
		string sign = limitsCoefs[i].second;
		if (sign != "=")
		{
			Fraction el;
			el = sign == "<=" ? 1 : -1;
			limitsCoefs[i].first.push_back(el);
			limitsCoefs[i].second = "=";

			for (int j = 0; j < limitsCoefs.size(); j++)
			{
				if (j != i)
				{
					limitsCoefs[j].first.push_back(0);
				}
			}

			int size = limitsCoefs[i].first.size();
			vector<Fraction> temp(size);
			for (int j = 0; j < size; j++)
			{
				if (j == size - 1) temp[j] = 1;
				else temp[j] = 0;
			}
			limitsCoefs.push_back(make_pair(temp, ">="));
			limitsConstants.push_back(0);

			objFunc.push_back(0);

			vars++;
			limits++;
		}
		if (limitsConstants[i] < 0)
		{
			limitsConstants[i] *= -1;
			for (int j = 0; j < limitsCoefs[i].first.size(); j++)
			{
				limitsCoefs[i].first[j] *= -1;
			}
		}
	}

	//standardToFile(filename);
}

void SymplexInterraction::standardToFile(string filename)
{
	ofstream ofs(filename);

	if (ofs.is_open())
	{
		ofs << setw(10) << "Базис\t\t";
		for (int i = 0; i < vars; i++)
		{
			ofs << setw(5) << "x" << (i + 1) << "\t\t";
		}
		ofs << setw(10) << "Свободные члены" << endl;
		ofs << string(100, '-') << endl;

		for (int i = 0; i < limits - vars; i++)
		{
			ofs << setw(10) << "\t\t";
			for (int j = 0; j < limitsCoefs[i].first.size(); j++)
			{
				ofs << setw(5) << limitsCoefs[i].first[j] << "\t\t";
			}
			ofs << setw(10) << limitsConstants[i] << endl;
		}
		ofs << endl;

		ofs << setw(10) << "f\t\t";
		for (int i = 0; i < objFunc.size(); i++)
		{
			ofs << setw(5) << objFunc[i] << "\t\t";
		}
		ofs << setw(10) << funcConst;
	}
	else throw exception("Не удалось открыть/создать файл вывода");
}

void SymplexInterraction::standardToFile(string filename, vector<int> baseID, vector<int> rowBaseID)
{
	ofstream ofs(filename);

	if (ofs.is_open())
	{
		ofs << setw(10) << "Базис\t\t";
		for (int i = 0; i < vars; i++)
		{
			ofs << setw(5) << "x" << (i + 1) << "\t\t";
		}
		ofs << setw(10) << "Свободные члены" << endl;
		ofs << string(100, '-') << endl;

		for (int i = 0; i < limits - vars; i++)
		{
			ofs << setw(7) << "x" << baseID[rowBaseID[i]] + 1 << "\t\t";
			for (int j = 0; j < limitsCoefs[i].first.size(); j++)
			{
				ofs << setw(5) << limitsCoefs[i].first[j] << "\t\t";
			}
			ofs << setw(10) << limitsConstants[i] << endl;
		}
		ofs << endl;

		ofs << setw(10) << "f\t\t";
		for (int i = 0; i < objFunc.size(); i++)
		{
			ofs << setw(5) << objFunc[i] << "\t\t";
		}
		ofs << setw(10) << funcConst;
	}
	else throw exception("Не удалось открыть/создать файл вывода");
}

void SymplexInterraction::symplex2ToFile(string filename, pair<vector<Fraction>, Fraction>& subFunc)
{
	ofstream ofs(filename);

	if (ofs.is_open())
	{
		ofs << setw(10) << "Базис\t\t";
		for (int i = 0; i < vars; i++)
		{
			ofs << setw(5) << "x" << (i + 1) << "\t\t";
		}
		ofs << setw(10) << "Свободные члены" << endl;
		ofs << string(100, '-') << endl;

		for (int i = 0; i < limits - vars; i++)
		{
			ofs << setw(10) << "\t\t";
			for (int j = 0; j < limitsCoefs[i].first.size(); j++)
			{
				ofs << setw(5) << limitsCoefs[i].first[j] << "\t\t";
			}
			ofs << setw(10) << limitsConstants[i] << endl;
		}
		ofs << endl;

		ofs << setw(10) << "f\t\t";
		for (int i = 0; i < objFunc.size(); i++)
		{
			ofs << setw(5) << objFunc[i] << "\t\t";
		}
		ofs << setw(10) << funcConst << endl;

		ofs << setw(11) << "f'\t\t";
		for (int i = 0; i < subFunc.first.size(); i++)
		{
			ofs << setw(5) << subFunc.first[i] << "\t\t";
		}
		ofs << setw(10) << subFunc.second;
	}
	else throw exception("Не удалось открыть/создать файл вывода");
}

Fraction SymplexInterraction::getObjFuncValue(vector<Fraction>& solution)
{
	Fraction result;
	int sizeFunc = objFunc.size();
	int sizeSolution = solution.size();
	if (sizeFunc != sizeSolution) throw exception("Размеры векторов целевой функции и решения не совпадают");

	for (int i = 0; i < sizeFunc; i++)
	{
		result += objFunc[i] * solution[i];
	}
	result += funcConst;

	return result;
}

vector<Fraction> SymplexInterraction::isCanonical(string filename)
{
	//castToStandard(filename);

	vector<Fraction> result(vars);
	vector<int> baseID, rowBaseID;

	for (int i = 0; i < vars; i++)
	{
		int cnt = 0, ID = -1;
		Fraction coef = 0;
		for (int j = 0; j < limits - vars; j++)
		{
			if (limitsCoefs[j].first[i] != 0)
			{
				cnt++;
				coef = limitsCoefs[j].first[i];
				ID = j;
			}	
		}

		if (coef == 1 && cnt == 1)
		{
			result[i] = limitsConstants[ID];
			baseID.push_back(i);
			rowBaseID.push_back(ID);
		}
	}

	int cntNonZero = 0;
	for (auto& el : result)
	{
		if (el != 0) cntNonZero++;
	}

	for (int k = 0; k < baseID.size(); k++)
	{
		for (int i = 0; i < objFunc.size(); i++)
		{
			if (objFunc[i] != 0 && i == baseID[k])
			{
				Fraction coefToDelete = objFunc[i];
				for (int j = 0; j < objFunc.size(); j++)
				{
					objFunc[j] -= limitsCoefs[rowBaseID[k]].first[j] * coefToDelete;
				}
				funcConst -= limitsConstants[rowBaseID[k]] * coefToDelete;
			}
		}
	}

	if ((cntNonZero == limits - vars))
	{
		standardToFile(filename, baseID, rowBaseID);
		return result;
	}
	else return vector<Fraction>();
}

int SymplexInterraction::findEnteringVar()
{
	Fraction minValue = 1000000;
	int ID = -1;
	for (int i = 0; i < vars; i++)
	{
		if (objFunc[i] < minValue)
		{
			minValue = objFunc[i];
			ID = i;
		}
	}
	return ID;
}

int SymplexInterraction::findLeadRow(int leadColID)
{
	Fraction minValue = 1000000;
	int ID = -1;
	for (int i = 0; i < limits - vars; i++)
	{
		if (limitsCoefs[i].first[leadColID] > 0)
		{
			Fraction temp = limitsConstants[i] / limitsCoefs[i].first[leadColID];
			if (temp < minValue)
			{
				minValue = temp;
				ID = i;
			}
		}
	}
	for (int i = 0; i < limits - vars; i++)
	{
		if (i != ID && limitsCoefs[i].first[leadColID] > 0)
		{
			Fraction temp = limitsConstants[i] / limitsCoefs[i].first[leadColID];
			if (temp == minValue) throw exception("Ведущую строку можно выбрать не единственным образом\nF = inf");
		}
	}

	return ID;
}

void SymplexInterraction::symplexIteration()
{
	int leadColID = findEnteringVar();
	if (leadColID == -1) throw exception("Нельзя выбрать ведущий столбец");

	int leadRowID = findLeadRow(leadColID);
	if (leadRowID == -1) throw exception("Нельзя выбрать ведущую строку");

	Fraction coef = limitsCoefs[leadRowID].first[leadColID];

	for (int i = 0; i < limitsCoefs[leadRowID].first.size(); i++)
	{
		limitsCoefs[leadRowID].first[i] /= coef;
	}
	limitsConstants[leadRowID] /= coef;

	for (int i = 0; i < limits - vars; i++)
	{
		if (i != leadRowID)
		{
			Fraction coefToDelete = limitsCoefs[i].first[leadColID];
			for (int j = 0; j < limitsCoefs[i].first.size(); j++)
			{
				limitsCoefs[i].first[j] -= limitsCoefs[leadRowID].first[j] * coefToDelete;
			}
			limitsConstants[i] -= limitsConstants[leadRowID] * coefToDelete;
		}
	}

	Fraction coefToDelete = objFunc[leadColID];
	for (int i = 0; i < objFunc.size(); i++)
	{
		objFunc[i] -= limitsCoefs[leadRowID].first[i] * coefToDelete;
	}
	funcConst -= limitsConstants[leadRowID] * coefToDelete;
}

bool SymplexInterraction::isEndSymplex()
{
	int cntNegative = 0;
	for (auto& el : objFunc)
	{
		if (el < 0) cntNegative++;
	}
	return cntNegative == 0;
}

int SymplexInterraction::findEnteringVar(vector<Fraction>& subFunc)
{
	Fraction minValue = 1000000;
	int ID = -1;
	for (int i = 0; i < vars; i++)
	{
		if (subFunc[i] < minValue)
		{
			minValue = subFunc[i];
			ID = i;
		}
	}
	return ID;
}

void SymplexInterraction::symplex2Iteration(pair<vector<Fraction>, Fraction>& subFunc)
{
	int leadColID = findEnteringVar(subFunc.first);
	if (leadColID == -1) throw exception("Нельзя выбрать ведущий столбец");
	
	int leadRowID = findLeadRow(leadColID);
	if (leadRowID == -1) throw exception("Нельзя выбрать ведущую строку");

	Fraction coef = limitsCoefs[leadRowID].first[leadColID];

	for (int i = 0; i < limitsCoefs[leadRowID].first.size(); i++)
	{
		limitsCoefs[leadRowID].first[i] /= coef;
	}
	limitsConstants[leadRowID] /= coef;

	for (int i = 0; i < limits - vars; i++)
	{
		if (i != leadRowID)
		{
			Fraction coefToDelete = limitsCoefs[i].first[leadColID];
			for (int j = 0; j < limitsCoefs[i].first.size(); j++)
			{
				limitsCoefs[i].first[j] -= limitsCoefs[leadRowID].first[j] * coefToDelete;
			}
			limitsConstants[i] -= limitsConstants[leadRowID] * coefToDelete;
		}
	}

	Fraction coefToDelete = objFunc[leadColID];
	for (int i = 0; i < objFunc.size(); i++)
	{
		objFunc[i] -= limitsCoefs[leadRowID].first[i] * coefToDelete;
	}
	funcConst -= limitsConstants[leadRowID] * coefToDelete;

	coefToDelete = subFunc.first[leadColID];
	for (int i = 0; i < objFunc.size(); i++)
	{
		subFunc.first[i] -= limitsCoefs[leadRowID].first[i] * coefToDelete;
	}
	subFunc.second -= limitsConstants[leadRowID] * coefToDelete;
}

bool SymplexInterraction::isEndSymplex2(vector<Fraction>& subFunc)
{
	int cntNegative = 0;
	for (auto& el : subFunc)
	{
		if (el < 0) cntNegative++;
	}
	return cntNegative == 0;
}

bool SymplexInterraction::isSolvedSubFunc(vector<Fraction>& subFunc)
{
	int cntZero = 0;
	for (auto& el : subFunc)
	{
		if (el == 0) cntZero++;
	}
	return cntZero == subFunc.size();
}

void SymplexInterraction::symplexMethod(string filename)
{
	int iteration = 1;
	while (!isEndSymplex())
	{
		cout << "Итерация одноэтапного симплекс-метода " << iteration << endl;
		symplexIteration();
		vector<Fraction> FBS = isCanonical(filename);
		cout << "ДБР: ";
		for (auto& el : FBS)
		{
			cout << el << " ";
		}
		cout << endl;

		Fraction valueFunc = getObjFuncValue(FBS);
		cout << "F = " << valueFunc << endl << endl;
		iteration++;
	}
}

void SymplexInterraction::symplex2Method(string filename)
{
	pair<vector<Fraction>, Fraction> subFunc;
	for (int i = 0; i < vars; i++)
	{
		Fraction el = 0;
		for (int j = 0; j < limits - vars; j++)
		{
			el += limitsCoefs[j].first[i];
		}
		subFunc.first.push_back(el * -1);
	}

	Fraction constEl;
	for (int i = 0; i < limits - vars; i++)
	{
		constEl += limitsConstants[i];
	}
	subFunc.second = constEl;

	int iteration = 1;
	while (!isEndSymplex2(subFunc.first))
	{
		cout << "Итерация двухэтапного симплекс-метода " << iteration << endl;
		symplex2Iteration(subFunc);
		symplex2ToFile(filename, subFunc);
		cout << "f': ";
		for (auto& el : subFunc.first)
		{
			cout << el << " ";
		}
		cout << "| " << subFunc.second << endl << endl;
		iteration++;

		/*vector<Fraction> FBS = isCanonical(filename);
		for (auto& el : FBS)
		{
			cout << el << " ";
		}
		cout << endl;

		Fraction valueFunc = getObjFuncValue(FBS);
		cout << "F = " << valueFunc << endl << endl;*/	
	}

	if (!isSolvedSubFunc(subFunc.first)) throw exception("Вспомогательная функция не имеет решения");

	universalSymplex(filename);
}

void SymplexInterraction::universalSymplex(string filename)
{
	castToStandard(filename);

	vector<Fraction> IFBS = isCanonical(filename);
	if (IFBS.empty())
	{
		cout << "Система не является канонической" << endl << endl;
		symplex2Method(filename);
		cout << endl;
	}
	else
	{
		cout << "Система является канонической\nНДБР: ";
		for (auto& el : IFBS)
		{
			cout << el << " ";
		}
		cout << endl;

		Fraction valueFunc = getObjFuncValue(IFBS);
		cout << "F = " << valueFunc << endl << endl;

		symplexMethod(filename);
	}
}