#import "VoidCore.cp"
#import "VulkanCore.cp"
#import "PictureSpecs.cp"

vTexture := class
{
	memObj := vMemObj
	itImg := VkImage
	itImgView := VkImageView
	itW,itH := int

	CreateObject := !(int w, int h) -> int
	{
		itW = w
		itH = h
		newImg := new VkImageCreateInfo() ; $temp
		newImg.extent.width = w
		newImg.extent.height = h
		newImg.extent.depth = 1
		newImg.format = VK_FORMAT_R8G8B8A8_UNORM
		newImg.imageType = VK_IMAGE_TYPE_2D
		newImg.samples = VK_SAMPLE_COUNT_1_BIT
		newImg.tiling = VK_IMAGE_TILING_OPTIMAL
		newImg.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT or_b VK_IMAGE_USAGE_SAMPLED_BIT
		newImg.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		newImg.mipLevels = 1
		newImg.arrayLayers = 1
		//newImg.queueFamilyIndexCount = 0
		newImg.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED

		memInfp := VkMemoryRequirements
		vkFuncs.vkCreateImage(vkLogCard,newImg,null,itImg&)
		vkFuncs.vkGetImageMemoryRequirements(vkLogCard,itImg,memInfp&)

		memObj.CreateObject(memInfp.size,memInfp.memoryTypeBits,null)

		vkFuncs.vkBindImageMemory(vkLogCard,itImg,memObj.Get(),0)

		vi := new VkImageViewCreateInfo() ; $temp
		vi.image = itImg
		vi.viewType = VK_IMAGE_VIEW_TYPE_2D
		vi.format = VK_FORMAT_R8G8B8A8_UNORM
		//vi.components = VK_COM
		vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
		vi.subresourceRange.levelCount = 1
		vi.subresourceRange.layerCount = 1

		vkFuncs.vkCreateImageView(vkLogCard,vi,null,itImgView&)

		return memInfp.memoryTypeBits
	}
	CreateTexture := !(vRepoFile^ itFile) -> void
	{	

		ext := itFile.objName[-3..0]
	

		if ext == "bmp"
		{
		mp := itFile.Map()->{u8^}
		defer itFile.Unmap()

		bmpH := mp->{BMPMainHeader^}
		imgH := mp[BMPMainHeader->TypeSize]&->{BMPImgHeaderOld^}

		//printf("bmp size %i\n",bmpH.fileSize)
		//printf("header size %i , w %i , h %i, bpx %i\n",imgH.itSize,imgH.itW,imgH.itH,imgH.bitsPerPixel)
		offToData := mp[BMPMainHeader->TypeSize + 40]&
		itW = imgH.itW&->{int^}^
		itH = imgH.itH&->{int^}^

		memTyp := CreateObject(itW,itH)
		ptrToSet := gStageMem.Map()->{u8^}
		if imgH.bitsPerPixel == 24
		{
			for i : itW*itH
			{
				c := i*4
				ptrToSet[c] = offToData[2]
				ptrToSet[c + 1] = offToData[1]
				ptrToSet[c + 2] = offToData[0]
				ptrToSet[c + 3] = 255
				offToData = offToData[3]&
			}
		}else{
			for i : itW*itH
			{
				c := i*4
				ptrToSet[c] = offToData[2]
				ptrToSet[c + 1] = offToData[1]
				ptrToSet[c + 2] = offToData[0]
				ptrToSet[c + 3] = offToData[3]
				offToData = offToData[4]&
			}
		}
		gStageMem.Unmap()
		vStageCpyToImage(itImg,itW,itH)
		}

	}
	LoadNotFound := !() -> void
	{
		ptrToSet := gStageMem.Map()->{u8^}
		for i : itW
		for j : itH
		{
			c := (j*itW + i)*4
			switch (i + j) % 4
			{
			case 0
				ptrToSet[c    ] = 255
				ptrToSet[c + 1] = 0
				ptrToSet[c + 2] = 50
				ptrToSet[c + 3] = 0
			case 1
				ptrToSet[c    ] = 0
				ptrToSet[c + 1] = 0
				ptrToSet[c + 2] = 0
				ptrToSet[c + 3] = 255
			case 2
				ptrToSet[c    ] = 0
				ptrToSet[c + 1] = 50
				ptrToSet[c + 2] = 255
				ptrToSet[c + 3] = 255
			case 3
				ptrToSet[c    ] = 0
				ptrToSet[c + 1] = 255
				ptrToSet[c + 2] = 0
				ptrToSet[c + 3] = 255
			}
		}
		gStageMem.Unmap()

		vStageCpyToImage(itImg,itW,itH)
	}
}
