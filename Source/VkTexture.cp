webpLib := Library^

webpGetFeatures := !(void^,u64,void^,int)^ -> int
webpGetDataRGB := !(void^,u64,void^,u64,int)^ -> void^
webpGetDataRGBA := !(void^,u64,void^,u64,int)^ -> void^

InitWebP := !() -> bool
{
	if webpLib != null return true

	itLib := new Library("libwebp.so","libwebp.so.6","libwebp-7.dll")
	
	webpGetFeatures = itLib.Get("WebPGetFeaturesInternal")
	webpGetDataRGB = itLib.Get("WebPDecodeRGBInto")
	webpGetDataRGBA = itLib.Get("WebPDecodeRGBAInto")
	
	webpLib = itLib
	return true
}


vTexture := class
{
	memObj := vMemObj
	itImg := VkImage
	itImgView := VkImageView
	itW,itH := int

	refCounter := int
	
	Img := !() -> VkImage { return itImg }
	View := !() -> VkImageView { return itImgView }
	CreateObject := !(int w, int h) -> int
	{
		CreateObject(w,h,(x,y) ==> {})
	}
	CreateObject := !(int w, int h,!(VkImageCreateInfo^,VkImageViewCreateInfo^)&-> void createSettings) -> int
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

		vi := new VkImageViewCreateInfo() ; $temp
		vi.image = itImg
		vi.viewType = VK_IMAGE_VIEW_TYPE_2D
		vi.format = VK_FORMAT_R8G8B8A8_UNORM
		//vi.components = VK_COM
		vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
		vi.subresourceRange.levelCount = 1
		vi.subresourceRange.layerCount = 1

		createSettings(newImg,vi)
		vi.format = newImg.format 

		memInfp := VkMemoryRequirements
		vkFuncs.vkCreateImage(vkLogCard,newImg,null,itImg&)

		if itImg == null
			throw new Exception("Could not create texture")

		vkFuncs.vkGetImageMemoryRequirements(vkLogCard,itImg,memInfp&)

		memObj.CreateObject(memInfp.size,memInfp.memoryTypeBits,null)

		vkFuncs.vkBindImageMemory(vkLogCard,itImg,memObj.Get(),0)

		vi.image = itImg

		vkFuncs.vkCreateImageView(vkLogCard,vi,null,itImgView&)

		return memInfp.memoryTypeBits
	}
	Recolor := !(u8^ arr,int pixels, int strip) -> void
	{
		return void
		for i : pixels
		{
			//Y := 0.299*arr[0] + 0.587*arr[1] + 0.114*arr[2]
			//U := (arr[2] - Y)*0.565
			//V := (arr[0] - Y)*0.713
			Y := (0.299*arr[0] + 0.587*arr[1] + 0.114*arr[2])
			Cb := (128 + (-0.168736*arr[0] + (- 0.331264*arr[1]) + 0.5*arr[2]))
			Cr := (128 + (0.5*arr[0] + (- 0.418688*arr[1]) + (- 0.081312*arr[2])))

			// R = Y + 1.403*V
			// G = Y - 0.344*U - 0.714*V
			// B = Y + 1.770*U

			arr[0] = Y->{int}->{u8}
			arr[1] = Cb->{int}->{u8}
			arr[2] = Cr->{int}->{u8}

			arr = arr[strip]&
		}
	}
	toPwr2 := !(int x) -> int
	{
		i := 1
		while x > i
			i *= 2
		return i
	}
	CreateTexture := !(vRepoFile^ itFile) -> void
	{	

		ext := itFile.objName[-3..0]

		if ext in !["tga","jpg","png"]
		{
			imgType := IL_TGA
			switch ext
			{
				case "jpg" imgType = IL_JPG
				case "png" imgType = IL_PNG
			}

			ptr := itFile.Map()
			defer itFile.Unmap()
		
			ilImg := int
			ilGenImages(1,ilImg&)
			ilBindImage(ilImg)
			ilLoadL(imgType,ptr,itFile.Size())
			w := ilGetInteger(IL_IMAGE_WIDTH)
			h := ilGetInteger(IL_IMAGE_HEIGHT)
			frmt := ilGetInteger(IL_IMAGE_FORMAT)

			containAlpha := frmt in ![IL_RGBA,IL_BGRA]
			if not vkRGB8Support
				containAlpha = true

			itW = toPwr2(w)
			itH = toPwr2(h)

			ptrToSet := gStageMem.Map()->{u8^}
		//ilCopyPixels := !(int xOff,int yOff,int zOff,int w, int h, int d,int format,int fType,void^ destin)^ -> void
			ilCopyPixels(0,0,0,itW,itH,1,containAlpha ?: IL_RGBA : IL_RGB, IL_UNSIGNED_SHORT,ptrToSet)
			gStageMem.Unmap()
			vStageCpyToImage(itImg,itW,itH)

		}else if ext == "bmp"
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
			Recolor(ptrToSet,itW*itH,4)
			gStageMem.Unmap()
			vStageCpyToImage(itImg,itW,itH)
		}else 
		if itFile.objName[-5..0] == ".webp"
		{
			InitWebP()

			mp := itFile.Map()->{u8^}
			defer itFile.Unmap()

			webpInf := int[10] // width,height,has_alpha, has_animation,type, 5 x for feature

			webpGetFeatures(mp,itFile.Size(),webpInf&,0x0208)

			has_alpha := webpInf[2] != 0
			has_animation := webpInf[3] != 0
			if not vkRGB8Support
				has_alpha = true

			if has_animation
			{
				//FEATURE:
			}else{
				memTyp := CreateObject(webpInf[0],webpInf[1],(x,y) ==> {
					x.format = VK_FORMAT_R8G8B8_UNORM
					if has_alpha x.format = VK_FORMAT_R8G8B8A8_UNORM
				})
				ptrToSet := gStageMem.Map()->{u8^}

				if has_alpha
				{
					webpGetDataRGBA(mp,itFile.Size(),ptrToSet,itW*itH*4,itW*4)
					Recolor(ptrToSet,itW*itH,4)
				}else{
					webpGetDataRGB(mp,itFile.Size(),ptrToSet,itW*itH*3,itW*3)
					Recolor(ptrToSet,itW*itH,3)
				}
				gStageMem.Unmap()
				vStageCpyToImage(itImg,itW,itH)
			}
		}

	}
	Reload := !(vRepoFile^ itFile) -> void
	{
		Destroy()
		CreateTexture(itFile)
	}
	Destroy := !() -> void
	{
		vkFuncs.vkDestroyImageView(vkLogCard,itImgView,null)
		vkFuncs.vkDestroyImage(vkLogCard,itImg,null)
		memObj.DestroyVK()
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
		Recolor(ptrToSet,itW*itH,4)
		gStageMem.Unmap()

		vStageCpyToImage(itImg,itW,itH)
	}
}
