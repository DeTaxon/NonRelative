
vGenTexture := !(InfoNode^ itObj,vRepoFile^ itFile) -> vTexture^
{
	
	try
	{
		preRes := gBadTexture
		switch itObj.SubList[^].Name
		{
			case "file"
				nFile := itFile.GetFile(it.ValueStr)
				if nFile == null
					return gBadTexture
				preRes = new vTexture
				preRes.CreateTexture(nFile)
				gFsTextures[nFile] = preRes
		}
		return preRes
	}catch(IException^ ex)
	{
		return gBadTexture
	}
	assert(false)
}
//vGetTexture := !(char^ fileName) -> vTexture^
//{
//	temp := new vTexture
//	temp.CreateObject(64,64)
//	temp.LoadNotFound()
//	return temp
//}
