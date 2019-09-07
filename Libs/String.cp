sprintf := !(char^ buf,char^ frmt, ...) -> int declare
strcmp := !(char^ a,char^ b) -> int declare


ToString := !(int x) -> char^
{
	toRet := new char[13]
	sprintf(toRet,"%i",x)
	return toRet
}
ToString := !(float x) -> char^
{
	toRet := new char[30]
	sprintf(toRet,"%f",x)
	return toRet
}
"<=>" := !(char^ a, char^ b) -> int
{
	if a->{int^} == b->{int^} return 0
	if a == null return 1
	if b == null return -1
	cmpRes := strcmp(a,b)
	if cmpRes > 0 return 1
	if cmpRes < 0 return -1
	return 0
}
"<" := !(char^ a, char^ b) -> bool
{
	if a == null return false
	if b == null return false
	return strcmp(a,b) < 0
}
StrCmp := !(char^ a,char^ b) -> bool
{
	if a->{int^} == null or b->{int^} == null return false
	if a->{int^} == b->{int^} return true
	return strcmp(a,b) == 0
}
"==" := !(char^ a,char^ b) -> bool
{
	return StrCmp(a,b)
}
"!=" := !(char^ a,char^ b) -> bool
{
	return strcmp(a,b) != 0
}


StrSize := !(char^ a) -> int
{
	Si := 0
	while a[Si] != 0 Si += 1
	return Si
}
Size := !(char^ this) -> int
{
	Si := 0
	while this[Si] != 0 Si += 1
	return Si
}

StrCopy := !(char^ a) .{} -> char^
{
	Si := StrSize(a)
	Pre := new char[Si+1]
	memcpy(Pre->{void^},a,Si)
	Pre[Si] = 0

	return Pre
}

Find := !(char^ this,char val) -> int
{
	for i,j : this
	{
		if i == val 
			return j
	}
	return -1
}

"in" := !(int chr, char^ str) -> bool
{
	return ChrInStr(chr,str)
}

ChrInStr := !(int chr,char^ str) -> bool
{
	i := 0
	while str[i] != 0 
	{
		if chr == str[i] return true
		i += 1
	}
	return false
}

DivideStr := !(char^ what,int to) . {}-> List.{StringSpan}
{
	result."this"()
	i := 0
	j := 0
	while what[j] != 0
	{
		if what[j] == to
		{
			if j != i result.Emplace(what[i]&,j - i)
			i = j+1
			j += 1
		} else
		{
			j += 1
		}
	}
	if j - i != 0
	{
		result.Emplace(what[i]&,j - i)
	}		
}
DivideStr := !(char^ what,char^ to) . {} -> List.{StringSpan}
{
	result."this"()
	i := 0
	j := 0
	while what[j] != 0
	{
		if what[j] in to
		{
			if j != i result.Emplace(what[i]&,j - i)
			i = j+1
			j += 1
		} else
		{
			j += 1
		}
	}
	if j - i != 0
	{
		result.Emplace(what[i]&,j - i)
	}		
}

StrToInt := !(char^ a) -> int 
{
	Sum := 0
	i := 0
	Min := false
	if ( a[0] == '-')
	{
		i = 1
		Min = true
	}
	while a[i] != 0 and a[i] <= '9' and a[i] >= '0'
	{
		Sum *= 10
		Sum += a[i] - '0'
		i += 1	
	}
	if Min return -Sum
	return Sum
}

StrToFloat := !(char^ a) -> float
{
	Sum := 0.0
	i := 0
	Min := false
	if (a[0] == '-')
	{
		i += 1
		Min = true
	}
	while a[i] != 0 and a[i] <= '9' and a[i] >= '0'
	{
		Sum *= 10.0
		Sum += a[i] - '0'
		i += 1	
	}
	if (a[i] == '.')
	{
		i += 1
		Q := 1.0
		while a[i] != 0 and a[i] <= '9' and a[i] >= '0'
		{
			Q *= 0.1
			Sum += Q*(a[i] - '0')
			i += 1
		}
	}
	if Min return 0.0-Sum
	return Sum
}

StringIterator := class
{
	itr := char^
	ind := int
	this := !(char^ itm) -> void {	itr = itm ind = 0}
	Inc := !() -> void { ind += 1}
	IsEnd := !() -> bool { return itr[ind] == 0}
	"^" := !() -> ref char { return itr[ind] }
}
"~For" := !(char^ str) -> StringIterator { return StringIterator(str)}

