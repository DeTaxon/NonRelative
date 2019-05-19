#import "VulkanCore.cp"


vMemObj := class
{
	memObj := VkDeviceMemory
	objSize := int
	gpuSide := bool
	tempMem := VkDeviceMemory

	CreateObject := !(int size, bool gpuSideI) -> void
	{
		gpuSide = gpuSideI
		if vkCpuMemId == vkGpuMemId or vkGpuMemId == -1
			gpuSide = false
		allc1 := new VkMemoryAllocateInfo() ; $temp
		allc1.allocationSize =  size
		if gpuSide
		{
			allc1.memoryTypeIndex = vkGpuMemId
		}else{
			allc1.memoryTypeIndex = vkCpuMemId
		}

		vkFuncs.vkAllocateMemory(vkLogCard,allc1,null,memObj&)
		objSize = size
	}
	Map := !() -> void^
	{
		memToRet := void^
		vkFuncs.vkMapMemory(vkLogCard,memObj,0,objSize,0,memToRet&)
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

