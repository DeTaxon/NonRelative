#import "VulkanCore.cp"
#import "Model.cp"
#import "CmdBuffer.cp"
#import "Shader.cp"
#import "Prop.cp"

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

	prp := new Prop
	prp.modelPtr = mdl

	//extraCmdBuf := CmdBuffer
	//extraCmdBuf.CreateBuffer()

	simpleShader := new Shader
	simpleShader.LoadShader("Shaders/LearnVert.vert","Shaders/LearnFrag.frag")

	i := 1
	while not glfwWindowShouldClose(glfwWindow)
	{
		i++
		FlushTempMemory()
		glfwPollEvents()

		StartDraw()

		if i div 2 * 2 != i
		{
			prp.posItem.pos = vec4f(0.6f,0.0f,0.0f,0.7f)
		}else{
			prp.posItem.pos = vec4f(0.3f,0.0f,0.0f,0.4f)
		}

		//extraCmdBuf.Reset()
		//extraCmdBuf.Start()
		simpleShader.ApplyShaderToQueue(mainCmd.Get())
		//mdl.AddToCmdBuffer(mainCmd.Get())
		prp.AddToCmdBuffer(mainCmd.Get())
		//extraCmdBuf.Stop()
		//extraCmdBuf.Submit()

		StopDraw()
	}
		
	return 0

}

