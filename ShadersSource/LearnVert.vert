#version 420

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_uv;

layout(location = 0) out vec4 o_Color;

void main() {
	gl_Position = vec4(i_Position.xyz*0.4 + vec3(0.6,0.0,0.0),1.0);
	//o_Color = vec4(i_Position.w,i_Normal.x,i_Normal.y,1.0);
	o_Color = vec4(i_Normal,1.0);
}
