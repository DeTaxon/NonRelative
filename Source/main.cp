titleBuf := char[256]

gQuit := false
gUV := uvLoop^

main := !(int argc, char^^ argv) -> int
{
	vPreInit()

	uvInit()

	gUV = new uvLoop()

	CreateWindow(1700,900)
	defer DestroyWindow() 

	InitVulkan()
	defer DestroyVulkan()

	vInit()
	ScriptInit()
	
	gCam.camPos = vec4f(0.0f,0.0f,0.0f,1.0f)
	gCam.upDownAng = 0.0f
	gCam.leftRightAng = 0.0f

	gHotloadInit()

	nMap := vGetMap("FirstMap")

	//mt := vAddProp("Mitr")

	cnvs := vAddProp("Canvas")
	cnvs.modelPos.pos = vec4f(-2.0f,2.0f,2.0f,1.0f)
	cnvs.modelPos.ang = quantfAt(0.0f,1.0f,0.0f,90deg)

	drawState := false
	drawMutex := new uvMutex()
	drawCond := new uvCond()

	gUV.Timer(0,0.001,(x) ==> {
		prevTime := glfwGetTime()
		while true
		{
			if gQuit {
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

	gUV.Timer(0,0.01, (x) ==> [fpsCounter&]{
		
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
				gQuit = true
				gHotloadStop()
				x.Stop()
			}
			yield void
		}
	})

	ev := gUV.Event(() ==> [drawState&,drawMutex,drawCond,fpsCounter&,ev&]{

		prevTime := glfwGetTime()

		while  true 
		{
			if gQuit {
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

			gHotloadCheck()
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
		uvThread(() ==> [drawState&,drawMutex,drawCond]{
			while not gQuit
			{
				drawMutex.Lock()
				while drawState
				{
					drawCond.Wait(drawMutex^,0.1)
				}
				if gQuit {
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

