strlen := !(char^ str) -> int declare
strncmp := !(char^ a, char^ b, int size) -> int declare

StringSpan := class{
	ptr := char^
	itSize := int
	this := !() -> void {ptr = null itSize = 0}
	this := !(char^ st) -> void{ptr = st itSize = StrSize(st)}
	this := !(char^ st,int si) -> void{ptr = st itSize = si}

	this := !(char^ st, int si,range ran) -> void{
		startInd := 0
		if ran->begin < 0 {
			startInd = si + ran->begin
		}else{
			startInd = ran->begin
		}
		endInd := 0
		if ran->end <= 0 {
			endInd = si + ran->end - 1
		}else{
			endInd = startInd + ran->end - 1
		}
		ptr = st[startInd]&
		itSize = endInd - startInd + 1
	}
	Get := !() -> char^ { return ptr }
	Size := !() -> int { return itSize }
	"[]" := !(int ind) -> ref char { return ptr[ind] }
	"[]" := !(range ran) -> StringSpan
	{
		return StringSpan(ptr,itSize,ran)
	}
	"!=" := !(char^ toCmp) -> bool { return this <=> toCmp != 0 }
	"==" := !(char^ toCmp) -> bool { return this <=> toCmp == 0 }
	"<=>" := !(char^ toCmp) -> int {
		if toCmp == null return 1
		for itSize {
			if toCmp[it] != ptr[it] return toCmp[it] <=> ptr[it]
			if toCmp[it] == 0 return -1
			if ptr[it] == 0 return 1
		}
		if toCmp[itSize] == 0 return 0
		return 1
	}
	"=" := !(StringSpan toGet) -> ref StringSpan
	{
		ptr = toGet.ptr
		itSize = toGet.itSize
		return this
	}
	"<<" := !(StringSpan toGet) -> ref StringSpan
	{
		minVal := itSize
		if toGet.itSize < minVal minVal = toGet.itSize
		memcpy(ptr,toGet.ptr,minVal)
		return this
	}
	"==" := !(StringSpan toCmp) -> bool {
		if toCmp.itSize != itSize return false
		return strncmp(toCmp.ptr,ptr,itSize) == 0
	}
	Str := !() -> string {
		preRet := new char[itSize+1]
		preRet[itSize] = 0
		memcpy(preRet->{void^},ptr,itSize)
		return preRet
	}
	"->{}" := !() .{char^}-> char^
	{
		return Str() ; $temp
	}
	DivideStr := !(char^ to) . {} -> List.{StringSpan}
	{
		result."this"()
		i := 0
		j := 0
		while ptr[j] != 0 and j != itSize
		{
			if ptr[j] in to
			{
				if j != i result.Emplace(ptr[i]&,j - i)
				i = j+1
				j += 1
			} else
			{
				j += 1
			}
		}
		if j - i != 0
		{
			result.Emplace(ptr[i]&,j - i)
		}		
	}
}

"[]" := !(string this, range ran) -> StringSpan
{
	return StringSpan(this,strlen(this),ran)
}

