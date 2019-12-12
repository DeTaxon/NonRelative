titleBuf := char[256]

main := !(int argc, char^^ argv) -> int
{
	vPreInit()

	CreateWindow(700,700)
	defer DestroyWindow() 

	InitVulkan()
	defer DestroyVulkan()

	vInit()

	prp := vAddProp("Snooker/SnookerTable")
	prp.modelPos.ang = quantfAt(1.0f,0.0f,0.0f,0deg)
	prp.modelPos.pos = vec4f(0.0f,0.0f,0.0f,1.0f)

	spheres := PhysSphere^[4]
	spheres[^] = new PhysSphere(0.1)
	spheresProps := vProp^[4]
	spheresProps[^] = vAddProp("HiResBox")

	spheres[0].System.pos = vec4f( 0.3f,0.0f,0.0f,0.1f)
	spheres[1].System.pos = vec4f(-0.9f,0.3f,0.0f,0.1f)
	spheres[2].System.pos = vec4f( 0.3f,0.3f,0.0f,0.1f)
	spheres[3].System.pos = vec4f( 0.0f,-0.3f,0.0f,0.1f)
	spheres[0].ImpulseV = vec4f(-0.3f,1.7f,0.0f,0.0f)
	spheres[0].ImpulseA = vec4f(-0.3f,1.7f,0.0f,0.0f)

	infP := PhysInfPlane^[4]
	infP[0] = new PhysInfPlane(vec4f(0.0f,1.1f,0.0f,0.0f),vec4f(0.0f,-1.0f,0.0f,0.0f))
	infP[1] = new PhysInfPlane(vec4f(0.0f,-1.1f,0.0f,0.0f),vec4f(0.0f,1.0f,0.0f,0.0f))
	infP[2] = new PhysInfPlane(vec4f(1.1f,1.1f,0.0f,0.0f),vec4f(-1.0f,0.0f,0.0f,0.0f))
	infP[3] = new PhysInfPlane(vec4f(-1.1f,1.1f,0.0f,0.0f),vec4f(1.0f,0.0f,0.0f,0.0f))

	gCam.camPos = vec4f(1.5f,1.0f,-1.0f,1.0f)
	gCam.upDownAng = -45deg
	gCam.leftRightAng = 45deg

	prevTime := glfwGetTime()
	walkM := 0.5f

	resizeState := false

	fpsCounter := 0
	lastCheckedTime := 0.0

	//mapMap := MappedFile("./FirstMap.ply")
	//asMdl := new RawModel() ; $temp
	//asMdl.MapFromPLY(mapMap.Get(),mapMap.Size())
	//mapP := new PhysHeightMap
	//mapP.CreateDots(asMdl)
	//mapMap.Close()

	while not glfwWindowShouldClose(glfwWindow)
	{
		FlushTempMemory()
		glfwPollEvents()
		
		nowTime := glfwGetTime()
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
		
		for i : 0..2 
			for j : (i+1)..3
				PhysCheckSvS(spheres[i],spheres[j])
		for i : 4
			PhysCheckSvIP(spheres[i],infP[^])

		for spheres
		{
			it.System.pos += it.ImpulseV*deltaTime
		}
		
		spheresProps[^i].modelPos.pos = spheres[i].System.pos
		spheresProps[^i].modelPos.pos.w = 0.1f

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

		vDraw()

		StopDraw()
		fpsCounter++
	}
		
	return 0

}

