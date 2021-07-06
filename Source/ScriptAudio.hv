
gsClassAudio := SqObject

sqAudioUnmap := !(void^ usrPntr,s64 itSize) -> s64 
{
	
}
sqAudioLoad := !(void^ vm) -> s64
{
	itStr := char^
	sq_getstring(vm,2,itStr&)
	audFile := gRepo.GetFile(itStr)
	if audFile == null return 0
	itAudio := vLoadAudio(audFile)
	if itAudio != null
	{
		sq_setreleasehook(vm,1, sqAudioUnmap)
		sq_setinstanceup(vm,1, itAudio)
	}
	return 0 
}
sqAudioPlay := !(void^ vm) -> s64
{
	itAudio := AudioObject^()
	sq_getinstanceup(vm,1,itAudio&->{void^},0)
	if itAudio != null
	{
		vPlayAudioGlobal(itAudio)
	}
	return 0
}

ScriptInitAudio := !(void^ vm) -> void
{
	sq_pushroottable(vm)
	sq_pushstring(vm,"Audio",-1)
	sq_newclass(vm,false)

	gsClassAudio.Get(vm,-1)
	gsClassAudio.IncRef(vm)

	sq_pushstring(vm,"PlayGlobal",-1)
	sq_newclosure(vm,sqAudioPlay,0)
	sq_newslot(vm,-3,false)

	sq_pushstring(vm,"constructor",-1)
	sq_newclosure(vm,sqAudioLoad,0)
	sq_setparamscheck(vm,2,".s")
	sq_newslot(vm,-3,false)

	sq_newslot(vm,-3,false)
	sq_pop(vm,1)
}
