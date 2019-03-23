
vkLoadAddr := PFN_vkGetInstanceProcAddr
vkEnumerateInstanceExtensionProperties := PFN_vkEnumerateInstanceExtensionProperties
vkEnumerateInstanceLayerProperties := PFN_vkEnumerateInstanceLayerProperties
vkCreateInstance := PFN_vkCreateInstance

vkDllHandle := void^

vkFuncs := VkFuncsHolder
vkInstance := void^

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
	
	toUseLayers := Queue.{string}() ; $temp $uniq 
	if wantedLayers.Contain(lays[^].layerName)
		toUseLayers.Push(it.layerName)

	toUseExts := Queue.{string}() ; $temp $uniq
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
	for i :VkFuncsHolderCount
	{
		itr[i] = vkLoadAddr(vkInstance,VkFuncsHolderStrs[i])
	}

	debPrint := new VkDebugReportCallbackCreateInfoEXT ; $temp
	debPrint.sType = 1000011000
	debPrint.pfnCallback = VkDebugCallback

	a := vkFuncs.vkEnumeratePhysicalDevices//(vkInstance,null,null)
	a(vkInstance,null,null)
	//vkFuncs.vkCreateDebugReportCallbackEXT//(vkInstance,null,null)
	//vkFuncs.vkCreateDevice

	
	return 0
}

VkDebugCallback := !(int flags,int bojType,u64 object,u64 location,int msgCode,char^ prefix,char^ msg,void^ usrData)
{
	printf("VkError <%s>\n",msg)
}

DestroyVulkan := !() -> void
{
	//if vkInstance != null
	//	vkDestroyInstance(vkInstance)
	dlclose(vkDllHandle)
}
