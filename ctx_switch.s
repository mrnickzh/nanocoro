.globl _nc_save_ctx
.def _nc_save_ctx; .scl 2; .type 32; .endef
_nc_save_ctx:
    movl 4(%esp), %eax     
    movl %eax, (%eax)   
    movl %ebx, 4(%eax)
    movl %ecx, 8(%eax)
    movl %edx, 12(%eax)
    movl %esi, 16(%eax)
    movl %edi, 20(%eax)
    movl %ebp, 24(%eax)
    leal 4(%esp), %ecx    
    movl %ecx, 28(%eax)
    movl (%esp), %ecx 
    movl %ecx, 32(%eax)
    ret

.globl _nc_restore_ctx
.def _nc_restore_ctx; .scl 2; .type 32; .endef
_nc_restore_ctx:
    movl 4(%esp), %eax      
    movl 28(%eax), %esp     
    pushl 32(%eax)         
    movl 4(%eax), %ebx
    movl 8(%eax), %ecx
    movl 12(%eax), %edx
    movl 16(%eax), %esi
    movl 20(%eax), %edi
    movl 24(%eax), %ebp
    movl (%eax), %eax      
    ret
