#import "Model.cp"

Prop := class 
{	
	posItem := centf
	modelPtr := Model^

	testPos := float
	AddToCmdBuffer := !(VkCommandBuffer cmdB) -> void
	{
		newPos := new float[8] ; $temp
		newPos[^] = testPos
		vkFuncs.vkCmdPushConstants(cmdB,vkLayout,VK_SHADER_STAGE_VERTEX_BIT,0,centf->TypeSize,posItem&)
		modelPtr.AddToCmdBuffer(cmdB)
	}
}
