vAddProp := !(char^ modelName) -> vProp^
{
	itMod := vGetModel(modelName)
	if itMod == null return null
	return vAddProp(itMod,null)
}
vAddProp := !(vModel^ itModel,vMap^ mapObj) -> vProp^
{
	//pVoidMP.Push()
	//defer pVoidMP.Pop()
	DebugLog("Created Prop Model:%p Map:%p\n",itModel,mapObj)

	newProp := vProp^()
	if mapObj != null
	{

		newProp = new vProp
		mapObj.mapProps.Push(newProp)
	}else{
		itProps.Emplace()
		newProp = itProps.Back()&
	}
	newProp.modelPos.ang = quantfAt(-1.0f,1.0f,0.0f,0.0f)
	newProp.modelPos.pos = vec4f(0.0f,0.0f,0.0f,1.0f)
	newProp.modelPtr = itModel
	newProp.modelShader = newProp.modelPtr.ReqShader

	if itModel.physType != null
	{
		if itModel.physType == "hmap"
		{
			hMap := new PhysHeightMap()
			newProp.physObj = hMap->{PhysCommon^}
			hMap.Dots = itModel.physExtraData->{PhysHeightMap^}.Dots
			hMap.Triangles = itModel.physExtraData->{PhysHeightMap^}.Triangles
			hMap.Spheres = itModel.physExtraData->{PhysHeightMap^}.Spheres
		}
	}

	newSetCR := new VkDescriptorSetAllocateInfo() ; $temp
	newSetCR.descriptorPool = gObjectLayoutSets
	newSetCR.descriptorSetCount = 1
	newSetCR.pSetLayouts = vkPerObjectLayout&

	vkFuncs.vkAllocateDescriptorSets(vkLogCard,newSetCR,newProp.modelTextureSet&)
	vSetTexture(newProp.modelTextureSet,newProp.modelPtr.ReqTexture,gSamplerNearest)

	if itModel.scriptUnit != null
	{
		ScriptRun(itModel.scriptUnit,newProp)
	}
	return newProp
}
