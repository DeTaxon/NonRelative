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
	//vec3 fragColor = texture(Gcolor, inUV*vec2(1700.0 / 2048,900.0 / 1024.0)).rgb;
	vec3 fragColor = texture(Gcolor, inUV).rgb;
	
 	outFragcolor = vec4(fragColor, 1.0);	
 	//outFragcolor = vec4(
	//	fragColor.x + 1.403*fragColor.z,
	//	fragColor.x - 0.344*fragColor.y - 0.714*fragColor.z,
	//	fragColor.x + 1.770*fragColor.y 
	//, 1.0);	
 	//outFragcolor = vec4(
	//	(fragColor.x*298.082 + 408.583*fragColor.z - 222.921) / 256,
	//	(fragColor.x*298.082 - 100.291*fragColor.y - 208.120*fragColor.z + 135.576) / 256,
	//	(fragColor.x*298.082 + 516.412*fragColor.y) / 256 ,
	//	1.0);	
}
