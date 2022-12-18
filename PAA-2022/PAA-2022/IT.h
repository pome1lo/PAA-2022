#pragma once
#include "stdafx.h"
#define ID_MAXSIZE		10				
#define TI_MAXSIZE		4096			
#define TI_INT_DEFAULT	0x00000000		
#define TI_STR_DEFAULT	0x00			
#define TI_BOOL_DEFAULT 0				
#define TI_NULLIDX		0xffffffff		
#define TI_STR_MAXSIZE	255				
#define TI_SCOPE_DEFAULT "global"		

namespace IT
{
	enum class IDDATATYPE { UINT = 1, STR = 2, BOOL = 3 }; 
	enum class IDTYPE { V = 1, F = 2, P = 3, L = 4 };	   

	struct Entry					
	{
		int			idxfirstLE;		
		std::string id;				
		std::string scope;			
		IDDATATYPE	iddatatype;		
		IDTYPE		idtype;			
		union
		{
			int vint;					
			struct
			{
				char str[TI_STR_MAXSIZE - 1];	
				int len;						
			} vstr;								
			bool vbool;
		} value;						 
		Entry() = default;
		Entry(int idxfirstLE, std::string id, std::string scope, IDTYPE idtype, int value);
		Entry(int idxfirstLE, std::string id, std::string scope, IDTYPE idtype, const char* value);
		Entry(int idxfirstLE, std::string id, std::string scope, IDTYPE idtype, bool value);
		Entry(int idxfirstLE, std::string id, std::string scope, IDDATATYPE iddatatype, IDTYPE idtype);
	};

	struct IdTable						
	{
		int maxsize;					
		int size;						
		Entry* table;					
	};

	IdTable Create(						
		int size
	);

	void Add(							
		IdTable& idtable,
		Entry entry
	);

	Entry GetEntry(						
		IdTable& idtable,
		int n
	);

	int IsId(							
		IdTable& idtable,
		std::string id,
		std::vector<std::string> scope
	);

	int isLit(
		IdTable& idtable,
		std::string lit
	);
	int isLit(
		IdTable& idtable,
		int lit
	);
	int isLit(
		IdTable& idtable,
		bool lit
	);
	void ShowTable(IdTable& idtable, std::ofstream* stream);
	void Delete(IdTable& idtable);
};