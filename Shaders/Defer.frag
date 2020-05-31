#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform sampler2D Gcolor;

layout (location = 0) in vec2 inUV;
layout (location = 0) out vec4 outFragcolor;

void main() 
{
	// Get G-Buffer values
	vec3 fragColor = texture(Gcolor, inUV*vec2(1700.0 / 2048,900.0 / 1024.0)).rgb;
	
 	outFragcolor = vec4(fragColor, 1.0);	
}
