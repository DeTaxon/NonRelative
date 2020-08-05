
gFsMap := AVLMap.{uvFsEvent^,vRepoFolder^}

gFsPreTest := List.{vRepoFile^} ; $keep
gFsTextures := RBMap.{vRepoFile^,vTexture^}
gFsModelsModel := AVLMap.{vRepoFile^,vModel^}

gHotloadInit := !() -> void
{
	if not gHotload return void

	htl := new List.{vRepoFolder^} ; $temp


	htl.Push(gRepo.rootFolder)
	while htl.Size() != 0
	{
		str := htl.Pop()
		gRepo.ExamineFolder(str)

		if (not str.IsVirtual()) and (str.objName[0] != '.' or str.objName == "./")
		{
			mntr := str.GetPath()
			newEv := gUV.DirectoryMonitor(str.GetPath(),(x,name) ==>{

				itF := gRepo.GetFile(name,gFsMap[x])

				if itF != null
				{
					gFsPreTest.Push(itF)
				}
			})
			gFsMap[newEv] = str

			for it : str.subFolders
			{
				htl.Push(it)
			}
		}
	}

}

gHotloadCheck := !() -> void
{	
	if not gHotload return void

	while gFsPreTest.Size() != 0
	{
		onTest := gFsPreTest.Pop()

		fndInText := gFsTextures.TryFind(onTest)
		if fndInText != null
		{
			printf("reloading texture %s\n",onTest.GetPath())
			fndInText^.Reload(onTest)
		}

		fndInModelsModel := gFsModelsModel.TryFind(onTest)
		if fndInModelsModel != null
		{
			printf("reloading model.model %s\n",onTest.GetPath())
			fndInModelsModel^.ReloadModel(onTest)
		}

		//fndInModelsScript := gFsModelsScript.TryFind(onTest)
		//if fndInModelsScript != null
		//{
		//	printf("reloading model.script %s\n",onTest.GetPath())
		//	fndInModelsScript^.ReloadScript(onTest)
		//}

	}
}

gHotloadStop := !() -> void
{
	for it,ind : gFsMap
	{
		ind.Stop()
	}
}
