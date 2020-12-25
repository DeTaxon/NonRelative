vModel := class 
{
	memObjs := vMemObj[2]
	hndls := VkBuffer[2]
	indexCount := int
	ReqShader := vShader^
	ReqTexture := vTexture^

	physType := char^
	physExtraData := void^

	scriptFile := vRepoFile^
	scriptUnit := ScriptUnit^

	vertexInfo := vShaderVertexOptions

	indexType := VKType


	LoadFile := !(vRepoFile^ itFl) -> bool
	{
		rFile := new RawModel ; $temp
		if not rFile.LoadFromFile(itFl)
			return false

		vertexInfo.positionType = rFile.PositionType
		vertexInfo.normalType = rFile.NormalType
		vertexInfo.textureType = rFile.UVType
	
		indexType = rFile.IndexType
		indexCount = rFile.IndexCount
		
		vertSize :=  rFile.VertexCount*rFile.GetVertSize()

		bufC := new VkBufferCreateInfo() ; $temp
		bufC.size = vertSize
		bufC.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT or_b VK_BUFFER_USAGE_TRANSFER_DST_BIT
		bufC.sharingMode = VK_SHARING_MODE_EXCLUSIVE

		memInfo := VkMemoryRequirements

		vkFuncs.vkCreateBuffer(vkLogCard,bufC,null,hndls[0]&)
		vkFuncs.vkGetBufferMemoryRequirements(vkLogCard,hndls[0],memInfo&)
		isVertGpu := bool
		memObjs[0].CreateObject(memInfo.size,memInfo.memoryTypeBits,isVertGpu&)
		
		vkFuncs.vkBindBufferMemory(vkLogCard,hndls[0],memObjs[0].Get(),0)

		memO := memObjs[0]&
		if isVertGpu
			memO = gStageMem

		memPoint := memO.Map()
		memcpy(memPoint,rFile.verts,vertSize)
		memO.Unmap()

		if isVertGpu
			vStageCpyToBuffer(hndls[0],vertSize)
		indSize := rFile.IndexCount*rFile.IndexType.GetSize()
		bufC.size = indSize
		bufC.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT or_b VK_BUFFER_USAGE_TRANSFER_DST_BIT
		vkFuncs.vkCreateBuffer(vkLogCard,bufC,null,hndls[1]&)
		vkFuncs.vkGetBufferMemoryRequirements(vkLogCard,hndls[1],memInfo&)

		isIndGpu := bool
		memObjs[1].CreateObject(memInfo.size,memInfo.memoryTypeBits,isIndGpu&)

		vkFuncs.vkBindBufferMemory(vkLogCard,hndls[1],memObjs[1].Get(),0)

		memO2 := memObjs[1]&
		if isIndGpu
			memO2 = gStageMem
		memPoint = memO2.Map()
		memcpy(memPoint,rFile.inds->{void^},indSize)
		memO2.Unmap()
		if isIndGpu
			vStageCpyToBuffer(hndls[1],indSize)
	}
	DestroyVK := !() -> void
	{
		vkFuncs.vkDestroyBuffer(vkLogCard,hndls[0],null)
		vkFuncs.vkDestroyBuffer(vkLogCard,hndls[1],null)
		memObjs[0].DestroyVK()
		memObjs[1].DestroyVK()
	}
	ReloadModel := !(vRepoFile^ itFl) -> void
	{
		DestroyVK()
		LoadFile(itFl)
	}
	//ReloadScript := !(vRepoFile^ itFl) -> void
	//{
	//	scr := ScriptCompile(itFl)
	//	if scr != null
	//	{
	//		cpy := new List.{ScriptThread^} ; $temp
	//		prp := new AVLSet.{vProp^} ; $temp

	//		for it : hlRunThreads
	//		{
	//			cpy.Push(it)
	//			prp.Insert(it.thrdVObject->{vProp^})
	//		}

	//		for it : cpy^
	//		{
	//			it.Destroy()
	//		}

	//		for it : prp^
	//		{
	//			ScriptRun(scr,it)
	//		}
	//		//TODO: destroy CompileUnit
	//		
	//		printf("reloaded script\n")
	//	}else{
	//		printf("failed reload\n")
	//	}
	//}
	AddToCmdBuffer := !(VkCommandBuffer cmdB) -> void
	{
		offsets := new VkDeviceSize() ; $temp
		vkFuncs.vkCmdBindVertexBuffers(cmdB,0,1,hndls[0]&,offsets)
		vkFuncs.vkCmdBindIndexBuffer(cmdB,hndls[1],0,indexType.GetBaseIndexType())
		vkFuncs.vkCmdDrawIndexed(cmdB,indexCount,1,0,0,0)
	}
}
