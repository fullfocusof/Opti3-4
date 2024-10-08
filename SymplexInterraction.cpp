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

	standardToFile(filename);
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

vector<Fraction> SymplexInterraction::isCanonical(string filename)
{
	castToStandard(filename);

	vector<Fraction> result(vars);

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
		}
	}

	int cntNonZero = 0;
	for (auto& el : result)
	{
		if (el != 0) cntNonZero++;
	}

	if (cntNonZero == limits - vars) return result;
	else return vector<Fraction>();
}

int SymplexInterraction::findEnteringVar()
{
	Fraction minValue = INT_MAX;
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
	Fraction minValue = INT_MAX;
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
	return ID;
}

void SymplexInterraction::symplexIteration(string filename)
{
	vector<Fraction> IFBS = isCanonical(filename);
	if (IFBS.empty()) throw exception("Система не является канонической");
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
	}

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