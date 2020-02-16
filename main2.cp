

StaticSizeAllocator := class .{@ObjSize,@Multi}
{
	mals := List.{void^}
	freeMem := List.{void^} ; $keep

	curMal := void^
	curSiz := int
	
	Push := !(void^ obj) -> void
	{
		freeMem.Push(obj)
	}
	Pop := !() -> void^
	{
		if not freeMem.IsEmpty()
			return freeMem.Pop()
		if curMal == null
		{
			curMal = malloc(ObjSize*Multi)
			mals << curMal
			curSiz = 0
		}

		preRet := curMal->{u8^}[ObjSize*curSiz]&
		curSiz += 1
		if curSiz == Multi
		{
			curMal = null
		}
		return preRet
	}
	Destroy := !() -> void
	{
		free(mals[^])
		mals.Destroy()
	}
}

main := !() -> int
{
	uvInit()

	uv := new uvLoop()
	buff := char[256]
	
	uv.TCP().Listen("127.0.0.1",2000,1,(x,y) ==>[buff&]{
		printf("SomeoneConnected\n")
		y.Recv(buff,(z,len) ==> [buff&]{
			printf("recved %s\n",buff)
		})
	})
	uv.TCP().Connect("127.0.0.1",2000,x ==> {
		printf("on connect %p\n",x)
		o2 := 0
		uv.Timer(0,1,T ==> [o2&]{
			printf("obj %i\n",o2)
			x.Send("Hello"[0]&,6)
			return void
		})
		uv.Timer(0,1,x ==> [o2&]{
			printf("heh\n")
			o2 += 1
			yield void
			o2 += 1
			printf("hoh\n")
			return void
		})
	})


	uv.Run()
	return 0
}

