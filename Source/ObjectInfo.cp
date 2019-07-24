ObjInfo := class
{
	isValue := bool
	Name := StringSpan
	ValueStr := StringSpan
	SubList := List.{ObjInfo^}

	IsValue := !() -> bool
	{
		return isValue
	}
	Print := !(int s) -> void
	{
		for s printf("-")
		if isValue
		{
			printf("%s is %s\n",Name.Str(),ValueStr.Str())
		}else{
			printf("%s :\n",Name.Str())
			SubList[^].Print(s + 1)
		}
	}
}

ParseInfo := !(void^ dataPtr, int dataSize) .{} -> ObjInfo^
{
	res := new ObjInfo
	prvtParseInfo(dataPtr->{u8^},dataSize,res,0)
	return res
}
prvtParseInfo := !(u8^ dataPtr, int dataSize,ObjInfo^ toFill,int indSize) .{} -> int
{
	valIter := 0
	while true
	{
		if valIter >= dataSize 
			break

		i := valIter
		while dataPtr[i] == '\t' and i < dataSize
		{
			i++
		}
		if i == dataSize
			return i

		if i - valIter != indSize
		{
			return valIter
		}
		nameStart := i

		while dataPtr[i] != ':' and i < dataSize
			i++
		if i == dataSize 
			return i

		dim := i
		metWord := false

		while dataPtr[i] != '\n' and i < dataSize
		{
			if not (dataPtr[i] in " :\t\n")
			{
				metWord = true
			}
			i++
		}

		//if i == dataSize
		//	return i

		nameS := nameStart
		while not (dataPtr[nameS] in " \t:")
			nameS++

		newObj := new ObjInfo
		newObj.Name = StringSpan(dataPtr[nameStart]&,nameS - nameStart)
		newObj.isValue = metWord
		if metWord
		{
			dim++
			while dataPtr[dim] in " \t"
				dim++
			valS := dim
			while dataPtr[valS] != '\n' and valS < dataSize
				valS++
			while dataPtr[valS] in "\n\t "
				valS--
			newObj.ValueStr = StringSpan(dataPtr[dim]&,valS - dim + 1 )
		}else{
			i += prvtParseInfo(dataPtr[i+1]&,dataSize - i - 1,newObj,indSize + 1)
		}
		toFill.SubList << newObj

		valIter = i + 1
	}
}

