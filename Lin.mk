INCDIRS  += -I ${LIN_PATH}/src \
	    -I ${LIN_PATH}/api	\
	    -I ${LIN_PATH}/cfg \
	    -I ${LIN_PATH}/src/bsp/

LDFLAGS  += -g
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += ${INCDIRS} -g

$(info compiling Lin source files)


LIN_OBJS := \
	${LIN_PATH}/src/Lin.o \
	${LIN_PATH}/cfg/Lin_cfg.o

