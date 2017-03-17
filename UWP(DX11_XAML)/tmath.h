//**********************************************//
//Copyright (c) 2016-2017, Tianjiao Huang		//
//All rights reserved.							//
//**********************************************//


#pragma once

#ifndef TMATH_H


namespace tmathlib
{
	struct Exception
	{
	public:
		std::wstring err;
		std::wstring file;
		std::wstring function;
		long line;
	};
#define Throw(err) { throw err; }
#define tThrow(err) {Exception exception = { err, __FILE__ ,__FUNCTION__, __LINE__ }; throw (exception); }


	enum intMethod { LeftRiemann, Simpson, ALGLIB };


	std::wstring parse(std::wstring exp);
	bool hasHigherPrecedence(std::wstring stack, std::wstring token);
	unsigned int getOperLvl(std::wstring buffer);
	double evaluate(std::wstring exp, double x);
	double operate(double operL, double operR, wchar_t opert);
	double operate(double operL, std::wstring opert);
	bool isNumber(wchar_t x);
	bool isOperator(std::wstring x);
	std::wstring getOperator(std::wstring exp, unsigned int i);
	double intergal(std::wstring exp, double a, double b, unsigned long n, intMethod method);
	std::wstring PutTimesSimbolInFrontOfXAndOtherStuff(std::wstring& exp);
	double Clamp(double x, double low, double high);
	double IfOverflowThenReset(double x, double low, double high);

	// Newton's method stuff
	double derivative(std::wstring exp, double x);
	bool FindZero(std::wstring exp, double* x);
}

#define TMATH_H
#endif
