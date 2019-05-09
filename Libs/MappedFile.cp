#import "lib.cp"
#import "PosixFuncs.cp"

FILE_READ := 1
FILE_WRITE := 2
FILE_CREATE := 3



ArrayIterMappedFile := class
{	
	x := u64
	pFile := void^
	this := !(void^ pF) -> void
	{
		x = 0
		pFile = pF
	}
	"^" := !() -> ref u8
	{
		pP := pFile->{MappedFile^}
		return pP.point[x]
	}
	Inc := !() -> void
	{
		x += 1
	}
	IsEnd := !() -> bool
	{
		pP := pFile->{MappedFile^}
		return x >= pP.fileSize
	}
	IsInvalid := !() -> bool
	{
		pP := pFile->{MappedFile^}
		return pP.IsInvalid()
	}
}
MappedFile := class
{
	fileId := u64
	fileSize  := u64
	mapId := u64
	point := u8^
	isValid := bool
	this := !(char^ fileName) -> void
	{
		fileId = 0
		point = null
		Open(fileName,FILE_READ,0)
	}
	this := !(char^ fileName,int flg) -> void
	{
		fileId = 0
		point = null
		if flg == FILE_CREATE
			Open(fileName,FILE_WRITE,0)
		else
			Open(fileName,flg,0)
	}
	this := !(char^ fileName,int flg, u64 itSize) -> void
	{
		fileId = 0
		point = null
		Open(fileName,flg,itSize)
	}
	Open := !(char^ fileName) -> void
	{
		Open(fileName,FILE_READ,0)
	}
	Open := !(char^ fileName,int flg, u64 itSize) -> void
	{
		isValid = false
		if prvtOpenFile(fileName,fileId&,flg,0,gMallocTemporary) == 0
			return void
	
		fileSize = itSize
		if prvtMapFile(fileId,mapId&,point&,fileSize&,flg,gMallocTemporary) == 0
		{
			prvtCloseFile(fileId)
			return void
		}

		isValid = true
	}
	//Resize := !(s64 newSize) -> void
	//{
	//	if IsInvalid() return void
	//	if newSize < 0 return void

	//	if point != null munmap(point,size)
	//	ftruncate(itemId,newSize)
	//	prvtSetFileSize(f
	//	size = newSize
	//	makeMap()
	//}
	IsInvalid := !() -> bool
	{
		return not isValid
	}

	Get := !() -> u8^
	{
		return point
	}

	Size := !() -> int
	{
		return fileSize
	}
	"~For" := !() -> ArrayIterMappedFile
	{
		return ArrayIterMappedFile(this&)
	}
	"[]" := !(int pos) -> ref u8
	{	
		return point[pos]
	}
	Close := !() -> void
	{
		prvtUnmapFile(mapId,point,fileSize)
		prvtCloseFile(fileId)
	}
}
