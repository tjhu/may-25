//**********************************************//
//Copyright (c) 2016, Tianjiao Huang			//
//All rights reserved.							//
//**********************************************//


#pragma once

#ifndef TMATH_H
#define TMATH_H
#endif

#include "stdafx.h"
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


enum intMethod { LeftRiemann, Simpson };


std::string parse(std::string exp);
bool hasHigherPrecedence(std::string stack, std::string token);
unsigned int getOperLvl(std::string buffer);
float evaluate(std::string exp, float x);
float operate(float operL, float operR, char opert);
float operate(float operL, std::string opert);
bool isNumber(char x);
bool isOperator(std::string x);
std::string getOperator(std::string exp, unsigned int i);
float intergal(std::string exp, float a, float b, unsigned long n, intMethod method);
std::string PutTimesSimbolInFrontOfX(std::string exp);
float Clamp(float x, float low, float high);
float IfOverflowThenReset(float x, float low, float high);

// Newton's method stuff
float derivative(std::string exp, float x);
bool FindZero(std::string exp, float* x);