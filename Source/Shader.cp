#import "VulkanCore.cp"

vShader := class 
{
	itPipe := VkPipeline
	"this" := !() -> void {}

	ApplyShaderToQueue := !(VkCommandBuffer itBuf) -> void
	{
		vp := VkViewport 
		vp.x = 0.0f
		vp.y = 0.0f
		vp.width = gWindowW->{float}
		vp.height = gWindowH->{float}
		vp.minDepth = 0.0f
		vp.maxDepth = 1.0f

		sc := VkRect2D
		sc.offset.x = 0
		sc.offset.y = 0
		sc.extent.width = gWindowW
		sc.extent.height = gWindowH

		vkFuncs.vkCmdBindPipeline(itBuf,VK_PIPELINE_BIND_POINT_GRAPHICS,itPipe)
		vkFuncs.vkCmdSetViewport(itBuf,0,1,vp&)
		vkFuncs.vkCmdSetScissor(itBuf,0,1,sc&)
	}
	LoadShader := !(void^ vertPoint,int vertSize,void^ fragPoint,int fragSize) -> void
	{

		vert := VkShaderModule
		vSC := new VkShaderModuleCreateInfo() ; $temp
		vSC.codeSize = vertSize
		vSC.pCode = vertPoint
		vkFuncs.vkCreateShaderModule(vkLogCard,vSC,null,vert&)
		defer vkFuncs.vkDestroyShaderModule(vkLogCard,vert,null)

		frag := VkShaderModule
		fSC := new VkShaderModuleCreateInfo() ; $temp
		fSC.codeSize = fragSize
		fSC.pCode = fragPoint
		vkFuncs.vkCreateShaderModule(vkLogCard,fSC,null,frag&)
		defer vkFuncs.vkDestroyShaderModule(vkLogCard,frag,null)

		sStg := new VkPipelineShaderStageCreateInfo[2] ; $temp
		sStg[^]."this"()

		sStg[0].stage = VK_SHADER_STAGE_VERTEX_BIT
		sStg[0].module = vert
		sStg[0].pName = "main"->{void^}
		sStg[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT
		sStg[1].module = frag
		sStg[1].pName = "main"->{void^}


		inputsBind := new VkVertexInputBindingDescription ; $temp

		inputsBind.binding = 0
		inputsBind.stride = 8*4
		inputsBind.inputRate = VK_VERTEX_INPUT_RATE_VERTEX

		inputsStuf := new VkVertexInputAttributeDescription[2] ; $temp
		//vertex
		inputsStuf[0].location = 0
		inputsStuf[0].binding = inputsBind.binding
		inputsStuf[0].format = VK_FORMAT_R32G32B32A32_SFLOAT
		inputsStuf[0].offset = 0
		//normal
		inputsStuf[1].location = 1
		inputsStuf[1].binding = inputsBind.binding
		inputsStuf[1].format = VK_FORMAT_R32G32B32A32_SFLOAT
		inputsStuf[1].offset = 4*4
		//texture
		//inputsStuf[2].location = 2
		//inputsStuf[2].binding = inputsBind.binding
		//inputsStuf[2].format = VK_FORMAT_R32G32_SFLOAT
		//inputsStuf[2].offset = 6*3
	
		plC := new VkPipelineVertexInputStateCreateInfo() ; $temp

		plC.vertexBindingDescriptionCount = 1
		plC.pVertexBindingDescriptions = inputsBind
		plC.vertexAttributeDescriptionCount = inputsStuf->len
		plC.pVertexAttributeDescriptions = inputsStuf


		paC1 := new VkPipelineInputAssemblyStateCreateInfo() ; $temp
		paC1.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST

		vp := VkViewport 
		vp.x = 0.0f
		vp.y = 0.0f
		vp.width = 700.0f
		vp.height = 700.0f
		vp.minDepth = 0.0f
		vp.maxDepth = 1.0f

		sc := VkRect2D
		sc.offset.x = 0
		sc.offset.y = 0
		sc.extent.width = gWindowW
		sc.extent.height = gWindowH

		stts := VkDynamicState[2]
		stts[0] = VK_DYNAMIC_STATE_VIEWPORT
		stts[1] = VK_DYNAMIC_STATE_SCISSOR

		dins := new VkPipelineDynamicStateCreateInfo() ; $temp
		dins.dynamicStateCount = 2
		dins.pDynamicStates = stts[0]&

		pVSC := new VkPipelineViewportStateCreateInfo() ; $temp
		pVSC.viewportCount = 1
		pVSC.pViewports = null //vp&
		pVSC.scissorCount = 1
		pVSC.pScissors = null //sc&

		rasterC := new VkPipelineRasterizationStateCreateInfo() ; $temp
		rasterC.polygonMode = VK_POLYGON_MODE_FILL
		rasterC.cullMode = VK_CULL_MODE_BACK_BIT
		rasterC.cullMode = VK_CULL_MODE_NONE
		rasterC.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE
		rasterC.lineWidth = 1.0f
		//  VK_FALSE, VkBool32   depthClampEnable
		//  VK_FALSE, VkBool32   rasterizerDiscardEnable
		//  VK_FALSE, VkBool32   depthBiasEnable
		//  0.0f,     float      depthBiasConstantFactor
		//  0.0f,     float      depthBiasClamp
		//  0.0f,     float      depthBiasSlopeFactor

		sampleC := new VkPipelineMultisampleStateCreateInfo() ; $temp
		sampleC.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
		sampleC.minSampleShading = 1.0f
		//  VK_FALSE,   VkBool32              sampleShadingEnable
		//  nullptr,    const VkSampleMask   *pSampleMask
		//  VK_FALSE,   VkBool32              alphaToCoverageEnable
		//  VK_FALSE    VkBool32              alphaToOneEnable

		blndPre := new VkPipelineColorBlendAttachmentState ; $temp
		blndPre.srcColorBlendFactor = VK_BLEND_FACTOR_ONE
		blndPre.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO
		blndPre.colorBlendOp = VK_BLEND_OP_ADD
		blndPre.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE
		blndPre.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO
		blndPre.alphaBlendOp = VK_BLEND_OP_ADD
		blndPre.colorWriteMask = VK_COLOR_COMPONENT_R_BIT or_b VK_COLOR_COMPONENT_G_BIT or_b VK_COLOR_COMPONENT_B_BIT or_b VK_COLOR_COMPONENT_A_BIT
		//  VK_FALSE,  VkBool32   blendEnable

		blndC := new VkPipelineColorBlendStateCreateInfo() ; $temp
		blndC.logicOp = VK_LOGIC_OP_COPY
		blndC.attachmentCount = 1
		blndC.pAttachments = blndPre
		//  VK_FALSE,                     VkBool32   logicOpEnable
		//  { 0.0f, 0.0f, 0.0f, 0.0f }    float      blendConstants[4]

		pLC := new VkPipelineLayoutCreateInfo() ; $temp
		//  0,        VkPipelineLayoutCreateFlags    flags
		//  0,        uint32_t                       setLayoutCount
		//  nullptr,  const VkDescriptorSetLayout   *pSetLayouts
		//  0,        uint32_t                       pushConstantRangeCount
		//  nullptr   const VkPushConstantRange     *pPushConstantRanges

		dsC := new VkPipelineDepthStencilStateCreateInfo() ; $temp
		dsC.depthTestEnable = VK_TRUE
		dsC.depthWriteEnable = VK_TRUE
		dsC.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL
		dsC.depthBoundsTestEnable = VK_FALSE
		dsC.minDepthBounds = 0.0f
		dsC.maxDepthBounds = 0.0f
		dsC.stencilTestEnable = VK_FALSE
		dsC.back.failOp = VK_STENCIL_OP_KEEP
		dsC.back.passOp = VK_STENCIL_OP_KEEP
		dsC.back.compareOp = VK_COMPARE_OP_ALWAYS
		dsC.back.compareMask = 0
		dsC.back.reference = 0
		dsC.back.depthFailOp = VK_STENCIL_OP_KEEP
		dsC.back.writeMask = 0
		dsC.front.failOp = VK_STENCIL_OP_KEEP
		dsC.front.passOp = VK_STENCIL_OP_KEEP
		dsC.front.compareOp = VK_COMPARE_OP_ALWAYS
		dsC.front.compareMask = 0
		dsC.front.reference = 0
		dsC.front.depthFailOp = VK_STENCIL_OP_KEEP
		dsC.front.writeMask = 0


		gpC := new VkGraphicsPipelineCreateInfo() ; $temp
		gpC.stageCount = 2
		gpC.pStages = sStg->{void^}
		gpC.pVertexInputState = plC
		gpC.pInputAssemblyState = paC1
		gpC.pViewportState = pVSC
		gpC.pRasterizationState = rasterC
		gpC.pMultisampleState = sampleC
		gpC.pColorBlendState = blndC
		gpC.layout = vkLayout
		gpC.renderPass = vkRenderPass
		gpC.basePipelineIndex = -1
		gpC.pDepthStencilState = dsC
		gpC.pDynamicState = dins
		//  nullptr,        const VkPipelineTessellationStateCreateInfo   *pTessellationState
		//  nullptr,        const VkPipelineDynamicStateCreateInfo        *pDynamicState
		//  0,              uint32_t                                       subpass
		//  VK_NULL_HANDLE, VkPipeline                                     basePipelineHandle
		vkFuncs.vkCreateGraphicsPipelines(vkLogCard,null,1,gpC,null,itPipe&)
	}
}
