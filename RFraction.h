#pragma once
#include <iostream>
#include <numeric>

using namespace std;

class Fraction
{
	int numerator, denominator;

	void simplify();

public:

	Fraction(int num = 0, int denom = 1) : numerator(num), denominator(denom)
	{
		simplify();
	};

	Fraction operator- (const Fraction& other) const;
	Fraction operator+ (const Fraction& other) const;
	Fraction operator/ (const Fraction& other) const;
	Fraction operator* (const Fraction& other) const;

	Fraction& operator-=(const Fraction& other);
	Fraction& operator+=(const Fraction& other);
	Fraction& operator/=(const Fraction& other);
	Fraction& operator*=(const Fraction& other);

	bool operator==(const Fraction& other) const;
	bool operator!=(const Fraction& other) const;
	bool operator<(const Fraction& other) const;
	bool operator<=(const Fraction& other) const;
	bool operator>(const Fraction& other) const;
	bool operator>=(const Fraction& other) const;

	friend istream& operator>>(istream& is, Fraction& r);
	friend ostream& operator<<(ostream& os, const Fraction& r);
};

int getGcd(int a, int b);