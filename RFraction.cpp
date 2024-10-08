#include "RFraction.h"

istream& operator>>(istream& is, Fraction& r)
{
	char slash;
	is >> r.numerator >> slash >> r.denominator;
	r.simplify();
	return is;
}

ostream& operator<<(ostream& os, const Fraction& r)
{
	os << r.numerator << "/" << r.denominator;
	return os;
}

int getGcd(int a, int b)
{
	while (b != 0) 
	{
		int t = b;
		b = a % b;
		a = t;
	}
	return a;
}

void Fraction::simplify()
{
	if (denominator == 0) throw std::invalid_argument("Знаменатель не может быть равен нулю");

	int gcd = getGcd(numerator, denominator);
	numerator /= gcd;
	denominator /= gcd;

	if (denominator < 0) 
	{
		numerator = -numerator;
		denominator = -denominator;
	}
}

Fraction Fraction::operator-(const Fraction& other) const
{
	int num = numerator * other.denominator - other.numerator * denominator;
	int denom = denominator * other.denominator;
	return Fraction(num, denom);
}

Fraction Fraction::operator+(const Fraction& other) const
{
	int num = numerator * other.denominator + other.numerator * denominator;
	int denom = denominator * other.denominator;
	return Fraction(num, denom);
}

Fraction Fraction::operator/(const Fraction& other) const
{
	if (other.numerator == 0) throw std::invalid_argument("Деление на ноль");
	int num = numerator * other.denominator;
	int denom = denominator * other.numerator;
	return Fraction(num, denom);
}

Fraction Fraction::operator*(const Fraction& other) const
{
	int num = numerator * other.numerator;
	int denom = denominator * other.denominator;
	return Fraction(num, denom);
}

Fraction& Fraction::operator-=(const Fraction& other)
{
	*this = *this - other;
	return *this;
}

Fraction& Fraction::operator+=(const Fraction& other)
{
	*this = *this + other;
	return *this;
}

Fraction& Fraction::operator/=(const Fraction& other)
{
	if (other.numerator == 0) throw std::invalid_argument("Деление на ноль");
	*this = *this / other;
	return *this;
}

Fraction& Fraction::operator*=(const Fraction& other)
{
	*this = *this * other;
	return *this;
}

bool Fraction::operator==(const Fraction& other) const
{
	return numerator * other.denominator == other.numerator * denominator;
}

bool Fraction::operator!=(const Fraction& other) const
{
	return !(*this == other);
}

bool Fraction::operator<(const Fraction& other) const
{
	return numerator * other.denominator < other.numerator * denominator;
}

bool Fraction::operator<=(const Fraction& other) const
{
	return !(*this > other);
}

bool Fraction::operator>(const Fraction& other) const
{
	return other < *this;
}

bool Fraction::operator>=(const Fraction& other) const
{
	return !(*this < other);
}