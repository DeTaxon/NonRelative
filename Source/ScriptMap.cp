
scriptCurrentMap := vMap^

ScriptSpawnProp := !(void^ vm) -> s64
{
	itStr := char^
	sq_getstring(vm,2,itStr&)
	//if not gsNowScript.thrdVObject is vMap
	//	return 0
	propObj := gsNowScript.thrdVObject->{vMap^}.AddProp(itStr)
	if propObj != null
	{
		//TODO: remove class after use
		gsClassProp.Push(vm)
		sq_createinstance(vm,-1)
		sq_setinstanceup(vm,-1,propObj)
		return 1
	}
	return 0
}
ScriptSpawnProp2 := !(void^ vm) -> s64
{
	itStr := char^
	sq_getstring(vm,2,itStr&)
	//if not gsNowScript.thrdVObject is vMap
	//	return 0
	nowMap := vMap^()
	sq_getinstanceup(vm,1,nowMap&->{void^},0)
	propObj := nowMap.AddProp(itStr)
	if propObj != null
	{
		//TODO: remove class after use
		gsClassProp.Push(vm)
		sq_createinstance(vm,-1)
		sq_setinstanceup(vm,-1,propObj)
		return 1
	}
	return 0
}

gsClassMap := SqObject

ScriptLoadMap := !(void^ vm) -> s64
{
		
	itStr := char^
	sq_getstring(vm,2,itStr&)
	itMap := vGetMap(itStr)
	if itMap == null
		return 0
	if gPlayerMap == null
	{
		gPlayerMap = itMap
	}
	gsClassMap.Push(vm)
	sq_createinstance(vm,-1)
	sq_setinstanceup(vm,-1,itMap)

	return 1
}

ScriptInitMap := !(void^ vm) -> void
{
	sq_pushroottable(vm)

	sq_pushstring(vm,"SpawnProp",-1)
	sq_newclosure(vm,ScriptSpawnProp,0)
	sq_setparamscheck(vm,2,".s")
	sq_setnativeclosurename(vm,2,"SpawnProp")
	sq_newslot(vm,-3,true)

	sq_pushstring(vm,"GetMap",-1)
	sq_newclosure(vm,ScriptLoadMap,0)
	sq_setparamscheck(vm,2,".s")
	sq_setnativeclosurename(vm,2,"GetMap")
	sq_newslot(vm,-3,true)

	sq_pushstring(vm,"Map",-1)
	sq_newclass(vm,false)

	gsClassMap.Get(vm,-1)
	gsClassMap.IncRef(vm)

	sq_pushstring(vm,"SpawnProp",-1)
	sq_newclosure(vm,ScriptSpawnProp2,0)
	sq_setparamscheck(vm,2,".s")
	sq_setnativeclosurename(vm,2,"SpawnProp")
	sq_newslot(vm,-3,true)

	sq_newslot(vm,-3,false)

	sq_pop(vm,1)
}


