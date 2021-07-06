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
vkCmdPool := VkCommandPool
//vkCmdBufs := VkCommandBuffer[]
vkFence := VkFence
vkLayout := VkPipelineLayout
vkGraphPipe := VkPipeline

vkPerspLayout := VkDescriptorSetLayout
vkPerObjectLayout := VkDescriptorSetLayout
vkDescPool := VkDescriptorPool

vkPerspSet := VkDescriptorSet

mainCmd := CmdBuffer


vkRGB8Support := bool
vkHalfFloatSupport := bool

vkCpuMemId := int
vkGpuMemId := int

SwapImageFormat := int
SwapImageColorSpace := int
SwapImageMode := int

gVulkanWindowW := int
gVulkanWindowH := int

gUserModes := List.{int}

gDeviceMem := List.{int}
gHostMem := List.{int}

InitVulkan := !() -> bool
{
	vkGpuMemId = -1
	vkCpuMemId = -1
	vkDllHandle = OpenLib("libvulkan.so")

	if vkDllHandle == null
	{
		vkDllHandle = OpenLib("vulkan-1.dll")
		if vkDllHandle == null
		{
			printf("fail at loading dll\n")
			return false
		}
	}

	vkLoadAddr = LoadFuncLib(vkDllHandle,"vkGetInstanceProcAddr")

	if vkLoadAddr == null{
		printf("cant get function ProcAddr\n")
		return false
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
	wantedLayers << "VK_LAYER_MESA_overlay"
	wantedLayers << "VK_LAYER_KHRONOS_validation"
	wantedLayers << "VK_LAYER_LUNARG_vktrace"

	wantedExtensions := AVLSet.{string}() ; $temp
	wantedExtensions << "VK_EXT_debug_report"
	wantedExtensions << "VK_KHR_wayland_surface"
	wantedExtensions << "VK_KHR_xcb_surface"
	wantedExtensions << "VK_KHR_xlib_surface"
	wantedExtensions << "VK_KHR_win32_surface"
	wantedExtensions << "VK_KHR_surface"
	wantedExtensions << "VK_KHR_get_physical_device_properties2" 
	wantedExtensions << "VK_KHR_storage_buffer_storage_class"

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

	if vkFuncs.vkCreateDebugReportCallbackEXT != null
	{
		debPrint := new VkDebugReportCallbackCreateInfoEXT() ; $temp
		//debPrint.sType = 1000011000
		debPrint.pfnCallback = VkDebugCallback->{void^}
		debPrint.flags = 2 or_b 4 or_b 8
		vkFuncs.vkCreateDebugReportCallbackEXT(vkInstance,debPrint,null,vkDebugObj&)
	}

	
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
	devSupports := AVLSet.{char^}() ; $temp

	devExtsCount := 0
	vkFuncs.vkEnumerateDeviceExtensionProperties(vkPhysCard,null,devExtsCount&,null)
	if devExtsCount != 0
	{
		devExts := new VkExtensionProperties[devExtsCount] ; $temp
		vkFuncs.vkEnumerateDeviceExtensionProperties(vkPhysCard,null,devExtsCount&,devExts)

		printf("dev extensions\n")
		for devExtsCount
		{
			devSupports.Insert(devExts[it].extensionName)
			printf("- %s\n",devExts[it].extensionName&)
		}
	}else{
		printf("no device extensions found\n")
	}

	
	physExts := List.{string}() ; $temp
	physExts << "VK_KHR_swapchain"

	halfFloatExt := "VK_KHR_16bit_storage"
	if devSupports.Contain(halfFloatExt)
	{
		physExts << halfFloatExt 
		//physExts << "VK_KHR_get_physical_device_properties2" 
		physExts << "VK_KHR_storage_buffer_storage_class"
		vkHalfFloatSupport = true
	}

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
		printf("-format %i\n",it&->{int^}^)
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
	reqMod := VK_PRESENT_MODE_IMMEDIATE_KHR
	for gUserModes
	{
		if it in pMods
			reqMod = it
	}
	memReq := new VkPhysicalDeviceMemoryProperties ; $temp
	vkFuncs.vkGetPhysicalDeviceMemoryProperties(vkPhysCard,memReq)

	for i : (memReq.memoryTypeCount)&->{int^}^
	{
		printf("mem info %i %X\n",i,memReq.memoryTypes[i]&->{int^}^)
		if (memReq.memoryTypes[i]&->{int^}^ 
			and_b VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0
			{
				printf("VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT\n")
				if vkCpuMemId == -1
					vkCpuMemId = i
				gHostMem << i
			}
		if (memReq.memoryTypes[i]&->{int^}^ 
			and_b VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0 
				printf("VK_MEMORY_PROPERTY_HOST_COHERENT_BIT\n")
		if (memReq.memoryTypes[i]&->{int^}^ 
			and_b VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0 
			{	
				if vkGpuMemId == -1
					vkGpuMemId = i
				//if (memReq.memoryTypes[i]&->{int^}^ and_b VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0
					gDeviceMem << i
				printf("VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT\n")
			}
	}

	//TODO: format priority
	SwapImageFormat = formts[1].format
	SwapImageColorSpace = formts[1].colorSpace
	SwapImageMode = reqMod
	//CreateSwapchain(gWindowW,gWindowH)

	vkFuncs.vkGetDeviceQueue(vkLogCard,0,0,vkQueue&)

	cmdPoolC := new VkCommandPoolCreateInfo() ; $temp
	cmdPoolC.queueFamilyIndex = 0
	cmdPoolC.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	vkFuncs.vkCreateCommandPool(vkLogCard,cmdPoolC,null,vkCmdPool&)

	vkFuncs.vkResetCommandPool(vkLogCard,vkCmdPool,VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT)

	cmdBufC := new VkCommandBufferAllocateInfo() ; $temp

	crtFence := new VkFenceCreateInfo() ; $temp
	vkFuncs.vkCreateFence(vkLogCard,crtFence,null,vkFence&)

	

	oneDesc := new VkDescriptorSetLayoutBinding ; $temp
	oneDesc.binding = 0
	oneDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
	oneDesc.descriptorCount = 1
	oneDesc.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
	oneDesc.pImmutableSamplers = null

	setDesc := new VkDescriptorSetLayoutCreateInfo() ; $temp
	setDesc.bindingCount = 1
	setDesc.pBindings = oneDesc->{void^}

	twoDesc := new VkDescriptorSetLayoutBinding ; $temp
	twoDesc.binding = 0
	twoDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	twoDesc.descriptorCount = 1
	twoDesc.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
	twoDesc.pImmutableSamplers = null

	set2Desc := new VkDescriptorSetLayoutCreateInfo() ; $temp
	set2Desc.bindingCount = 1
	set2Desc.pBindings = twoDesc->{void^}

	vkFuncs.vkCreateDescriptorSetLayout(vkLogCard,setDesc,null,vkPerspLayout&)
	vkFuncs.vkCreateDescriptorSetLayout(vkLogCard,set2Desc,null,vkPerObjectLayout&)

	layouts := new void^[2] ; $temp
	layouts[0] = vkPerspLayout
	layouts[1] = vkPerObjectLayout

	pcrC := new VkPushConstantRange ; $temp
	pcrC.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
	pcrC.offset = 0
	pcrC.size = 2*4*4
	ppC := new VkPipelineLayoutCreateInfo() ; $temp
	ppC.pushConstantRangeCount = 1
	ppC.pPushConstantRanges = pcrC->{void^}
	ppC.setLayoutCount = 2
	ppC.pSetLayouts = layouts->{void^}

	vkFuncs.vkCreatePipelineLayout(vkLogCard,ppC,null,vkLayout&)
	mainCmd.CreateBuffer()
	
	descPoolSize := new VkDescriptorPoolSize ; $temp
	descPoolSize.descriptorCount = 10
	descPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER

	test := u32[2]
	test[0] = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
	test[1] = 1 //count


	descPoolC := new VkDescriptorPoolCreateInfo() ; $temp
	descPoolC.maxSets = 1
	descPoolC.poolSizeCount = 1
	descPoolC.pPoolSizes = test[0]&->{void^}//descPoolSize

	vkFuncs.vkCreateDescriptorPool(vkLogCard,descPoolC,null,vkDescPool&)

	allDesc := new VkDescriptorSetAllocateInfo() ; $temp
	allDesc.descriptorPool = vkDescPool
	allDesc.descriptorSetCount = 1
	allDesc.pSetLayouts = vkPerspLayout&

	vkFuncs.vkAllocateDescriptorSets(vkLogCard,allDesc,vkPerspSet&)

	testTextureFormat := VkFormatProperties
	vkFuncs.vkGetPhysicalDeviceFormatProperties(vkPhysCard,VK_FORMAT_R8G8B8_UNORM,testTextureFormat&)
	vkRGB8Support = (testTextureFormat.linearTilingFeatures and_b VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) != 0
	//printf("with alpha %x %x %x\n",tst.linearTilingFeatures,tst.optimalTilingFeatures,tst.bufferFeatures)

	printf("RGB8SUpport %s\n",vkRGB8Support ?: "yes" : "no")
	printf("HalfFloat support %s\n",vkHalfFloatSupport ?: "yes" : "no")

	printf("finished\n")

	return 0
}

emitedDbg := AVLSet.{StringSpan}
VkDebugCallback := !(int flags,int bojType,u64 object,u64 location,int msgCode,char^ prefix,char^ msg,void^ usrData) -> int
{
	if emitedDbg.Contain(StringSpan(msg))
		return 0
	emitedDbg.Insert(StringSpan(StrCopy(msg)))
	printf("VkError <%s>\n",msg)
	return 0
}

DestroyVulkan := !() -> void
{
	//if vkInstance != null
	//	vkDestroyInstance(vkInstance)
	CloseLib(vkDllHandle)
}
