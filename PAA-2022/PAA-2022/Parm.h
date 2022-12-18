#pragma once

#define PARM_IN "-in:"					
#define PARM_OUT "-out:"				
#define PARM_LOG "-log:"				
#define PARM_MFST "-mfst:"				
#define PARM_MAX_SIZE 300				
#define PARM_OUT_DEFAULT_EXT ".asm"		
#define PARM_LOG_DEFAULT_EXT ".log"		
#define PARM_MFST_DEFAULT_EXT ".mfst"	

namespace Parm
{
	struct PARM
	{
		std::string in = "";   
		std::string out = "";  
		std::string log = "";  
		std::string mfst = ""; 
	};

	PARM getParm(int argc, char* argv[]);
}