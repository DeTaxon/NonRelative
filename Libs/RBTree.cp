RBColorBlack := 0
RBColorRed := 1

RBTree := class .{@DATA}
{
	Start := CommonTreeNode.{DATA}^
	
	this := !() -> void
	{
		Start = null
	}
	FindNode := !(DATA dat) -> CommonTreeNode.{DATA}^
	{
		itr := Start
		while itr != null
		{
			switch itr.data <=> dat
			{
				case 1
					itr = itr.Right
				case 0
					return itr
				case -1
					itr = itr.Left
			}
		}
		return itr
	}
	FindOrCreate := !(DATA dat,CommonTreeNode.{DATA}^^ toRet) -> bool //bool isCreated
	{
		if Start == null 
		{
			Start = new CommonTreeNode.{DATA}()
			Start.color = RBColorBlack
			toRet^ = Start
			return true
		}
		prev := Start
		iter := Start
		
		while iter != null
		{
			setTest := iter.data <=> dat
			if setTest == 0{
				break
			}else{
				prev = iter
			}
			if setTest == 1 	iter = iter.Right
			else 			iter = iter.Left
		}
		if iter != null {
			toRet^ = iter
			return false
		}
		
		if prev.data <=> dat == 1{
			prev.Right = new CommonTreeNode.{DATA}()
			prev.Right.color = RBColorRed
			prev.Right.Up = prev
			iter = prev.Right
		}else{
			prev.Left = new CommonTreeNode.{DATA}()
			prev.Left.color = RBColorRed
			prev.Left.Up = prev
			iter = prev.Left
		}
		
		RBRepair(iter)

		toRet^ = iter
		return true
	}
	RotateLeft := !(CommonTreeNode.{DATA}^ toRotate) -> void
	{
		dwNode := toRotate.Right
		toRotate.Right = dwNode.Left
		if toRotate.Right != null toRotate.Right.Up = toRotate
		dwNode.Up = toRotate.Up
		toRotate.Up = dwNode
		dwNode.Left = toRotate
		if dwNode.Up == null {
			Start = dwNode
		}else{
			if dwNode.Up.Left == toRotate dwNode.Up.Left = dwNode
			else dwNode.Up.Right = dwNode
		}
	}
	RotateRight := !(CommonTreeNode.{DATA}^ toRotate) -> void
	{
		dwNode := toRotate.Left
		toRotate.Left = dwNode.Right
		if toRotate.Left != null toRotate.Left.Up = toRotate
		dwNode.Up = toRotate.Up
		toRotate.Up = dwNode
		dwNode.Right = toRotate
		if dwNode.Up == null {
			Start = dwNode
		}else{
			if dwNode.Up.Left == toRotate dwNode.Up.Left = dwNode
			else dwNode.Up.Right = dwNode
		}
	}
	RBRepair := !(CommonTreeNode.{DATA}^ toRep) -> void
	{
		if toRep.Up == null
		{
			toRep.color = RBColorBlack
			return void
		}
		if toRep.Up.color == RBColorBlack return void

		GrandNode := toRep.Up.Up

		UnclColorB := true
		UnclNode := CommonTreeNode.{DATA}^
		UnclNode = null
		if GrandNode.Left == toRep.Up 	UnclNode = GrandNode.Right
		else				 UnclNode = GrandNode.Left

		if UnclNode != null UnclColorB = UnclNode.color == RBColorBlack
		
		if not UnclColorB {
			toRep.Up.color = RBColorBlack
			UnclNode.color = RBColorBlack
			GrandNode.color = RBColorRed
			RBRepair(GrandNode)
			return void
		}

		if GrandNode.Left == toRep.Up and toRep.Up.Right == toRep {
			RotateLeft(toRep.Up)
		}
		if GrandNode.Right == toRep.Up and toRep.Up.Left == toRep {
			RotateRight(toRep.Up)
		}
		if GrandNode.Left != null and GrandNode.Left.color == RBColorRed{
			RotateRight(GrandNode)
		}else{
			RotateLeft(GrandNode)
		}
		GrandNode.color = RBColorRed
		GrandNode.Up.color = RBColorBlack
	}
	RemoveNode := !(CommonTreeNode.{DATA}^ toRem) -> void
	{
		if toRem.Left == null and toRem.Right == null
		{
			fth := toRem.Up
			if fth != null
			{
				if fth.Left == toRem
				{
					fth.Left = null
					if toRem.color == RBColorRed return void
					RebalanceDown(fth,null)
				}else{
					fth.Right = null
					if toRem.color == RBColorRed return void
					RebalanceDown(fth,null)
				}
			}else{
				Start = null
			}
			delete toRem
			return void
		}
		if toRem.Left == null
		{
			if toRem.Up == null
			{
				Start = toRem.Right
				Start.Up = null
				Start.color = RBColorBlack
			}else{
				fth := toRem.Up
				if fth.Left == toRem
				{
					fth.Left = toRem.Right
					toRem.Right.Up = fth
					if toRem.color == RBColorRed return void
					if fth.Left.color == RBColorRed
					{
						fth.Left.color = RBColorBlack
						return void
					}
					RebalanceDown(fth,fth.Left)
				}else{
					fth.Right = toRem.Right
					toRem.Right.Up = fth
					if toRem.color == RBColorRed return void
					if fth.Right.color == RBColorRed
					{
						fth.Right.color = RBColorBlack
						return void
					}
					RebalanceDown(fth,fth.Right)
				}
			}
			delete toRem
			return void
		}
		if toRem.Right == null
		{
			if toRem.Up == null
			{
				Start = toRem.Left
				Start.Up = null
				Start.color = RBColorBlack
			}else{
				fth := toRem.Up
				if fth.Left == toRem
				{
					fth.Left = toRem.Left
					toRem.Left.Up = fth
					if toRem.color == RBColorRed return void
					if fth.Left.color == RBColorRed
					{
						fth.Left.color = RBColorBlack
						return void
					}
					RebalanceDown(fth,fth.Left)
				}else{
					fth.Right = toRem.Left
					toRem.Left.Up = fth
					if toRem.color == RBColorRed return void
					if fth.Right.color == RBColorRed
					{
						fth.Right.color = RBColorBlack
						return void
					}
					RebalanceDown(fth,fth.Right)
				}
			}
			delete toRem
			return void
		}
		//else Left != null and Right != null

		cand := toRem.Right
		while cand.Left != null
			cand = cand.Left

		cUp := cand.Up
		remUp := toRem.Up
		if cUp.Left == cand
		{
			cUp.Left = toRem
		}else{
			cUp.Right = toRem
		}
		if remUp != null
		{
			if remUp.Left == toRem
			{
				remUp.Left = cand
			}else{
				remUp.Right = cand
			}
		}else{
			Start = cand
		}
		toRem.Up = cUp
		cand.Up = remUp

		cand.Left = toRem.Left
		toRem.Left = null

		tmp := toRem.Right
		toRem.Right = cand.Right
		cand.Right = tmp

		if cand.Left != null cand.Left.Up = cand
		if cand.Right != null cand.Right.Up = cand
		if toRem.Right != null toRem.Right.Up = toRem
		
		tmpCol := cand.color
		cand.color = toRem.color
		toRem.color = tmpCol
		RemoveNode(toRem)
	}
	RebalanceDown := !(CommonTreeNode.{DATA}^ par, CommonTreeNode.{DATA}^ Repl) -> void
	{
		//par  - unknown
		//Repl - must be RBColorBlack

		if par == null return void

		sib := CommonTreeNode.{DATA}^
		if par.Left == Repl {
			sib = par.Right
		} else {
			sib = par.Left
		}

		if sib.color == RBColorRed
		{
			par.color = RBColorRed
			sib.color = RBColorBlack
			if sib == par.Left
			{
				RotateRight(par)
			}else{
				RotateLeft(par)
			}
			if par.Left == Repl
			{
				sib = par.Right
			}else{
				sib = par.Left
			}
		}
		Sl := RBColorBlack
		Sl = sib.Left?.color
		Sr := RBColorBlack
		Sr = sib.Right?.color
		if par.color == RBColorBlack 
		{	

			if Sl == Sr and Sl == RBColorBlack
			{
				sib.color = RBColorRed
				RebalanceDown(par.Up,par)
				return void
			}
		}
		if par.color == RBColorRed and Sl == Sr and Sr == RBColorBlack
		{
			sib.color = RBColorRed
			par.color = RBColorBlack
			return void
		}

		if par.Left == sib
		{
			if Sr == RBColorRed
			{
				RotateLeft(sib)
				sib.color = RBColorRed
				sib.Up.color = RBColorBlack
				sib = sib.Up
			}
			sib.Left.color = RBColorBlack
			sib.color = sib.Up.color
			sib.Up.color = RBColorBlack
			RotateRight(par)

		}else{
			if Sl == RBColorRed
			{
				RotateRight(sib)
				sib.color = RBColorRed
				sib.Up.color = RBColorBlack
				sib = sib.Up
			}
			sib.Right.color = RBColorBlack
			sib.color = sib.Up.color
			sib.Up.color = RBColorBlack
			RotateLeft(par)
		}
	}
}
