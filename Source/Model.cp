#import "VulkanCore.cp"
#import "RawModel.cp"

Model := class 
{
	memsValue := VkDeviceMemory[2]
	hndls := VkBuffer[2]
	indexCount := int
	LoadFile := !(char^ fName) -> bool
	{
		printf("loading %s\n",fName)
		rFile := new RawModel ; $temp
		if not rFile.LoadFromPLY(fName)
		{
			printf("nope\n")
			return false
		}
	
		indexCount = rFile.inds->len
		
		allc1 := new VkMemoryAllocateInfo() ; $temp
		allc1.allocationSize =  rFile.verts->len*4
		allc1.memoryTypeIndex = 1

		bufC := new VkBufferCreateInfo() ; $temp
		bufC.size = allc1.allocationSize
		bufC.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		bufC.sharingMode = VK_SHARING_MODE_EXCLUSIVE

		memInfo := VkMemoryRequirements

		vkFuncs.vkCreateBuffer(vkLogCard,bufC,null,hndls[0]&)
		vkFuncs.vkGetBufferMemoryRequirements(vkLogCard,hndls[0],memInfo&)
		vkFuncs.vkAllocateMemory(vkLogCard,allc1,null,memsValue[0]&)
		
		vkFuncs.vkBindBufferMemory(vkLogCard,hndls[0],memsValue[0],0)

		memPoint := void^
		vkFuncs.vkMapMemory(vkLogCard,memsValue[0],0,allc1.allocationSize,0,memPoint&)
		//memcpy(memPoint,rFile.verts->{void^},allc1.allocationSize)

		asFlt := memPoint->{float^}
		vertCount := rFile.verts->len div 8
		for i : vertCount
		{
			asFlt[i*8    ] = rFile.verts[i*8]
			asFlt[i*8 + 1] = rFile.verts[i*8 + 1]
			asFlt[i*8 + 2] = rFile.verts[i*8 + 2]

			asFlt[i*8 + 4] = rFile.verts[i*8 + 3]
			asFlt[i*8 + 5] = rFile.verts[i*8 + 4]
			asFlt[i*8 + 6] = rFile.verts[i*8 + 5]

			asFlt[i*8 + 3] = rFile.verts[i*8 + 6]
			asFlt[i*8 + 7] = rFile.verts[i*8 + 7]
		}


		flushRange := new VkMappedMemoryRange() ; $temp
		flushRange.memory = memsValue[0]
		flushRange.offset = 0
		flushRange.size = allc1.allocationSize
		vkFuncs.vkFlushMappedMemoryRanges(vkLogCard,1,flushRange)
		vkFuncs.vkUnmapMemory(vkLogCard,memsValue[0])

		allc1.allocationSize = rFile.inds->len*4
		bufC.size = allc1.allocationSize
		bufC.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT
		vkFuncs.vkCreateBuffer(vkLogCard,bufC,null,hndls[1]&)
		vkFuncs.vkGetBufferMemoryRequirements(vkLogCard,hndls[1],memInfo&)
		vkFuncs.vkAllocateMemory(vkLogCard,allc1,null,memsValue[1]&)
		vkFuncs.vkBindBufferMemory(vkLogCard,hndls[1],memsValue[1],0)
		vkFuncs.vkMapMemory(vkLogCard,memsValue[1],0,allc1.allocationSize,0,memPoint&)
		memcpy(memPoint,rFile.inds->{void^},allc1.allocationSize)

		flushRange.memory = memsValue[1]
		flushRange.offset = 0
		flushRange.size = allc1.allocationSize
		vkFuncs.vkFlushMappedMemoryRanges(vkLogCard,1,flushRange)
		vkFuncs.vkUnmapMemory(vkLogCard,memsValue[1])
	}
	AddToCmdBuffer := !(VkCommandBuffer cmdB) -> void
	{
		offsets := new VkDeviceSize() ; $temp
		vkFuncs.vkCmdBindVertexBuffers(cmdB,0,1,hndls[0]&,offsets)
		vkFuncs.vkCmdBindIndexBuffer(cmdB,hndls[1],0,VK_INDEX_TYPE_UINT32)
		vkFuncs.vkCmdDrawIndexed(cmdB,indexCount,1,0,0,0)
	}
}
