
CodeSpawn := !(void^ vm) -> int
{
	newNode := ref gsNowScript.thrdVObject.sThreads.Emplace()
	newNode.thrdVObject = gsNowScript.thrdVObject
	
	res2 := sq_gettype(vm,2)
	if res2 == 0x8001000
	{
		sq_getstackobj(vm,2,newNode.thrdObj&)
		sq_addref(vm,newNode.thrdObj&)

		sq_getthread(vm,2,newNode.thrdVM&)
	}else{
		//0x8000100
		vm2 := vm
		newNode.thrdVM = sq_newthread(vm2,1024)
		sq_getstackobj(vm2,-1,newNode.thrdObj&)
		sq_addref(vm2,newNode.thrdObj&)
		sq_pop(vm2,1)
		sq_move(newNode.thrdVM,vm2,2)
		sq_pushroottable(newNode.thrdVM)
	}
	TSpawnTask(() ==>[newNode&] {
		asProp := newNode.thrdVObject
		iRunScript(newNode&)
	})
	return 0
}

iScriptASleep := !(void^ vm) -> s64
{
	waitTime := float
	sq_getfloat(vm,2,waitTime&)
	if(waitTime < 0) waitTime = 0
	TSleep(waitTime)
	//newOnce := gUV.Once(waitTime,gsNowScript,(x) => {
	//	if gQuit return void //TODO: remove
	//	asSc := x->{ScriptThread^}
	//	asSc.onceTimer = null
	//	iRunScript(asSc)
	//})
	//resVal := sq_suspendvm(vm)
	//return resVal
	return 0
}

ScriptInitGlobals := !(void^ vm) -> void
{
	sq_pushroottable(vm)

	sq_pushstring(vm,"Spawn",-1)
	sq_newclosure(vm,CodeSpawn,0)
	sq_setparamscheck(vm,2,".v|c")
	sq_setnativeclosurename(vm,2,"Spawn")
	sq_newslot(vm,-3,true)

	sq_pushstring(vm,"ASleep",-1)
	sq_newclosure(vm,iScriptASleep,0)
	sq_setparamscheck(vm,2,".f")
	sq_setnativeclosurename(vm,2,"ASleep")
	sq_newslot(vm,-3,true)

	sq_pop(vm,1)
}
