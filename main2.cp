//printf := !(char^ a,...) -> int declare




abc := uvUDP^
thAddr := uvAddr

buffe := char[1024]


Rand := !(range itR) -> int
{
	diff := itR->end - itR->begin
	return rand() % diff + itR->begin
}


main := !() -> int
{
	TLock(null)
	TUnlock(null)
	uvInit()

	mn := uvLoop()
	for 4
	{
		mn.Work(x => {
			TLock(null)
			defer TUnlock(null)

			for 4
			{
				printf("heh %p\n",x)
				TSleep(0.7)
			}
		})
	}
	//mn.Work(x => {
	//	for 8
	//	{
	//		printf("heh %p\n",x)
	//		sleep(1)
	//	}
	//	Barrier(null,2)
	//})
	mn.Run()
	return 0
}

