
gQuit := false

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


fpsCounter := 0
lastCheckedTime := 0.0

resizeState := false
preQuit := false
main := !(int argc, char^^ argv) -> int
{
	gRepo.AddZipRoot("Dlls.zip")

	//libNuklearInit()

	//w := 600
	//h := 600
	//imageData := new u8[w*h*4]
	//fontData := new u8[512*512]
	//fbContx := nk_rawfb_init2(imageData,fontData,w,h)

	//rekt := nk_rect(0,0,w,h)
	//if nk_begin(fbContx,"Test",rekt,NK_WINDOW_BORDER or_b NK_WINDOW_NO_SCROLLBAR)
	//{
	//	nk_layout_row_dynamic(fbContx,35,1)
	//	for 6
	//		nk_button_label(fbContx,"wow")
	//	nk_end(fbContx)
	//}

	//nk_rawfb_render2(fbContx)

	//imgFile := TFile("tst.ppm","wb")
	//defer imgFile.Close()
	//imgFile << "P6\n" << w << " " << h << "\n255\n"
	//for ix : w for iy : w
	//{
	//	imgFile.Write(imageData[(ix*h + iy)*4]&->{void^},3)	
	//}

	//return 0

	if ToolMain(argc,argv)
		return 0
	
	ExpectWorkers(1)
	vPreInit()
	
	//DevILInit()

	////fil := gRepo.GetFile("duf.jpg")
	////fil := gRepo.GetFile("Shakal.gif")
	//fil := gRepo.GetFile("animation.gif")
	//ptr := fil.Map()

	//imgId := int
	//ilGenImages(1,imgId&)
	//ilBindImage(imgId)
	//ilLoadL(IL_GIF,ptr,fil.Size())
	//printf("w %i\n",ilGetInteger(IL_IMAGE_WIDTH))
	//printf("h %i\n",ilGetInteger(IL_IMAGE_HEIGHT))
	//printf("d %i\n",ilGetInteger(IL_IMAGE_DEPTH))
	//printf("faces %i\n",ilGetInteger(IL_NUM_FACES))
	//printf("num %i\n",ilGetInteger(IL_NUM_IMAGES))
	//printf("layers %i\n",ilGetInteger(IL_NUM_LAYERS))
	//printf("dur %i\n",ilGetInteger(IL_IMAGE_DURATION))
	//printf("channels %i\n",ilGetInteger(IL_IMAGE_CHANNELS))
	//printf("cur image %i\n",ilGetInteger(IL_CUR_IMAGE))
	//return 0

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
	VoidAudioInit()


	mainScriptObject := ScriptCompile(mainScript)
	if mainScriptObject == null
	{
		printf("script not compiled\n")
		return 0
	}
	mainSBox := ScriptBox
	memset(mainSBox&,0,ScriptBox->TypeSize)
	ScriptRun(mainScriptObject,mainSBox&)


	SpawnTask(() ==> {
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

	SpawnTask( () ==> []{
		
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

	prevTime := glfwGetTime()

	while  true 
	{
		if gQuit {
			return 0
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
			//Quit()
			gQuit = true
			return 0
		}

		gHotloadCheck()
		fpsCounter++

	}
		
	return 0

}

