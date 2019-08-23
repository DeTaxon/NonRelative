
intrRepoBrowser := vRepo^

LibDBObject := class
{
	hndl := void^
	fileFd := int
	loadedFuncs := AVLMap.{char^,void^}

	Get := !(char^ name) -> void^
	{
		inMap := loadedFuncs.TryFind(name)
		if inMap != null
			return inMap^

		newFunc := LoadFuncLib(hndl,name)
		if newFunc  == null
			throw new Exception(0,"Can not load function <"sbt + name + ">")
		loadedFuncs[StrCopy(name)] = newFunc
		
		return newFunc
	}
}
LibDatabaseType := class
{
	loadedLibs := AVLMap.{char^,LibDBObject}

	SetRepo := !(vRepo^ toS) -> void
	{
		intrRepoBrowser = toS
	}
	GetLib := !(char^ name) -> LibDBObject^
	{
		inMap := loadedLibs.TryFind(name)
		if inMap != null
			return inMap

		newObj := LibDBObject

		hndl := OpenLib(name)

		if hndl != null
		{
			nameCpy := StrCopy(name)
			inMap2 := ref loadedLibs[nameCpy]
			inMap2.hndl = hndl
			return inMap2&
		}
		
		if intrRepoBrowser != null
		{
			inFS := intrRepoBrowser.GetFile(name)
			if inFS != null
			{
				if inFS.IsVirtual()
				{
					pt := inFS.Map()

					tmpNamePre := "/tmp/"sbt + name + "XXXXXX"
					tmpName := tmpNamePre.Str() ; $temp

					fd := mkstemp(tmpName)
					write(fd,pt->{void^},inFS.Size())

					hndl = OpenLib(tmpName)
					if hndl == null
						return null

					nameCpy := StrCopy(name)
					inMap2 := ref loadedLibs[nameCpy]
					inMap2.hndl = hndl
					inMap2.fileFd = fd
					return inMap2&
				}else{
					toLoad := inFS.GetPath()
					toLoad2 := Path(toLoad)
					toLoad = toLoad2.FullPath().Get()

					hndl = OpenLib(toLoad)


					if hndl == null
						return null
					inMap3 := ref loadedLibs[StrCopy(name)]
					inMap3.hndl = hndl
					return inMap3&
				}
			}
		}

		return null
	}
}
LibDB := LibDatabaseType

Library := class 
{
	hndl := LibDBObject^
	this := !(args...) -> void
	{
		Open(args...)
	}
	Open := !(char^ name,args...) -> void
	{
		hndl = LibDB.GetLib(name)
		if hndl == null
			Open(args...)
	}
	Open := !(char^ name) -> void
	{
		hndl = LibDB.GetLib(name)
		if hndl == null
			throw new Exception(0,"can not open library")
	}
	Get := !(char^ funcName) -> void^
	{
		return hndl.Get(funcName)
	}
}


