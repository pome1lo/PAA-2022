@ECHO OFF
rd /S /Q Result
call "D:\Programs FIle\vs\VC\Auxiliary\Build\vcvarsall.bat" x86
mkdir Result
cd Result
call ..\..\Debug\PAA-2022.exe -in:..\in.txt -out:asm.asm -log:log.txt -mfst:mfst.txt
ml /c /coff /asm.lst asm.asm
link /OPT:NOREF /DEBUG ..\..\Debug\PAA_Lib.lib asm.obj  /SUBSYSTEM:CONSOLE libucrt.lib
call asm.exe
pause