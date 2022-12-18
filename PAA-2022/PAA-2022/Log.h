#pragma once

#include "In.h"
#include "Error.h"
#include "Parm.h"

namespace Log
{
	struct LOG					
	{
		std::string logfile;    
		std::ofstream* stream;  
	};

	static const LOG INITLOG = { "", NULL };     
	LOG getlog(std::string logfile);			 
	void WriteLine(LOG log, const char* c, ...); 
	void WriteLine(LOG log, wchar_t* c, ...);	 
	void WriteLog(LOG log);						 
	void WriteParm(LOG log, Parm::PARM parm);	 
	void WriteIn(LOG log, In::IN in);		     
	void WriteError(LOG log, Error::ERROR e);	 
	void Close(LOG log);						 
}
