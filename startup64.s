.code64
.section .text
    .global startup64
    startup64:
        add $KNL_HIGH_VMA, %rsp
        
        call kmain