#import "main.cp"
#import "CmdBuffer.cp"


vkLoadAddr := PFN_vkGetInstanceProcAddr
vkEnumerateInstanceExtensionProperties := PFN_vkEnumerateInstanceExtensionProperties
vkEnumerateInstanceLayerProperties := PFN_vkEnumerateInstanceLayerProperties
vkCreateInstance := PFN_vkCreateInstance

vkDllHandle := void^

vkFuncs := VkFuncsHolder
vkInstance := void^
vkDebugObj := VkDebugReportCallbackEXT
vkPhysCard := VkPhysicalDevice
vkLogCard := VkDevice
vkSurface := VkSurfaceKHR
vkQueue := VkQueue
vkSwapchain := VkSwapchainKHR
vkImages := VkImage[]
vkRenderPass := VkRenderPass
vkImageViews := VkImageView[]
vkFramebuffers := VkFramebuffer[]
vkCmdPool := VkCommandPool
//vkCmdBufs := VkCommandBuffer[]
vkFence := VkFence
vkLayout := VkPipelineLayout
vkGraphPipe := VkPipeline

mainCmd := CmdBuffer

InitVulkan := !() -> bool
{
	vkDllHandle = dlopen("libvulkan.so.1",2)

	vkLoadAddr = dlsym(vkDllHandle,"vkGetInstanceProcAddr")

	if vkLoadAddr == null{
		printf("cant get function ProcAddr\n")
		return 0
	}

	newFunc := void^
	newFunc = vkLoadAddr(null,"vkEnumerateInstanceExtensionProperties")
	if newFunc == null{ printf("cant get func props\n")	return 0	}
	vkEnumerateInstanceExtensionProperties = newFunc->{PFN_vkEnumerateInstanceExtensionProperties}

	newFunc = vkLoadAddr(null,"vkEnumerateInstanceLayerProperties")
	if newFunc == null{ printf("cant get func props\n")	return 0	}
	vkEnumerateInstanceLayerProperties = newFunc->{PFN_vkEnumerateInstanceLayerProperties}

	newFunc = vkLoadAddr(null,"vkCreateInstance")
	if newFunc == null{ printf("cant get func props\n")	return 0	}
	vkCreateInstance = newFunc->{PFN_vkCreateInstance}

	count := s32

	vkEnumerateInstanceLayerProperties(count&,null)
	lays := new VkLayerProperties[count] ; $temp
	vkEnumerateInstanceLayerProperties(count&,lays)

	printf("%i : %s\n",i,lays[^i].layerName)

	vkEnumerateInstanceExtensionProperties(null,count&,null)
	exts := new VkExtensionProperties[count] ; $temp
	vkEnumerateInstanceExtensionProperties(null,count&,exts)

	printf("%i : %s\n",i,exts[^i].extensionName)

	wantedLayers := AVLSet.{string}() ; $temp
	wantedLayers << "VK_LAYER_LUNARG_standard_validation"

	wantedExtensions := AVLSet.{string}() ; $temp
	wantedExtensions << "VK_EXT_debug_report"
	wantedExtensions << "VK_KHR_wayland_surface"
	wantedExtensions << "VK_KHR_xcb_surface"
	wantedExtensions << "VK_KHR_xlib_surface"
	wantedExtensions << "VK_KHR_surface"
	//wantedExtensions << "VK_KHR_get_physical_device_properties2"
	
	toUseLayers := List.{string}() ; $temp $uniq 
	if wantedLayers.Contain(lays[^].layerName)
		toUseLayers.Push(it.layerName)

	toUseExts := List.{string}() ; $temp $uniq
	if wantedExtensions.Contain(exts[^].extensionName)
		toUseExts << it.extensionName

	printf("using %i %s\n",i,toUseLayers[^i])
	printf("using %i %s\n",i,toUseExts[^i])

	appInfo := new VkApplicationInfo() ; $temp
	appInfo.pApplicationName = "no one care"
	appInfo.applicationVersion = 1
	appInfo.pEngineName = "void engine"
	appInfo.engineVersion = 1
	appInfo.apiVersion = (1 << 22) + (0 << 11) + 21


	instCr := new VkInstanceCreateInfo() ; $temp
	instCr.pApplicationInfo = appInfo

	instCr.enabledLayerCount = toUseLayers.Size()
	instCr.ppEnabledLayerNames = toUseLayers.ToArray() ; $temp
	instCr.enabledExtensionCount = toUseExts.Size()
	instCr.ppEnabledExtensionNames = toUseExts.ToArray() ; $temp
	instRes := vkCreateInstance(instCr,null,vkInstance&)

	if instRes != 0
		return instRes

	itr := vkFuncs&->{void^^}
	for i : VkFuncsHolderCount
	{
		itr[i] = vkLoadAddr(vkInstance,VkFuncsHolderStrs[i])

		if itr[i] == null
		{
			printf("cant load object %s\n",VkFuncsHolderStrs[i])

		}
	}

	debPrint := new VkDebugReportCallbackCreateInfoEXT ; $temp
	debPrint.sType = 1000011000
	debPrint.pfnCallback = VkDebugCallback->{void^}
	debPrint.flags = 2 or_b 4 or_b 8
	vkFuncs.vkCreateDebugReportCallbackEXT(vkInstance,debPrint,null,vkDebugObj&)

	
	devCount := u32
	vkFuncs.vkEnumeratePhysicalDevices(vkInstance,devCount&,null)
	if devCount == 0 {
		printf("no physical devices\n")
		return 0
	}
	devs := new VkPhysicalDevice[devCount->{s64}->{s32}] ; $temp
	vkFuncs.vkEnumeratePhysicalDevices(vkInstance,devCount&,devs)

	for it, i : devs
	{
		printf("device id %i\n",i)

		devProp := new VkPhysicalDeviceProperties ; $temp
		vkFuncs.vkGetPhysicalDeviceProperties(it,devProp)
			
		printf("-name %s\n",devProp.deviceName)
		printf("-type %i\n",devProp.deviceType)

		propsCount := u32
		vkFuncs.vkGetPhysicalDeviceQueueFamilyProperties(it,propsCount&,null)
		prps := new VkQueueFamilyProperties[propsCount->{s64}->{s32}] ; $temp
		vkFuncs.vkGetPhysicalDeviceQueueFamilyProperties(it,propsCount&,prps)

		for it,i : prps
		{
			printf("-quque family %i\n",i)
			printf("-- queueCount %i\n",it.queueCount)
			printf("-- queueFlags %X\n",it.queueFlags)
		}
	}
	vkPhysCard = devs[0]

	//devExtsCount := 0
	//vkFuncs.vkEnumerateDeviceExtensionProperties(vkPhysCard,null,devExtsCount&,null)
	//devExts := new VkExtensionProperties[devExtsCount] ; $temp
	//vkFuncs.vkEnumerateDeviceExtensionProperties(vkPhysCard,null,devExts&,devExts)

	//printf("dev extensions\n")
	//for 5
	//{
	//	printf("- %s\n",devExts[it]&)
	//}

	physExts := List.{string}() ; $temp
	physExts << "VK_KHR_swapchain"

	queueCreateInf := new VkDeviceQueueCreateInfo() ; $temp
	queueCreateInf.queueFamilyIndex = 0
	zeroPrior := float
	zeroPrior = 0.0
	queueCreateInf.queueCount = 1
	queueCreateInf.pQueuePriorities = zeroPrior&
	
	zeroFeature := new VkPhysicalDeviceFeatures ; $temp

	logDevice := new VkDeviceCreateInfo() ; $temp

	logDevice.queueCreateInfoCount = 1
	logDevice.pQueueCreateInfos = queueCreateInf
	logDevice.enabledLayerCount = toUseLayers.Size()
	logDevice.ppEnabledLayerNames = toUseLayers.ToArray() ; $temp
	logDevice.enabledExtensionCount = physExts.Size() //toUseExts.Size()
	logDevice.ppEnabledExtensionNames = physExts.ToArray()->{void^} ; $temp
		//toUseExts.ToArray()->{void^} ; $temp
	logDevice.pEnabledFeatures = zeroFeature

	
	vkFuncs.vkCreateDevice(vkPhysCard,logDevice,null,vkLogCard&)

	
	resC := glfwCreateWindowSurface(vkInstance,glfwWindow,null,vkSurface&)

	isPres := 0
	vkFuncs.vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysCard,0,vkSurface,isPres&)


	surfAb := new VkSurfaceCapabilitiesKHR ; $temp
	vkFuncs.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysCard,vkSurface,surfAb)

	printf("surface settings\n")
	printf("img count min = %i , max = %i\n",surfAb.minImageCount,surfAb.maxImageCount)

	formsCount := 0
	vkFuncs.vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysCard,vkSurface,formsCount&,null)
	formts := new VkSurfaceFormatKHR[formsCount] ; $temp
	vkFuncs.vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysCard,vkSurface,formsCount&,formts)

	printf("formats\n")
	for formts
	{
		printf("-format %i\n",it)
	}

	presMods := 0
	vkFuncs.vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysCard,vkSurface,presMods&,null)
	pMods := new VkPresentModeKHR[presMods] ; $temp
	vkFuncs.vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysCard,vkSurface,presMods&,pMods)

	printf("supported mods\n")
	for pMods
	{
		switch it
		{
			case VK_PRESENT_MODE_IMMEDIATE_KHR
				printf("- IMMEDIATE\n")
			case VK_PRESENT_MODE_MAILBOX_KHR
				printf("- MAILBOX\n")
			case VK_PRESENT_MODE_FIFO_KHR
				printf("- FIFO\n")
			case VK_PRESENT_MODE_FIFO_RELAXED_KHR
				printf("- FIFO_RELAXED\n")
			case else
				printf("- unknown %i\n",it)
		}
	}

	crtSwap := new VkSwapchainCreateInfoKHR ; $temp
	crtSwap.sType = 1000001000
	crtSwap.surface = vkSurface
	crtSwap.minImageCount = 3
	crtSwap.imageFormat = formts[0].format
	crtSwap.imageColorSpace = formts[0].colorSpace
	crtSwap.imageExtent.width = startW
	crtSwap.imageExtent.height = startH
	crtSwap.imageArrayLayers = 1
	crtSwap.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
	crtSwap.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE
	crtSwap.queueFamilyIndexCount = 0
	crtSwap.pQueueFamilyIndices = null
	crtSwap.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
	crtSwap.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
	crtSwap.presentMode = VK_PRESENT_MODE_MAILBOX_KHR
	crtSwap.clipped = 0
	crtSwap.oldSwapchain = null
	
	vkFuncs.vkCreateSwapchainKHR(vkLogCard,crtSwap,null,vkSwapchain&)

	imgCount := 0
	vkFuncs.vkGetSwapchainImagesKHR(vkLogCard,vkSwapchain,imgCount&,null)
	vkImages = new VkImage[imgCount]
	vkFuncs.vkGetSwapchainImagesKHR(vkLogCard,vkSwapchain,imgCount&,vkImages)

	attmDesc := new VkAttachmentDescription ; $temp
	attmDesc.format = formts[0].format
	attmDesc.samples = VK_SAMPLE_COUNT_1_BIT
	attmDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR
	attmDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE
	attmDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE
	attmDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
	attmDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	attmDesc.finalLayout  = 1000001002//1000111000//VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL

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

	vkFuncs.vkCreateRenderPass(vkLogCard,rpC,null,vkRenderPass&)

	vkImageViews = new VkImageView[vkImages->len]
	vkFramebuffers = new VkFramebuffer[vkImages->len]
	for it,i : vkImages
	{
		imgViewC := new VkImageViewCreateInfo() ; $temp
		imgViewC.image = it
		imgViewC.viewType = VK_IMAGE_VIEW_TYPE_2D
		imgViewC.format = formts[0].format
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
		fbC.renderPass = vkRenderPass
		fbC.attachmentCount = 1
		fbC.pAttachments = vkImageViews[i]&
		fbC.width = surfAb.currentExtent.width
		fbC.height = surfAb.currentExtent.height
		fbC.layers = 1

		vkFuncs.vkCreateFramebuffer(vkLogCard,fbC,null,vkFramebuffers[i]&)
	}

	vkFuncs.vkGetDeviceQueue(vkLogCard,0,0,vkQueue&)

	cmdPoolC := new VkCommandPoolCreateInfo() ; $temp
	cmdPoolC.queueFamilyIndex = 0
	cmdPoolC.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	vkFuncs.vkCreateCommandPool(vkLogCard,cmdPoolC,null,vkCmdPool&)

	vkFuncs.vkResetCommandPool(vkLogCard,vkCmdPool,VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT)

	cmdBufC := new VkCommandBufferAllocateInfo() ; $temp

	crtFence := new VkFenceCreateInfo() ; $temp
	vkFuncs.vkCreateFence(vkLogCard,crtFence,null,vkFence&)

	
	memReq := new VkPhysicalDeviceMemoryProperties ; $temp
	vkFuncs.vkGetPhysicalDeviceMemoryProperties(vkPhysCard,memReq)

	for i : (memReq.memoryTypeCount)&->{int^}^
	{
		printf("mem info %i\n",i)
		if (memReq.memoryTypes[i]&->{int^}^ 
			and_b VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 
				printf("VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT\n")
		if (memReq.memoryTypes[i]&->{int^}^ 
			and_b VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0 
				printf("VK_MEMORY_PROPERTY_HOST_COHERENT_BIT\n")
		if (memReq.memoryTypes[i]&->{int^}^ 
			and_b VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0 
				printf("VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT\n")
	}

	pcrC := new VkPushConstantRange ; $temp
	pcrC.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
	pcrC.offset = 0
	pcrC.size = 2*4*4
	ppC := new VkPipelineLayoutCreateInfo() ; $temp
	ppC.pushConstantRangeCount = 1
	ppC.pPushConstantRanges = pcrC->{void^}

	vkFuncs.vkCreatePipelineLayout(vkLogCard,ppC,null,vkLayout&)
	mainCmd.CreateBuffer()
	

	printf("finished\n")

	return 0
}
nowImg := int
StartDraw := !() -> void
{
	vkFuncs.vkResetFences(vkLogCard,1,vkFence&)
	vkFuncs.vkAcquireNextImageKHR(vkLogCard,vkSwapchain,10000000,null,vkFence,nowImg&)
	vkFuncs.vkWaitForFences(vkLogCard,1,vkFence&,1,10000000)

	mainCmd.Reset()
	mainCmd.Start()

	rpC := new VkRenderPassBeginInfo() ; $temp

	clrValues := new float[4] ; $temp

	clrValues[0] = 1.0f
	clrValues[1] = 0.5f
	clrValues[2] = 0.0f
	clrValues[3] = 1.0f

	rpC.renderPass = vkRenderPass
	rpC.framebuffer = vkFramebuffers[nowImg]
	rpC.renderArea.extent.width = 700 //surfAb.currentExtent.width
	rpC.renderArea.extent.height = 700 //surfAb.currentExtent.height
	rpC.clearValueCount = 1
	rpC.pClearValues = clrValues->{void^}
	
	vkFuncs.vkCmdBeginRenderPass(mainCmd.Get(),rpC,VK_SUBPASS_CONTENTS_INLINE)
	
}
StopDraw := !() -> void
{

	vkFuncs.vkCmdEndRenderPass(mainCmd.Get())
		
	imgBarC := new VkImageMemoryBarrier() ; $temp
	imgBarC.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT
	imgBarC.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT//VK_ACCESS_COLOR_ATTACHMENT_READ_BIT or_b VK_ACCESS_MEMORY_READ_BIT
	imgBarC.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED
	imgBarC.newLayout = 1000001002 //VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL//1000111000 //VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	imgBarC.srcQueueFamilyIndex = 0
	imgBarC.dstQueueFamilyIndex = 0
	imgBarC.image = vkImages[nowImg]
	imgBarC.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
	imgBarC.subresourceRange.baseMipLevel = 0
	imgBarC.subresourceRange.levelCount = 1
	imgBarC.subresourceRange.baseArrayLayer = 0
	imgBarC.subresourceRange.layerCount = 1

	vkFuncs.vkCmdPipelineBarrier(mainCmd.Get(),VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,0,0,null,0,null,1,imgBarC)
	mainCmd.Stop()

	//waitMsk := VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
	//submInf := new VkSubmitInfo() ; $temp
	//submInf.waitSemaphoreCount = 0
	//submInf.pWaitSemaphores = null
	//submInf.pWaitDstStageMask = waitMsk&->{void^}
	//submInf.commandBufferCount = 1
  	//submInf.pCommandBuffers = vkCmdBufs[nowImg]&;
  	//submInf.signalSemaphoreCount = 0;
  	//submInf.pSignalSemaphores = null
	//vkFuncs.vkQueueSubmit(vkQueue, 1, submInf, null)
	mainCmd.Submit()
	vkFuncs.vkQueueWaitIdle(vkQueue)

	res := VkResult
	pI := new VkPresentInfoKHR ; $temp
	pI.sType = 1000001001//VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	pI.pNext = null
	pI.waitSemaphoreCount = 0
	pI.pWaitSemaphores = null
	pI.swapchainCount = 1
	pI.pSwapchains = vkSwapchain&->{void^}
	pI.pImageIndices = nowImg&->{void^}
	pI.pResults = res&

	vkFuncs.vkQueuePresentKHR(vkQueue,pI)
}
VkDebugCallback := !(int flags,int bojType,u64 object,u64 location,int msgCode,char^ prefix,char^ msg,void^ usrData) -> int
{
	printf("VkError <%s>\n",msg)
}

DestroyVulkan := !() -> void
{
	//if vkInstance != null
	//	vkDestroyInstance(vkInstance)
	dlclose(vkDllHandle)
}
