#pragma once
#include "stdafx.h";
#define IN_MAX_LEN_TEXT 1024*1024   
#define IN_CODE_SEP '|'				


#define IN_CODE_TABLE {\
	IN::F,   IN::F,  IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F,  IN::SPC, IN::S,  IN::F,  IN::F,   IN::F,  IN::F,  IN::F, \
	IN::F,   IN::F,  IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F,  IN::F,   IN::F,  IN::F,  IN::F,   IN::F,  IN::F,  IN::F, \
	IN::SPC, IN::LX, IN::F, IN::F, IN::F, IN::LX, IN::F, IN::T, IN::LX, IN::LX,  IN::LX, IN::LX, IN::LX,  IN::LX, IN::F,  IN::LX,\
	IN::T,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::LX, IN::LX,  IN::LX, IN::LX, IN::F, \
	IN::F,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::T,  IN::T,   IN::T,  IN::T,  IN::T, \
	IN::T,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::F,  IN::F,   IN::F,  IN::F,  IN::F, \
	IN::F,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::T,  IN::T,   IN::T,  IN::T,  IN::T, \
	IN::T,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::LX, IN::F,   IN::LX, IN::F,  IN::F, \
																													\
	IN::F,   IN::F,  IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F,  IN::F,   IN::F,  IN::F,  IN::F,   IN::F,  IN::F,  IN::F, \
	IN::F,   IN::F,  IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F,  IN::F,   IN::F,  IN::F,  IN::F,   IN::F,  IN::F,  IN::F, \
	IN::F,   IN::F,  IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F,  IN::F,   IN::F,  IN::F,  IN::F,   IN::F,  IN::F,  IN::F, \
	IN::F,   IN::F,  IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F,  IN::F,   IN::F,  IN::F,  IN::F,   IN::F,  IN::F,  IN::F, \
	IN::T,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::T,  IN::T,   IN::T,  IN::T,  IN::T, \
	IN::T,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::T,  IN::T,   IN::T,  IN::T,  IN::T, \
	IN::T,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::T,  IN::T,   IN::T,  IN::T,  IN::T, \
	IN::T,   IN::T,  IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T,  IN::T,   IN::T,  IN::T,  IN::T,   IN::T,  IN::T,  IN::T, \
}

namespace In
{
	struct IN
	{
		enum { T = 1024, F = 2048, I = 4096, SPC = 8192, S = 16384, LX = 32568 };
		int size = 0;
		int lines = 0;
		int ignor = 0;
		std::string text = "";
		std::array<int, 256> code = IN_CODE_TABLE;
	};
	IN getin(std::string infile);					
}