#version 420

layout(location = 0) in vec2 i_uv;
layout(location = 0) out vec4 out_Color;

layout(set=1,binding=0) uniform sampler2D u_Texture;

void main()
{
	if (i_uv.x > 1.0 || i_uv.x < 0.0 || i_uv.y > 1.0 || i_uv.y < 0.0)
	{
		out_Color = vec4(1.0,1.0,1.0,1.0);
	}else
	{
		out_Color = texture(u_Texture,i_uv);
	}
}
