StringBuilderTemporary := class .{@BufSize}{
	ptr := char^
	inPtrSize := int
	pages := List.{char^} ; $temp

	this := !() -> void { ptr = null inPtrSize = 0 pages."this"()}

	"+" := !(char^ adding) self_return
	{
		sLen := strlen(adding)
		addObjs(adding,sLen)
		return this
	}
	"<<" := !(char^ adding) self_return
	{
		sLen := strlen(adding)
		addObjs(adding,sLen)
		return this
	}
	"<<" := !(StringSpan adding) self_return
	{
		addObjs(adding.Get(),adding.Size())
		return this
	}

	addObjs := !(char^ adding, int sLen) -> void
	{
		if sLen == 0 return void

		if ptr == null ptr = new char[BufSize] ; $temp
		if sLen + inPtrSize > BufSize
		{
			moved := 0
			diff := sLen + inPtrSize - BufSize
			memcpy(ptr[inPtrSize]&,adding,sLen - diff)
			moved += sLen - diff
			
			pages.PushFront(ptr) ; $temp
			while sLen - moved > BufSize {
				preSet := new char[BufSize] ; $temp
				memcpy(preSet[0]&,adding[moved]&,BufSize)
				pages << preSet
				moved += BufSize
			}
			ptr = new char[BufSize] ; $temp
			memcpy(ptr[0]&,adding[moved]&,sLen - moved)
			inPtrSize = sLen - moved

		}else{
			memcpy(ptr[inPtrSize]&,adding,sLen)
			inPtrSize += sLen
		}
	}
	"<<" := !(int x) self_return
	{
		this + x
		return this
	}
	"+" := !(int x) self_return
	{
		miniBuf := char[32]
		miniBuf[31] = 0
		k := 30
		si := x < 0
		if si x = -x
		if x == 0 {
			miniBuf[k] = '0'
			k -= 1
		}
		while x != 0
		{
			miniBuf[k] = x % 10
			miniBuf[k] += '0'
			k -= 1
			x = x div 10
		}
		if si {
			miniBuf[k] = '-'
			k -= 1
		}
		this << miniBuf[k+1]&
		return this
	}
	Str := !() .{} -> string {
		if inPtrSize == 0{
			return ""
		}
		outSize := pages.Size()*BufSize + inPtrSize

		indItr := outSize - inPtrSize
		res := new char[outSize]
		res[outSize] = 0
		memcpy(res[indItr]&,ptr,inPtrSize)
		indItr -= BufSize

		for pages
		{
			memcpy(res[indItr]&,it,BufSize)
			indItr -= BufSize
		}
		return res
	}
	"->{}" := !() .{char^} -> string
	{
		return Str() ; $temp
	}
}

"sbt" := !(string toAp) -> StringBuilderTemporary.{256}
{
	result."this"()
	result << toAp
	return void
}