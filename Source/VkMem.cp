#import "VulkanCore.cp"


vMemObj := class
{
	memObj := VkDeviceMemory
	objSize := int
	gpuSide := bool
	tempMem := VkDeviceMemory

	CreateObject := !(int size,int memType, bool^ gpuSideI) -> bool
	{

		memId := -1
		if(memType == 0)
		{
			memId = gHostMem[0]
			gpuSide = false
		}else
		{
			if ((1 << gDeviceMem[^]) and_b memType) != 0
			{
				memId = it
				break
			}
			if memId == -1
			{
				if ((1 << gHostMem[^]) and_b memType) != 0
				{
					memId = it
					break
				}
			}
			gpuSide = true
			if gHostMem[^] == memId
			{
				gpuSide = false
				break
			}
		}
		if gpuSideI != null
			gpuSideI^ = false

		allc1 := new VkMemoryAllocateInfo() ; $temp
		allc1.allocationSize =  size
		allc1.memoryTypeIndex = memId

		vkFuncs.vkAllocateMemory(vkLogCard,allc1,null,memObj&)
		objSize = size
		return true
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

