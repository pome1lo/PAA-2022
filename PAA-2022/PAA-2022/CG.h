#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"

namespace CG
{
	struct Generation
	{
		LT::LexTable lexTable;
		IT::IdTable idTable;
		std::ofstream out;
		Generation(LT::LexTable lexT, IT::IdTable idT, std::string outFile);
		void start();

	private:
		bool isMain;
		bool isFunc;
		IT::IDDATATYPE typeOfFunction;
		int stackRet;
		int indOflex;
		int ifStatement;
		int flagelse;
		int indOfFunc;
		int Ifsn;
		int blockOfCode;

		void head();
		void constants();
		void data();
		void code();

		void generateFunctionProto(int& i);
		void generateFunctioinBody(int& i);
		void generateFunctionReturn(int& i);

		void generatePrint(int& i);
		void generateEqual(int& i);
		void generateIfStatement(int& i);
	};
}