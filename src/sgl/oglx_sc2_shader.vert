attribute highp vec4 pf_vertex_array;
attribute highp vec2 pf_tex_coord;
uniform highp mat4	pf_transformation_matrix;
varying highp vec2	v_tex_coord;
uniform vec4 pf_clipPlane0;
varying highp float f_clipDist0;
uniform int b_clipPlane0;
uniform vec4 pf_clipPlane1;
varying highp float f_clipDist1;
uniform int b_clipPlane1;
uniform vec4 pf_clipPlane2;
varying highp float f_clipDist2;
uniform int b_clipPlane2;
uniform vec4 pf_clipPlane3;
varying highp float f_clipDist3;
uniform int b_clipPlane3;
uniform vec4 pf_clipPlane4;
varying highp float f_clipDist4;
uniform int b_clipPlane4;
uniform vec4 pf_clipPlane5;
varying highp float f_clipDist5;
uniform int b_clipPlane5;
void main(void)
{
	if (b_clipPlane0 == 1) {
		highp vec4 loc_vertex = pf_transformation_matrix * pf_vertex_array;
		f_clipDist0 = dot(loc_vertex.xyz, pf_clipPlane0.xyz) + pf_clipPlane0.w;
	}
	else {
		f_clipDist0 = 0.0;
	}
	if (b_clipPlane1 == 1) {
		highp vec4 loc_vertex = pf_transformation_matrix * pf_vertex_array;
		f_clipDist1 = dot(loc_vertex.xyz, pf_clipPlane1.xyz) + pf_clipPlane1.w;
	}
	else {
		f_clipDist1 = 0.0;
	}
	if (b_clipPlane2 == 1) {
		highp vec4 loc_vertex = pf_transformation_matrix * pf_vertex_array;
		f_clipDist2 = dot(loc_vertex.xyz, pf_clipPlane2.xyz) + pf_clipPlane2.w;
	}
	else {
		f_clipDist2 = 0.0;
	}
	if (b_clipPlane3 == 1) {
		highp vec4 loc_vertex = pf_transformation_matrix * pf_vertex_array;
		f_clipDist3 = dot(loc_vertex.xyz, pf_clipPlane3.xyz) + pf_clipPlane3.w;
	}
	else {
		f_clipDist3 = 0.0;
	}
	if (b_clipPlane4 == 1) {
		highp vec4 loc_vertex = pf_transformation_matrix * pf_vertex_array;
		f_clipDist4 = dot(loc_vertex.xyz, pf_clipPlane4.xyz) + pf_clipPlane4.w;
	}
	else {
		f_clipDist4 = 0.0;
	}
	if (b_clipPlane5 == 1) {
		highp vec4 loc_vertex = pf_transformation_matrix * pf_vertex_array;
		f_clipDist5 = dot(loc_vertex.xyz, pf_clipPlane5.xyz) + pf_clipPlane5.w;
	}
	else {
		f_clipDist5 = 0.0;
	}
	v_tex_coord = pf_tex_coord;
	gl_Position = pf_transformation_matrix * pf_vertex_array;
}

