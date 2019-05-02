#import "VulkanCore.cp"


GetMemObj := !(int size) -> vMemObj
{
	
}

vMemObj := class
{
	memObj := VkDeviceMemory
	objSize := int

	CreateObject := !(int size) -> void
	{
		allc1 := new VkMemoryAllocateInfo() ; $temp
		allc1.allocationSize =  size
		allc1.memoryTypeIndex = 1

		vkFuncs.vkAllocateMemory(vkLogCard,allc1,null,memObj&)
		objSize = size
	}
	Map := !() -> void^
	{
		memToRet := void^
		memPoint := void^
		vkFuncs.vkMapMemory(vkLogCard,memObj
		,0,objSize,0,memToRet&)

		return memToRet
		
	}
	Unmap := !() -> void
	{
		flushRange := new VkMappedMemoryRange() ; $temp
		flushRange.memory = memObj
		flushRange.offset = 0
		flushRange.size = objSize
		vkFuncs.vkFlushMappedMemoryRanges(vkLogCard,1,flushRange)
		vkFuncs.vkUnmapMemory(vkLogCard,memObj)
	}
	Get := !() -> VkDeviceMemory
	{
		return memObj
	}
}

