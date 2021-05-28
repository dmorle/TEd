.code

te_strlen proc

		mov rdx, rcx
		mov rax, rcx
		mov ecx, 0
		
	STRLEN_LOOP:
		mov cl, byte ptr [rax]
		jecxz STRLEN_RET
		inc rax
		jmp STRLEN_LOOP

	STRLEN_RET:
		sub rax, rdx
		ret

te_strlen endp

te_strncpy proc
		
		mov r9, rcx
		mov r10, r8

		; Checking if sz is 0
		or r8, r8
		jz CPY_RET0
		mov ecx, 0

		mov cl, byte ptr [rdx]
	CPY_LOOP:
		mov byte ptr [r9], cl
		
		dec r8
		jz CPY_STOP

		inc r9
		inc rdx
		mov cl, byte ptr [rdx]
		jecxz CPY_STOP

		mov byte ptr [r9], cl
		
		dec r8
		jz CPY_STOP

		inc r9
		inc rdx
		mov cl, byte ptr [rdx]
		jecxz CPY_STOP

		jmp CPY_LOOP

	CPY_STOP:
		mov rax, r10
		sub rax, r8
		ret

	CPY_RET0:
		mov rax, 0
		ret

te_strncpy endp

end
