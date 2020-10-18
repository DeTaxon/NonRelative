#version 420
#extension GL_AMD_gpu_shader_half_float : enable

#VERT_INPUT_DATA_VARS

layout(push_constant) uniform PushConsts{
	vec4 quantAng;
	vec4 g_pos;
}consts;

layout(location = 0) out vec2 o_uv;
layout(location = 1) out vec3 o_norm;

layout(set = 0,binding = 0) uniform perspInfo
{
	vec4 prespData;
};

vec4 quat_conj(vec4 q)
{ 
  return vec4(-q.x, -q.y, -q.z, q.w); 
}

vec4 quat_mult(vec4 q1, vec4 q2)
{ 
  vec4 qr;
  qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return qr;
}

vec3 rotate_vertex_position(vec3 position)
{ 
  vec4 qr_conj = quat_conj(consts.quantAng);
  vec4 q_pos = vec4(position.x, position.y, position.z, 0);
  
  vec4 q_tmp = quat_mult(consts.quantAng, q_pos);
  return quat_mult(q_tmp, qr_conj).xyz;
}
void main() {
	#VERT_INPUT_DATA_INIT
	vec4 prePosition = vec4((rotate_vertex_position(i_Position.xyz*consts.g_pos.w)) + consts.g_pos.xyz,1.0f);
	prePosition = prePosition.yzxw * vec4(1.0f,-1.0f,1.0f,1.0f);
	gl_Position.x = prePosition.x*prespData.x;
	gl_Position.y = prePosition.y*prespData.y;
	gl_Position.z = -(prePosition.z*prespData.z + prespData.w);
	gl_Position.w = -prePosition.z;
	o_uv = vec2(i_uv.x,1.0 - i_uv.y);
	o_norm = i_Normal;
}
