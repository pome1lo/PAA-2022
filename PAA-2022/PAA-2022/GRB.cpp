#include "stdafx.h"
#include "GRB.h"
#define GRB_ERROR_SERIES 600
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)
namespace GRB
{
#pragma region CONSTRUCTORS
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		this->nt = new GRBALPHABET[this->size = psize];

		int* ptr = (int*)&s;
		for (int i = 0; i < psize; i++)
			this->nt[i] = (short)ptr[i];
	}

	Rule::Rule(GRBALPHABET pnn, int iderroe, short psize, Chain c, ...)
	{
		this->nn = pnn;
		this->iderror = iderroe;
		this->chains = new Chain[this->size = psize];
		Chain* ptr = &c;
		for (int i = 0; i < psize; i++)
			this->chains[i] = ptr[i];
	}
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...)
	{
		this->startN = pstartN;
		this->stbottomT = pstbottomT;
		this->rules = new Rule[this->size = psize];
		Rule* ptr = &r;
		for (int i = 0; i < psize; i++)
			rules[i] = ptr[i];
	}
#pragma endregion
#pragma region METHODS
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1, k = 0;
		while (k < this->size && rules[k].nn != pnn)
			k++;
		if (k < this->size)
			prule = rules[rc = k];
		return rc;
	}
	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < this->size)
			rc = rules[n];
		return rc;
	}
	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[200];
		b[0] = Chain::alphabet_to_char(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0;
		this->chains[nchain].getCChain(buf);
		strcat_s(b, sizeof(buf) + 5, buf);

		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;

		while (j < this->size && this->chains[j].nt[0] != t)
			j++;

		rc = (j < this->size ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	}
	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
			b[i] = Chain::alphabet_to_char(this->nt[i]);
		b[this->size] = 0;
		return b;
	}
#pragma endregion
#pragma region GRB_rules	
	Greibach greibach(NS('S'), TS('$'),			
		12,										

		Rule(NS('S'), GRB_ERROR_SERIES + 0, 3,					// Неверная структура программы	
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), NS('P'), NS('T'), NS('S')),
			Rule::Chain(2, TS('m'), NS('T')),
			Rule::Chain(7, TS('d'), TS('t'), TS('f'), TS('i'), NS('P'), TS(';'), NS('S'))
		),

		Rule(NS('T'), GRB_ERROR_SERIES + 2, 2,					// Ошибка в теле функции
			Rule::Chain(5, TS('{'), TS('r'), NS('V'), TS(';'), TS('}')),
			Rule::Chain(6, TS('{'), NS('K'), TS('r'), NS('V'), TS(';'), TS('}'))
		),

		Rule(NS('P'), GRB_ERROR_SERIES + 1, 2,					// Не найден список параметров функции	
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 4, 2,					// Ошибка в списке параметров функции						
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('E')),
			Rule::Chain(2, TS('t'), TS('i'))
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 5, 2,					// Ошибка в вызове функции						
			Rule::Chain(3, TS('('), NS('N'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

		Rule(NS('N'), GRB_ERROR_SERIES + 6, 4,					// Ошибка в списке параметров функции		
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('N')),
			Rule::Chain(3, TS('l'), TS(','), NS('N'))
		),

		Rule(NS('Z'), GRB_ERROR_SERIES + 7, 5,					// Ошибка в условии условного выражения	
			Rule::Chain(5, TS('('), TS('i'), NS('L'), TS('i'), TS(')')),
			Rule::Chain(5, TS('('), TS('i'), NS('L'), TS('l'), TS(')')),
			Rule::Chain(5, TS('('), TS('l'), NS('L'), TS('i'), TS(')')),
			Rule::Chain(5, TS('('), TS('l'), NS('L'), TS('l'), TS(')')),
			Rule::Chain(3, TS('('), NS('V'), TS(')'))
		),

		Rule(NS('L'), GRB_ERROR_SERIES + 8, 6,					// Неверный условный оператор		
			Rule::Chain(1, TS('<')),
			Rule::Chain(1, TS('>')),
			Rule::Chain(1, TS('$')),
			Rule::Chain(1, TS('#')),
			Rule::Chain(1, TS('~')),
			Rule::Chain(1, TS('&'))
		),

		Rule(NS('A'), GRB_ERROR_SERIES + 9, 5,					// Неверный арифметический оператор
			Rule::Chain(1, TS('+')),
			Rule::Chain(1, TS('-')),
			Rule::Chain(1, TS('*')),
			Rule::Chain(1, TS('/')),
			Rule::Chain(1, TS('%'))
		),

		Rule(NS('V'), GRB_ERROR_SERIES + 10, 2,					// Неверное выражение. Ожидаются только идентификаторы и литералы
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i'))
		),

		Rule(NS('W'), GRB_ERROR_SERIES + 11, 10,				// Ошибка в арифметичском выражении
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('A'), NS('W')),
			Rule::Chain(2, TS('i'), NS('F')),
			Rule::Chain(3, TS('i'), NS('A'), NS('W')),
			Rule::Chain(4, TS('i'), NS('F'), NS('A'), NS('W')),
			Rule::Chain(3, TS('l'), NS('A'), NS('W')),
			Rule::Chain(3, TS('i'), NS('L'), NS('W')),
			Rule::Chain(3, TS('l'), NS('L'), NS('W'))
		),

		Rule(NS('K'), GRB_ERROR_SERIES + 12, 16,				// Недопустимая синтаксическая конструкция
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('W'), TS(';'), NS('K')),
			Rule::Chain(4, TS('i'), TS('='), NS('W'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('K')),
			Rule::Chain(5, TS('i'), TS('='), NS('W'), TS(';'), NS('K')),
			Rule::Chain(4, TS('p'), NS('V'), TS(';'), NS('K')),
			Rule::Chain(3, TS('{'), NS('K'), TS('}')),
			Rule::Chain(4, TS('{'), NS('K'), TS('}'), NS('K')),
			Rule::Chain(6, TS('e'), NS('Z'), TS('{'), NS('K'), TS('}'), NS('K')),
			Rule::Chain(10, TS('e'), NS('Z'), TS('{'), NS('K'), TS('}'), TS('y'), TS('{'), NS('K'), TS('}'), NS('K')),
			Rule::Chain(4, TS('i'), NS('F'), TS(';'), NS('K')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('W'), TS(';')),
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('p'), NS('V'), TS(';')),
			Rule::Chain(5, TS('e'), NS('Z'), TS('{'), NS('K'), TS('}')),
			Rule::Chain(9, TS('e'), NS('Z'), TS('{'), NS('K'), TS('}'), TS('y'), TS('{'), NS('K'), TS('}')),
			Rule::Chain(3, TS('i'), NS('F'), TS(';'))
		)
	);
#pragma endregion

	Greibach getGreibach() { return greibach; }
}