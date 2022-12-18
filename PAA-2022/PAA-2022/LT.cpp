#include "stdafx.h";
#include "LT.h"
#include "Error.h"
#include "Parm.h"

namespace LT {
	LexTable Create(int size) {
		LexTable* table = new LexTable;		
		if (size > LT_MAXSIZE) {			
			throw ERROR_THROW(120);
		}
		table->maxsize = size;				
		table->size = 0;
		table->table = new Entry[size];		

		return *table;
	}

	void Add(LexTable& lextable, Entry entry) {
		if (lextable.size + 1 > lextable.maxsize) {
			throw ERROR_THROW(121);
		}
		lextable.table[lextable.size] = entry;
		++lextable.size;
	}

	Entry GetEntry(LexTable& lextable, int n) {
		if (n > lextable.size) {
			throw ERROR_THROW(123);
		}
		return lextable.table[n];
	}

	void Delete(LexTable& lextable) {
		delete[] lextable.table;
	}

	void ShowTable(LexTable& lextable, std::ofstream* stream) {
		for (int i = 0; i < lextable.size; ++i) {
			if (lextable.table[i].lexema == LEX_SEMICOLON) *stream << lextable.table[i].lexema << std::endl;
			else {
				*stream << lextable.table[i].lexema;
				if (lextable.table[i].idxTI != -1) *stream << "<" << lextable.table[i].idxTI << ">";
			}
		}
		*stream << std::endl << std::endl;
	}
}
