#import "ZipSpec.cp"

vZipEntry := class
{
	ptrToObj := vZipObject^
	offset := int
	realSize := u32
	zipSize := u32
	objName := StringSpan
	comprType := int
	subFolders := List.{vZipEntry}

	"this" := !() -> void {}

	Size := !() -> size_t
	{
		return realSize
	}
	Map := !() -> void^
	{
		ptrToObj.AddUser()
		return ptrToObj.asMapped.Get()[offset]&
	}
	Unmap := !() -> void
	{
		ptrToObj.DecUser()
	}

	Print := !(int de) -> void
	{
		for i : de printf("-")
		printf("%s %i %i %i\n",objName.Str(),realSize,zipSize,comprType)
		subFolders[^].Print(de+1)
	}
}

vZipObject := class
{
	filesInUse := int
	examined := bool
	asMapped := MappedFile
	upFolder := vRepoFolder^
	fileName := char^

	zipRoot := vZipEntry

	"this" := !() -> void
	{
		zipRoot.objName = StringSpan("/")
		zipRoot.ptrToObj = this&
	}
	AddUser := !() -> void
	{
		filesInUse++
		if filesInUse == 1
			asMapped."this"(fileName)
	}
	DecUser := !() -> void
	{
		filesInUse--
		if filesInUse == 0
			asMapped.Close()
	}

	AnalizeFile := !(char^ fileToLoad) -> bool
	{
		fileName = StrCopy(fileToLoad) ; $pool
		AddUser()
		defer DecUser()

		ptrToFl := asMapped.Get()
		eod := ptrToFl[asMapped.Size() - zipEndOfDirectory->TypeSize]&->{zipEndOfDirectory^}

		tableCount := eod.numberOfCentralDirectoryHere
		cdTable := ptrToFl[eod.offsetToStartOfCD]&->{zipCentralDirectory^}

		for i : tableCount
		{	
			ptrToStr := cdTable[1]&->{char^}
			newStr := StringSpan(ptrToStr,cdTable.fileNameLen)
			if newStr[-1..0] == "/"
			{
				cdTable = cdTable->{u8^}[zipCentralDirectory->TypeSize 
				+ cdTable.fileNameLen 
				+ cdTable.extraFieldsLen
				+ cdTable.commentLen
				]&->{zipCentralDirectory^}
				continue
			}
			fldrs := newStr.DivideStr("/") ; $temp
			itmIter := zipRoot&


			for itm,j : fldrs
			{	
				found := false
				if itmIter.subFolders[^].objName == itm
				{
					itmIter = it&
					found = true
					break
				}
				if not found 
				{
					itmIter.subFolders.Emplace()
					nI := ref itmIter.subFolders.Back()
					nI.ptrToObj = this&
					nI.objName = StringSpan(itm.Str()) ; $pool

					if  j == fldrs.Size() - 1
					{
						nI.realSize = cdTable.realSize
						nI.zipSize = cdTable.compressedSize

						ptTH := ptrToFl[cdTable.offsetToFileHeader]&->{zipFileHeader^}

						nI.offset = cdTable.offsetToFileHeader + zipFileHeader->TypeSize + ptTH.fileNameLen + ptTH.extraFieldsLen
					}else{
						itmIter = nI&
					}
				}
			}

			cdTable = cdTable->{u8^}[zipCentralDirectory->TypeSize 
			+ cdTable.fileNameLen 
			+ cdTable.extraFieldsLen
			+ cdTable.commentLen
			]&->{zipCentralDirectory^}
		}
	}
}

vRepoObject := class
{
	objName := StringSpan
	upFolder := vRepoFolder^

	GetPath := !() -> char^
	{
		itms := List.{vRepoObject^}() ; $temp

		iterUp := this&

		while iterUp != null
		{
			itms.PushFront(iterUp)
			iterUp = iterUp.upFolder
		}

		strB := StringBuilder.{256}()

		for it,i : itms
		{
			strB << it.objName
			if i != itms.Size() - 1
			{
				strB << "/"
			}
		}

		return strB.Str() ; $temp
	}
}

vRepoFolder := class extend vRepoObject
{
	examined := bool
	virtualFolder := bool
	subZipFolders := List.{vZipEntry^}
	subFolders := List.{vRepoFolder^}
	subFiles := List.{vRepoFile^}
}

vRepoFile := class extend vRepoObject
{
	ptrToRepo := vRepo^
	ptrToZip := vZipEntry^
	mFile := MappedFile

	this := !() -> void
	{
		ptrToProxy = null
	}
	Map := !() -> void^
	{
		if ptrToZip != null
		{
			return ptrToZip.Map()
		}

		mFile.Open(GetPath())

		if mFile.IsInvalid()
			return null
		return mFile.point

	}
	Unmap := !() -> void
	{
		if ptrToZip != null
		{
			ptrToZip.Unmap()
		}else{
			mFile.Close()
		}
	}
	Size := !() -> size_t
	{
		if ptrToZip != null
		{
			return ptrToZip.Size()
		}
		return 0
	}
	IsValid := !() -> bool
	{
	}
}

vRepo := class
{
	rootFolder := vRepoFolder^
	repoMP := AllocOnlyMP.{4096,true}^
	Init := !(char^ pathName)-> void
	{
		repoMP = new AllocOnlyMP.{4096,true}
		repoMP.Push()
		defer repoMP.Pop()
		rootFolder = new vRepoFolder ; $pool
		startName := StrCopy(pathName) ; $pool 
		rootFolder.objName = StringSpan(startName)
	}

	GetFile := !(char^ fileName) -> vRepoFile^
	{
		repoMP.Push()
		defer repoMP.Pop()
		
		itms := DivideStr(fileName,'/') ; $temp
		iterFolder := rootFolder

		for cheks,i : itms
		{
			if i == 0 and cheks == "." continue

			if not iterFolder.examined
			{	

				zips := List.{char^}() ; $temp

				if not iterFolder.virtualFolder
				{
					fldPath := Path(iterFolder.GetPath())
					for subF : fldPath
					{
						if subF.IsFolder()
						{
							newStr := subF.Name().Str() ; $pool
							newObj := new vRepoFolder ; $pool
							newObj.objName = StringSpan(newStr)
							newObj.upFolder = iterFolder
							iterFolder.subFolders << newObj ; $pool
						}else{
							if subF.Get()[-4..0] == ".zip"
							{
								zips << StrCopy(subF.Get()) ; $temp
							}else{
								newStr := subF.Name().Str() ; $pool
								newObj := new vRepoFile ; $pool
								newObj.upFolder = iterFolder
								newObj.objName = StringSpan(newStr)
								iterFolder.subFiles << newObj ; $pool
							}
						}
					}
					for z : zips
					{
						tt := new vZipObject ; $pool
						tt.AnalizeFile(z)

						itPP :=Path(z)
						itP := itPP.Name()

						rpFld := null->{vRepoFolder^}

						if iterFolder.subFolders[^].objName == itP
						{
							rpFld = it
							break
						}
						if rpFld == null
						{
							rpFld = new vRepoFolder ; $pool
							rpFld.objName = StringSpan(itP[0..-4].Str()) ; $pool
							rpFld.virtualFolder = true
							iterFolder.subFolders << rpFld ; $pool
						}
						rpFld.subZipFolders << tt.zipRoot& ; $pool
					}
				}

				for entrs : iterFolder.subZipFolders
				{
					for subItm : entrs.subFolders
					{
						found := false
						if iterFolder.subFiles[^].objName == subItm.objName
						{
							found = true
							break
						}
						if found continue

						if iterFolder.subFolders[^].objName == subItm.objName
						{
							found = true
							if subItm.realSize == 0
								it.subZipFolders << subItm& ; $pool
							break
						}
						if found continue

						if subItm.realSize == 0
						{
							rpFld := new vRepoFolder ; $pool
							rpFld.objName = subItm.objName
							rpFld.virtualFolder = true
							iterFolder.subFolders << rpFld ; $pool
							rpFld.subZipFolders << subItm&
						}else{
							newObj := new vRepoFile ; $pool
							newObj.upFolder = iterFolder
							newObj.objName = subItm.objName
							iterFolder.subFiles << newObj ; $pool
							newObj.ptrToZip = subItm&
						}
					}
				}
			}
			found := false
			if iterFolder.subFolders[^].objName == cheks
			{
				iterFolder = it
				found = true
				break
			}
			
			if found continue

			if iterFolder.subFiles[^].objName == cheks
			{
				return it
			}
			return null

		}
		return null

	}
}
