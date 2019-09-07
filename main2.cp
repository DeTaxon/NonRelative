//printf := !(char^ a,...) -> int declare





main := !() -> int
{
	tst := RBMap.{int,int}()

	tst[1] = 10
	tst[6] = 60
	tst[3] = 30

	printf("heh %i\n", 3 in tst)
	printf("heh %i\n", 7 in tst)
	
	//uvInit()

	//mn := uvLoop()
	//mn.Timer(0,0.5s,x => {
	//	printf("heh\n")
	//})
	//mn.Run()

	return 0
}

