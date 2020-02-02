titleBuf := char[256]

btnPress := int

KeyPress := !(int ch,bool isPress) -> void
{
	if isPress and ch == 'b' btnPress += 1
}

gUV := uvLoop^
main := !(int argc, char^^ argv) -> int
{
	uvInit()

	gUV = new uvLoop()


	//gUV.Run()

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
	drawState := false
	drawMutex := new uvMutex()
	drawCond := new uvCond()
	quit := false

	gUV.Timer(0,0.001,(x) ==> [quit&]{
		prevTime := glfwGetTime()
		while true
		{
			if quit {
				x.Stop()
				return void
			}
			nowTime := glfwGetTime()
			deltaTime := nowTime - prevTime
			vPhysStage(deltaTime)
			prevTime = nowTime
			yield void
		}
	})

	fpsCounter := 0
	lastCheckedTime := 0.0

	resizeState := false

	gUV.Timer(0,0.01, (x) ==> [quit&,fpsCounter&]{
		
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
		
		if nowTime - prevTime > 1.0
		{
			ttlPre := "Hi again! fps = "sbt + fpsCounter
			strcpy(titleBuf[0]&,ttlPre.Str()) ; $temp
			glfwSetWindowTitle(glfwWindow,titleBuf[0]&)
			fpsCounter = 0
			prevTime = nowTime
		}

		if glfwWindowShouldClose(glfwWindow)
		{
			quit = true
			x.Stop()
		}
			yield void
		}
	})

	ev := gUV.Event(() ==> [quit&,drawState&,drawMutex,drawCond,fpsCounter&,ev&]{

		prevTime := glfwGetTime()

		while  true 
		{
			if quit {
				ev.Close()
				return void
			}


			nowTime := glfwGetTime()
			gNowTime = nowTime
			deltaTime := nowTime - prevTime
			
			prevTime = nowTime
			gCam.InputCheck(deltaTime)
			gCam.BindDescriptor(mainCmd.Get())
			
			vDraw()

			StopDraw()
			fpsCounter++

			drawMutex.Lock()
			drawState = false
			drawCond.Notify()
			drawMutex.Unlock()

			yield void
		}
	})
	if false
	{
		
	}else{
		uvThread(() ==> [quit&,drawState&,drawMutex,drawCond]{
			while not quit
			{
				drawMutex.Lock()
				while drawState
				{
					drawCond.Wait(drawMutex^,0.1)
				}
				if quit {
					ev.Emit()
					drawMutex.Unlock()
					return void
				}
				drawMutex.Unlock()

				if StartDraw()
				{
					drawState = true
					ev.Emit()
				}else{
					TSleep(0.1)
				}
			}
		})
	}
	gUV.Run()
		
	return 0

}

