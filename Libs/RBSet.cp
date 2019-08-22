RBSet := class .{@DATA}
{
	itTree := RBTree.{DATA}
	itSize := int

	this := !() -> void
	{
		itTree."this"()
		itSize = 0
	}

	Insert := !(DATA dat) -> void
	{
		resl := CommonTreeNode.{DATA}^
		if(itTree.FindOrCreate(dat,resl&))
		{
			resl.data = dat
			itSize += 1
		}
		
	}
	Remove := !(DATA dat) -> void
	{
		resl := itTree.FindNode(dat)
		if resl != null
		{
			itTree.RemoveNode(resl)
			itSize -= 1
		}
	}
	Size := !() -> int { return itSize }
	"~For" := !() -> CommonSetIterator.{DATA}
	{
		return CommonSetIterator.{DATA}(itTree.Start)
	}
	"<<" := !(DATA dat) -> ref RBSet.{DATA}
	{
		Insert(dat)
		return this
	}
}

