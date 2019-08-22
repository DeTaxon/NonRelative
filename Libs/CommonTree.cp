CommonTreeNode := class .{@DATA}
{
	data := DATA
	balance := s8
	color := s8
	Left,Right,Up := CommonTreeNode.{DATA}^
	this := !() -> void
	{
		Left = null
		Right = null
		Up = null
		balance = 0
	}
	PrintIt := !(int i) -> void
	{
		for i printf("-")
		printf("%i %i\n",data,this.color->{int})
		if Left != null and Left.Up != this& printf("incorrect left father\n")
		if Right != null and Right.Up != this& printf("incorrect Right father\n")
		if Left != null Left.PrintIt(i+1) else {for i+1 printf("-") printf("null\n") }
		if Right != null Right.PrintIt(i+1) else {for i+1 printf("-") printf("null\n") }
	}
	TestSizeAVL := !(bool^ isTrue) -> int
	{
		if Left == null and Right == null
		{
			if balance != 0 isTrue^ = false
			return 1
		}
		hL := 0
		hR := 0
		hL = Left?.TestSizeAVL(isTrue)
		hR = Right?.TestSizeAVL(isTrue)

		if balance != hR - hL
		{
			isTrue^ = false
			printf("bad balance at %i should be %i\n",data,hR - hL)
		}
		if Left != null and Left.Up != this&
		{
			isTrue^ = false
		}
		if Right != null and Right.Up != this&
		{
			isTrue^ = false
		}
		if hL > hR
			return hL + 1
		return hR + 1
	}
	TestSizeRB := !(bool^ isTrue) -> int
	{
		hL := 1
		hR := 1
		hL = Left?.TestSizeRB(isTrue)
		hR = Right?.TestSizeRB(isTrue)

		if hR !=  hL
		{
			isTrue^ = false
			printf("bad balance at %i should be %i %i\n",data,hL hR)
		}
		if Left != null and Left.Up != this&
		{
			isTrue^ = false
		}
		if Right != null and Right.Up != this&
		{
			isTrue^ = false
		}
		if color == RBColorBlack
			return hL + 1
		return hL
	}
	IsCorrect := !() -> bool
	{
		if Left != null and Left.data <=> data == -1 return false
		if Right != null and Right.data <=> data == 1 return false

		if Left != null and not Left.IsCorrect() return false
		if Right != null and not Right.IsCorrect() return false
		return true
	}
	CheckHeight := !(bool^ res) -> int
	{

		sizL := 0
		sizR := 0

		if Left != null sizL = Left.CheckHeight(res)
		if Right != null sizR = Right.CheckHeight(res)

		if sizR - sizL != balance res^ = false
		if balance >= 2 or balance <= -2 res^ = false
		if sizR > sizL return sizR + 1
		return sizL + 1
	}
	Height := !() -> int
	{
		leftB := 0
		rightB := 0

		leftB = Left?.Height()
		rightB = Right?.Height()

		if leftB > rightB
			return leftB + 1
		return rightB + 1
	}
}
