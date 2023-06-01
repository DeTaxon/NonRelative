#version 420

layout(location = 0) in vec2 i_uv;
layout(location = 1) in vec3 i_norm;
layout(location = 2) in vec4 i_vert;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 out_Normal;
layout(location = 2) out vec4 out_Vert;

layout(set=1,binding=0) uniform sampler2D u_Texture;

void main()
{
	vec2 uv = gl_FragCoord.xy / vec2(2048.0,2048.0);
	out_Color = texture(u_Texture,uv);
	out_Normal = vec4(i_norm,1.0);
	out_Vert = i_vert;
}
