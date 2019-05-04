#import "string.cp"

realpath := !(char^ inp, char^ outp) -> char^ declare

opendir := !(char^ inp) -> void^ declare
closedir := !(void^ dirFd) ->int declare
readdir := !(void^ dirFd) -> void^ declare
stat := !(char^ pth, void^ outStat) ->int declare

GetDirectoryName := !(void^ dirFd) -> char^
{
	itAsChar := dirFd->{char^}
	return itAsChar[19]&
}
//st_mode 24 size = 144
DirectoryIterator := class
{
	toUse := Path^
	dirFd := void^
	nowEntry := void^
	nowPath := Path
	this := !(Path^ tu) -> void
	{
		toUse = tu
		dirFd = opendir(toUse.itStr)
		nowEntry =  readdir(dirFd)
		if GetDirectoryName(nowEntry) == "." nowEntry = readdir(dirFd)
		if GetDirectoryName(nowEntry) == ".." nowEntry = readdir(dirFd)
		if nowEntry != null
		{
			nowPath.itStr = toUse^.itStr + "/" + GetDirectoryName(nowEntry)
		}
	}
	"~this" := !() -> void
	{
		if dirFd != null closedir(dirFd)
	}
	"^" := !() -> ref Path
	{
		return nowPath
	}
	Inc := !() -> void
	{
		nowEntry = readdir(dirFd)
		if nowEntry != null
		{
			newName := GetDirectoryName(nowEntry)
			nowPath.itStr = toUse.itStr + "/" + GetDirectoryName(nowEntry)
		}
	}
	IsEnd := !() -> bool
	{
		return nowEntry == null
	}
}

DirectoryWildcardIterator := class
{
	dirFd := void^
	nowEntry := void^
	nowPath := Path
	buff := char[256]
	addToBuff := int
	wildCard := char[80]
	this := !(char^ tu) -> void
	{
		strL := strlen(tu)

		for c,u : tu
		{
			buff[u] = c
		}
		buff[strL] = 0

		dirL := strL - 1

		while dirL >= 0
		{
			if buff[dirL] in "/\\"
				break
			dirL--
		}
		dirL++

		for i,j : dirL..(strL-1)
		{
			wildCard[j] = buff[i]
			buff[i] = 0
		}
		addToBuff = dirL
		if dirL == 0
		{
			buff[0] = '.'
			buff[1] = '/'
			buff[2] = 0
			addToBuff = 2
		}

		wildCard[strL - dirL] = 0

		dirFd = opendir(buff[0]&)
		Inc()
		nowPath.itStr = buff[0]&
	}
	"~this" := !() -> void
	{
		if dirFd != null closedir(dirFd)
	}
	"^" := !() -> ref Path
	{
		return nowPath
	}
	Inc := !() -> void
	{
		nowEntry = readdir(dirFd)
		while nowEntry != null
		{
			newName := GetDirectoryName(nowEntry)

			if not IsInWildcard(newName)
			{
				nowEntry = readdir(dirFd)
				continue
			}

			preSet := this.addToBuff
			for c : newName
			{
				buff[preSet] = c
				preSet++
			}
			buff[preSet] = 0

			break

		}
	}
	IsInWildcard := !(char^ str) -> bool
	{
		return IsInWildcardReq(str,wildCard[0]&)
	}
	IsInWildcardReq := !(char^ toCmp,char^ inWild) -> bool
	{
		j := 0
		k := 0

		while inWild[k] != 0
		{
			if inWild[k] == '*'
			{
				numAfter := inWild[k+1]
				if numAfter == 0 return true

				while toCmp[j] != 0
				{
					if toCmp[j] == numAfter and IsInWildcardReq(toCmp[j]&,inWild[k+1]&)
						return true
					j += 1
				}
				k += 1
			}else
			{
				if toCmp[j] != inWild[k]
					return false
				j += 1
				k += 1
			}
		}
		return true
	}
	IsEnd := !() -> bool
	{
		return nowEntry == null
	}
}
Wildcard := class
{
	toParse := char^
	this := !(char^ toIt) -> void
	{
		toParse = toIt
	}
	"~For" := !() -> DirectoryWildcardIterator
	{
		return DirectoryWildcardIterator(toParse)
	}

}


Path := class
{
	itStr := string
	this := !() -> void
	{
		itStr = null
	}
	this := !(string st) -> void
	{
		itStr = st
	}
	this := !(Path st) -> void
	{
		itStr = st.itStr
	}
	Get := !() -> char^
	{
		return itStr
	}
	"==" := !(Path pt2) -> bool
	{
		idX := u64
		idY := u64
		if prvtGetFileInfo(itStr,null,null,idX&,null,gMallocTemporary) == 0
			return false
		if prvtGetFileInfo(pt2.itStr,null,null,idY&,null,gMallocTemporary) == 0
			return false
		return idX == idY
	}
	IsExist := !() -> bool
	{
		fInd := u64
		return prvtGetFileInfo(pt2.itStr,null,null,fInd&,null,gMallocTemporary) == 1
	}

	FullPath := !() -> Path
	{
		newLine := char^
		if(prvtGetFileInfo(itStr,newLine&,null,null,null,gMallocTemporary) == 0)
			return Path("")
		if newLine == null return Path("")
		return Path(newLine)
	}
	"~For" := !() -> DirectoryIterator
	{
		return DirectoryIterator(this&)
	}
	IsFolder := !() -> bool
	{
		isF := int
		if prvtGetFileInfo(itStr,null,null,null,isF&,gMallocTemporary) == 0
			return false
		return isF == 1
	}
	"=" := !(Path pt) -> void
	{
		itStr = pt.itStr
	}
	"=" := !(string pt) -> void
	{
		itStr = pt
	}
	"/=" := !(string str) -> void
	{
		itStr = itStr + "/" + str
	}
	"/" := !(string pt2) -> Path
	{
		ToRet.itStr = this.itStr + "/" + pt2
	}
	"/" := !(Path pt2) -> Path
	{
		return Path(itStr + "/" + pt2.itStr)
	}
	Ext := !() -> StringSpan
	{
		size := StrSize(itStr)
		pos := size - 1
		if size == 0 return StringSpan("",0)

		while pos >= 0 and itStr[pos] != '.' {
			if itStr[pos] in "/\\"  return StringSpan("",0)
			pos--
		}
		if pos < 0 return StringSpan("",0)

		return StringSpan(itStr[pos]&, size - pos)
	}
	Name := !() -> StringSpan
	{
		size := StrSize(itStr)
		pos := size - 1
		if size == 0 return StringSpan("",0)

		while pos >= 0 {
			if itStr[pos] in "/\\"  break
			pos--
		}
		if pos < 0 return StringSpan("",0)
		if size - pos == 1 return StringSpan("",0)

		return StringSpan(itStr[pos + 1]&, size - pos - 1)
	}
	FolderName := !() -> string
	{
		size := StrSize(itStr)
		pos := size - 1
		if size == 0 return ""

		while pos >= 0 {
			if itStr[pos] in "/\\"  break
			pos--
		}
		if pos < 0 return ""
		if size - pos == 1 return ""
		pos++
		newStr :=  malloc(pos + 1)->{char^}

		for i : pos newStr[i] = itStr[i]
		newStr[pos] = 0

		return newStr
	}
	JustName := !() -> StringSpan
	{
		itName := this.Name()
		if itName == "" return StringSpan("",0)
		
		itSize := itName.Size()
		pos := itSize - 1

		while pos >= 0 and itName[pos] != '.' pos--

		if pos < 0 return itName
		return StringSpan(itName.Get(),pos)
	}
}
