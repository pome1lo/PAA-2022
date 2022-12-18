#include "stdafx.h"
#include "Log.h"
#include "Error.h"

namespace Log
{
	LOG getlog(std::string const logfile) {	
		LOG log;
		log.stream = new std::ofstream;
		log.stream->open(logfile);
		if (log.stream->fail()) {
			throw ERROR_THROW(112);		
		}
		log.logfile = logfile;
		return log;
	}

	void WriteLine(LOG log, const char* c, ...) {
		if (log.stream) {
			const char** ptr = &c;
			while (*ptr != "") {
				*log.stream << *ptr;
				ptr++;
			}
			*log.stream << "\n";
		}
	}

	void WriteLine(LOG log, const wchar_t* c, ...) {
		if (log.stream) {
			size_t i;
			const wchar_t** ptr = &c;
			char temp[100];
			while (*ptr != L"") {
				wcstombs_s(&i, temp, sizeof(temp), *ptr++, sizeof(temp));
				*log.stream << temp;
			}
			*log.stream << "\n";
		}
	}

	void WriteLog(LOG log) {	
		time_t now = time(0);
		char dt[100];
		tm u;
		localtime_s(&u, &now);
		ctime_s(dt, 100, &now);
		strftime(dt, 40, "%d.%m.%Y %H:%M:%S ", &u);
		*log.stream << "------ Протокол ------\nДата: " << dt << " \n\n";
	}

	void WriteParm(LOG log, Parm::PARM parm) {
		*log.stream << "------ Параметры ------\n" <<
			PARM_IN  << parm.in << "\n" <<
			PARM_OUT << parm.out << "\n" <<
			PARM_LOG << parm.log << "\n" <<
			PARM_MFST << parm.mfst << "\n" << "\n";
	}

	void WriteIn(LOG log, In::IN in) {
		*log.stream << "------ Исходные данные ------\n" <<
			"Количество символов: " << in.size <<
			"\nПроигнорировано: " << in.ignor <<
			"\nКоличество строк: " << in.lines << "\n\n";
	}

	void WriteError(LOG log, Error::ERROR e) {
		if (log.stream) {
			*log.stream << "Ошибка " << e.id << ": " << e.message;
			if (e.place.col != -1 && e.place.line != -1) {
				*log.stream << ", строка " << e.place.line << " ,позиция " << e.place.col << std::endl;
			}
			else if (e.word.line != -1 && e.word.word != "") {
				*log.stream << ", строка " << e.word.line << " ,слово " << e.word.word << std::endl;
			}
			else if (e.place.col == -1 && e.place.line != -1) {
				*log.stream << ", строка " << e.place.line << std::endl;
			}
		}
		else {
			std::cout << "Ошибка " << e.id << ": " << e.message;
		}

	}
	void Close(LOG log) {
		if (log.stream) log.stream->close();
	}
}