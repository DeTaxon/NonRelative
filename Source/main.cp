titleBuf := char[256]

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

	//spheres := PhysSphere^[4]
	//spheres[^] = new PhysSphere(0.1)
	spheresProps := vProp^[3]
	spheresProps[^] = vAddProp("Sphere")

	//spheres[0].System.pos = vec4f( 0.3f,0.0f,0.0f,1.0f)
	//spheres[1].System.pos = vec4f(-0.9f,0.3f,0.0f,1.0f)
	//spheres[2].System.pos = vec4f( 0.3f,0.3f,0.0f,1.0f)
	//spheres[3].System.pos = vec4f( 0.0f,-0.3f,0.0f,1.0f)
	//spheres[0].ImpulseV = vec4f(-0.3f,1.7f,0.0f,0.0f)
	//spheres[0].ImpulseA = vec4f(-0.3f,1.7f,0.0f,0.0f)

	//infP := PhysInfPlane^[4]
	//infP[0] = new PhysInfPlane(vec4f(0.0f,1.1f,0.0f,0.0f),vec4f(0.0f,-1.0f,0.0f,0.0f))
	//infP[1] = new PhysInfPlane(vec4f(0.0f,-1.1f,0.0f,0.0f),vec4f(0.0f,1.0f,0.0f,0.0f))
	//infP[2] = new PhysInfPlane(vec4f(1.1f,1.1f,0.0f,0.0f),vec4f(-1.0f,0.0f,0.0f,0.0f))
	//infP[3] = new PhysInfPlane(vec4f(-1.1f,1.1f,0.0f,0.0f),vec4f(1.0f,0.0f,0.0f,0.0f))

	gCam.camPos = vec4f(0.0f,0.0f,0.0f,1.0f)
	//gCam.upDownAng = -45deg
	//gCam.leftRightAng = 45deg
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
		
		//for i : 0..2 
		//	for j : (i+1)..3
		//		PhysCheckSvS(spheres[i],spheres[j])
		//for i : 4
		//	PhysCheckSvIP(spheres[i],infP[^])

		//for spheres
		//{
		//	it.System.pos += it.ImpulseV*deltaTime
		//}
		
		//spheresProps[^i].modelPos.pos = spheres[i].System.pos
		//spheresProps[^i].modelPos.pos.w = 0.1f

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
		
		itPlayer.System.pos = gCam.camPos
		itPlayer.System.pos.w = 1.7f
		printf("heh %f %f %f\n",gCam.camPos.x,gCam.camPos.y,gCam.camPos.z)
		tmpsS := new List.{vec4f} ; $temp
		PhysCheckPlayerVSHMap(itPlayer,mapP,tmpsS)

		if tmpsS.Size() == 0
		{
			spheresProps[0].modelPos.pos = vec4f(0.0f,0.0f,-100.0f,1.0f)
			spheresProps[1].modelPos.pos = vec4f(0.0f,0.0f,-100.0f,1.0f)
			spheresProps[2].modelPos.pos = vec4f(0.0f,0.0f,-100.0f,1.0f)
		}else{
			kk := (dotIter div 40) % (tmpsS.Size() div 3)
			spheresProps[0].modelPos.pos = tmpsS^[kk*3]
			spheresProps[0].modelPos.pos.w = 0.2
			spheresProps[1].modelPos.pos = tmpsS^[kk*3+1]
			spheresProps[1].modelPos.pos.w = 0.2
			spheresProps[2].modelPos.pos = tmpsS^[kk*3+2]
			spheresProps[2].modelPos.pos.w = 0.2
		}


		vDraw()

		StopDraw()
		fpsCounter++
		dotIter += 1
	}
		
	return 0

}

