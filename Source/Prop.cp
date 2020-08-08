vProp := class extend ScriptBox //TODO:  
{	
	modelPos := centf
	calculatedPos := centf

	modelPtr := vModel^
	modelShader := vShader^
	modelTextureSet := VkDescriptorSet

	physObj := PhysCommon^

	AddToCmdBuffer := !(VkCommandBuffer cmdB, centf relativePos) -> void
	{
		//if gHotload
		//	vSetTexture(modelTextureSet,modelPtr.ReqTexture,gSamplerNearest)

		globalPos := relativePos<*>modelPos
		gCam.ApplyCamera(globalPos,calculatedPos)
		sts := VkDescriptorSet[2]
		sts[0] = vkPerspSet
		sts[1] = modelTextureSet
		vkFuncs.vkCmdBindDescriptorSets(cmdB,VK_PIPELINE_BIND_POINT_GRAPHICS,vkLayout,0,2,sts[0]&,0,null)
		vkFuncs.vkCmdPushConstants(cmdB,vkLayout,VK_SHADER_STAGE_VERTEX_BIT,0,centf->TypeSize,calculatedPos&)
		modelPtr.AddToCmdBuffer(cmdB)
	}
}
