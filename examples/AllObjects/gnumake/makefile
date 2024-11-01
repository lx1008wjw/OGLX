##############
# Makefile to build example in a 64 bits environment
##############

OS := $(shell uname -s | cut -c1-5)

# Set exe name here
EXENAME = oglx_application

# Set tools here
CC = gcc
RM = rm -rf
MKDIR = mkdir -p
CP = cp
GREP = grep

ifneq ($(wildcard ../../../lib/libOGLX.a),) 
    OGLXLIB = ../../../lib/libOGLX.a
else 
    OGLXLIB = ../../../lib64/OGLX.lib
endif

# Change flags here
ifeq ($(OS),MINGW) 
CFLAGS = -DFREEGLUT_STATIC -DANIMATION -O -std=c99 -Wall -I/usr/include -I../../../extras/freeglut32 -I../../../extras/utils/include
LDFLAGS = -mconsole -mwin32 $(OGLXLIB) ../../../extras/freeglut32/freeglut_static64.lib -L/usr/lib -lOpenGL32 -lgdi32 -lwinmm
endif


# Files specification
VPATH := ../src ../code ../../../extras/utils/src
CFLAGS += -I../../../include $(foreach dir, $(VPATH), -I$(dir))
WGLSRCS := ../src/window_manager.c ../src/main_wgl.c
SRCS := $(filter-out $(WGLSRCS),$(foreach dir, $(VPATH), $(wildcard $(dir)/*.c)))
OBJS := $(addprefix objs/, $(notdir $(SRCS:.c=.o)))

# Main rules
all: $(EXENAME).exe $(EXTRAFILES)

clean::
	$(RM) $(EXENAME).exe objs

# Rule for exe creation or update
$(EXENAME).exe: $(OBJS)
	$(CC) $(CCFLAGS) -o $@ objs/*.o $(LDFLAGS)

# Rules for C files compilation
objs/%.o: %.c
	@$(MKDIR) objs
	$(CC) -c $(CFLAGS) $< -o $@
