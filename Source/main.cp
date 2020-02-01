titleBuf := char[256]

btnPress := int

KeyPress := !(int ch,bool isPress) -> void
{
	if isPress and ch == 'b' btnPress += 1
}

main := !(int argc, char^^ argv) -> int
{
	//ScriptTest()
	//return 0;
	vPreInit()

	CreateWindow(1700,900)
	defer DestroyWindow() 

	InitVulkan()
	defer DestroyVulkan()

	vInit()
	
	gCam.camPos = vec4f(0.0f,0.0f,0.0f,1.0f)
	gCam.upDownAng = 0.0f
	gCam.leftRightAng = 0.0f

	nMap := vGetMap("FirstMap")

	//itPlayer := new PhysPlayer

	prevTime := glfwGetTime()

	resizeState := false

	fpsCounter := 0
	lastCheckedTime := 0.0

	dotIter := 0
	while not glfwWindowShouldClose(glfwWindow)
	{
		FlushTempMemory()
		glfwPollEvents()
		
		nowTime := glfwGetTime()
		gNowTime = nowTime
		deltaTime := nowTime - prevTime
		if resizeState 
		{
			if nowTime - gLastTimeResized > 1.0
			{
				CreateSwapchain(gWindowW,gWindowH)
				gCam.UpdatePerspective(gWindowW->{float},gWindowH->{float})
				resizeState = false
			}else{
				TSleep(0.1)
				continue
			}
		}
		
		if nowTime - lastCheckedTime > 1.0
		{
			ttlPre := "Hi again! fps = "sbt + fpsCounter
			strcpy(titleBuf[0]&,ttlPre.Str()) ; $temp
			glfwSetWindowTitle(glfwWindow,titleBuf[0]&)
			fpsCounter = 0
			lastCheckedTime = nowTime
		}

		if not StartDraw()
		{
			resizeState = true
			continue
		}

		prevTime = nowTime
		gCam.InputCheck(deltaTime)
		gCam.BindDescriptor(mainCmd.Get())
		
		vPhysStage(deltaTime)
		vDraw()

		StopDraw()
		fpsCounter++
		dotIter += 1
	}
		
	return 0

}

