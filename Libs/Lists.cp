ListNode := class .{@T}
{
	Data := T
	Next := ListNode.{T}^
	this := !() -> void
	{
	}
	this := !(T To) -> void
	{
		Data = To
		Next = null
	}
	this := !(T To,ListNode.{T}^ Ne) -> void
	{
		Data = To
		Next = Ne
	}
}


ListIter := class .{@T}
{
	iter := ListNode.{T}^
	this := !(ListNode.{T}^ toStart) -> void {	iter = toStart }
	"^" := !() -> ref T { return iter.Data }
	Inc := !() -> void { iter = iter.Next }
	IsEnd := !() -> bool { return iter == null }
}

Stack := class .{@T}
{
	Start := ListNode.{T}^
	Counter := int

	if $keep
		CreatedNodes := ListNode.{T}^

	Push := !(T toAdd) .{} -> void
	{
		this << toAdd
	}
	"<<" := !(T toAdd) .{} -> ref Stack.{T}
	{

		if $uniq 
		{
			if this[^] == toAdd
				return this
		}

		newNode := ListNode.{T}^()

		if $keep and CreatedNodes != null
		{
			newNode = CreatedNodes
			CreatedNodes = CreatedNodes.Next
		}
		if newNode == null
		{
			newNode = new ListNode.{T}
		}

		newNode.Data = toAdd
		newNode.Next = Start
		Start = newNode
		Counter++

		return this
	}
	Pop := !()  .{} -> T
	{
		oldVal := Start.Data
		oldNode := Start
		Start = Start.Next
		if $keep 
		{
			oldNode = CreatedNodes
			CreatedNodes = oldNode
		}else{
			if not $temp
				delete oldNode
		}
		Counter--
		return oldVal
	}
	IsEmpty := !() -> bool
	{
		return Counter == 0
	}
	"~For" := !() -> ListIter.{T}
	{
		return ListIter.{T}(Start)
	}
}
Queue := class .{@T}
{
	Start := ListNode.{T}^
	Counter := int

	if $keep
		CreatedNodes := ListNode.{T}^

	this := !() -> void
	{
		Start = null
		Counter = 0
		if $keep CreatedNodes = null
	}
	Push := !(T toAdd) .{} -> void
	{
		this << toAdd
	}
	"<<" := !(T toAdd) .{} -> ref Queue.{T}
	{

		if $uniq 
		{
			if this[^] == toAdd
				return this
		}

		newNode := ListNode.{T}^()

		if $keep and CreatedNodes != null
		{
			newNode = CreatedNodes
			CreatedNodes = CreatedNodes.Next
		}
		if newNode == null
		{
			newNode = new ListNode.{T}
		}

		newNode.Data = toAdd
		newNode.Next = Start
		Start = newNode
		Counter++

		return this
	}
	Pop := !()  -> T
	{
		oldVal := Start.Data
		oldNode := Start
		Start = Start.Next
		if $keep 
		{
			oldNode = CreatedNodes
			CreatedNodes = oldNode
		}else{
			if not $temp
				delete oldNode
		}
		Counter--
		return oldVal
	}
	IsEmpty := !() -> bool
	{
		return Counter == 0
	}
	"~For" := !() -> ListIter.{T}
	{
		return ListIter.{T}(Start)
	}
}
