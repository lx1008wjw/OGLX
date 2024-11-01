To compile OGLX with an OpenGL SC 2.0 driver, the following actions shall be performed before compiling OGLX:

1) Produce the binary shader with OpenGL provider compiler tool. 
(OGLX shaders can be found under src\sgl directory, files oglx_sc2_shader.frag and oglx_sc2_shader.vert).

2) The binary that is produced shall then be converted to a textual format, each byte shal be separated by a comma.

3) Move produced file into directory src/sgl and name it oglx_sc2_shader.dat.

4) Compile OGLX, defining pre-processor variable SC2_DEV_ENV.

Note: attribute ul_binary_format of parameter par_p_parameters of sglInit shall be set with the OpenGL driver provider binary format constant.