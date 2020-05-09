
slModules := AVLMap.{vRepoFile^,vShaderModule}

vGetShader := !(char^ sName) -> Shader^
{
	if itShaders.Contain(sName)
		return itShaders[sName]&

	//pVoidMP.Push()
	//defer pVoidMP.Pop()

	flName := char^
	stB := "Shaders/"sbt
	stB << sName
	stB << ".inf"
	flName = stB.Str() ; $temp

	fl := gRepo.GetFile(flName)

	if fl == null
		return null

	heh := fl.Map()

	if heh == null
		return null
	defer fl.Unmap()

	cc := ParseInfoFile(fl.Map(),fl.Size()) ; $temp

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

	cacheDir := ""sbt + fl.GetUpFolder().GetPath() + "/ShaderCache/" <-
	if not Path(cacheDir).IsExist()
		TCreateDir(cacheDir)

	vertMod := slCompileShaderFile(fl.GetFile(vertName),"vert")
	fragMod := slCompileShaderFile(fl.GetFile(fragName),"frag")

	itSh := ref itShaders[StrCopy(sName)]

	itSh.LoadShader(vertMod,fragMod)
	return itSh&
}
slCompileShaderFile := !(vRepoFile^ itF,char^ typ) -> vShaderModule^
{
	if slModules.Contain(itF)
		return slModules[itF]&

	
	cName := "ShaderCache/"sbt + itF.GetName() <-
	cFile := itF.GetFile(cName)

	cpFile := cFile == null

	if cFile != null and gHotload
	{
		cachPath := Path(cFile.GetPath())
		orgPath := Path(itF.GetPath())
		if orgPath.IsExist() and cachPath.IsExist() and cachPath.GetTime() < orgPath.GetTime()
		{
			cpFile = true
		}
	}

	if not cpFile
	{
		itMod := ref slModules[itF]
		on_exception slModules.Remove(itF)

		ptrF := cFile.Map()
		defer cFile.Unmap()
		itMod.LoadShaderModule(ptrF,cFile.Size(),typ)

		return itMod&
	}

	ptrToF := itF.Map()
	defer itF.Unmap()

	resFileName := ""sbt + slInputFile + "." + typ <-
	outFileName := ""sbt + slOutputFile + "." + typ <-

	keyw := new char^[2] ; $temp
	keyw[0] = "#VERT_INPUT_DATA_VARS"
	keyw[1] = "#VERT_INPUT_DATA_INIT"
	replacedSt := ReplaceKeywords(StringSpan(ptrToF->{char^},itF.Size()),keyw,keyw->len, (a1,a2) ==>{
		switch a1
		{
			case "#VERT_INPUT_DATA_VARS"
				return "layout(location = 0) in vec3 i_Position;layout(location = 1) in vec3 i_Normal;layout(location = 2) in vec2 i_uv;\n"
			case "#VERT_INPUT_DATA_INIT"
				return "\n"
		}
	})

	fWrt := file(resFileName,"w")
	//TODO: defer close

	fWrt.Write(replacedSt,StrSize(replacedSt))
	fWrt.Close()
	
	exe := ""sbt + slCompilerPath  + " " + resFileName + " -o " + outFileName  + " " <-
	exeRes := system(exe)

	if exeRes != 0
		throw new Exception("failed parsing shader")

	oFl := Path(outFileName)
	resSize := oFl.Size()
	fRd := file(outFileName,"r")
	defer fRd.Close()

	preRes := new u8[resSize->{int}] ; $temp
	fRd.Read(preRes->{void^},resSize->{int})

	itMod := ref slModules[itF]
	on_exception slModules.Remove(itF)

	itMod.LoadShaderModule(preRes->{void^},resSize->{int},typ)

	if slKeepResult
	{
		cName2 := ""sbt + itF.GetUpFolder().GetPath() + "/ShaderCache/" + itF.GetName() <-
		outCFile := file(cName2,"w")
		defer outCFile.Close()
		outCFile.Write(preRes->{void^},resSize)
	}

	return itMod&
}

