#include "stdafx.h"
#include "Sem.h"
#include "Error.h"

Sem::OperationWithTypes::OperationWithTypes(IT::IDDATATYPE returnType, char operation, int countOfParmTypes, IT::IDDATATYPE parmType, ...)
	: returnType(returnType), operation(operation)
{
	IT::IDDATATYPE* p = &parmType;
	for (int i = 0; i < countOfParmTypes; ++i)
	{
		parmTypes.push_back(*p);
		p++;
	}
}

void Sem::SemanticAnalysis::checkTypeOfReturn(int& i)
{
	if (isMain && (idTable.table[lexTable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::UINT)) {
		throw ERROR_THROW_IN(300, lexTable.table[i].sn, -1);
	}
	else if (!isMain && (idTable.table[lexTable.table[i + 1].idxTI].iddatatype != typeOfFunction)) {
		throw ERROR_THROW_IN(301, lexTable.table[i].sn, -1);
	}
	if (!isMain && (idTable.table[lexTable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::UINT) && idTable.table[lexTable.table[i + 1].idxTI].idtype == IT::IDTYPE::L && idTable.table[lexTable.table[i + 1].idxTI].value.vint < 0) {
		throw ERROR_THROW_IN(310, lexTable.table[i].sn, -1);
	}
}

void Sem::SemanticAnalysis::checkIfExpression(int& i)
{
	if (lexTable.table[i + 3].lexema == LEX_RIGHTTHESIS && idTable.table[lexTable.table[i + 2].idxTI].iddatatype != IT::IDDATATYPE::BOOL) {
		throw ERROR_THROW_IN(309, lexTable.table[i].sn, -1);
	}
	char* findElement = std::find(std::begin(operators), std::end(operators), lexTable.table[i + 3].lexema);
	auto oper = operTypes[std::distance(operators, findElement)];
	if (idTable.table[lexTable.table[i + 2].idxTI].iddatatype != idTable.table[lexTable.table[i + 4].idxTI].iddatatype || std::find(std::begin(oper.parmTypes), std::end(oper.parmTypes), idTable.table[lexTable.table[i + 2].idxTI].iddatatype) == std::end(oper.parmTypes)) {
		throw ERROR_THROW_IN(308, lexTable.table[i].sn, -1);
	}
}

void Sem::SemanticAnalysis::checkCallOfFunction(int& i)
{
	int parmsPassed = (int)lexTable.table[i + 1].lexema - 48;
	int parmsRequired = 1;
	while (idTable.table[lexTable.table[i].idxTI + parmsRequired].idtype == IT::IDTYPE::P)
		++parmsRequired;
	if (parmsRequired - 1 != parmsPassed) throw ERROR_THROW_IN(303, lexTable.table[i].sn, -1);
	for (int j = parmsPassed; j > 0; --j)
	{
		IT::IDDATATYPE paramType = idTable.table[lexTable.table[i - j].idxTI].iddatatype;
		if (paramType != idTable.table[lexTable.table[i].idxTI + parmsPassed - j + 1].iddatatype) {
			throw ERROR_THROW_IN(302, lexTable.table[i].sn, -1);
		}
	}
}

void Sem::SemanticAnalysis::checkExpressioin(int& i)
{
	if (lexTable.table[i - 1].lexema == LEX_ID && idTable.table[lexTable.table[i - 1].idxTI].iddatatype == IT::IDDATATYPE::UINT
		&& idTable.table[lexTable.table[i + 1].idxTI].value.vint < 0 && lexTable.table[i + 2].lexema == LEX_SEMICOLON) {
		throw ERROR_THROW_IN(307, lexTable.table[i].sn, -1);
	}
	IT::IDDATATYPE typeOfIdentificator = idTable.table[lexTable.table[i - 1].idxTI].iddatatype;
	std::stack<IT::IDDATATYPE> types;
	for (int j = i; lexTable.table[j].lexema != LEX_SEMICOLON; ++j) {
		char* findElement = std::find(std::begin(operators), std::end(operators), lexTable.table[j].lexema);

		if (lexTable.table[j].lexema == LEX_ID || lexTable.table[j].lexema == LEX_LITERAL) {
			if (idTable.table[lexTable.table[j].idxTI].idtype != IT::IDTYPE::F && lexTable.table[j + 1].lexema == LEX_LEFTTHESIS) {
				throw ERROR_THROW_IN(305, lexTable.table[j].sn, -1);
			}
			types.push(idTable.table[lexTable.table[j].idxTI].iddatatype);
		}
		else if (lexTable.table[j].lexema == LEX_COMMERCIAL_AT && idTable.table[lexTable.table[j].idxTI].idtype == IT::IDTYPE::F) {
			types.push(idTable.table[lexTable.table[j].idxTI].iddatatype);
		}
		else if (findElement != std::end(operators)) {
			IT::IDDATATYPE elem1 = types.top();
			types.pop();
			IT::IDDATATYPE elem2 = types.top();
			types.pop();
			auto oper = operTypes[std::distance(operators, findElement)];
			if (elem1 == elem2 && std::find(std::begin(oper.parmTypes), std::end(oper.parmTypes), elem1) != std::end(oper.parmTypes)) {
				types.push(oper.returnType);
			}
			else
				throw ERROR_THROW_IN(308, lexTable.table[j].sn, -1);
		}
	}
	if (types.top() != typeOfIdentificator) {


		throw ERROR_THROW_IN(304, lexTable.table[i].sn, -1);
	}

}

bool Sem::SemanticAnalysis::start()
{
	isMain = false;

	for (int i = 0; i < lexTable.size; ++i)
	{
		auto token = lexTable.table[i].lexema;

		if (token == LEX_RETURN) checkTypeOfReturn(i);
		else if (token == LEX_MAIN) isMain = true;
		else if (token == LEX_COMMERCIAL_AT) checkCallOfFunction(i);
		else if (token == LEX_ID && idTable.table[lexTable.table[i].idxTI].idxfirstLE != i && lexTable.table[i - 2].lexema == LEX_DECLARE) {
			std::cout << i << "\t" << idTable.table[lexTable.table[i].idxTI].idxfirstLE << std::endl;
			throw ERROR_THROW_IN(306, lexTable.table[i].sn, -1);
		}
		else if (token == LEX_FUNCTION && ((i - 2 < 0) || lexTable.table[i - 2].lexema != LEX_DECLARE)) {
			typeOfFunction = idTable.table[lexTable.table[i + 1].idxTI].iddatatype;
			isMain = false;
		}
		else if (token == LEX_EQUAL) checkExpressioin(i);
		else if (token == LEX_IF) checkIfExpression(i);
	}

	return true;
}
