.globl _nc_save_ctx
.def _nc_save_ctx; .scl 2; .type 32; .endef
_nc_save_ctx:
    movl 4(%esp), %eax
    movl (%esp), %ecx
    movl %ecx, 8(%eax)
    movl %esp, (%eax)
    movl %ebp, 4(%eax)
    ret


.globl _nc_restore_ctx
.def _nc_restore_ctx; .scl 2; .type 32; .endef
_nc_restore_ctx:
    movl 4(%esp), %eax
    movl (%eax), %esp
    movl 4(%eax), %ebp
    jmp *8(%eax)
