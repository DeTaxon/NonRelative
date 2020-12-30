
gQuit := false
gTask := TaskBox^

gDisableMouse := false

DebugLog := !(char^ frmt,...) -> void
{
	vars := int[32]
	va_start(vars&)
	if $posix
		vprintf(frmt,vars&)
	if $win32
		vprintf(frmt,vars&->{void^^}^)
	va_end(vars&)
}

main := !(int argc, char^^ argv) -> int
{
	if ToolMain(argc,argv)
		return 0

	gTask = CreateTaskBox(1024*1024)
	gTask.ExpectWorkers(1)

	vPreInit()

	mainScript := gRepo.GetFile("main.sq")
	if mainScript == null
	{
		printf("main script not found\n")
		return 0
	}
	//uvInit()

	CreateWindow(gWinStartW,gWinStartH)
	defer DestroyWindow() 

	InitVulkan()
	defer DestroyVulkan()

	vInit()
	ScriptInit()
	
	gCam.camPos = vec4f(0.0f,0.0f,0.0f,1.0f)
	gCam.upDownAng = 0.0f
	gCam.leftRightAng = 0.0f

	//gHotloadInit()
	//VoidAudioInit()
	//StartTroll()


	mainScriptObject := ScriptCompile(mainScript)
	if mainScriptObject == null
	{
		printf("script not compiled\n")
		return 0
	}
	mainSBox := ScriptBox
	memset(mainSBox&,0,ScriptBox->TypeSize)
	ScriptRun(mainScriptObject,mainSBox&)


	gTask.Spawn(() ==> {
		prevTime := glfwGetTime()
		while true
		{
			FlushTempMemory()
			if gQuit {
				return void
			}
			nowTime := glfwGetTime()
			deltaTime := nowTime - prevTime
			vPhysStage(deltaTime)
			prevTime = nowTime
			TSleep(0.005)
		}
	})

	fpsCounter := 0
	lastCheckedTime := 0.0

	resizeState := false
	preQuit := false

	gTask.Spawn( () ==> [fpsCounter&,preQuit&]{
		
		prevTime := glfwGetTime()
		
		while true
		{
			FlushTempMemory()
			glfwPollEvents()

			nowTime := glfwGetTime()

			//if resizeState 
			//{
			//	if nowTime - gLastTimeResized > 1.0
			//	{
			//		CreateSwapchain(gWindowW,gWindowH)
			//		gCam.UpdatePerspective(gWindowW->{float},gWindowH->{float})
			//		resizeState = false
			//	}else{
			//		yield void
			//	}
			//}

			if glfwWindowShouldClose(glfwWindow)
			{
				preQuit = true
				return void
			}
			TSleep(0.01)
		}
	})

	gTask.Spawn(() ==> [fpsCounter&,preQuit&]{

		prevTime := glfwGetTime()

		while  true 
		{
			if gQuit {
				return void
			}
			FlushTempMemory()
			AwaitWork(() ==>{
				DrawGetImage()
			})
			StartDraw()
			nowTime := glfwGetTime()
			gNowTime = nowTime
			deltaTime := nowTime - prevTime
			
			prevTime = nowTime
			gCam.InputCheck(deltaTime)
			gCam.BindDescriptor(mainCmd.Get())
			
			vDraw()
		
			StopDraw()

			if preQuit
			{
				gTask.Quit()
				return void
			}

			gHotloadCheck()
			fpsCounter++

		}
	})
	gTask.Run()
		
	return 0

}

