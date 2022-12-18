.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/PAA_Lib.lib
ExitProcess PROTO : DWORD
_strCmp PROTO : DWORD, :DWORD
_strLen PROTO : DWORD
_outStr PROTO : DWORD
_outBool PROTO : DWORD
_outInt PROTO : DWORD


.stack 4096
.const
	_DIVISION_BY_ZERO_ERROR BYTE 'Ошибка выполнения: деление на ноль', 0
	_OVERFLOW_ERROR BYTE 'Ошибка выполнения: переполнение', 0
	_NEGATIVE_RESULT_ERROR BYTE 'Ошибка выполнения: попытка присвоения отрицательного значения', 0
	L0 DWORD 1
	L1 DWORD 0
	L2 BYTE 'aaa', 0
	L3 BYTE 'bbb', 0
	L4 BYTE 'Строка 1 больше строки 2', 0
	L5 DWORD 2
	L6 BYTE 'Строка 2 больше строки 1', 0
	L7 DWORD 3
	L8 BYTE 'Длина строк не совпадает,или передана пустая строка', 0
	L9 BYTE 'hello', 0
.data
	_factorialret		DWORD 0 ; uint
	_scope_2y		DWORD 0 ; uint
	_strCmpTestres		DWORD 0 ; uint
	_strCmpTestoutStr		DWORD ? ; str
	_mainx		DWORD 0 ; uint
	_mainy		DWORD 0 ; uint
	_mainz		DWORD ? ; str

.code
_factorial PROC _factorialx: DWORD
	push		L0
	pop			_factorialret

	cmp			_factorialret, 0
	jl			NEGATIVE_RESULT
	push		_factorialx
	push		L1
	pop		ebx
	pop		eax
	cmp		eax, ebx
	jnb		FALSE4
	push		L1
	pop			_factorialret

	cmp			_factorialret, 0
	jl			NEGATIVE_RESULT
FALSE4: 
	push		_factorialx
	push		L1
	pop		ebx
	pop		eax
	cmp		eax, ebx
	jne		FALSE8
	push		L0
	pop			_factorialret

	cmp			_factorialret, 0
	jl			NEGATIVE_RESULT
FALSE8: 
	push		_factorialx
	push		L1
	pop		ebx
	pop		eax
	cmp		eax, ebx
	jna		FALSE12
	push		_factorialx
	push		L0

	pop		ebx
	pop		eax
	sub		eax, ebx
	jo		EXIT_OVERFLOW
	push		eax

	pop			_scope_2y

	cmp			_scope_2y, 0
	jl			NEGATIVE_RESULT
	push		_scope_2y
	call		_factorial
	push		eax
	push		_factorialx

	pop		ebx
	pop		eax
	imul		eax, ebx
	jo		EXIT_OVERFLOW
	push		eax

	pop			_factorialret

	cmp			_factorialret, 0
	jl			NEGATIVE_RESULT
FALSE12: 


	jmp EXIT
EXIT_DIV_ON_NULL:
	push offset _DIVISION_BY_ZERO_ERROR
	call _outStr 
	push -1
	call ExitProcess

EXIT_OVERFLOW:
	push offset _OVERFLOW_ERROR
	call _outStr 
	push -2
	call ExitProcess

NEGATIVE_RESULT: 
	push offset _NEGATIVE_RESULT_ERROR
	call _outStr 
	push -3
	call ExitProcess

EXIT:
	mov		eax, _factorialret
	ret		4
_factorial ENDP

_strCmpTest PROC 
	push		offset L2
	push		offset L3
	call		_strCmp
	push		eax
	pop			_strCmpTestres

	cmp			_strCmpTestres, 0
	jl			NEGATIVE_RESULT
	push		_strCmpTestres
	push		L0
	pop		ebx
	pop		eax
	cmp		eax, ebx
	jne		FALSE25
	push		offset L4
	pop			_strCmpTestoutStr

FALSE25: 
	push		_strCmpTestres
	push		L5
	pop		ebx
	pop		eax
	cmp		eax, ebx
	jne		FALSE29
	push		offset L6
	pop			_strCmpTestoutStr

FALSE29: 
	push		_strCmpTestres
	push		L7
	pop		ebx
	pop		eax
	cmp		eax, ebx
	jne		FALSE33
	push		offset L8
	pop			_strCmpTestoutStr

FALSE33: 


	jmp EXIT
EXIT_DIV_ON_NULL:
	push offset _DIVISION_BY_ZERO_ERROR
	call _outStr 
	push -1
	call ExitProcess

EXIT_OVERFLOW:
	push offset _OVERFLOW_ERROR
	call _outStr 
	push -2
	call ExitProcess

NEGATIVE_RESULT: 
	push offset _NEGATIVE_RESULT_ERROR
	call _outStr 
	push -3
	call ExitProcess

EXIT:
	mov		eax, _strCmpTestoutStr
	ret		0
_strCmpTest ENDP

main PROC
	push		offset L9
	call		_strLen
	push		eax
	pop			_mainx

	cmp			_mainx, 0
	jl			NEGATIVE_RESULT
	push		_mainx
	call		_factorial
	push		eax
	pop			_mainy

	cmp			_mainy, 0
	jl			NEGATIVE_RESULT
	push		_mainy
	call		_outInt

	call		_strCmpTest
	push		eax
	pop			_mainz

	push		_mainz
	call		_outStr 



	jmp EXIT
EXIT_DIV_ON_NULL:
	push offset _DIVISION_BY_ZERO_ERROR
	call _outStr 
	push -1
	call ExitProcess

EXIT_OVERFLOW:
	push offset _OVERFLOW_ERROR
	call _outStr 
	push -2
	call ExitProcess

NEGATIVE_RESULT: 
	push offset _NEGATIVE_RESULT_ERROR
	call _outStr 
	push -3
	call ExitProcess

EXIT:
	push		L1
	call		ExitProcess

main ENDP
end main
