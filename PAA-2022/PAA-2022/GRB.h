#pragma once
#include "Error.h"


typedef short GRBALPHABET;				
namespace GRB
{
	struct Rule							
	{
		GRBALPHABET nn;					
		int iderror;					
		short size;						
		struct Chain					
		{
			short size;					
			GRBALPHABET* nt;			
			Chain()
			{
				this->size = 0; 		
				this->nt = 0;			
			};
			Chain(short psize, GRBALPHABET s, ...);						
			char* getCChain(char* b);									
			static GRBALPHABET T(char t) { return short(t); };	
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); };
			static bool isT(GRBALPHABET s) { return s > 0; };		
			static bool isN(GRBALPHABET s) { return !isT(s); };		
			static char alphabet_to_char(GRBALPHABET s)				
			{
				return isT(s) ? char(s) : char(-s);
			};
		}*chains;			
		Rule()
		{
			this->nn = 0x00;
			this->size = 0;
		}
		Rule(GRBALPHABET pnn, int iderroe, short psize, Chain c, ...);
		
		char* getCRule(char* b, short nchain); 
		short getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j); 
	};
	struct Greibach			
	{
		short size;			
		GRBALPHABET startN;	
		GRBALPHABET stbottomT;
		Rule* rules;			
		Greibach() { this->size = 0; this->startN = 0; this->stbottomT = 0; this->rules = 0; };
		Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...);
		
		short getRule(GRBALPHABET pnn, Rule& prule);
		
		Rule getRule(short n);
	};
	Greibach getGreibach();		
}
