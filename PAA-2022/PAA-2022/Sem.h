#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"

#define NUMBER_OF_OPERATORS 11
#define OPERATORS {'+', '-', '*', '/', '%', '<', '>', '$', '#', '~', '&'};
#define ARITHMETIC_OPERATORS  {IT::IDDATATYPE::UINT, '+', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::UINT, '-', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::UINT, '*', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::UINT, '/', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::UINT, '%', 1, IT::IDDATATYPE::UINT} \

#define BOOLEAN_OPERATORS	  {IT::IDDATATYPE::BOOL, '<', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::BOOL, '>', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::BOOL, '$', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::BOOL, '#', 1, IT::IDDATATYPE::UINT},\
							  {IT::IDDATATYPE::BOOL, '~', 3, IT::IDDATATYPE::UINT, IT::IDDATATYPE::STR, IT::IDDATATYPE::BOOL},\
							  {IT::IDDATATYPE::BOOL, '&', 3, IT::IDDATATYPE::UINT, IT::IDDATATYPE::STR, IT::IDDATATYPE::BOOL}\

namespace Sem {
	struct OperationWithTypes
	{
		char operation;
		std::vector<IT::IDDATATYPE> parmTypes;
		IT::IDDATATYPE returnType;

		OperationWithTypes(IT::IDDATATYPE returnType, char operation, int countOfParmTypes, IT::IDDATATYPE parmType, ...);
	};

	struct SemanticAnalysis
	{
		LT::LexTable lexTable;
		IT::IdTable idTable;

		char operators[NUMBER_OF_OPERATORS] = OPERATORS;

		SemanticAnalysis(LT::LexTable& lextable, IT::IdTable& idtable)
		{
			lexTable = lextable;
			idTable = idtable;
		}

		OperationWithTypes operTypes[NUMBER_OF_OPERATORS] = { ARITHMETIC_OPERATORS, BOOLEAN_OPERATORS };

		bool start();
	private:
		bool isMain;
		IT::IDDATATYPE typeOfFunction;

		void checkTypeOfReturn(int& i);
		void checkCallOfFunction(int& i);
		void checkExpressioin(int& i);
		void checkIfExpression(int& i);
	};

}