//**********************************************//
//Copyright (c) 2016, Tianjiao Huang			//
//All rights reserved.							//
//**********************************************//


#pragma once

#ifndef TMATH_H
#define TMATH_H
#endif

#include <cmath>
#include <string>
#include <stack>
#include <cctype>
#include <stdlib.h>
#include <stdexcept>
#include <list>

struct Exception
{
public:
	std::basic_string <char> err;
	std::string file;
	std::string function;
	long line;
};
#define tThrow(err) {Exception exception = { err, __FILE__ ,__FUNCTION__, __LINE__ }; throw (exception); }


enum intMethod { LeftRiemann, Simpson, ALGLIB };


std::string parse(std::string exp);
bool hasHigherPrecedence(std::string stack, std::string token);
unsigned int getOperLvl(std::string buffer);
double evaluate(std::string exp, double x);
double operate(double operL, double operR, char opert);
double operate(double operL, std::string opert);
bool isNumber(char x);
bool isOperator(std::string x);
std::string getOperator(std::string exp, unsigned int i);
double intergal(std::string exp, double a, double b, unsigned long n, intMethod method);
std::string PutTimesSimbolInFrontOfXAndOtherStuff(std::string& exp);
double Clamp(double x, double low, double high);
double IfOverflowThenReset(double x, double low, double high);

// Newton's method stuff
double derivative(std::string exp, double x);
bool FindZero(std::string exp, double* x);