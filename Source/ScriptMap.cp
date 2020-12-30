
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
	return 0
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

	sq_pop(vm,1)
}


