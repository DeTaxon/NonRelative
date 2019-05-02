
zipFileHeader := packed_class
{
	signature := u32
	versNeededToExtract := u16
	flags := u16
	compressionMethod := u16
	lastModifTime := u16
	lastModifDate := u16
	crc32 := u32
	compressedSize := u32
	realSize := u32
	fileNameLen := u16
	extraFieldsLen := u16
	//fileName
	//extraFields
}
zipCentralDirectory := packed_class
{
	signature := u32
	madeWithVersion := u16
	versionNeedToExtract := u16
	flags := u16
	compressionMethod := u16
	lastModifTime := u16
	lastModifDate := u16
	crc32 := u32
	compressedSize := u32
	realSize := u32
	fileNameLen := u16
	extraFieldsLen := u16
	commentLen := u16
	fileStartsAtDisk := u16
	internalFileAttrs := u16
	externalFileAttrs := u32
	offsetToFileHeader := u32
	// file name
	// extra fields
	// comments
}
zipEndOfDirectory := packed_class
{
	signature := u32
	diskNumber := u16
	centrDirStartDisk := u16
	numberOfCentralDirectoryHere := u16
	totalNumberOfCentralDirectory := u16
	sizeOfCentralDirectoryBytes := u32
	offsetToStartOfCD := u32
	commentLength := u16
	//comments[len]
}

vZipEntry := class
{
	offset := int
	realSize := u32
	zipSize := u32
	objName := StringSpan
	comprType := int
	subFolders := List.{vZipEntry}

	"this" := !() -> void {}

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
	fileName := StringSpan

	zipRoot := vZipEntry

	"this" := !() -> void
	{
		zipRoot.objName = StringSpan("/")
	}

	AnalizeFile := !(char^ fileToLoad) -> bool
	{
		filesInUse++
		if filesInUse == 1
			asMapped."this"(fileToLoad)

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
				for itS : itmIter.subFolders
				{
					itmIter = itS&
					found = true
					break
				}
				if not found 
				{
					itmIter.subFolders.Emplace()
					nI := ref itmIter.subFolders.Back()
					nI.objName = StringSpan(itm.Str()) ; $pool

					if  j == fldrs.Size() - 1
					{
						nI.realSize = cdTable.realSize
						nI.zipSize = cdTable.compressedSize
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

vZipProxy := class
{
	ptrToZip := vZipObject^
	itemNR := int
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
	subFolders := List.{vRepoFolder^}
	subFiles := List.{vRepoFile^}
}

vRepoFile := class extend vRepoObject
{
	ptrToRepo := vRepo^
	ptrToProxy := vZipProxy^
	mFile := MappedFile

	this := !() -> void
	{
		ptrToProxy = null
	}
	Map := !() -> void^
	{
		if ptrToProxy != null
		{
			//TODO: zip support
			return null
		}

		mFile.open(fileName)

		if mFile.IsInvalid()
			return null
		return mFile.point

	}
	Unmap := !() -> void
	{
		if ptrToProxy != null
		{
			//TODO: zip support
		}
		mFile.Close()
	}
	GetSize := !() -> size_t
	{
		if ptrToProxy != null
		{
			//TODO: zip support
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
				fldPath := Path(iterFolder.GetPath())

				for subF : fldPath
				{
					if subF.IsFolder()
					{
						newStr := subF.Name().Str() ; $pool
						newObj := new vRepoFolder ; $pool
						newObj.objName = StringSpan(newStr)
						newObj.upFolder = iterFolder
						iterFolder.subFolders << newObj
					}else{
						if subF.Get()[-4..0] == ".zip"
						{
							printf("hoh %s\n",subF.Get())
							tt := new vZipObject ; $pool
							tt.AnalizeFile(subF.Get())
							tt.zipRoot.Print(0)
							return null
						}else{
							newStr := subF.Name().Str() ; $pool
							newObj := new vRepoFile ; $pool
							newObj.upFolder = iterFolder
							newObj.objName = StringSpan(newStr)
							iterFolder.subFiles << newObj
						}
					}
				}
			}
			
			found := false
			printf("ho ho %s & %s\n",iterFolder.subFolders[^].objName.Str(),cheks.Str())
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
