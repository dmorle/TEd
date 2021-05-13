.code

te_strncpy proc
		mov r9, rcx
		mov r10, r8

		; Checking if sz is 0
		mov rcx, r8
		jrcxz CPY_RET0
		mov ecx, 0

	CPY_LOOP:
		mov bl, byte ptr [rdx]
		mov byte ptr [r9], bl
		
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
