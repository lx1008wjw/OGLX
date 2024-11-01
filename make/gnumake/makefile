##############
# Makefile to build OGLX library
##############

# Set library name here
LIBNAME = OGLX

# Set tools here
CC = gcc
AR = ar
RM = rm -f
MKDIR = mkdir -p
CP = cp

# Change flags here
CFLAGS = -Wall -Wextra -std=c99 -pedantic -Wno-overlength-strings
ARFLAGS = ruv

# Files specification
CFLAGS += -I../../include -I../../src/sgl -I../../src/mth 
VPATH := ../../src/sgl ../../src/sgl/appearance ../../src/sgl/clipping ../../src/sgl/errors ../../src/sgl/initialization ../../src/sgl/interactivity ../../src/sgl/modes ../../src/sgl/primitives ../../src/sgl/text ../../src/sgl/transformation  ../../src/sgl/static_sequences ../../src/mth
SRCS := $(foreach dir, $(VPATH), $(wildcard $(dir)/*.c))
OBJS := $(addprefix objs/, $(notdir $(SRCS:.c=.o)))

# Main rules
all: lib$(LIBNAME).a

GL_13: CFLAGS += -I../../extras/opengl_1.3 -DGL_APIENTRY=APIENTRY
GL_13: LIBNAME :=OGLX
GL_13: all

SC: CFLAGS += -DSC101_DEV_ENV -I../../extras/opengl_sc
SC: LIBNAME :=OGLXSC
SC: all

ES_1_1: CFLAGS += -DES11_DEV_ENV -I../../extras/opengl_es1.1
ES_1_1: LIBNAME :=OGLXES11
ES_1_1: all

ES_2: CFLAGS += -DES2_DEV_ENV -DGL_GLEXT_PROTOTYPES -I../../extras/opengl_es2.0
ES_2: LIBNAME :=OGLXES2
ES_2: all

SC_2: CFLAGS += -DSC2_DEV_ENV -DGL_GLEXT_PROTOTYPES -I../../extras/opengl_sc2.0
SC_2: LIBNAME :=OGLXSC2
SC_2: all

clean::
	$(RM) lib*.a $(OBJS)

# Rule for library creation or update
lib$(LIBNAME).a: $(OBJS)
	@$(AR) $(ARFLAGS) lib$(LIBNAME).a $(OBJS)
	$(MKDIR) ../../lib
	cp lib$(LIBNAME).a ../../lib

# Rules for C files compilation
objs/%.o: %.c
	@$(MKDIR) objs
	$(CC) -c $(CFLAGS) $< -o $@
