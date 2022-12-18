#include "stdafx.h"
#include "In.h"
#include "Error.h"

In::IN In::getin(std::string const infile) {
	auto result = new IN();
	std::ifstream fin(infile);
	if (!fin) throw ERROR_THROW(110);				
	unsigned char symbol;
	int numberInLine = 0;
	while (result->size < IN_MAX_LEN_TEXT) {
		symbol = fin.get();							
		if (fin.eof()) {							
			result->text += IN_CODE_SEP;			
			++result->lines;						
			break;									
		}
		switch (result->code[(unsigned int)symbol]) {	
		case IN::T:
			result->text += symbol;
			++result->size;
			++numberInLine;
			break;
		case IN::F:
			throw ERROR_THROW_IN(111, result->lines + 1, numberInLine + 1); 
		case IN::I:			
			++numberInLine;				
			++result->ignor;
			break;
		case IN::SPC:		
			if ((result->size - 1) < 0 || (result->code[(unsigned int)((unsigned char)result->text[result->size - 1])] != IN::SPC && result->code[(unsigned int)((unsigned char)result->text[result->size - 1])] != IN::LX)) {
				result->text += symbol;	
				++result->size;			
			}
			else {
				++result->ignor;
			}
			++numberInLine;
			break;
		case IN::S:						
			if ((result->size - 1 > 0) && (int)result->text[result->size - 1] != (int)IN_CODE_SEP) {
				result->text += IN_CODE_SEP;	
				++result->size;

				numberInLine = 0;				
			}
			else
			{
				++result->ignor;				
			}
			++result->lines;	
			break;
		case IN::LX:							
			if (((result->size - 1 < 0) && result->code[(unsigned int)((unsigned char)result->text[result->size - 1])] != IN::SPC) || ((result->size - 1 > 0) && result->code[(unsigned int)((unsigned char)result->text[result->size - 1])] == IN::SPC)) {
				result->text[result->size - 1] = symbol;
			}
			else {
				result->text += symbol;
				++result->size;
				++numberInLine;
			}
			break;
		default:
			result->text += result->code[(unsigned int)symbol]; 
			++result->size;
			++numberInLine;
			break;
		}
	}
	fin.close();
	return *result;
}