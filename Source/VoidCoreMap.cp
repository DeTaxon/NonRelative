
gMapLinks := List.{vMapLink}

vAddMapLink := !(char^ map1,char^ map2, centf diffPos) -> vMapLink^
{
	itMap1 := vGetMap(map1)
	itMap2 := vGetMap(map2)
	newLink := new vMapLink //TODO: wtf? why not gMapLinks
	newLink.linkFrom = itMap1
	newLink.linkTo = itMap2
	newLink.relativePos = diffPos
	newLink.renderCost = 1

	linksToMap[itMap2].Push(newLink)
	linksFromMap[itMap1].Push(newLink)

	return newLink
}

vGetMap := !(char^ mapName) -> vMap^
{
	if itMaps.Contain(mapName)
		return itMaps[mapName]&

	//pVoidMP.Push()
	//defer pVoidMP.Pop()

	flName := char^
	stB := "Maps/"sbt << mapName << ".inf"

	asF := gRepo.GetFile(stB)

	if asF == null 
	{
		asF = gRepo.GetFile("Maps/"sbt + mapName + "/" + mapName + ".inf")
		if asF == null
			return null
	}

	mapFile := asF.Map()
	defer asF.Unmap()

	cc := ParseInfoFile(mapFile,asF.Size()) ; $temp

	itCpyName := StrCopy(mapName)
	newMap := ref itMaps[itCpyName]
	newMap.mapName = itCpyName

	for itPreList : cc.SubList
	{
		if itPreList.Name == "script"
		{
			scFile := asF.GetFile(itPreList.ValueStr)
			scObj := ScriptCompile(scFile)
			ScriptRun(scObj,newMap&)
		}
		if itPreList.Name == "prop"
		{
			for md : itPreList.SubList
			{
				if md.isValue
				{
					//TODO?
				}else{
					//TODO: not anon
					newMod := vGenModel("anon",md,asF)
					if newMod == null throw new Exception("Can not load map  , incorrect model file")

					newProp := vAddProp(newMod,newMap&)
				}
			}
		}
	}
	return newMap&
}
