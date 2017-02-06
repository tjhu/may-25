#include <sbml/SBMLTypes.h>
#pragma comment(lib, "libsbml.lib")
#pragma comment(lib, "bzip2.lib")
#pragma comment(lib, "iconv.lib")
#pragma comment(lib, "libxml2.lib")
#pragma comment(lib, "zdll.lib")

#include <iostream>


int
main(int argc, char *argv[])
{
	const char* expected = "1 + f(x)";
	const char* s = "<?xml version='1.0' encoding='UTF-8'?>"
		"<math xmlns='http://www.w3.org/1998/Math/MathML'>"
		"  <apply> <plus/> <cn> 1 </cn>"
		"                  <apply> <ci> f </ci> <ci> x </ci> </apply>"
		"  </apply>"
		"</math>";
	ASTNode* ast = readMathMLFromString(s);
	char*    result = SBML_formulaToString(ast);
	if (strcmp(result, expected) == 0)
		std::cout << "Got expected result" << std::endl;
	else
		std::cout << "Mismatch after readMathMLFromString()" << std::endl;
	ASTNode* new_mathml = SBML_parseFormula(result);
	char*    new_s = writeMathMLToString(new_mathml);
	std::cout << "Result of writing AST:" << std::endl << new_s << std::endl;
}