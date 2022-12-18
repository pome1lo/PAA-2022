#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"


namespace PN {
	void PolishNotation(LT::LexTable& lexTable, IT::IdTable& idTable);
	void ConvertToPolishNotation(LT::LexTable& lexTable, IT::IdTable& idTable, int startIndex, LT::Entry* expression);
}