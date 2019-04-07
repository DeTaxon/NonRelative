#import "VulkanCore.cp"
#import "Model.cp"
#import "CmdBuffer.cp"
#import "Shader.cp"

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

	mdl := new Model
	mdl.LoadFile("Models/HiResBox.ply")

	extraCmdBuf := CmdBuffer
	extraCmdBuf.CreateBuffer()

	simpleShader := new Shader
	//simpleShader.LoadShader("Shaders/LearnVert.vert","Shaders/LearnFrag.frag")

	while not glfwWindowShouldClose(glfwWindow)
	{
		FlushTempMemory()
		glfwPollEvents()

		StartDraw()

		extraCmdBuf.Reset()
		extraCmdBuf.Start()
		simpleShader.ApplyShaderToQueue(extraCmdBuf.Get())
		//mdl.AddToCmdBuffer(extraCmdBuf.Get())
		extraCmdBuf.Stop()
		//extraCmdBuf.Submit()

		StopDraw()
	}
		
	return 0

}

