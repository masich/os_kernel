                AREA |.text|,CODE,READONLY,ALIGN=2
                THUMB
                EXTERN current_pt
                EXPORT SysTick_Handler
				EXPORT os_scheduler_launch

SysTick_Handler             ; By invoking this interrupt the CPU automaticly saves R0,R1,R2,R3,R12,LR,PC,xPSR 
    CPSID   I               ; Disable interrupts
    PUSH    {R4-R11}        ; Save R4...R11 registers
    LDR     R0,=current_pt  ; R0 points to current_pt
    LDR     R1,[R0]         ; R1 = current_pt
    STR     SP,[R1]         ; current_pt->stack_pt = SP
    LDR     R1,[R1,#4]      ; R1 = current_pt->next
    STR     R1,[R0]         ; current_pt = R1
    LDR     SP,[R1]         ; SP = current_pt->stack_pt
    POP     {R4-R11}        ; Restore registers of the new thread from the stack
    CPSIE   I               ; Enable interrupts
    BX      LR              ; After returning the processor will automaticly restore R0,R1,R2,R3,R12,LR,PC,xPSR of the new thread


os_scheduler_launch
    LDR     R0,=current_pt  ; R0 points to current_pt
    LDR     R2,[R0]         ; R2 = current_pt
    LDR     SP,[R2]         ; SP = current_pt->stack_pt
    POP     {R4-R11}        ; Restore registers R4-R11
    POP     {R0-R3}         ; Restore registers R0-R3
    POP     {R12}           ; Restore R12
    ADD     SP,SP,#4        ; Move on the stack pointer
    POP     {LR}            ; Restore link register
    ADD     SP,SP,#4        ; Move on the stack pointer
    CPSIE   I               ; Enable interrunts
    BX      LR              ; Return from this subroutine

                ALIGN
                END
