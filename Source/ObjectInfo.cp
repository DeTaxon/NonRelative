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
	Walk := !(char^ path) -> ObjInfo^
	{
		pth := DivideStr(path,'.') ; $temp

		if pth.Size() == 0 {
			return this&
		}

		itr := this&
		for p : pth, k : 0
		{
			found := false
			if itr.SubList[^].Name == p
			{
				itr = it
				found = true
				break
			}
			if not found
				throw new Exception(0,"Node not found , path = <"sbt + path + "> number = <" + k + ">")
		}
		return itr
	}
	Str := !() .{} -> char^
	{
		if not isValue
			throw new Exception(0,"Node does not contain value")
		return ValueStr.Str()
	}
	Int := !() -> int
	{
		if not isValue
			throw new Exception(0,"Node does not contain value")
		val := 0
		mins := false

		for i : ValueStr.Size()
		{
			if ValueStr[i] in '0'..'9'
			{
				val *= 10
				val += ValueStr[i] - '0'
			}else{
				if i == 0 and ValueStr[i] == '-'
				{
					mins = true
				}else{
					throw new Exception(0,"Can not cast to integer value")
				}
			}
		}
		if mins return -val
		return val
	}
	Float := !() -> float
	{
		if not isValue
			throw new Exception(0,"Node does not contain value")
		
		mins := false
		val := 0.0
		k := 0
		if ValueStr[k] == '-'
		{
			mins = true
			k++
		}

		while k < ValueStr.Size() and ValueStr[k] in '0'..'9'
		{
			val *= 10.0
			val += ValueStr[k] - '0'
			k++
		}
		if k < ValueStr.Size()
		{
			if not ValueStr[k] in ".eE"
				throw new Exception(0,"Can not parse float, . expected")
			if ValueStr[k] == '.'
			{
				k++
				qunt := 0.1
				while k < ValueStr.Size() and ValueStr[k] in '0'..'9'
				{
					val += (ValueStr[k] - '0')*qunt
					qunt *= 0.1
					k++
				}
			}
			if k < ValueStr.Size()
			{
				if not ValueStr[k] in "eE"
					throw new Exception(0,"Can not parse float, unknown end")

				k++
				if k == ValueStr.Size()
					throw new Exception(0,"Can not parse float, unexpected end")
				minsE := ValueStr[k] == '-'
				if ValueStr[k] in "-+"
					k++
				if k == ValueStr.Size()
					throw new Exception(0,"Can not parse float, unexpected end")
				epx := 0
				while k < ValueStr.Size() and ValueStr[k] in '0'..'9'
				{
					epx = epx*10 + (ValueStr[k] - '0')
					k++
				}
				if k != ValueStr.Size()
					throw new Exception(0,"Can not parse float, garbage at the end end")
				if minsE
					epx = -epx
				if epx > 0
				{
					for epx	val *= 10.0
				}else{
					//TODO:
					for -epx val *= 0.1
				}
				
			}
		}
		if mins return -val
		return val

	}
}

//ParseInfo := !(void^ dataPtr, int dataSize) .{} -> ObjInfo^
//{
//	res := new ObjInfo
//	prvtParseInfo(dataPtr->{u8^},dataSize,res,0)
//	return res
//}
//prvtParseInfo := !(u8^ dataPtr, int dataSize,ObjInfo^ toFill,int indSize) .{} -> int
//{
//	valIter := 0
//	while true
//	{
//		if valIter >= dataSize 
//			break
//
//		i := valIter
//		while dataPtr[i] == '\t' and i < dataSize
//		{
//			i++
//		}
//		if i == dataSize
//			return i
//
//		if i - valIter != indSize
//		{
//			return valIter
//		}
//		nameStart := i
//
//		while dataPtr[i] != ':' and i < dataSize
//			i++
//		if i == dataSize 
//			return i
//
//		dim := i
//		metWord := false
//
//		while dataPtr[i] != '\n' and i < dataSize
//		{
//			if not (dataPtr[i] in " :\t\n")
//			{
//				metWord = true
//			}
//			i++
//		}
//
//		//if i == dataSize
//		//	return i
//
//		nameS := nameStart
//		while not (dataPtr[nameS] in " \t:")
//			nameS++
//
//		newObj := new ObjInfo
//		newObj.Name = StringSpan(dataPtr[nameStart]&,nameS - nameStart)
//		newObj.isValue = metWord
//		if metWord
//		{
//			dim++
//			while dataPtr[dim] in " \t"
//				dim++
//			valS := dim
//			while dataPtr[valS] != '\n' and valS < dataSize
//				valS++
//			while dataPtr[valS] in "\n\t "
//				valS--
//			newObj.ValueStr = StringSpan(dataPtr[dim]&,valS - dim + 1 )
//		}else{
//			i += prvtParseInfo(dataPtr[i+1]&,dataSize - i - 1,newObj,indSize + 1)
//		}
//		toFill.SubList << newObj
//
//		valIter = i + 1
//	}
//}

ParseInfoFile := !(void^ dat,int siz) .{} ->  ObjInfo^
{
	itAsSpan := StringSpan(dat->{char^},siz)
	
	lines := itAsSpan.DivideStr("\n") ; $temp

	if lines.Size() == 0
		return null

	tabsCnt := new int[lines.Size()] ; $temp

	for it : lines, tb : tabsCnt
	{
		for c : it
		{
			if c != '\t'
				break
			tb += 1
		}
	}

	expectedTab := 0
	resVal := new ObjInfo
	resIter := resVal
	resB := List.{ObjInfo^}() ; $temp $keep
	for it,i : lines, tb : tabsCnt
	{	
		nameStart := -1
		nameEnd := -1
		valueStart := -1
		valueEnd := -1
		k := 0
		while k < it.Size()
		{
			if it[k] != '\t' {
				nameStart = k
				k++
				break
			}
			k++
		}
		if nameStart == -1
			continue
		while k < it.Size()
		{
			if it[k] == ':'
			{
				k++
				break
			}
			if not (it[k] in " \t")
				nameEnd = k
			k++
		}

		if nameEnd == -1
			throw new Exception(0,"Invalid file data at line "sbt + (i+ 1))
		
		while k < it.Size()
		{
			if not (it[k] in " \t")
			{
				valueStart = k
				k++
				break
			}
			k++
		}

		if valueStart != -1
		{
			k = it.Size() - 1
			while it[k] in " \t"
				k -= 1
			valueEnd = k
		}

		nameSpan := it[nameStart..(nameEnd-nameStart+1)]

		if tb < expectedTab for expectedTab - tb
		{
			expectedTab -= 1
			resIter = resB.Pop()
		}
		if valueStart == -1
		{	
			if tb > expectedTab
				throw new Exception(0,"Too many tabs at "sbt + (i+1))
			
			expectedTab += 1
			newVal := new ObjInfo
			newVal.Name = nameSpan
			resIter.SubList.Push(newVal)
			resB.PushFront(resIter)
			resIter = newVal
		}else{
			valueSpan := it[valueStart..(valueEnd-valueStart+1)]
			newVal := new ObjInfo
			newVal.Name = nameSpan
			newVal.ValueStr = valueSpan
			newVal.isValue = true
			resIter.SubList.Push(newVal)
		}

	}
	return resVal
}
