#import "ZipFS.cp"
#import "ObjectInfo.cp"
#import "Shader.cp"

VoidCore := class
{
	itRepo := vRepo

	itShaders := List.{Shader}

	Init := !() -> void
	{
		itRepo.Init("./")
	}
	LoadModel := !(char^ sName) -> bool
	{
		return true
	}
	LoadShader := !(char^ sName) -> bool
	{
		flName := char^
		stB := "Shaders/"sbt
		stB << sName
		stB << ".inf"
		flName = stB.Str() ; $temp

		fl := itRepo.GetFile(flName)

		if fl == null
			return false

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
						return false
				}
				fndSh = true
			}
		}
		vertFile := vRepoFile^()
		fragFile := vRepoFile^()

		if vertName != ""{
			vertFile = fl.GetFile(vertName)
			if vertFile == null
				return false
		}
		if fragName != ""{
			fragFile = fl.GetFile(fragName)
			if fragFile == null
				return false
		}
		itShaders.Emplace()
		itSh := ref itShaders.Back()

		itSh.LoadShader(vertFile.Map(),vertFile.Size(),fragFile.Map(),fragFile.Size())


		return true
	}
}
