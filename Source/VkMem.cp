AlignUp := !(int value, int align) -> int
{
	assert(align != 0)
	extra := value mod align
	if extra != 0
		value += align - extra
	return value
}

vMemObj := class
{
	memObj := VkDeviceMemory
	objSize := int
	gpuSize := int
	gpuSide := bool

	CreateObject := !(int size,int memType, bool^ gpuSideI) -> bool
	{
		memId := -1
		if(memType == 0)
		{
			memId = gHostMem[0]
			gpuSide = false
		}else
		{
			if ((1 << gHostMem[^]) and_b memType) != 0
			{
				memId = it
				break
			}
			if memId == -1
			{
				if ((1 << gDeviceMem[^]) and_b memType) != 0
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
			gpuSideI^ = gpuSide

		gpuSize = AlignUp(size,0x40) //TODO unconst
		gpuSize = max(gpuSize,0x500000)
		allc1 := new VkMemoryAllocateInfo() ; $temp
		allc1.allocationSize =  gpuSize
		allc1.memoryTypeIndex = memId

		vkFuncs.vkAllocateMemory(vkLogCard,allc1,null,memObj&)
		objSize = size
		return true
	}
	DestroyVK := !() -> void
	{
		vkFuncs.vkFreeMemory(vkLogCard, memObj,null)
	}
	Map := !() -> void^
	{
		memToRet := void^
		vkFuncs.vkMapMemory(vkLogCard,memObj,0,gpuSize,0,memToRet&)
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

