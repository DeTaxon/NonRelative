smVec4fObj := SqObject
smQuantfObj := SqObject
smCentfObj := SqObject

smVec4This := !(void^ vm) -> s64
{
	newVec := new vec4f
	newVec^.w = 1.0f
	sq_setinstanceup(vm,1, newVec)
	sq_setreleasehook(vm,1, vec4ReleaseHook)
	sq_setreleasehook(vm,1, vec4ReleaseHook) //TODO to the memory pool
	return smVec4intrSet(vm,newVec)
}
smVec4getvec := !(void^ vm,s64 itInd) -> vec4f^
{
	itVec := vec4f^
	sq_getinstanceup(vm,itInd,itVec&->{void^},0)
	if itVec == null
	{
		itVec = new vec4f
		itVec^.w = 1.0f
		sq_setinstanceup(vm,itInd,itVec)
		sq_setreleasehook(vm,itInd, vec4ReleaseHook) //TODO to the memory pool
	}
	return itVec
}
smVec4SetPos := !(void^ vm) -> s64
{
	itVec := smVec4getvec(vm,1)
	return smVec4intrSet(vm,itVec)
}
smVec4intrSet := !(void^ vm,vec4f^ setVec) -> s64
{
	stSize := sq_gettop(vm)
	assert(setVec != null)
	if stSize >= 2
	{
		sq_getfloat(vm,2,setVec^.x&)
		if stSize >= 3
		{
			sq_getfloat(vm,3,setVec^.y&)
			if stSize >= 4
			{
				sq_getfloat(vm,4,setVec^.z&)
			}
		}
	}
	return 0
}

smVec4Dot := !(void^ vm) -> s64
{
	vec1 := smVec4getvec(vm,1) 
	vec2 := smVec4getvec(vm,2) 
	res := vec1^.xyz0 <+> vec2^.xyz0
	sq_pushfloat(vm,res)
	return 1
}

smVec4Cross := !(void^ vm) -> s64
{
	vec1 := smVec4getvec(vm,1) 
	vec2 := smVec4getvec(vm,2) 
	res := new vec4f //TODO: memory pool
	res^ = vec1^ <*> vec2^
	res^.w = vec1^.w * vec2^.w
	smVec4fObj.Push(vm)
	sq_createinstance(vm,-1)
	sq_setinstanceup(vm,-1,res)
	return 0
}


vec4ReleaseHook := !(void^ usrPntr,s64 itSize) -> s64
{
	delete usrPntr
	return 0
}
smAddVec4 := !(void^ vm) ->void
{
	sq_pushroottable(vm)
	sq_pushstring(vm,"vec4f",-1)
	sq_newclass(vm,false)

	smVec4fObj.Get(vm,-1)
	smVec4fObj.IncRef(vm)

	sq_pushstring(vm,"SetPos",-1)
	sq_newclosure(vm,smVec4SetPos,0)
	sq_newslot(vm,-3,false)

	sq_pushstring(vm,"Dot",-1)
	sq_newclosure(vm,smVec4Dot,0)
	sq_newslot(vm,-3,false)

	sq_pushstring(vm,"Cross",-1)
	sq_newclosure(vm,smVec4Cross,0)
	sq_newslot(vm,-3,false)
	
	sq_pushstring(vm,"constructor",-1)
	sq_newclosure(vm,smVec4This,0)
	sq_newslot(vm,-3,false)

	sq_newslot(vm,-3,false)
	sq_pop(vm,1)
}


smQuantfThis := !(void^ vm) -> s64
{
	itQ := new quantf()
	sq_setinstanceup(vm,1,itQ)
	sq_setreleasehook(vm,1, vec4ReleaseHook) //TODO to the memory pool
	smQuantsetrot(vm,itQ)
	return 0
}
smQuantsetrot := !(void^ vm,quantf^ toSet) -> void
{
	stSize := sq_gettop(vm)
	if stSize > 1
	{
		itAng := float
		sq_getfloat(vm,2,itAng&)
		if stSize == 5
		{
			itVec := vec4f
			sq_getfloat(vm,3,itVec.x&)
			sq_getfloat(vm,4,itVec.y&)
			sq_getfloat(vm,5,itVec.z&)
		}
	}
}

smAddQuantf := !(void^ vm) -> void
{
	sq_pushroottable(vm)
	sq_pushstring(vm,"quantf",-1)
	sq_newclass(vm,false)

	//sq_pushstring(vm,"SetPos",-1)
	//sq_newclosure(vm,PropSetPos,0)
	//sq_newslot(vm,-3,false)
	
	sq_pushstring(vm,"constructor",-1)
	sq_newclosure(vm,smQuantfThis,0)
	sq_newslot(vm,-3,false)

	sq_newslot(vm,-3,false)
	sq_pop(vm,1)
}
smAddCentf := !(void^ vm) -> void
{
	sq_pushroottable(vm)
	sq_pushstring(vm,"centf",-1)
	sq_newclass(vm,false)

	//sq_pushstring(vm,"SetPos",-1)
	//sq_newclosure(vm,PropSetPos,0)
	//sq_newslot(vm,-3,false)
	
	//sq_pushstring(vm,"constructor",-1)
	//sq_newclosure(vm,PropSetPos,0)
	//sq_newslot(vm,-3,false)

	sq_newslot(vm,-3,false)
	sq_pop(vm,1)
}

smDegreeTr := !(void^ vm) -> s64
{
	fl := float
	sq_getfloat(vm,2,fl&)
	sq_pushfloat(vm,fl*3.14/180)
	return 1
}

ScriptInitMath := !(void^ vm) -> void
{
	sq_pushroottable(vm)
	sq_pushstring(vm,"Degree",-1)
	sq_newclosure(vm,smDegreeTr,0)
	sq_newslot(vm,-3,false)
	sq_pop(vm,1)

	smAddVec4(vm)
	//smAddQuantf(vm)
	//smAddCentf(vm)
}

