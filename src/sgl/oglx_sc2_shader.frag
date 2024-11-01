uniform sampler2D sampler2d;
uniform highp vec4 v_color;
varying highp vec2	v_tex_coord;
uniform int glob_i_texture_active;
varying highp float f_clipDist0;
varying highp float f_clipDist1;
varying highp float f_clipDist2;
varying highp float f_clipDist3;
varying highp float f_clipDist4;
varying highp float f_clipDist5;
void main (void)
{
	if ((f_clipDist0 < 0.0) || (f_clipDist1 < 0.0) || (f_clipDist2 < 0.0) || (f_clipDist3 < 0.0) || (f_clipDist4 < 0.0) || (f_clipDist5 < 0.0)) discard;
	if(glob_i_texture_active < 1)
	{
		gl_FragColor = v_color;
	}
	else
	{
		if(glob_i_texture_active < 3)
		{
			gl_FragColor = texture2D(sampler2d,v_tex_coord);
		}
		else
		{
			if(glob_i_texture_active < 5)
			{
				highp vec4 loc_texture_color = texture2D(sampler2d,v_tex_coord);
				gl_FragColor =  vec4(v_color[0], v_color[1], v_color[2], loc_texture_color[0]);
			}
			else
			{
				gl_FragColor = texture2D(sampler2d,v_tex_coord) * v_color;
			}
		}
	}
}
