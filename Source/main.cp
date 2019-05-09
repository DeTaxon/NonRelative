#import "VulkanCore.cp"
#import "Model.cp"
#import "CmdBuffer.cp"
#import "Shader.cp"
#import "Prop.cp"
#import "Window.cp"
#import "Camera.cp"
#import "ZipFS.cp"

gCam := vCamera

main := !(int argc, char^^ argv) -> int
{
	test := vRepo
	test.Init(".")

	test.AddZipRoot("./TestFolder.zip")

	cc := test.GetFile("TestFolder/Hello")
	printf(" a %p\n",cc)

	r := cc.Map()

	outF := MappedFile("/tmp/hoh",FILE_CREATE,cc.Size())

	outF[^i] = r->{u8^}[i]
	cc.Unmap()
	outF.Close()

	return 0

	startW = 700
	startH = 700

	CreateWindow()
	defer DestroyWindow() 


	InitVulkan()
	defer DestroyVulkan()

	mdl := new vModel
	mdl.LoadFile("Models/HiResBox.ply")

	prp := new Prop
	prp.modelPtr = mdl

	simpleShader := new Shader
	simpleShader.LoadShader("Shaders/LearnVert.vert","Shaders/LearnFrag.frag")

	prp.modelPos.ang = quantfAt(1.0f,0.0f,0.0f,0.0f)
	prp.modelPos.pos = vec4f(1.0f,0.0f,0.0f,0.4f)

	gCam."this"()
	gCam.SetPerspective(700.0f,700.0f,0.01f,100.0f,90deg)
	prevTime := glfwGetTime()
	walkM := 0.5f

	while not glfwWindowShouldClose(glfwWindow)
	{
		FlushTempMemory()
		glfwPollEvents()

		deltaTime := glfwGetTime() - prevTime
		prevTime = glfwGetTime()
		StartDraw()
		gCam.BindDescriptor(mainCmd.Get())
	
		addLR := 0.0f
		addFB := 0.0f
		if buttons['a'] addLR = -walkM*deltaTime
		if buttons['d'] addLR =  walkM*deltaTime
		if buttons['w'] addFB =  walkM*deltaTime
		if buttons['s'] addFB = -walkM*deltaTime
		if buttons['q'] gCam.AddAngs(-deltaTime*0.5f,0.0f)
		if buttons['e'] gCam.AddAngs( deltaTime*0.5f,0.0f)
		if buttons['r'] gCam.AddAngs(0.0f, deltaTime*0.5f)
		if buttons['f'] gCam.AddAngs(0.0f,-deltaTime*0.5f)

		gCam.addLocal(vec4f(addLR,0.0f,addFB,0.0f))

		simpleShader.ApplyShaderToQueue(mainCmd.Get())
		prp.AddToCmdBuffer(mainCmd.Get())

		StopDraw()
	}
		
	return 0

}

