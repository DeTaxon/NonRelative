#import "VulkanCore.cp"

glfwWindow := void^

startW := int
startH := int

main := !(int argc, char^^ argv) -> int
{
	startW = 700
	startH = 700

	glfwInit()
	defer glfwTerminate()

	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API)
	glfwWindow = glfwCreateWindow(startW,startH,"Hi again",null,null)

	InitVulkan()
	defer DestroyVulkan()

	while not glfwWindowShouldClose(glfwWindow)
	{
		FlushTempMemory()
		glfwPollEvents()

		StartDraw()
	}
		
	return 0

}

