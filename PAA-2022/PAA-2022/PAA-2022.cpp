#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "In.h"
#include "Log.h"
#include "IT.h"
#include "LT.h"
#include "Lex.h"
#include "GRB.h"
#include "MFST.h"
#include "PN.h"	
#include "Sem.h"
#include "CG.h"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getParm(argc, argv);  // структура со всеми именами файлов
		log = Log::getlog(parm.log);				  // создание .log файла
		Log::WriteLog(log);					          // создаем шапку протокола .log ------ Протокол ------ Дата: 03.12.2022 23 : 55 : 27
		Log::WriteParm(log, parm);					  // записываем в лог все входные параметры
		In::IN in = In::getin(parm.in);				  // проверка символов исходного кода на доступность в нашей языке | возврат in - структура с кодом размером игнором и тд
		Log::WriteIn(log, in);						  // закидываем в журнал из пред. строки Количество символов Проигнорировано Колво строк и тд (из структуры in из прошлой строки)
		IT::IdTable idTable = IT::Create(in.size);    // создали IT с макс размером на колво символов из in (из структуры (из исходного кода) )
		LT::LexTable lexTable = LT::Create(in.size);  // создали LT с макс размером на колво символов из in (из структуры (из исходного кода) )
	    Lex::Scan(lexTable, idTable, in, parm, log);  /* токенизируем исходный код	        На этом этапе исходный код прогрраммы распознается лексером и каждому идентификатору или лексеме
																							(из дэфайнов файлов Regs.h и LT.h), а точнее каждому слову сапостовяется определенный дэфайн и 
																							результат, в виде таблицы лексем и таблицы идентификаторов сохраняется в файл */
		
		MFST::Mfst mfst(lexTable, GRB::getGreibach(), parm.mfst);							// как я понял инициализация мп автомата и присваение всего всего
		mfst.start(*log.stream);							// ПЕРЕСМОТРЕТЬ РАЗОБРАТЬСЯ!!!!	// мп автомат разбор 
		Log::WriteLine(log, "------ Таблица идентификаторов ------", "");					// закидываем в журнал строку
		IT::ShowTable(idTable, log.stream);													// закидываем в журнал IT	
		Log::WriteLine(log, "------ Таблица лексем ------", "");						    // закидываем в журнал строку
		LT::ShowTable(lexTable, log.stream);												// закидываем в журнал LT
		Log::WriteLine(log, "------ Синтаксический анализ выполнен без ошибок ------", ""); // закидываем в журнал строку
		mfst.savededucation();
		mfst.printrules();																	// получаем правила и печает в лог
		PN::PolishNotation(lexTable, idTable);												// польская натация		ищем знак = и после разбираем все что до него
		Log::WriteLine(log, "------ Польская нотация выполнена без ошибок ------", "");	    // закидываем в журнал строку

		LT::ShowTable(lexTable, log.stream);												

		Sem::SemanticAnalysis sem = Sem::SemanticAnalysis(lexTable, idTable);				// как я понял инициализуем все вхлдные данные  в исходники семантического анализатора
		sem.start();
		Log::WriteLine(log, "------ Семантический анализ выполнен без ошибок ------", "");	// закидываем в журнал строку
		CG::Generation writeToOut = CG::Generation(lexTable, idTable, parm.out);
		writeToOut.start();
		Log::WriteLine(log, "------ Генерация кода выполнен без ошибок ------", "");		// закидываем в журнал строку
#ifdef _DEBUG
		CG::Generation writeToASM = CG::Generation(lexTable, idTable, "../PAA-ASM/asm.asm");
		writeToASM.start();
#endif 
		Log::Close(log);																	// закрываем лог
	}
	catch (Error::ERROR err)
	{
		std::cout << "Ошибка: " << err.id << ' ' << err.message << ' ' << std::endl;
		std::cout << "Строка: " << err.place.line << std::endl;
		Log::WriteError(log, err);
		Log::Close(log);
	}
	return 0;
}