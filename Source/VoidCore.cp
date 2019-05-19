#import "ZipFS.cp"
#import "ObjectInfo.cp"
#import "Shader.cp"
#import "VulkanCOre.cp"

//VoidCore := class
//{
	itRepo := vRepo

	itShaders := AVLMap.{char^,vShader}
	itModels := AVLMap.{char^,vModel}
	itProps := List.{vProp}

	gStageMem := vMemObj^
	gStageMemBuffer := VkBuffer

	gCam := vCamera

	gDoubleMem := bool

	vStageCpyToBuffer := !(VkBuffer tCpy,int nSize) -> void
	{
		cpyCmd := new VkBufferCopy ; $temp
		cpyCmd.size = nSize
		
		//vkFuncs.vkBeginCommandBuffer(mainCmd.Get(),strtCmd)
		mainCmd.Reset()
		mainCmd.Start()
		vkFuncs.vkCmdCopyBuffer(mainCmd.Get(),gStageMemBuffer,tCpy,1,cpyCmd)
		memBar := new VkBufferMemoryBarrier() ; $temp
		memBar.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT
		memBar.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
		memBar.srcQueueFamilyIndex = 0
		memBar.srcQueueFamilyIndex = not_b memBar.srcQueueFamilyIndex
		memBar.dstQueueFamilyIndex = memBar.srcQueueFamilyIndex
		memBar.buffer = tCpy
		memBar.size = nSize

		vkFuncs.vkCmdPipelineBarrier(mainCmd.Get(),VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,0,0,null,1,memBar,0,null)
		mainCmd.Stop()
		mainCmd.Submit()

	}
	vInit := !() -> void
	{
		gDoubleMem = true
		if vkGpuMemId == vkCpuMemId or vkGpuMemId == -1
			gDoubleMem = false

		itRepo.Init("./")
		gStageMem = new vMemObj

		stSize := 0x4c4c00
		gStageMem.CreateObject(stSize,false)
		bufC := new VkBufferCreateInfo() ; $temp
		bufC.size = stSize
		bufC.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		bufC.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		vkFuncs.vkCreateBuffer(vkLogCard,bufC,null,gStageMemBuffer&)
		vkFuncs.vkBindBufferMemory(vkLogCard,gStageMemBuffer,gStageMem.Get(),0)

		gCam."this"()
		gCam.SetPerspective(700.0f,700.0f,0.01f,100.0f,90deg)

	}
	vGetModel := !(char^ sName) -> vModel^
	{
		if itModels.Contain(sName)
			return itModels[sName]&
		flName := char^
		stB := "Models/"sbt
		stB << sName
		stB << ".inf"

		asF := itRepo.GetFile(stB)

		if asF == null
			return null

		heh := asF.Map()
		defer asF.Unmap()

		cc := ParseInfo(heh,asF.Size())

		itMd := ref itModels[StrCopy(sName)]
		reqShader := vShader^()
		switch cc.SubList[^].Name
		{
			case "model"
			switch it.SubList.Size()
			{
				case 1
					switch it.SubList[0].Name
					{
						case "file"
							cdP := ref it.SubList[0]
							itF := asF.GetFile(cdP.ValueStr)
							if itF == null 
								return null
							itMd.LoadFile(itF.Map(),itF.Size())
							itF.Unmap()
							
						case void
							return null
					}
				case void
					return null
			}
			case "shader"
			if it.IsValue()
			{
				reqShader = vGetShader(it.ValueStr)
			}else{
				//TODO
			}
			case "texture"
		}
		itMd.ReqShader = reqShader

		return itMd&
	}
	vAddProp := !(char^ modelName) -> vProp^
	{
		itProps.Emplace()
		newPr := ref itProps.Back()
		newPr.modelPtr = vGetModel(modelName)
		newPr.modelShader = newPr.modelPtr.ReqShader
		newPr.modelPos.ang = quantf(0.0f,1.0f,0.0f,0.0f)
		newPr.modelPos.pos = vec4f(0.0f,0.0f,0.0f,1.0f)
		return newPr&
	}
	vDraw := !() -> void
	{
		for itProps
		{
			it.modelShader.ApplyShaderToQueue(mainCmd.Get())
			it.AddToCmdBuffer(mainCmd.Get())
		}
	}
	vGetShader := !(char^ sName) -> Shader^
	{
		if itShaders.Contain(sName)
			return itShaders[sName]&
		flName := char^
		stB := "Shaders/"sbt
		stB << sName
		stB << ".inf"
		flName = stB.Str() ; $temp

		fl := itRepo.GetFile(flName)

		if fl == null
			return null

		heh := fl.Map()

		if heh == null
			return null
		defer fl.Unmap()

		cc := ParseInfo(fl.Map(),fl.Size())
	
		fndSh := false
		vertName := StringSpan()
		fragName := StringSpan()
		for shIn : cc.SubList
		{
			if shIn.Name == "shader"
			{
				switch shIn.SubList[^].Name
				{
					case "vertex"
						vertName = it.ValueStr
					case "fragment"
						fragName = it.ValueStr
					case void 
						return null
				}
				fndSh = true
			}
		}
		vertFile := vRepoFile^()
		fragFile := vRepoFile^()

		if vertName != ""{
			vertFile = fl.GetFile(vertName)
			if vertFile == null
				return null
		}
		if fragName != ""{
			fragFile = fl.GetFile(fragName)
			if fragFile == null
				return null
		}
		itSh := ref itShaders[StrCopy(sName)]

		itSh.LoadShader(vertFile.Map(),vertFile.Size(),fragFile.Map(),fragFile.Size())
		vertFile.Unmap()
		fragFile.Unmap()


		return itSh&
	}
//}
