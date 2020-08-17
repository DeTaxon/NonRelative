
gRenderPassModel := VkRenderPass
gRenderPassLight := VkRenderPass

vkImages := VkImage[]
vkImageViews := VkImageView[]
vkFramebuffers := VkFramebuffer[]

gModelFramebuffer := VkFramebuffer

depthTexture := vTexture^
fbTextures := vTexture^[2]

gLightShader := vShader^

gLightLayout := VkPipelineLayout
gLightObjectLayout := VkDescriptorSetLayout
gGBufferLayoutSets := VkDescriptorPool
gGBufferTextureSet := VkDescriptorSet

CreateSwapchain := !(int inW,int inH) -> void
{
	oldSwapchain := vkSwapchain

	gVulkanWindowW = inW
	gVulkanWindowH = inH

	surfAb := new VkSurfaceCapabilitiesKHR ; $temp
	vkFuncs.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysCard,vkSurface,surfAb)

	crtSwap := new VkSwapchainCreateInfoKHR() ; $temp
	crtSwap.surface = vkSurface
	crtSwap.minImageCount = 3
	crtSwap.imageFormat = SwapImageFormat
	crtSwap.imageColorSpace = SwapImageColorSpace
	crtSwap.imageExtent.width = inW
	crtSwap.imageExtent.height = inH
	crtSwap.imageArrayLayers = 1
	crtSwap.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
	crtSwap.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE
	crtSwap.queueFamilyIndexCount = 0
	crtSwap.pQueueFamilyIndices = null
	crtSwap.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
	crtSwap.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
	crtSwap.presentMode = SwapImageMode
	crtSwap.clipped = 0
	crtSwap.oldSwapchain = null
	
	vkFuncs.vkCreateSwapchainKHR(vkLogCard,crtSwap,null,vkSwapchain&)

	if oldSwapchain != null
	{
		vkFuncs.vkDestroySwapchainKHR(vkLogCard,oldSwapchain,null)
	}

	imgCount := 0
	vkFuncs.vkGetSwapchainImagesKHR(vkLogCard,vkSwapchain,imgCount&,null)
	vkImages = new VkImage[imgCount]
	vkFuncs.vkGetSwapchainImagesKHR(vkLogCard,vkSwapchain,imgCount&,vkImages)

	attmDesc := new VkAttachmentDescription[1] ; $temp
	attmDesc[0].format = SwapImageFormat
	attmDesc[0].samples = VK_SAMPLE_COUNT_1_BIT
	attmDesc[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR
	attmDesc[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE
	attmDesc[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE
	attmDesc[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
	attmDesc[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	attmDesc[0].finalLayout  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR

	attmRef := new VkAttachmentReference ; $temp
	attmRef.attachment = 0
	attmRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL

	subpass := new VkSubpassDescription ; $temp
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
	subpass.inputAttachmentCount = 0
	subpass.pInputAttachments = null
	subpass.colorAttachmentCount = 1
	subpass.pColorAttachments = attmRef
	subpass.pResolveAttachments = null
	subpass.pDepthStencilAttachment = null
	subpass.preserveAttachmentCount = 0
	subpass.pPreserveAttachments = null

	rpC := new VkRenderPassCreateInfo() ; $temp
	rpC.attachmentCount = 1
	rpC.pAttachments = attmDesc
	rpC.subpassCount = 1
	rpC.pSubpasses = subpass
	rpC.dependencyCount = 0
	rpC.pDependencies = null

	vkFuncs.vkCreateRenderPass(vkLogCard,rpC,null,gRenderPassLight&)

	vkImageViews = new VkImageView[vkImages->len]
	vkFramebuffers = new VkFramebuffer[vkImages->len]
	for it,i : vkImages
	{
		imgViewC := new VkImageViewCreateInfo() ; $temp
		imgViewC.image = it
		imgViewC.viewType = VK_IMAGE_VIEW_TYPE_2D
		imgViewC.format = SwapImageFormat
		imgViewC.components.r = VK_COMPONENT_SWIZZLE_R
		imgViewC.components.g = VK_COMPONENT_SWIZZLE_G
		imgViewC.components.b = VK_COMPONENT_SWIZZLE_B
		imgViewC.components.a = VK_COMPONENT_SWIZZLE_A

		imgViewC.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
		imgViewC.subresourceRange.baseMipLevel = 0
		imgViewC.subresourceRange.levelCount = 1
		imgViewC.subresourceRange.baseArrayLayer = 0
		imgViewC.subresourceRange.layerCount = 1

		vkFuncs.vkCreateImageView(vkLogCard,imgViewC,null,vkImageViews[i]&)

		fbC := new VkFramebufferCreateInfo() ; $temp
		fbC.renderPass = gRenderPassLight
		fbC.attachmentCount = 1
		fbC.pAttachments = vkImageViews[i]&
		fbC.width = inW
		fbC.height = inH
		fbC.layers = 1

		vkFuncs.vkCreateFramebuffer(vkLogCard,fbC,null,vkFramebuffers[i]&)
	}
}
CreateFB := !() -> void
{
	nowW := 2048
	nowH := 1024

	if depthTexture != null
	{
		depthTexture.Destroy()
		delete depthTexture
		depthTexture = null
	}

	fbTextures[0] = new vTexture
	fbTextures[0].CreateObject(nowW,nowH, (img,viw) ==> {
		img.usage = VK_IMAGE_USAGE_SAMPLED_BIT or_b VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT or_b VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
	})
	fbTextures[1] = new vTexture
	fbTextures[1].CreateObject(nowW,nowH, (img,viw) ==> {
		img.usage = VK_IMAGE_USAGE_SAMPLED_BIT or_b VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT or_b VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
		img.format = VK_FORMAT_R32G32B32A32_SFLOAT
	})

	//create deth
	depthTexture = new vTexture
	depthTexture.CreateObject(nowW,nowH,(img,viv)==> {
		img.format = VK_FORMAT_D16_UNORM
		img.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
		viv.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT
	})


	attmDesc := new VkAttachmentDescription[3] ; $temp
	attmDesc[1].format = VK_FORMAT_R8G8B8A8_UNORM
	attmDesc[1].samples = VK_SAMPLE_COUNT_1_BIT
	attmDesc[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR
	attmDesc[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE
	attmDesc[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE
	attmDesc[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
	attmDesc[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	attmDesc[1].finalLayout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL

	attmDesc[2].format = VK_FORMAT_R32G32B32A32_SFLOAT
	attmDesc[2].samples = VK_SAMPLE_COUNT_1_BIT
	attmDesc[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR
	attmDesc[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE
	attmDesc[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE
	attmDesc[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
	attmDesc[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	attmDesc[2].finalLayout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL

	attmDesc[0].format = VK_FORMAT_D16_UNORM
	attmDesc[0].samples = VK_SAMPLE_COUNT_1_BIT
	attmDesc[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR
	attmDesc[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
	attmDesc[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE
	attmDesc[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
	attmDesc[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	attmDesc[0].finalLayout  =VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL

	attmRef := new VkAttachmentReference[2] ; $temp
	attmRef[0].attachment = 1
	attmRef[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	attmRef[1].attachment = 2
	attmRef[1].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	attmRef2 := new VkAttachmentReference ; $temp
	attmRef2.attachment = 0
	attmRef2.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL

	subpass := new VkSubpassDescription ; $temp
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
	subpass.inputAttachmentCount = 0
	subpass.pInputAttachments = null
	subpass.colorAttachmentCount = 2
	subpass.pColorAttachments = attmRef
	subpass.pResolveAttachments = null
	subpass.pDepthStencilAttachment = attmRef2
	subpass.preserveAttachmentCount = 0
	subpass.pPreserveAttachments = null

	rpC := new VkRenderPassCreateInfo() ; $temp
	rpC.attachmentCount = 3
	rpC.pAttachments = attmDesc
	rpC.subpassCount = 1
	rpC.pSubpasses = subpass
	rpC.dependencyCount = 0
	rpC.pDependencies = null

	vkFuncs.vkCreateRenderPass(vkLogCard,rpC,null,gRenderPassModel&)

	extrV := VkImageView[3]
	extrV[1] = fbTextures[0].View()
	extrV[2] = fbTextures[1].View()
	extrV[0] = depthTexture.View()
	fbC := new VkFramebufferCreateInfo() ; $temp
	fbC.renderPass = gRenderPassModel
	fbC.attachmentCount = 3
	fbC.pAttachments = extrV[0]&
	fbC.width = nowW
	fbC.height = nowH
	fbC.layers = 1

	vkFuncs.vkCreateFramebuffer(vkLogCard,fbC,null,gModelFramebuffer&)

	twoDesc := new VkDescriptorSetLayoutBinding[2] ; $temp
	twoDesc[0].binding = 0
	twoDesc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	twoDesc[0].descriptorCount = 1
	twoDesc[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
	twoDesc[0].pImmutableSamplers = null
	twoDesc[1].binding = 1
	twoDesc[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	twoDesc[1].descriptorCount = 1
	twoDesc[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
	twoDesc[1].pImmutableSamplers = null

	set2Desc := new VkDescriptorSetLayoutCreateInfo() ; $temp
	set2Desc.bindingCount = 2
	set2Desc.pBindings = twoDesc->{void^}

	vkFuncs.vkCreateDescriptorSetLayout(vkLogCard,set2Desc,null,gLightObjectLayout&)

	layouts := new void^[2] ; $temp
	layouts[0] = gLightObjectLayout

	//pcrC := new VkPushConstantRange ; $temp
	//pcrC.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
	//pcrC.offset = 0
	//pcrC.size = 2*4*4
	ppC := new VkPipelineLayoutCreateInfo() ; $temp
	ppC.pushConstantRangeCount = 0
	ppC.pPushConstantRanges = null
	ppC.setLayoutCount = 1
	ppC.pSetLayouts = layouts->{void^}

	vkFuncs.vkCreatePipelineLayout(vkLogCard,ppC,null,gLightLayout&)

	gLightShader = vGetShader("Defer") 

	poolSize := u32[2] 
	poolSize[0] = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	poolSize[1] = 2

	nPool := new VkDescriptorPoolCreateInfo() ; $temp
	nPool.maxSets = 1
	nPool.poolSizeCount = 1
	nPool.pPoolSizes = poolSize&->{void^}

	vkFuncs.vkCreateDescriptorPool(vkLogCard,nPool,null,gGBufferLayoutSets&)

	newSetCR := new VkDescriptorSetAllocateInfo() ; $temp
	newSetCR.descriptorPool = gGBufferLayoutSets
	newSetCR.descriptorSetCount = 1
	newSetCR.pSetLayouts = gLightObjectLayout&

	vkFuncs.vkAllocateDescriptorSets(vkLogCard,newSetCR,gGBufferTextureSet&)

	imgI := new VkDescriptorImageInfo[2] ; $temp
	imgI[0].sampler = gSamplerNearest
	imgI[0].imageView = fbTextures[0].View()
	imgI[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	imgI[1].sampler = gSamplerNearest
	imgI[1].imageView = fbTextures[1].View()
	imgI[1].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL

	wrT := new VkWriteDescriptorSet() ; $temp
	wrT.descriptorCount = 2
	wrT.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	wrT.pImageInfo = imgI->{void^}
	wrT.dstSet = gGBufferTextureSet

	vkFuncs.vkUpdateDescriptorSets(vkLogCard,1,wrT,0,null)
}
nowImg := int
StartDraw := !() -> bool
{
	vkFuncs.vkResetFences(vkLogCard,1,vkFence&)
	resAq := vkFuncs.vkAcquireNextImageKHR(vkLogCard,vkSwapchain,not_b 0U,null,vkFence,nowImg&)

	if resAq ==  VK_ERROR_OUT_OF_DATE_KHR 
	{
		return false
	}

	vkFuncs.vkWaitForFences(vkLogCard,1,vkFence&,1,10000000)

	mainCmd.Reset()
	mainCmd.Start()


	rpC := new VkRenderPassBeginInfo() ; $temp

	clrValues := new float[13] ; $temp

	clrValues[0] = 1.0f
	clrValues[1] = 1.0f
	clrValues[2] = 1.0f
	clrValues[3] = 1.0f

	clrValues[4] = 1.0f
	clrValues[5] = 0.0f
	clrValues[6] = 0.0f
	clrValues[7] = 1.0f

	clrValues[8] = 0.0f
	clrValues[9] = 1.0f
	clrValues[10] = 0.0f
	clrValues[11] = 0.0f

	rpC.renderPass = gRenderPassModel
	rpC.framebuffer = gModelFramebuffer
	rpC.renderArea.extent.width = gVulkanWindowW //surfAb.currentExtent.width
	rpC.renderArea.extent.height = gVulkanWindowH //surfAb.currentExtent.height
	rpC.clearValueCount = 3
	rpC.pClearValues = clrValues->{void^}
	
	vkFuncs.vkCmdBeginRenderPass(mainCmd.Get(),rpC,VK_SUBPASS_CONTENTS_INLINE)
	return true
}
StopDraw := !() -> void
{

	vkFuncs.vkCmdEndRenderPass(mainCmd.Get())
		
	imgBarCPre := new VkImageMemoryBarrier[2] ; $temp


	rpC := new VkRenderPassBeginInfo() ; $temp

	clrValues := float[3]

	clrValues[0] = 0.0f
	clrValues[1] = 0.5f
	clrValues[2] = 1.0f

	rpC.renderPass = gRenderPassLight
	rpC.framebuffer = vkFramebuffers[nowImg]
	rpC.renderArea.extent.width = gVulkanWindowW //surfAb.currentExtent.width
	rpC.renderArea.extent.height = gVulkanWindowH //surfAb.currentExtent.height
	rpC.clearValueCount = 1
	rpC.pClearValues = clrValues[0]&->{void^}
	
	vkFuncs.vkCmdBeginRenderPass(mainCmd.Get(),rpC,VK_SUBPASS_CONTENTS_INLINE)

	gLightShader.ApplyShaderToQueue(mainCmd.Get())
	sts := VkDescriptorSet[2]
	sts[0] = gGBufferTextureSet
	vkFuncs.vkCmdBindDescriptorSets(mainCmd.Get(),VK_PIPELINE_BIND_POINT_GRAPHICS,gLightLayout,0,1,sts[0]&,0,null)
	vkFuncs.vkCmdDraw(mainCmd.Get(),3,1,0,0)

	//imgBarC := new VkImageMemoryBarrier() ; $temp
	//imgBarC.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT
	//imgBarC.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT//VK_ACCESS_COLOR_ATTACHMENT_READ_BIT or_b VK_ACCESS_MEMORY_READ_BIT
	//imgBarC.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED
	//imgBarC.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	//imgBarC.srcQueueFamilyIndex = 0
	//imgBarC.dstQueueFamilyIndex = 0
	//imgBarC.image = vkImages[nowImg]
	//imgBarC.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
	//imgBarC.subresourceRange.baseMipLevel = 0
	//imgBarC.subresourceRange.levelCount = 1
	//imgBarC.subresourceRange.baseArrayLayer = 0
	//imgBarC.subresourceRange.layerCount = 1

	vkFuncs.vkCmdEndRenderPass(mainCmd.Get())
	//vkFuncs.vkCmdPipelineBarrier(mainCmd.Get(),VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,0,0,null,0,null,1,imgBarC)
	mainCmd.Stop()

	mainCmd.Submit()
	vkFuncs.vkQueueWaitIdle(vkQueue)

	res := VkResult
	pI := new VkPresentInfoKHR() ; $temp
	pI.pNext = null
	pI.waitSemaphoreCount = 0
	pI.pWaitSemaphores = null
	pI.swapchainCount = 1
	pI.pSwapchains = vkSwapchain&->{void^}
	pI.pImageIndices = nowImg&->{void^}
	pI.pResults = res&

	vkFuncs.vkQueuePresentKHR(vkQueue,pI)
	vkFuncs.vkQueueWaitIdle(vkQueue)
}
