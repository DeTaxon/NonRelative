#import "VulkanCore.cp"
#import "Camera.cp"

vCamera := class
{
	upDownAng := float
	leftRightAng := float
	camPos := vec4f
	camNear,camFar,camPOV := float

	perspConsts := float[4]

	this := !() -> void
	{
		upDownAng = 0.0f
		leftRightAng = 0.0f
		camPos.w = 1.0f
		//vkPerspMem.CreateObject(4*4,true)
		vkPerspMem.CreateObject(256,true)

	}

	AddAngs := !(float lrA, float udA) -> void
	{
		upDownAng +=  udA
		leftRightAng += lrA

		fullSpin := 360deg
		almostSpin := 300deg
		if leftRightAng > almostSpin leftRightAng -= fullSpin
		if leftRightAng < -almostSpin leftRightAng += fullSpin

		limit := 89deg
		if upDownAng > limit upDownAng = limit
		if upDownAng < -limit upDownAng = -limit
	}
	//vkPerspMem := VkDeviceMemory
	vkPerspMem := vMemObj
	vkPerspBuffer := VkBuffer

	UpdatePerspective := !(float w, float h) -> void
	{
		aspect := w / h
		fovTM := 1.0f / tanf(camPOV*0.5f)
		range := camFar - camNear

		perspConsts[0] = fovTM / aspect
		perspConsts[1] = fovTM
		perspConsts[2] = (camNear + camFar) / range
		perspConsts[3] =  -(2.0f * camFar*camNear/range)
		
		itMem := vkPerspMem&
		if gDoubleMem
			itMem = gStageMem
		itMem2 := itMem.Map()
		memcpy(itMem2,perspConsts[0]&,4*4)
		itMem.Unmap()
		if gDoubleMem
			vStageCpyToBuffer(vkPerspBuffer,4*4)
	}

	SetPerspective := !(float w, float h, float near, float far, float pov) -> void
	{
		camNear = near
		camFar = far
		camPOV = pov

		allc1 := new VkMemoryAllocateInfo() ; $temp
		allc1.allocationSize =  4*4
		//allc1.memoryTypeIndex = 1

		bufC := new VkBufferCreateInfo() ; $temp
		bufC.size = allc1.allocationSize
		bufC.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT or_b VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT or_b VK_BUFFER_USAGE_TRANSFER_DST_BIT

		memInfo := VkMemoryRequirements

		vkFuncs.vkCreateBuffer(vkLogCard,bufC,null,vkPerspBuffer&)
		vkFuncs.vkGetBufferMemoryRequirements(vkLogCard,vkPerspBuffer,memInfo&)
		vkFuncs.vkBindBufferMemory(vkLogCard,vkPerspBuffer,vkPerspMem.Get(),0)

		bufInfo := new VkDescriptorBufferInfo ; $temp
		bufInfo.buffer = vkPerspBuffer 
		bufInfo.offset = 0
		bufInfo.range = 4*4
		someWrite := new VkWriteDescriptorSet() ; $temp
		someWrite.dstSet = vkPerspSet
		someWrite.dstBinding = 0
		someWrite.dstArrayElement = 0
		someWrite.descriptorCount = 1
		someWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
		someWrite.pBufferInfo = bufInfo->{void^}

		vkFuncs.vkUpdateDescriptorSets(vkLogCard,1,someWrite,0,null)

		UpdatePerspective(w,h)
	}

	addLocal := !(vec4f diffAdd) -> void
	{
		rotQ := quantfAt(-1.0f,0.0f,0.0f,90deg)<*>quantfAt(0.0f,1.0f,0.0f,leftRightAng)<*>quantfAt(1.0f,0.0f,0.0f,upDownAng)

		oldW := camPos.w
		camPos += (rotQ*diffAdd)* vec4f(-1.0f,-1.0f,-1.0f,0.0)
		camPos.w = oldW
	}
	ApplyCamera := !(centf propPos, centf posRes) -> void
	{
		tempCent := centf
		tempCent.ang = quantfAt(-1.0f,0.0f,0.0f,90deg)<*>quantfAt(0.0f,1.0f,0.0f,leftRightAng)<*>quantfAt(1.0f,0.0f,0.0f,upDownAng)
		tempCent.pos = camPos

		invC := tempCent.Inverse()
		posRes = invC<*>propPos
	}
	BindDescriptor := !(VkCommandBuffer cmdB) -> void
	{
		vkFuncs.vkCmdBindDescriptorSets(cmdB,VK_PIPELINE_BIND_POINT_GRAPHICS,vkLayout,0,1,vkPerspSet&,0,null)
	}
}

