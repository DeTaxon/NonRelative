#import "VulkanCore.cp"
#import "Model.cp"
#import "CmdBuffer.cp"
#import "Shader.cp"
#import "Prop.cp"
#import "Window.cp"
#import "Camera.cp"
#import "ZipFS.cp"
#import "VoidCore.cp"
#import "ObjectInfo.cp"

main := !(int argc, char^^ argv) -> int
{
	CreateWindow(700,700)
	defer DestroyWindow() 


	InitVulkan()
	defer DestroyVulkan()

	vInit()

	prp := vAddProp("Snooker/SnookerTable")
	prp.modelPos.ang = quantfAt(1.0f,0.0f,0.0f,0deg)
	prp.modelPos.pos = vec4f(0.0f,0.0f,0.0f,0.4f)

	//nn := vAddProp("HiResBox")
	//nn.modelPos.ang = quantfAt(1.0f,0.0f,0.0f,0.0f)
	//nn.modelPos.pos = vec4f(5.0f,0.0f,3.0f,1.0f)

	//pln := vAddProp("Plane/Plane")
	//pln.modelPos.ang = quantfAt(1.0f,0.0f,0.0f,0deg)
	//pln.modelPos.pos = vec4f(0.0f,0.0f,0.0f,0.4f)

	prevTime := glfwGetTime()
	walkM := 0.5f

	resizeState := false


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
	}
		
	return 0

}

