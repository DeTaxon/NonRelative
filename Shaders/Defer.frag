#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform sampler2D Gcolor;
layout (binding = 1) uniform sampler2D Gnormal;

layout (location = 0) in vec2 inUV;
layout (location = 0) out vec4 outFragcolor;

void main() 
{
	// Get G-Buffer values
	vec3 fragColor = texture(Gcolor, inUV).rgb;
	//fragColor += vec3(0.0,-0.5,-0.5);
 	//outFragcolor = vec4(
	//	(fragColor.x + 1.402*fragColor.z),
	//	(fragColor.x - 0.344136*fragColor.y - 0.714136*fragColor.z),
	//	(fragColor.x + 1.772*fragColor.y),
	//	1.0);	
 	outFragcolor = vec4(fragColor, 1.0);	
}
