#include "stdafx.h"
#include "PN.h"
#include "LT.h"

int Preorities(char operation) {
	if (operation == LEX_LEFTTHESIS || operation == LEX_RIGHTTHESIS) {
		return 1;
	}
	if (operation == LEX_FULL_EQUALS || operation == LEX_NOT_FULL_EQUALS) {
		return 2;
	}
	if (operation == LEX_LESS || operation == LEX_MORE ||
		operation == LEX_LESS_OR_EQUALS || operation == LEX_MORE_OR_EQUALS) {
		return 3;
	}
	if (operation == LEX_MINUS || operation == LEX_PLUS) {
		return 4;
	}
	if (operation == LEX_STAR || operation == LEX_DIRSLASH || LEX_REM_AFTER_DIVIDING) {
		return 5;
	}

}

void PN::ConvertToPolishNotation(LT::LexTable& lexTable, IT::IdTable& idtable, int index, LT::Entry* expression)
{
	std::stack<LT::Entry> stack;

	int expressionSize = 0;
	int startPosition = index;
	short leftThesis = 0;

	for (; lexTable.table[index].lexema != LEX_SEMICOLON; index++) {
		LT::Entry lex = lexTable.table[index];
		if ((lex.lexema == LEX_ID || lex.lexema == LEX_LITERAL) && idtable.table[lex.idxTI].idtype != IT::IDTYPE::F) {
			expression[expressionSize++] = lex;
		}
		else if (lex.lexema == LEX_LEFTTHESIS) {
			stack.push(lex);
		}
		else if (lex.lexema == LEX_RIGHTTHESIS) {
			while (stack.size()) {
				if (stack.top().lexema == LEX_LEFTTHESIS) break;
				expression[expressionSize++] = stack.top();
				stack.pop();
			}
			stack.pop();
		}
		else if (lex.lexema == LEX_ID && idtable.table[lexTable.table[index].idxTI].idtype == IT::IDTYPE::F)
		{
			LT::Entry copy = lexTable.table[index];
			copy.lexema = LEX_COMMERCIAL_AT;
			copy.idxTI = lexTable.table[index].idxTI;
			++index;
			int countOfParams = 0;
			while (lexTable.table[index].lexema != LEX_RIGHTTHESIS) {
				if (((lexTable.table[index].lexema != LEX_COMMA) && (lexTable.table[index].lexema == LEX_ID || lexTable.table[index].lexema == LEX_LITERAL)))
				{
					++countOfParams;
					expression[expressionSize++] = lexTable.table[index];
				}
				++index;
			}
			expression[expressionSize++] = copy;
			expression[expressionSize].lexema = std::to_string(countOfParams).front();
			expression[expressionSize++].idxTI = -1;
		}
		else if (stack.empty() || stack.top().lexema == LEX_LEFTTHESIS) {
			stack.push(lex);
		}
		else
		{
			while (stack.size()) {
				if (Preorities(lexTable.table[index].lexema) > Preorities(stack.top().lexema)) break;
				expression[expressionSize++] = stack.top();
				stack.pop();
			}
			stack.push(lexTable.table[index]);
		}
	}
	while (stack.size() != 0)
	{
		expression[expressionSize++] = stack.top();
		stack.pop();
	}
	for (int i = 0, j = startPosition; i < expressionSize; ++i, ++j) {
		lexTable.table[j] = expression[i];
	}
	for (int i = 0; i < index - (startPosition + expressionSize); ++i) {
		for (int j = startPosition + expressionSize; j < lexTable.size; ++j) {
			lexTable.table[j] = lexTable.table[j + 1];
		}
		--lexTable.size;
	}
	for (int i = 0; i < idtable.size; ++i) {
		if (idtable.table[i].idxfirstLE > startPosition)
			idtable.table[i].idxfirstLE -= index - (startPosition + expressionSize);
	}
}


void PN::PolishNotation(LT::LexTable& lexTable, IT::IdTable& idTable)
{
 	for (auto i = 0; i < lexTable.size; ++i) {
		if (lexTable.table[i].lexema == LEX_EQUAL) {
			LT::Entry expression[100];
			PN::ConvertToPolishNotation(lexTable, idTable, i + 1, expression);
		}
	}
}