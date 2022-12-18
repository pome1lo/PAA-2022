#include "stdafx.h"
#include "Lex.h"
#include "LT.h"
#include "In.h"
#include "REGS.h"
#include "Log.h"

char Lex::GetToken(std::string str) {
	std::array<std::string, 29> regexps = {
		REG_DECLARE, REG_STRING, REG_INTEGER, REG_FUNCTION, REG_RETURN, REG_PRINT,
		REG_MAIN, REG_INTEGER_LIT, REG_STRING_LIT, REG_SEMICOLON, REG_COMMA,
		REG_MREQUAL, REG_LSEQUAL, REG_LEFTTHESIS, REG_RIGHTTHESIS, REG_PLUS, REG_MINUS,
		REG_STAR, REG_DIRSLASH, REG_EQUAL,REG_IF, REG_ELSE, REG_MORE, REG_LESS, REG_EXCLAMATION, REG_BOOL, REG_BOOL_LIT, REG_REM_AFTER_DIVIDING,
		REG_ID
	};
	std::array<char, 29> tokens = {
		LEX_DECLARE, LEX_STRING, LEX_INTEGER, LEX_FUNCTION, LEX_RETURN, LEX_PRINT,
		LEX_MAIN, LEX_INTEGER_LIT, LEX_STRING_LIT, LEX_SEMICOLON, LEX_COMMA,
		LEX_MREQUAL, LEX_LSEQUAL, LEX_LEFTTHESIS, LEX_RIGHTTHESIS, LEX_PLUS, LEX_MINUS,
		LEX_STAR, LEX_DIRSLASH, LEX_EQUAL, LEX_IF, LEX_ELSE, LEX_MORE, LEX_LESS, LEX_EXCLAMATION, LEX_BOOL, LEX_BOOL_LIT, LEX_REM_AFTER_DIVIDING,
		LEX_ID
	};
	for (int i = 0; i < sizeof(regexps) / sizeof(regexps[0]); ++i) {
		if (std::regex_match(str, std::regex(regexps[i]))) {
			return tokens[i];
		}
	}
	return EOF;
}


void Lex::Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, Parm::PARM& parm, Log::LOG& log) {
	std::ofstream outfile(parm.out);						
	outfile << in.text << std::endl;						
	outfile << "0001 ";

	std::string word = "";	
	std::string cur_scope = "";								
	std::vector<std::string> ti_scope = { TI_SCOPE_DEFAULT };

	int counter = 0;										
	bool findStringLit = false;								
	bool inLineFunction = false;							
	int isMain = 0;											
	int scopeCounter = 0;									
	for (int i = 0, line = 1; i < in.text.size(); ++i)
	{
		IT::IDDATATYPE iddatatype;

		auto fillTable = [&] 
		{
			char token = Lex::GetToken(word);
			int ti_idx = TI_NULLIDX;

			iddatatype = (token == LEX_INTEGER || token == LEX_INTEGER_LIT) ? IT::IDDATATYPE::UINT :
				(token == LEX_STRING || token == LEX_STRING_LIT) ? IT::IDDATATYPE::STR :
				(token == LEX_BOOL || token == LEX_BOOL_LIT) ? IT::IDDATATYPE::BOOL : iddatatype;

			switch (token)
			{
			case LEX_MAIN:
				IT::Add(idtable, { lextable.size + 1, word, ti_scope.back(), IT::IDDATATYPE::UINT, IT::IDTYPE::F });
				cur_scope = "main";
				isMain == 0 ? ++isMain : throw ERROR_THROW(128);
				break;
			case LEX_ID:
				if (word.size() > 10) throw ERROR_THROW_IN(133, line, -1);
				if (lextable.size >= 3 && lextable.table[lextable.size - 2].lexema != LEX_DECLARE && lextable.table[lextable.size - 3].lexema != LEX_DECLARE) {
					ti_idx = IT::IsId(idtable, word, ti_scope);
				}

				if (ti_idx == TI_NULLIDX)
				{
					if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_FUNCTION)
					{
						IT::Add(idtable, { lextable.size, word, ti_scope.back(), iddatatype, IT::IDTYPE::F });
						cur_scope = word;
						if (lextable.size >= 3 && lextable.table[lextable.size - 3].lexema == LEX_DECLARE) {
							inLineFunction = true;
						}
					}
					else if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexema == LEX_DECLARE && lextable.table[lextable.size - 1].lexema == LEX_DATATYPE) {
						IT::Add(idtable, { lextable.size, word, ti_scope.back(), iddatatype, IT::IDTYPE::V });
					}
					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_DATATYPE) {
						IT::Add(idtable, { lextable.size, word, ti_scope.back(), iddatatype, IT::IDTYPE::P });
					}
					else {
						throw ERROR_THROW_IN(131, line, -1);
					}
					ti_idx = idtable.size - 1;
				}
				break;
			case LEX_LEFTTHESIS:
				if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexema == LEX_FUNCTION) {
					ti_scope.push_back(cur_scope);
				}
				break;
			case LEX_MREQUAL:
				if (cur_scope.empty()) {
					cur_scope = "scope_" + std::to_string(scopeCounter);
					++scopeCounter;
					ti_scope.push_back(cur_scope);
				}
				if (ti_scope.back() != cur_scope) {
					ti_scope.push_back(cur_scope);
				}
				cur_scope = "";
				break;
			case LEX_RIGHTTHESIS:
				if (inLineFunction) {
					ti_scope.pop_back();
					inLineFunction = false;
					cur_scope = "";
				}
				break;
			case LEX_LSEQUAL:
				ti_scope.pop_back();
				break;
			case LEX_STRING_LIT:
				word = word.substr(1, word.length() - 2);
				if (word.length() > TI_STR_MAXSIZE) throw ERROR_THROW_IN(124, line, -1);
				ti_idx = IT::isLit(idtable, word);
				if (ti_idx == TI_NULLIDX) {
					IT::Add(idtable, { lextable.size, "L" + std::to_string(counter), ti_scope.back(), IT::IDTYPE::L, word.c_str() });
					++counter;
					ti_idx = idtable.size - 1;
				}
				token = LEX_LITERAL;
				break;
			case LEX_INTEGER_LIT: {
				if (lextable.size >= 2 && lextable.table[lextable.size - 1].lexema == LEX_MINUS && lextable.table[lextable.size - 2].idxTI == -1)
				{
					word = "-" + word;
					--lextable.size;
				}
				if (word[word.length() - 1] == 'o') {
					word = std::to_string(std::stol(word, 0, 8));
				}
				if (std::stoll(word) > LONG_MAX || std::stoll(word) < LONG_MIN)
					throw ERROR_THROW_IN(130, line, -1);
				int temp = std::stoi(word);
				ti_idx = IT::isLit(idtable, std::stoi(word));
				if (ti_idx == TI_NULLIDX) {
					IT::Add(idtable, { lextable.size, "L" + std::to_string(counter), ti_scope.back(), IT::IDTYPE::L, static_cast<int>(temp) });
					++counter;
					ti_idx = idtable.size - 1;
				}
				token = LEX_LITERAL;
				break;
			}
			case LEX_BOOL_LIT: {
				word = (word == "true") ? "1\0" : "0\0";
				std::cin.ignore(0, '\n');
				bool temp = (bool)std::stoi(word);
				ti_idx = IT::isLit(idtable, temp);
				if (ti_idx == TI_NULLIDX) {
					IT::Add(idtable, { lextable.size, "L" + std::to_string(counter), ti_scope.back(), IT::IDTYPE::L, static_cast<bool>(temp) });
					++counter;
					ti_idx = idtable.size - 1;
				}
				token = LEX_LITERAL;
				break;
			}
			case LEX_EQUAL: {
				if (lextable.size >= 1 && (lextable.table[lextable.size - 1].lexema == LEX_MORE || lextable.table[lextable.size - 1].lexema == LEX_LESS ||
					lextable.table[lextable.size - 1].lexema == LEX_EQUAL || lextable.table[lextable.size - 1].lexema == LEX_EXCLAMATION)) {
					--lextable.size;
					switch (lextable.table[lextable.size].lexema)
					{
					case LEX_MORE:
						token = LEX_MORE_OR_EQUALS;
						break;
					case LEX_EQUAL:
						token = LEX_FULL_EQUALS;
						break;
					case LEX_LESS:
						token = LEX_LESS_OR_EQUALS;
						break;
					case LEX_EXCLAMATION:
						token = LEX_NOT_FULL_EQUALS;
						break;
					}
				}
				break;
			}
			case LEX_STRING:
			case LEX_INTEGER:
			case LEX_BOOL:
				token = LEX_DATATYPE;
				break;
			case EOF:
				throw ERROR_THROW_WORD(122, line, word);
			default:
				break;
			}
			LT::Add(lextable, { token, line, ti_idx });
			outfile << token;
			word.clear();
		};

		unsigned char ch = in.text[i];
		if (ch == '\'') findStringLit = !findStringLit;

		if ((In::IN::LX == in.code[ch] || In::IN::SPC == in.code[ch] || (int)ch == (int)IN_CODE_SEP) && !findStringLit)
		{
			if (!word.empty())
			{
				fillTable();
			}
			if (in.code[ch] == In::IN::LX) {
				word = ch;
				fillTable();
			}
			if ((int)ch == (int)IN_CODE_SEP) {
				if (in.text[i + 1]) {
					outfile << "\n" << std::setw(4) << std::setfill('0') << ++line << " ";
				}
			}
		}
		else if (in.code[ch] == In::IN::T || findStringLit) {
			if (findStringLit && word.size() > TI_STR_MAXSIZE)
				throw ERROR_THROW_IN(124, line, -1);
			else if (i + 1 == in.text.size() && in.text[i] != '\'')
				throw ERROR_THROW(132);
			word += ch;
		}
	}
	if (isMain == 0) throw ERROR_THROW(129);
}