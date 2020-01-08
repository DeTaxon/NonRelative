titleBuf := char[256]

btnPress := int

KeyPress := !(int ch,bool isPress) -> void
{
	if isPress and ch == 'b' btnPress += 1
}

main := !(int argc, char^^ argv) -> int
{
	vPreInit()

	CreateWindow(1700,900)
	defer DestroyWindow() 

	InitVulkan()
	defer DestroyVulkan()

	vInit()
	
	prp := vAddProp("FirstMap")
	prp.modelPos.ang = quantfAt(1.0f,0.0f,0.0f,0deg)
	prp.modelPos.pos = vec4f(0.0f,0.0f,0.0f,1.0f)

	spheres := vProp^[3]
	spheres[^] = vAddProp("Sphere")

	gCam.camPos = vec4f(0.0f,0.0f,0.0f,1.0f)
	gCam.upDownAng = 0.0f
	gCam.leftRightAng = 0.0f

	itPlayer := new PhysPlayer

	prevTime := glfwGetTime()

	resizeState := false

	fpsCounter := 0
	lastCheckedTime := 0.0

	mapMap := MappedFile("./Models/FirstMap.ply")
	asMdl := new RawModel() ; $temp
	asMdl.MapFromPLY(mapMap.Get(),mapMap.Size())
	mapP := new PhysHeightMap
	mapP.CreateDots(asMdl)
	mapMap.Close()

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
				//sleep
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
	

		pp := new List.{vec4f} ; $temp
		prevTime = nowTime
		gCam.InputCheck(deltaTime)
		gCam.BindDescriptor(mainCmd.Get())
		
		itPlayer.System.pos.w = 1.7f

		frW := 0.0f
		if buttons['w'] frW += 1.0f
		if buttons['s'] frW -= 1.0f
		if buttons['S'] frW *= 2.0f
		itPlayer.ImpulseV = vec4f(0.0f,0.0f,-2.0f,0.0f)
		itPlayer.ImpulseV += quantfAt(0.0f,0.0f,-1.0f,gCam.leftRightAng)*vec4f(frW,0.0f,0.0f,0.0f)
		if buttons['g'] itPlayer.ImpulseV.z = 1.0f

		PhysCheckPlayerVSHMap(itPlayer,mapP,pp^)
		itPlayer.System.pos += itPlayer.ImpulseV*deltaTime
		gCam.camPos = itPlayer.System.pos + vec4f(0.0f,0.0f,1.5f,0.0f)
		gCam.camPos.w = 1.0f
		
		printf("wut %i\n",pp.Size())
		if pp.Size() != 0
		{
			kk := btnPress % (pp.Size() div 3)
			printf("ps %i\n",kk)
			spheres[0].modelPos.pos = pp^[kk*3]
			spheres[1].modelPos.pos = pp^[kk*3 + 1]
			spheres[2].modelPos.pos = pp^[kk*3 + 2]
			spheres[0].modelPos.pos.w = 0.1f
			spheres[1].modelPos.pos.w = 0.1f
			spheres[2].modelPos.pos.w = 0.1f
		}else{
			spheres[0].modelPos.pos = vec4f(0.0f,0.0f,-10.0f,1.0f)	
			spheres[1].modelPos.pos = vec4f(0.0f,0.0f,-10.0f,1.0f)	
			spheres[2].modelPos.pos = vec4f(0.0f,0.0f,-10.0f,1.0f)	
		}

		if buttons['t'] itPlayer.System.pos = vec4f(0.0f,0.0f,0.3f,1.0f)

		vDraw()

		StopDraw()
		fpsCounter++
		dotIter += 1
	}
		
	return 0

}

