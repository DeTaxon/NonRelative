
scriptCurrentMap := vMap^

ScriptSpawnProp := !(void^ vm) -> s64
{
	itStr := char^
	sq_getstring(vm,2,itStr&)
	//TODO: check if vMap
	propObj := gsNowScript.thrdVObject->{vMap^}.AddProp(itStr)
	if propObj != null
	{
		//TODO: remove pust root table
		sq_pushroottable(vm)
		sq_pushstring(vm,"Prop",-1)
		sq_get(vm,-2)
		sq_createinstance(vm,-1)
		sq_setinstanceup(vm,-1,propObj)
		return 1
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

	sq_pop(vm,1)
}


