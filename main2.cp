//printf := !(char^ a,...) -> int declare


main := !() -> int
{
	srand(33)

	tst := RBSet.{int}()
	buf := int[1024]
	for 500
	{
		newS := rand() % 1024
		if newS > 600 or tst.Size() == 0
		{
			newS = rand() % 1024
			tst << newS
		}else{
			for i,j : tst
				buf[j] = i
			toDel := buf[rand() % tst.Size()]
			tst.Remove(toDel)
		}
	}

	return 0
}

