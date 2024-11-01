To compile OGLX with ArgusES2, the following actions shall be performed:

1) Produce the binary shader with CoreAVI compiler tool coreavicompiler.exe 
(OGLX shaders can be found under extras\opengl_es2.0, files oglx_shader.frag and oglx_shader.vert):
coreavicompiler.exe -o oglx_shader.bin oglx_shader.frag oglx_shader.vert

2) Produce the textual form of the shaders with CoreAVI Python script shader_to_h.py:
shader_to_h.py oglx_shader.bin

3) Move produced file oglx_shader.dat into directory src/sgl/initialization/

4) Copy the file sglReset.c into directory src/sgl/initialization/ 
(if necessary add #include <GLES2/gl2ext.h> in include/oglx_port.h just after #include <GLES2/gl2.h>)

5) Compile OGLX, defining pre-processor variables ES2_DEV_ENV.