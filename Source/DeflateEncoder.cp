BitWalker := class
{
	ptrToArray := u8^
	mpSize := u64
	bitIter := u64
	byteIter := u64
	this := !(void^ ptrToArr,u64 itSize) -> void
	{
		ptrToArray = ptrToArr->{u8^}
		mpSize = itSize
		bitIter = 0
		byteIter = 0
	}
	Get := !(int btSi) -> u32
	{
		preRes := u32
		sizeDiff := mpSize - byteIter

		switch sizeDiff
		{
		case 1
			preRes = ptrToArray[byteIter]
		case 2
			preRes = ptrToArray[byteIter]&->{u16^}^
		case 3
			preRes = ptrToArray[byteIter]&->{u16^}^
			preRes += ptrToArray[byteIter + 2]&->{u8}^ << 16
		case void
			preRes = ptrToArray[byteIter]&->{int^}^
		}

	}
	Move := !(int btToMove) -> void
	{
		bitiTer += btToMove
		byteIter += bitIter div 8
		bitIter = bitIter % 8
	}
}
DeflateEncode := !(void^ inpStrmP , u64 inpSize,void^ outpStrmP,u64 outSize) -> bool
{
	inpStrm := inpStrmP->{u8^}
	outStrm := outpStrmP->{u8^}
	mState := 0

	bW := BitWalker(inpStrm,inpSize)

	printf("byte %X\n",inpStrm[0])


	
}
