
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

	vertSize := u64()
	vertPtr := slCompileShaderFile(fl.GetFile(vertName),vertSize&,"vert")
	fragSize := u64()
	fragPtr := slCompileShaderFile(fl.GetFile(fragName),fragSize&,"frag")

	itSh := ref itShaders[StrCopy(sName)]

	itSh.LoadShader(vertPtr,vertSize,fragPtr,fragSize)
	return itSh&
}
slCompileShaderFile := !(vRepoFile^ itF,u64^ resSize,char^ typ) -> void^
{
	ptrToF := itF.Map()
	defer itF.Unmap()

	resFileName := ""sbt + slInputFile + "." + typ <-
	outFileName := ""sbt + slOutputFile + "." + typ <-

	fWrt := file(resFileName,"w")
	//TODO: defer close

	fWrt.Write(ptrToF,itF.Size())
	fWrt.Close()
	
	exe := ""sbt + slCompilerPath  + " " + resFileName + " -o " + outFileName  + " " <-
	exeRes := system(exe)

	if exeRes != 0
		throw new Exception("failed parsing shader")

	oFl := Path(outFileName)
	resSize^ = oFl.Size()
	fRd := file(outFileName,"r")
	defer fRd.Close()

	preRes := new u8[resSize^->{int}] ; $temp
	fRd.Read(preRes->{void^},resSize^->{int})

	return preRes->{void^}
}

