#import "ZipFS.cp"
#import "ObjectInfo.cp"
#import "Shader.cp"

VoidCore := class
{
	itRepo := vRepo

	itShaders := AVLMap.{char^,vShader}
	itModels := AVLMap.{char^,vModel}
	itProps := List.{vProp}

	Init := !() -> void
	{
		itRepo.Init("./")
	}
	GetModel := !(char^ sName) -> vModel^
	{
		if itModels.Contain(sName)
			return itModels[sName]&
		flName := char^
		stB := "Models/"sbt
		stB << sName
		stB << ".inf"

		asF := itRepo.GetFile(stB)

		if asF == null
			return null

		heh := asF.Map()
		defer asF.Unmap()

		cc := ParseInfo(heh,asF.Size())

		itMd := ref itModels[StrCopy(sName)]
		reqShader := vShader^()
		switch cc.SubList[^].Name
		{
			case "model"
			switch it.SubList.Size()
			{
				case 1
					switch it.SubList[0].Name
					{
						case "file"
							cdP := ref it.SubList[0]
							itF := asF.GetFile(cdP.ValueStr)
							if itF == null 
								return null
							itMd.LoadFile(itF.Map(),itF.Size())
							itF.Unmap()
							
						case void
							return null
					}
				case void
					return null
			}
			case "shader"
			if it.IsValue()
			{
				reqShader = GetShader(it.ValueStr)
			}else{
				//TODO
			}
			case "texture"
		}
		itMd.ReqShader = reqShader

		return itMd&
	}
	AddProp := !(char^ modelName) -> vProp^
	{
		itProps.Emplace()
		newPr := ref itProps.Back()
		newPr.modelPtr = GetModel(modelName)
		newPr.modelShader = newPr.modelPtr.ReqShader
		newPr.modelPos.ang = quantf(0.0f,1.0f,0.0f,0.0f)
		newPr.modelPos.pos = vec4f(0.0f,0.0f,0.0f,1.0f)
		return newPr&
	}
	Draw := !() -> void
	{
		for itProps
		{
			it.modelShader.ApplyShaderToQueue(mainCmd.Get())
			it.AddToCmdBuffer(mainCmd.Get())
		}
	}
	GetShader := !(char^ sName) -> Shader^
	{
		if itShaders.Contain(sName)
			return itShaders[sName]&
		flName := char^
		stB := "Shaders/"sbt
		stB << sName
		stB << ".inf"
		flName = stB.Str() ; $temp

		fl := itRepo.GetFile(flName)

		if fl == null
			return null

		heh := fl.Map()

		if heh == null
			return null
		defer fl.Unmap()

		cc := ParseInfo(fl.Map(),fl.Size())
	
		fndSh := false
		vertName := StringSpan()
		fragName := StringSpan()
		for shIn : cc.SubList
		{
			if shIn.Name == "shader"
			{
				switch shIn.SubList[^].Name
				{
					case "vertex"
						vertName = it.ValueStr
					case "fragment"
						fragName = it.ValueStr
					case void 
						return null
				}
				fndSh = true
			}
		}
		vertFile := vRepoFile^()
		fragFile := vRepoFile^()

		if vertName != ""{
			vertFile = fl.GetFile(vertName)
			if vertFile == null
				return null
		}
		if fragName != ""{
			fragFile = fl.GetFile(fragName)
			if fragFile == null
				return null
		}
		itSh := ref itShaders[StrCopy(sName)]

		itSh.LoadShader(vertFile.Map(),vertFile.Size(),fragFile.Map(),fragFile.Size())
		vertFile.Unmap()
		fragFile.Unmap()


		return itSh&
	}
}
