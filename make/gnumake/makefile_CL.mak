##############
# Makefile to build OGLX library
##############
.PHONY: all clean

# Set library name here
LIBNAME = OGLX

# Set extensions and prefixes here
OBJ_EXT=.obj
LIB_PRE=
LIB_EXT=.lib

# Set tools here
CC = cl.exe
AR = lib.exe
RM = rm -f
MKDIR = mkdir -p
CP = cp

# Change flags here
CFLAGS = -MD -nologo -W3 -Gm -GX -ZI -Od -Zm900 -D_NO_WINDOWS_H -DWIN32_LEAN_AND_MEAN 
ARFLAGS = -nologo -out:

# Files specification
CFLAGS += -I../../include -I../../src/sgl -I../../src/mth
VPATH := ../../src/sgl ../../src/sgl/appearance ../../src/sgl/clipping ../../src/sgl/errors ../../src/sgl/initialization ../../src/sgl/interactivity ../../src/sgl/modes ../../src/sgl/primitives ../../src/sgl/text ../../src/sgl/transformation ../../src/sgl/static_sequences ../../src/mth
SRCS := $(foreach dir, $(VPATH), $(wildcard $(dir)/*.c))
OBJS := $(addprefix objs/, $(notdir $(SRCS:.c=$(OBJ_EXT))))

# Main rules
all: $(LIB_PRE)$(LIBNAME)$(LIB_EXT)

GL13: CFLAGS += -DGL_API="extern" -I../../extras/opengl_1.3
GL13: LIBNAME :=OGLX_13
GL13: all


SC: CFLAGS += -DSC101_DEV_ENV -DGL_API="extern" -I../../extras/opengl_sc
SC: LIBNAME :=OGLX_sc
SC: all

SC_2: CFLAGS += -DSC2_DEV_ENV -DGL_GLEXT_PROTOTYPES -I../../extras/opengl_sc2.0 -DGL_APICALL="extern" -DGL_APIENTRY=""
SC_2: LIBNAME :=OGLX_sc2
SC_2: all

ES_1_1: CFLAGS += -DES11_DEV_ENV -DGL_API="extern" -I../../extras/opengl_es1.1 -DGL_APIENTRY=""
ES_1_1: LIBNAME :=OGLX_es1
ES_1_1: all

ES_2: CFLAGS += -DES2_DEV_ENV -I../../extras/opengl_es2.0 -DGL_APICALL="extern" -DGL_APIENTRY=""
ES_2: LIBNAME :=OGLX_es2
ES_2: all

clean:
	$(RM) $(LIB_PRE)*$(LIB_EXT) $(OBJS) $(wildcard *.?db)

	
# Rule for library creation or update
$(LIB_PRE)$(LIBNAME)$(LIB_EXT): $(OBJS)
	@$(AR) $(ARFLAGS)$(LIB_PRE)$(LIBNAME)$(LIB_EXT) $(OBJS)
	$(MKDIR) ../../lib
	$(CP) $(LIB_PRE)$(LIBNAME)$(LIB_EXT) ../../lib

# Rules for C files compilation
objs/%$(OBJ_EXT): %.c
	@$(MKDIR) objs
	$(CC) -c $(CFLAGS) $< -o $@
	@mv $(notdir $@) objs/
