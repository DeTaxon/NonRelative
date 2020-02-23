vGetModel := !(char^ sName) -> vModel^
{
	if itModels.Contain(sName)
		return itModels[sName]&

	//pVoidMP.Push()
	//defer pVoidMP.Pop()

	flName := char^
	stB := "Models/"sbt
	stB << sName
	stB << ".inf"

	asF := gRepo.GetFile(stB)

	if asF == null
	{
		asF = gRepo.GetFile("Models/"sbt + sName + "/" + sName + ".inf")
		if asF == null
		{
			printf("failed to load %s\n",stB.Str()) ; $temp
			return null
		}
	}

	heh := asF.Map()
	defer asF.Unmap()

	cc := ParseInfoFile(heh,asF.Size()) ; $temp
	
	return vGenModel(sName,cc,asF)
}
vGenModel := !(char^ modelName,InfoNode^ itObj,vRepoFile^ itFile) -> vModel^
{
	itMd := ref itModels[StrCopy(modelName)]
	reqShader := vShader^()
	itMd.ReqTexture = gBadTexture

	switch itObj.SubList[^].Name
	{
		case "phys"
			if it.SubList.Size() != 0
			{
				for pars : it.SubList
				{
					if pars.Name == "type"
					{
						switch pars.ValueStr
						{
							case "hmap"
								fileName := char^()
								for opt : it.SubList
								{
									if opt.Name == "file" fileName = opt.ValueStr.Str() ; $temp
								}
								if fileName == null  throw new Exception("Incorrect phys data")
								hFile := itFile.GetFile(fileName) 
								if hFile == null throw new Exception("Incorrect phys data, can not find file")
								hMap := new PhysHeightMap()
								mdl := new RawModel ; $temp
								mdl.MapFromPLY(hFile.Map(),hFile.Size())
								hMap.CreateDots(mdl)
								itMd.physExtraData = hMap->{PhysCommon^}
								itMd.physType = "hmap"
							case void
								throw new Exception("unknown physics type")
						}
						break
					}
				}
			}
		case "model"
			switch it.SubList.Size()
			{
				case 1
					switch it.SubList[0].Name
					{
						case "file"
							cdP := ref it.SubList[0]
							itF := itFile.GetFile(cdP.ValueStr)
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
				reqShader = vGetShader(it.ValueStr)
			}else{
				//TODO
			}
		case "texture"
			if it.IsValue()
			{
				//TODO
			}else{
				itMd.ReqTexture = vGenTexture(it,itFile)
			}
		case "script"
			if it.IsValue()
			{
				itMd.scriptFile = itFile.GetFile(it.ValueStr)
				if itMd.scriptFile != null
					itMd.scriptUnit = ScriptCompile(itMd.scriptFile)
			}else{
				assert(false)
			}
	}
	itMd.ReqShader = reqShader

	return itMd&
}
