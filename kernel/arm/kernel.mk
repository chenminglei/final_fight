ARM_OBJS := reg.o psr.o installHandler.o timeSetup.o userSetup.o S_Handler.o int_asm.o irq_handler.o C_SWI_HANDLER.o init_interrupt.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
