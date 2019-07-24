CmdBuffer := class
{
	itCmdBuf := VkCommandBuffer
	itPauseItm := VkSemaphore

	
	CreateBuffer := !() -> void
	{
		cmdBufC := new VkCommandBufferAllocateInfo() ; $temp

		cmdBufC.commandPool = vkCmdPool
		cmdBufC.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY 
		cmdBufC.commandBufferCount = 1
		
		vkFuncs.vkAllocateCommandBuffers(vkLogCard,cmdBufC,itCmdBuf&)

		semInf := new VkSemaphoreCreateInfo() ; $temp
		vkFuncs.vkCreateSemaphore(vkLogCard,semInf,null,itPauseItm&)
	}

	Reset := !() -> void
	{
		vkFuncs.vkResetCommandBuffer(itCmdBuf,VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT)
	}

	Get := !() -> VkCommandBuffer
	{
		return itCmdBuf
	}
	Start := !() -> void
	{
		inhC := new VkCommandBufferInheritanceInfo() ; $temp

		inhC.renderPass = vkRenderPass
		inhC.subpass = 0
		//TODO: WARNING, REMOVE FRAMEBUFFER
		inhC.framebuffer = vkFramebuffers[nowImg]
		inhC.occlusionQueryEnable = 0
		inhC.queryFlags = 0
		inhC.pipelineStatistics = 0

		biC := new VkCommandBufferBeginInfo() ; $temp
		biC.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
		biC.pInheritanceInfo = inhC->{void^}

		vkFuncs.vkBeginCommandBuffer(itCmdBuf,biC)
	}
	Stop := !() -> void
	{
		vkFuncs.vkEndCommandBuffer(itCmdBuf)
	}
	Submit := !() -> void
	{
		waitMsk := VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
		submInf := new VkSubmitInfo() ; $temp
		submInf.waitSemaphoreCount = 0
		submInf.pWaitSemaphores = null
		submInf.pWaitDstStageMask = waitMsk&->{void^}
		submInf.commandBufferCount = 1
  		submInf.pCommandBuffers = itCmdBuf&;
  		submInf.signalSemaphoreCount = 0
  		submInf.pSignalSemaphores = null

		vkFuncs.vkQueueSubmit(vkQueue, 1, submInf, null)
		vkFuncs.vkDeviceWaitIdle(vkLogCard)
	}
}
