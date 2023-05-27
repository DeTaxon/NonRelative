#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform sampler2D Gcolor;
layout (binding = 1) uniform sampler2D Gnormal;
layout (binding = 2) uniform sampler2D Gposition;

layout (location = 0) in vec2 inUV;
layout (location = 0) out vec4 outFragcolor;

void main() 
{
	// Get G-Buffer values
	vec3 fragColor = texture(Gcolor, inUV).rgb;
	vec3 pos = texture(Gposition,inUV).rgb;
	float dist =  distance(vec3(0.0,0.0,0.0),pos);
	float lightSize = 8;
	float magn = clamp((lightSize - dist) * ( 1.0 / lightSize),0.1,1.0);

 	outFragcolor = vec4(fragColor, 1.0)*vec4(magn,magn,magn,1.0);
}
