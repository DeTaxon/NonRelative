
scriptCurrentMap := vMap^

ScriptSpawnProp := !(void^ vm) -> void
{
	itStr := char^
	sq_getstring(vm,1,itStr&)
	printf("%s\n",itStr)
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


