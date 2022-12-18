#pragma once
#include "stdafx.h"
#include "IT.h"
#include "LT.h"
#include "Parm.h"
#include "Log.h"

namespace Lex {
	char GetToken(std::string str);
	void Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, Parm::PARM& parm, Log::LOG& log);
}