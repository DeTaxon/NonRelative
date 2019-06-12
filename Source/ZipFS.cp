#import "ZipSpec.cp"
#import "DeflateEncoder.cp"



z_stream := class
{
	next_in := void^
	avail_in := u32
	total_in := u64

	next_out := void^
	avail_out := u32
	total_out := u64

	msg := char^
	state := void^

	zalloc := void^
	zfree := void^
	opaque := void^

	data_type := int
	adler := u64
	reserved := u64
}

//prvtInitStream := !(z_stream^,int,char^,u64)^ -> int
//prvtInflate := !(z_stream^,int)^ -> int
//prvtInflateEnd := !(z_stream^)^ -> int
//
//prvtZipInited := bool
//prvtInitZip := !() -> void
//{
//	if prvtZipInited
//		return void
//	prvtZipInited = true
//
//	dllHandle := OpenLib("libz.so",gMallocTemporary)
//
//	if dllHandle == 0
//		dllHandle = OpenLib("zlib1.dll",gMallocTemporary)
//	if dllHandle == 0
//	{
//		return void
//	}
//
//	prvtInitStream = LoadFuncLib(dllHandle,"inflateInit2_")
//	prvtInflate = LoadFuncLib(dllHandle,"inflate")
//	prvtInflateEnd = LoadFuncLib(dllHandle,"inflateEnd")
//}

inflateInit2_ := !(z_stream^ a,int b,char^ c,u64 d) -> int declare
inflate := !(z_stream^ a,int b) -> int declare
inflateEnd := !(z_stream^ a) -> int declare

prvtDeflateInflate := !(void^ inD,u64 inpSize, void^ outD, u64 outSize) -> int declare

vZipEntry := class
{
	ptrToObj := vZipObject^
	offset := int
	realSize := u32
	zipSize := u32
	objName := StringSpan
	comprType := int
	subFolders := List.{vZipEntry}
	compressedPointer := void^

	"this" := !() -> void {}

	Size := !() -> size_t
	{
		return realSize
	}
	Map := !() -> void^
	{
		ptrToObj.AddUser()
		if comprType == 8
		{
			//if not prvtZipInited
			//	prvtInitZip()
			compressedPointer = malloc(realSize)

			resPtr := ptrToObj.asMapped.Get()[offset]&
	
			sStream := z_stream
			memset(sStream&,0,z_stream->TypeSize)
			sStream.avail_in = zipSize
			sStream.avail_out = realSize
			sStream.next_in = resPtr
			sStream.next_out = compressedPointer
			prvtDeflateInflate(resPtr,zipSize,compressedPointer,realSize)

			//prvtInitStream(sStream&,-15,"1.2.11",z_stream->TypeSize)
			//prvtInflate(sStream&,0)
			//prvtInflateEnd(sStream&)
			//aa := inflateInit2_(sStream&,-15,"1.2.11",z_stream->TypeSize)
			//bb := inflate(sStream&,0)
			//cc := inflateEnd(sStream&)
			//printf("test %i %i %i\n",aa,bb,cc)
			//printf("hoh %i\n",z_stream->TypeSize)

			return compressedPointer
		}
		resPtr := ptrToObj.asMapped.Get()[offset]&

		return resPtr
	}
	Unmap := !() -> void
	{
		if comprType == 8
		{
			free(compressedPointer)
		}
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
						nI.comprType = cdTable.compressionMethod

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
	fileSize := u64

	this := !() -> void
	{
		ptrToProxy = null
	}
	GetFile := !(char^ fileName) -> vRepoFile^
	{
		return ptrToRepo.GetFile(fileName,upFolder)
	}
	Map := !() -> u8^
	{
		if ptrToZip != null
		{
			return ptrToZip.Map()->{u8^}
		}

		mFile.Open(GetPath())

		if mFile.IsInvalid()
			return null->{u8^}
		return mFile.point->{u8^}

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
		return fileSize
	}
	IsValid := !() -> bool
	{
	}
}

vRepo := class
{
	rootFolder := vRepoFolder^
	repoMP := AllocOnlyMP.{4096,true}^
	ignZip := AVLSet.{u64}
	Init := !(char^ pathName)-> void
	{
		ignZip."this"()
		repoMP = new AllocOnlyMP.{4096,true}
		repoMP.Push()
		defer repoMP.Pop()
		rootFolder = new vRepoFolder ; $pool
		startName := StrCopy(pathName) ; $pool 
		rootFolder.objName = StringSpan(startName)
	}

	AddZipRoot := !(char^ fileName) -> bool
	{
		repoMP.Push()
		defer repoMP.Pop()

		itId := u64
		if prvtGetFileInfo(fileName,null,null,itId&,null,gMallocTemporary) == 0
			return false

		ignZip << itId ; $pool

		itObj := new vZipObject ; $pool
		itObj.AnalizeFile(fileName)
		rootFolder.subZipFolders << itObj.zipRoot&; $pool

	}
	GetFile := !(char^ fileName)-> vRepoFile^
	{
		return GetFile(fileName,rootFolder)
	}

	GetFile := !(char^ fileName,vRepoFolder^ rrF) -> vRepoFile^
	{
		repoMP.Push()
		defer repoMP.Pop()
		
		itms := DivideStr(fileName,'/') ; $temp
		iterFolder := rrF

		for cheks,i : itms
		{
			if i == 0 and cheks == "." continue

			if not iterFolder.examined
			{
				iterFolder.examined = true

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
							itmId := u64
							flSz := u64
							prvtGetFileInfo(subF.Get(),null,flSz&,itmId&,null,gMallocTemporary)

							//if itmId in ignZip
							if ignZip.Contain(itmId)
							{
								continue
							}
							if subF.Get()[-4..0] == ".zip"
							{
								zips << StrCopy(subF.Get()) ; $temp
							}else{
								

									
								newStr := subF.Name().Str() ; $pool
								newObj := new vRepoFile ; $pool
								newObj.upFolder = iterFolder
								newObj.objName = StringSpan(newStr)
								newObj.ptrToRepo = this&
								newObj.fileSize = flSz
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
							newObj.ptrToRepo = this&
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

			if cheks == ".."
			{
				iterFolder = iterFolder.upFolder
				if iterFolder == null
					return null
				continue
			}
			return null

		}
		return null

	}
}
