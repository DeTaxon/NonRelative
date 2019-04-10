#version 420

layout(location = 0) in vec4 i_Position;
layout(location = 1) in vec4 i_Normal;
//layout(location = 2) in vec2 i_uv;

layout(push_constant) uniform PushConsts{
	vec4 quantAng;
	vec4 g_pos;
}consts;

layout(location = 0) out vec2 o_uv;
layout(location = 1) out vec4 o_Color;

void main() {
	gl_Position = vec4(i_Position.xyz*consts.g_pos.w + consts.g_pos.xyz,1.0);
	o_Color = vec4(i_Normal.xyz,1.0);
	o_uv = vec2(i_Position.w,i_Normal.w);
}
