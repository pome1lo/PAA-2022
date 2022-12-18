#include "stdafx.h"
#include "CG.h"
#include "LT.h"
#include "IT.h"

CG::Generation::Generation(LT::LexTable lexT, IT::IdTable idT, std::string outFile)
{
	lexTable = lexT;
	idTable = idT;
	out = std::ofstream(outFile);
}

void CG::Generation::start()
{
	head();
	constants();
	data();
	code();
}

void CG::Generation::head()
{
	out << ".586\n";
	out << ".model flat, stdcall\n";
	out << "includelib libucrt.lib\n";
	out << "includelib kernel32.lib\n";
#ifdef _DEBUG
	out << "includelib ../Debug/PAA_Lib.lib\n";
#endif
#ifndef _DEBUG
	out << "includelib ../../Debug/PAA_Lib.lib\n";
#endif
	out << "ExitProcess PROTO : DWORD\n";
	out << "_strCmp PROTO : DWORD, :DWORD\n";
	out << "_strLen PROTO : DWORD\n";
	out << "_outStr PROTO : DWORD\n";
	out << "_outBool PROTO : DWORD\n";
	out << "_outInt PROTO : DWORD\n\n";
	out << "\n.stack 4096\n";
}

void CG::Generation::constants()
{
	out << ".const\n";
	out << "\t_DIVISION_BY_ZERO_ERROR BYTE 'Ошибка выполнения: деление на ноль', 0\n";
	out << "\t_OVERFLOW_ERROR BYTE 'Ошибка выполнения: переполнение', 0\n";
	out << "\t_NEGATIVE_RESULT_ERROR BYTE 'Ошибка выполнения: попытка присвоения отрицательного значения', 0\n";
	for (int i = 0; i < idTable.size; i++)
		if (idTable.table[i].idtype == IT::IDTYPE::L)
		{
			out << "\t" << idTable.table[i].id;
			if (idTable.table[i].iddatatype == IT::IDDATATYPE::STR)
			{
				if (idTable.table[i].value.vstr.len == 0)
					out << " BYTE " << "?";
				else
					out << " BYTE " << '\'' << idTable.table[i].value.vstr.str << '\'' << ", 0";
			}

			if (idTable.table[i].iddatatype == IT::IDDATATYPE::UINT)
				out << " DWORD " << idTable.table[i].value.vint;
			if (idTable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
				out << " DWORD " << idTable.table[i].value.vbool;
			out << '\n';
		}
}

void CG::Generation::data()
{
	out << ".data\n";
	for (int i = 0; i < idTable.size; i++)
	{
		if (idTable.table[i].idtype == IT::IDTYPE::V) {
			out << '\t';
			out << '_' << idTable.table[i].scope << idTable.table[i].id;
			switch (idTable.table[i].iddatatype)
			{
			case IT::IDDATATYPE::UINT:
				out << "\t\tDWORD 0 ; uint\n";
				break;
			case IT::IDDATATYPE::STR:
				out << "\t\tDWORD ? ; str\n";
				break;
			case IT::IDDATATYPE::BOOL:
				out << "\t\tDWORD 0 ; bool\n";
				break;
			}
		}
	}
}

void CG::Generation::generateFunctionProto(int& i)
{
	if (i >= 2 && lexTable.table[i - 2].lexema == LEX_DECLARE)
		return;
	if (isMain) {
		typeOfFunction = IT::IDDATATYPE::UINT;
		out << "main PROC\n";
		return;
	}
	typeOfFunction == idTable.table[lexTable.table[i + 1].idxTI].iddatatype;
	isFunc = true;
	indOfFunc = i + 1;
	out << '_' << idTable.table[lexTable.table[indOfFunc].idxTI].id << " PROC ";
	int backup = i;
	while (lexTable.table[i].lexema != LEX_RIGHTTHESIS)
		i++;
	while (lexTable.table[i].lexema != LEX_LEFTTHESIS)
	{
		if (lexTable.table[i].lexema == LEX_ID)
		{
			out << '_' << idTable.table[lexTable.table[i].idxTI].scope << idTable.table[lexTable.table[i].idxTI].id;
			out << ": DWORD";
			stackRet += 4;

			if (lexTable.table[i - 2].lexema != LEX_LEFTTHESIS)
				out << ", ";
		}
		--i;
	}
	i = backup;
	out << std::endl;
}

void CG::Generation::generateFunctionReturn(int& i)
{
	out << "\n\n\tjmp EXIT\nEXIT_DIV_ON_NULL:\n\tpush offset _DIVISION_BY_ZERO_ERROR\n\tcall _outStr \n\tpush -1\n\tcall ExitProcess";
	out << "\n\nEXIT_OVERFLOW:\n\tpush offset _OVERFLOW_ERROR\n\tcall _outStr \n\tpush -2\n\tcall ExitProcess";
	out << "\n\nNEGATIVE_RESULT: \n\tpush offset _NEGATIVE_RESULT_ERROR\n\tcall _outStr \n\tpush -3\n\tcall ExitProcess";
	out << "\n\nEXIT:\n";
	if (isMain) {
		out << "\tpush\t\t";
		if (lexTable.table[i + 1].lexema == LEX_ID) {
			out << '_' << idTable.table[lexTable.table[i + 1].idxTI].scope
				<< idTable.table[lexTable.table[i + 1].idxTI].id;
		}
		else
			out << idTable.table[lexTable.table[i + 1].idxTI].id;
		out << std::endl;
	}
	else
	{
		if (idTable.table[lexTable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR) {
			if (idTable.table[lexTable.table[i + 1].idxTI].idtype != IT::IDTYPE::L)
				out << "\tmov\t\teax, " << '_' << idTable.table[lexTable.table[i + 1].idxTI].scope
				<< idTable.table[lexTable.table[i + 1].idxTI].id << "\n\tret\t\t" << stackRet << std::endl;
			else
				out << "\tmov\t\teax, offset " << idTable.table[lexTable.table[i + 1].idxTI].id << "\n\tret\t\t" << stackRet << std::endl;
		}
		else {
			if (typeOfFunction == IT::IDDATATYPE::UINT) {
				out << "\tcmp\t\t\t" << '_' << idTable.table[lexTable.table[i + 1].idxTI].scope
					<< idTable.table[lexTable.table[i + 1].idxTI].id << ", 0\n";
				out << "\tjl\t\t\t" << "NEGATIVE_RESULT\n";
			}
			if (idTable.table[lexTable.table[i + 1].idxTI].idtype == IT::IDTYPE::L)
				out << "\tmov\t\teax, " << idTable.table[lexTable.table[i + 1].idxTI].id
				<< "\n\tret\t\t" << stackRet << std::endl;
			else
				out << "\tmov\t\teax, " << '_' << idTable.table[lexTable.table[i + 1].idxTI].scope
				<< idTable.table[lexTable.table[i + 1].idxTI].id << "\n\tret\t\t" << stackRet << std::endl;
		}
		stackRet = 0;
	}
}

void CG::Generation::generateFunctioinBody(int& i)
{
	if (lexTable.table[i].lexema == LEX_EQUAL)
		generateEqual(i);
	if (lexTable.table[i].lexema == LEX_PRINT)
		generatePrint(i);
	if (lexTable.table[i].lexema == LEX_IF)
		generateIfStatement(i);
	if (lexTable.table[i].lexema == LEX_MREQUAL)
	{
		if (lexTable.table[i - 1].lexema == LEX_SEMICOLON || lexTable.table[i - 1].lexema == LEX_LSEQUAL)
			++blockOfCode;
	}
	if (lexTable.table[i].lexema == LEX_LSEQUAL)
	{
		if (blockOfCode > 0)
		{
			--blockOfCode;
		}
		else if (ifStatement > 0)
		{
			if (flagelse == 1 && lexTable.table[i + 1].lexema == LEX_ELSE) {
				out << "\tjmp\t\tIFOUT" << Ifsn << "\n";
				out << "FALSE" << Ifsn << ": " << "\n";
				++ifStatement;
			}
			else if (flagelse == 0 && lexTable.table[i + 1].lexema != LEX_ELSE) {
				out << "FALSE" << Ifsn << ": " << "\n";
			}
			else if (flagelse == 1)
				out << "IFOUT" << Ifsn << ": " << "\n";
			--ifStatement;
		}
		else if (isFunc)
		{
			out << '_' << idTable.table[lexTable.table[indOfFunc].idxTI].id << " ENDP\n\n";
			isFunc = false;
			indOfFunc = 0;
		}
		else
		{
			out << "\tcall\t\tExitProcess\n\nmain ENDP\n";
		}
	}
}

void CG::Generation::generatePrint(int& i)
{
	if (lexTable.table[i + 1].lexema == LEX_ID) {
		if (idTable.table[lexTable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::UINT) {
			out << "\tpush\t\t" << '_' << idTable.table[lexTable.table[i + 1].idxTI].scope
				<< idTable.table[lexTable.table[i + 1].idxTI].id;
			out << "\n\tcall\t\t_outInt\n\n";
		}
		else if (idTable.table[lexTable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::BOOL) {
			out << "\tpush\t\t" << '_' << idTable.table[lexTable.table[i + 1].idxTI].scope
				<< idTable.table[lexTable.table[i + 1].idxTI].id;
			out << "\n\tcall\t\t_outBool\n\n";
		}
		else {
			out << "\tpush\t\t" << '_' << idTable.table[lexTable.table[i + 1].idxTI].scope
				<< idTable.table[lexTable.table[i + 1].idxTI].id;
			out << "\n\tcall\t\t_outStr \n\n";
		}
	}
	else if (lexTable.table[i + 1].lexema == LEX_LITERAL) {
		if (idTable.table[lexTable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::UINT) {
			out << "\tpush\t\t" << idTable.table[lexTable.table[i + 1].idxTI].id;
			out << "\n\tcall\t\t_outInt\n\n";
		}
		else if (idTable.table[lexTable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::BOOL) {
			out << "\tpush\t\t" << idTable.table[lexTable.table[i + 1].idxTI].id;
			out << "\n\tcall\t\t_outBool\n\n";
		}
		else {
			out << "\tpush\t\toffset " << idTable.table[lexTable.table[i + 1].idxTI].id;
			out << "\n\tcall\t\t_outStr \n\n";
		}
	}
}

void CG::Generation::generateEqual(int& i)
{
	indOflex = i - 1;
	while (lexTable.table[i].lexema != LEX_SEMICOLON) {
		if (lexTable.table[i].lexema == LEX_ID) {
			if (idTable.table[lexTable.table[i].idxTI].idtype != IT::IDTYPE::F)
			{
				out << "\tpush\t\t" << '_' << idTable.table[lexTable.table[i].idxTI].scope
					<< idTable.table[lexTable.table[i].idxTI].id << "\n";
			}
		}
		if (lexTable.table[i].lexema == LEX_LITERAL) {
			if (idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT ||
				idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
				out << "\tpush\t\t";
			else
				out << "\tpush\t\toffset ";
			out << idTable.table[lexTable.table[i].idxTI].id << "\n";


		}
		if (lexTable.table[i].lexema == LEX_COMMERCIAL_AT)
		{
			out << "\tcall\t\t" << '_' << idTable.table[lexTable.table[i].idxTI].id << "\n";
			out << "\tpush\t\teax\n";
		}
		if (lexTable.table[i].lexema == LEX_PLUS)
		{
			out << "\n";
			out << "\tpop\t\teax\n";
			out << "\tpop\t\tebx\n";
			out << "\tadd\t\teax, ebx\n";
			out << "\tjo\t\tEXIT_OVERFLOW\n";
			out << "\tpush\t\teax\n";
			out << "\n";
		}
		if (lexTable.table[i].lexema == LEX_MINUS)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\tsub\t\teax, ebx\n";
			out << "\tjo\t\tEXIT_OVERFLOW\n";
			out << "\tpush\t\teax\n";
			out << "\n";
		}
		if (lexTable.table[i].lexema == LEX_STAR)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\timul\t\teax, ebx\n";
			out << "\tjo\t\tEXIT_OVERFLOW\n";
			out << "\tpush\t\teax\n";
			out << "\n";
		}
		if (lexTable.table[i].lexema == LEX_DIRSLASH)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\ttest\t\tebx, ebx\n";
			out << "\tjz\t\tEXIT_DIV_ON_NULL\n";
			out << "\tcdq\n";
			out << "\tidiv\t\tebx\n";
			out << "\tjo\t\tEXIT_OVERFLOW\n";
			out << "\tpush\t\teax\n";
			out << "\n";
		}
		if (lexTable.table[i].lexema == LEX_REM_AFTER_DIVIDING)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\ttest\t\tebx, ebx\n";
			out << "\tjz\t\tEXIT_DIV_ON_NULL\n";
			out << "\tcdq\n";
			out << "\tidiv\t\tebx\n";
			out << "\tjo\t\tEXIT_OVERFLOW\n";
			out << "\tpush\t\tedx\n";
			out << "\n";
		}
		if (lexTable.table[i].lexema == LEX_MORE)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\tcmp\t\teax, ebx\n";
			out << "\tja\t\tMORE" << i << "\n";
			out << "\tpush\t\t0\n";
			out << "\tjbe\t\tLESS" << i << "\n";
			out << "MORE" << i << ":" << "\n";
			out << "\tpush\t\t1\n";
			out << "LESS" << i << ":";
			out << "\n";
		}

		if (lexTable.table[i].lexema == LEX_LESS)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\tcmp\t\teax, ebx\n";
			out << "\tjb\t\tLESS" << i << "\n";
			out << "\tpush\t\t0\n";
			out << "\tjae\t\tMORE" << i << "\n";
			out << "LESS" << i << ":" << "\n";
			out << "\tpush\t\t1\n";
			out << "MORE" << i << ":";
			out << "\n";
		}

		if (lexTable.table[i].lexema == LEX_FULL_EQUALS)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\tcmp\t\teax, ebx\n";
			out << "\tjz\t\tNOTEQUAL" << i << "\n";
			out << "\tpush\t\t1\n";
			out << "\tjnz\t\tEQUAL" << i << "\n";
			out << "EQUAL" << i << ":" << "\n";
			out << "\tpush\t\t0\n";
			out << "NOTEQUAL" << i << ":";
			out << "\n";
		}

		if (lexTable.table[i].lexema == LEX_NOT_FULL_EQUALS)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\tcmp\t\teax, ebx\n";
			out << "\tjne\t\tNOTEQUAL" << i << "\n";
			out << "\tpush\t\t0\n";
			out << "\tje\t\tEQUAL" << i << "\n";
			out << "NOTEQUAL" << i << ":" << "\n";
			out << "\tpush\t\t1\n";
			out << "EQUAL" << i << ":";
			out << "\n";
		}

		if (lexTable.table[i].lexema == LEX_MORE_OR_EQUALS)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\tcmp\t\teax, ebx\n";
			out << "\tjae\t\tMOREOREQUAL" << i << "\n";
			out << "\tpush\t\t0\n";
			out << "\tjnb\t\tLESS" << i << "\n";
			out << "MOREOREQUAL" << i << ":" << "\n";
			out << "\tpush\t\t1\n";
			out << "LESS" << i << ":";
			out << "\n";
		}

		if (lexTable.table[i].lexema == LEX_LESS_OR_EQUALS)
		{
			out << "\n";
			out << "\tpop\t\tebx\n";
			out << "\tpop\t\teax\n";
			out << "\tcmp\t\teax, ebx\n";
			out << "\tjbe\t\tLESSOREQUALS" << i << "\n";
			out << "\tpush\t\t0\n";
			out << "\tjna\t\tMORE" << i << "\n";
			out << "LESSOREQUALS" << i << ":" << "\n";
			out << "\tpush\t\t1\n";
			out << "MORE" << i << ":";
			out << "\n";
		}
		i++;
	}

	out << "\tpop\t\t\t" << '_' << idTable.table[lexTable.table[indOflex].idxTI].scope
		<< idTable.table[lexTable.table[indOflex].idxTI].id << "\n\n";
	if (idTable.table[lexTable.table[indOflex].idxTI].iddatatype == IT::IDDATATYPE::UINT) {
		out << "\tcmp\t\t\t" << '_' << idTable.table[lexTable.table[indOflex].idxTI].scope
			<< idTable.table[lexTable.table[indOflex].idxTI].id << ", 0\n";
		out << "\tjl\t\t\t" << "NEGATIVE_RESULT\n";
	}
}

void CG::Generation::generateIfStatement(int& i)
{

	flagelse = 0;
	++ifStatement;
	char operation = ' ';
	if (lexTable.table[i + 2].lexema == LEX_ID)
		out << "\tpush\t\t" << '_' << idTable.table[lexTable.table[i + 2].idxTI].scope << idTable.table[lexTable.table[i + 2].idxTI].id << "\n";
	else if (lexTable.table[i + 2].lexema == LEX_LITERAL)
		out << "\tpush\t\t" << idTable.table[lexTable.table[i + 2].idxTI].id << "\n";

	if (lexTable.table[i + 4].lexema == LEX_ID)
		out << "\tpush\t\t" << '_' << idTable.table[lexTable.table[i + 4].idxTI].scope << idTable.table[lexTable.table[i + 4].idxTI].id << "\n";
	else if (lexTable.table[i + 4].lexema == LEX_LITERAL)
		out << "\tpush\t\t" << idTable.table[lexTable.table[i + 4].idxTI].id << "\n";


	if (lexTable.table[i + 3].lexema != LEX_RIGHTTHESIS) {
		out << "\tpop\t\tebx\n";
		out << "\tpop\t\teax\n";
		out << "\tcmp\t\teax, ebx\n";
	}
	else {

		out << "\tpop\t\teax\n";
		out << "\tcmp\t\teax, 1\n";
	}
	Ifsn = lexTable.table[i].sn;
	int j = i;
	int countOfMrequals = 0;
	while (lexTable.table[j].lexema != LEX_LSEQUAL || countOfMrequals != 0) {
		if (lexTable.table[j].lexema == LEX_MREQUAL) ++countOfMrequals;
		else if (lexTable.table[j + 1].lexema == LEX_LSEQUAL) --countOfMrequals;
		j++;
	}
	if (lexTable.table[j + 1].lexema == LEX_ELSE)
		flagelse = 1;
	switch (lexTable.table[i + 3].lexema) {

	case LEX_FULL_EQUALS:
	{
		out << "\tjne\t\tFALSE" << Ifsn << "\n";
		if (flagelse == 1) {
			out << "\tje\t\tTRUE" << Ifsn << "\n";
			operation = LEX_FULL_EQUALS;
			out << "TRUE" << Ifsn << ": " << "\n";
		}
		break;
	}

	case LEX_NOT_FULL_EQUALS:
	{
		out << "\tje\t\tFALSE" << Ifsn << "\n";
		if (flagelse == 1) {
			out << "\tjne\t\tTRUE" << Ifsn << "\n";
			operation = LEX_EQUAL;
			out << "TRUE" << Ifsn << ": " << "\n";
		}
		break;
	}

	case LEX_MORE:
	{
		out << "\tjna\t\tFALSE" << Ifsn << "\n";
		if (flagelse == 1) {
			out << "\tjnb\t\tTRUE" << Ifsn << "\n";
			operation = LEX_MORE;
			out << "TRUE" << Ifsn << ": " << "\n";
		}
		break;
	}

	case LEX_LESS:
	{
		out << "\tjnb\t\tFALSE" << Ifsn << "\n";
		if (flagelse == 1) {
			out << "\tjna\t\tTRUE" << Ifsn << "\n";
			operation = LEX_LESS;
			out << "TRUE" << Ifsn << ": " << "\n";
		}
		break;
	}

	case LEX_MORE_OR_EQUALS:
	{
		out << "\tjb\t\tFALSE" << Ifsn << "\n";
		if (flagelse == 1) {
			out << "\tja\t\tTRUE" << Ifsn << "\n";
			operation = LEX_MORE_OR_EQUALS;
			out << "TRUE" << Ifsn << ": " << "\n";
		}
		break;
	}

	case LEX_LESS_OR_EQUALS:
	{
		out << "\tja\t\tFALSE" << Ifsn << "\n";
		if (flagelse == 1) {
			out << "\tjb\t\tTRUE" << Ifsn << "\n";
			operation = LEX_LESS_OR_EQUALS;
			out << "TRUE" << Ifsn << ": " << "\n";
		}
		break;
	}
	default:break;
	}
}

void CG::Generation::code()
{
	out << "\n.code\n";
	indOfFunc = -1;
	indOflex = -1;
	isFunc = false;
	isMain = false;
	ifStatement = 0;
	stackRet = 0;
	Ifsn = 0;
	flagelse = 0;
	blockOfCode = 0;
	for (int i = 0; i < lexTable.size; i++) {
		if (lexTable.table[i].lexema == LEX_FUNCTION || lexTable.table[i].lexema == LEX_MAIN)
		{
			if (lexTable.table[i].lexema == LEX_MAIN) isMain = true;
			generateFunctionProto(i);
		}
		generateFunctioinBody(i);
		if (lexTable.table[i].lexema == LEX_RETURN)
			generateFunctionReturn(i);
	}
	out << "end main\n";
}

