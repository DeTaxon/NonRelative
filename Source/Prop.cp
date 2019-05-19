#import "Model.cp"
#import "Shader.cp"
#Import "main.cp"

vProp := class 
{	
	modelPos := centf
	calculatedPos := centf

	modelPtr := vModel^
	modelShader := vShader^

	testPos := float
	AddToCmdBuffer := !(VkCommandBuffer cmdB) -> void
	{
		gCam.ApplyCamera(modelPos,calculatedPos)
		vkFuncs.vkCmdPushConstants(cmdB,vkLayout,VK_SHADER_STAGE_VERTEX_BIT,0,centf->TypeSize,calculatedPos&)
		modelPtr.AddToCmdBuffer(cmdB)
	}
}
