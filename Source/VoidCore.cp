vObject := class
{
	Destroy := virtual !() -> void {}
}

//VoidCore := class
//{
	gRepo := vRepo

	itModels := AVLMap.{char^,vModel}
	itMaps := AVLMap.{char^,vMap}
	itProps := List.{vProp}

	gStageMem := vMemObj^
	gStageMemBuffer := VkBuffer

	gCam := vCamera

	gDoubleMem := bool

	gObjectLayoutSets := VkDescriptorPool

	gBadTexture := vTexture^

	//pVoidMP := AllocOnlyMP.{4096,true}^

	gSamplerNearest := VkSampler
	gSamplerLinear := VkSampler

	gNowTime := double

	gPlayer := PhysPlayer^
	gPlayerMap := vMap^

	gHotload := bool

	slCompilerPath := char^
	slInputFile := char^
	slOutputFile := char^
	slKeepResult := bool

	gWinStartW := 100
	gWinStartH := 100

	pLoadCommonSettings := !() -> void
	{
		itSet := gRepo.GetFile("commonsettings.inf")

		if itSet == null
			return void
		
		ptrToFl := itSet.Map()
		if ptrToFl == null
			return void
		defer itSet.Unmap()
		itInf := ParseInfoFile(ptrToFl,itSet.Size()) ; $temp

		for sets : itInf.SubList
		{
			switch sets.Name
			{
				case "start_window"
					switch sets.SubList[^].Name
					{
						case "w"
							gWinStartW = ToInt(it.ValueStr)
						case "h"
							gWinStartH = ToInt(it.ValueStr)
					}
				case "present_priority"
					switch sets.SubList[^].ValueStr
					{
						case "FIFO"
							gUserModes.PushFront(VK_PRESENT_MODE_FIFO_KHR)
						case "FIFO_RELAXED"
							gUserModes.PushFront(VK_PRESENT_MODE_FIFO_RELAXED_KHR)
						case "MAILBOX"
							gUserModes.PushFront(VK_PRESENT_MODE_MAILBOX_KHR)
						case "IMMEDIATE"
							gUserModes.PushFront(VK_PRESENT_MODE_IMMEDIATE_KHR)
					}
				case "hotload"
					gHotload = sets.ValueStr == "true"
				case "shader_compiler"
					switch sets.SubList[^].Name
					{
						case "exe"
							slCompilerPath = it.ValueStr.Str()
						case "inp_temp"
							slInputFile = it.ValueStr.Str()
						case "out_temp"
							slOutputFile = it.ValueStr.Str()
						case "keep"
							slKeepResult = it.ValueStr == "true"
					}
			}
		}
		gUserModes.PushFront(VK_PRESENT_MODE_MAILBOX_KHR) ; $uniq
		gUserModes.PushFront(VK_PRESENT_MODE_FIFO_KHR) ; $uniq
		gUserModes.PushFront(VK_PRESENT_MODE_FIFO_RELAXED_KHR) ; $uniq
		gUserModes.PushFront(VK_PRESENT_MODE_IMMEDIATE_KHR) ; $uniq

	}
	vPreInit := !() -> void
	{
		gRepo.Init("./")
		gRepo.AddZipRoot("./Dlls.zip")
		LibDB.SetRepo(gRepo&)
		pLoadCommonSettings()
	}
	vInit := !() -> void
	{
		gDoubleMem = true
		if vkGpuMemId == vkCpuMemId or vkGpuMemId == -1
			gDoubleMem = false

		//pVoidMP = new AllocOnlyMP.{4096,true}

		gStageMem = new vMemObj

		stSize := 0x4c4c00
		bufC := new VkBufferCreateInfo() ; $temp
		bufC.size = stSize
		bufC.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		bufC.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		vkFuncs.vkCreateBuffer(vkLogCard,bufC,null,gStageMemBuffer&)
		memInfo := VkMemoryRequirements
		vkFuncs.vkGetBufferMemoryRequirements(vkLogCard,gStageMemBuffer,memInfo&)
		gStageMem.CreateObject(stSize,memInfo.memoryTypeBits,null)
		vkFuncs.vkBindBufferMemory(vkLogCard,gStageMemBuffer,gStageMem.Get(),0)

		gCam."this"()
		gCam.SetPerspective(gWindowW,gWindowH,0.01f,100.0f,90deg)

		poolSize := u32[2] 
		poolSize[0] = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
		poolSize[1] = 100 //TODO: size, add dynamicly

		nPool := new VkDescriptorPoolCreateInfo() ; $temp
		nPool.maxSets = 100
		nPool.poolSizeCount = 1
		nPool.pPoolSizes = poolSize&->{void^}

		vkFuncs.vkCreateDescriptorPool(vkLogCard,nPool,null,gObjectLayoutSets&)

		imV := new VkSamplerCreateInfo() ; $temp
		imV.magFilter = VK_FILTER_NEAREST
		imV.minFilter = VK_FILTER_NEAREST
		imV.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST
		imV.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
		imV.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
		imV.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
		//imV.mpLodBias = 0.0f
		//imV.anisotropyEnable = VK_FALSE
		imV.maxAnisotropy = 1.0f
		imV.compareEnable = VK_FALSE
		imV.compareOp = VK_COMPARE_OP_ALWAYS
		imV.minLod = 0.0f
		imV.maxLod = 0.0f
		imV.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK
		//imV.unnormalizedCoordinates = VK_FALSE

		vkFuncs.vkCreateSampler(vkLogCard,imV,null,gSamplerNearest&)
		imV.magFilter = VK_FILTER_LINEAR
		imV.minFilter = VK_FILTER_LINEAR
		vkFuncs.vkCreateSampler(vkLogCard,imV,null,gSamplerLinear&)

		gBadTexture = new vTexture
		gBadTexture.CreateObject(64,64)
		gBadTexture.LoadNotFound()

		CreateSwapchain(gWindowW,gWindowH)
		CreateFB()

		gPlayer = new PhysPlayer()
		gPlayer.Height = 1.7f
		gPlayer.System.pos = vec4f(1.0f,1.0f,2.0f,1.0f)

	}
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
	vStageCpyToImage := !(VkImage tCpy,int w, int h) -> void
	{
		mainCmd.Reset()
		mainCmd.Start()

		//isr := new VkImageSubresourceRange ; $temp
		//isr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
		//isr.levelCount = 1
		//isr.layerCount = 1

		stg1 := new VkImageMemoryBarrier() ; $temp

		stg1.srcAccessMask = 0
		stg1.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT
		stg1.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED
		stg1.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
		stg1.srcQueueFamilyIndex = 0
		stg1.srcQueueFamilyIndex = not_b stg1.srcQueueFamilyIndex
		stg1.dstQueueFamilyIndex = stg1.srcQueueFamilyIndex
		stg1.image = tCpy
		stg1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
		stg1.subresourceRange.levelCount = 1
		stg1.subresourceRange.layerCount = 1


		vkFuncs.vkCmdPipelineBarrier(mainCmd.Get(),VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,0,0,null,0,null,1,stg1)

		cpyCmd := new VkBufferImageCopy ; $temp
		cpyCmd.imageSubresource.layerCount = 1
		cpyCmd.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
		cpyCmd.imageExtent.width = w
		cpyCmd.imageExtent.height = h
		cpyCmd.imageExtent.depth = 1

		vkFuncs.vkCmdCopyBufferToImage(mainCmd.Get(),gStageMemBuffer,tCpy,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,cpyCmd)

		stg2 := new VkImageMemoryBarrier() ; $temp
		stg2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT
		stg2.dstAccessMask = VK_ACCESS_SHADER_READ_BIT
		stg2.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
		stg2.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		stg2.srcQueueFamilyIndex = 0
		stg2.srcQueueFamilyIndex = not_b stg2.srcQueueFamilyIndex
		stg2.dstQueueFamilyIndex = stg2.srcQueueFamilyIndex
		stg2.image = tCpy
		stg2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
		stg2.subresourceRange.levelCount = 1
		stg2.subresourceRange.layerCount = 1

		vkFuncs.vkCmdPipelineBarrier(mainCmd.Get(),VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,0,0,null,0,null,1,stg2)

		mainCmd.Stop()
		mainCmd.Submit()
	}
	vSetTexture := !(VkDescriptorSet toSet,vTexture^ donor,VkSampler smSampler) -> void
	{
		imgI := new VkDescriptorImageInfo ; $temp
		imgI.sampler = smSampler
		imgI.imageView = donor.itImgView
		imgI.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL

		wrT := new VkWriteDescriptorSet() ; $temp
		wrT.descriptorCount = 1
		wrT.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
		wrT.pImageInfo = imgI->{void^}
		wrT.dstSet = toSet

		vkFuncs.vkUpdateDescriptorSets(vkLogCard,1,wrT,0,null)
	}

	iDrawedMaps :=  AVLMap.{vMap^,List.{centf}}^ ; $temp
	vDraw := !() -> void
	{
		//for itProps
		//{
		//	it.modelShader.ApplyShaderToQueue(mainCmd.Get())
		//	it.AddToCmdBuffer(mainCmd.Get())
		//}

		iDrawedMaps = new AVLMap.{vMap^,List.{centf}} ; $temp

		initPos := centf()
		vDrawMap(gPlayerMap,10,initPos) //TODO: number to config
	}
	vDrawMap := !(vMap^ someMap,int depth,centf relPos) -> void
	{
		if depth < 0 
			return void
		for it : iDrawedMaps^[someMap] ; $temp
		{
			if it.NearDistance(relPos) < 1 //TODO: unconst
				return void
		}
		iDrawedMaps^[someMap].Emplace() = relPos



		for mod : someMap.mapProps
		{
			mod.modelShader.ApplyShaderToQueue(mainCmd.Get())
			mod.AddToCmdBuffer(mainCmd.Get(),relPos)
		}
		for it : linksFromMap[someMap]
		{
			newPos := relPos<*>it.relativePos
			vDrawMap(it.linkTo,depth - it.renderCost,newPos) 
		}
		for it : linksToMap[someMap]
		{
			newPos := relPos<*>it.relativePos.Inverse()
			vDrawMap(it.linkFrom,depth - it.renderCost,newPos) 
		}
	}

//}
