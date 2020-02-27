
sq_open := !(s64)^ -> void^
sq_close := !(void^)^ -> void^
sq_getvmstate := !(void^)^ -> s64
sq_newthread := !(void^,s64)^ -> void^
sq_compilebuffer := !(void^,char^,u64,char^,bool)^ -> int
sq_call := !(void^,s64,bool,bool)^ -> int
sq_resume := !(void^,bool,bool)^ -> int
sq_wakeupvm := !(void^,bool,bool,bool,bool)^ -> int
sq_suspendvm := !(void^)^ -> s64

sq_pushroottable := !(void^)^ -> int
sq_pushregistrytable := !(void^)^ -> int
sq_setprintfunc := !(void^,void^,void^)^-> int

sqstd_register_bloblib := !(void^)^->void
sqstd_register_iolib := !(void^)^->void
sqstd_register_systemlib := !(void^)^->void
sqstd_register_mathlib := !(void^)^->void
sqstd_register_stringlib := !(void^)^->void
sqstd_seterrorhandlers := !(void^)^->void

sq_move := !(void^,void^,s64)^ -> void
sq_gettype := !(void^,s64)^ -> s64
sq_gettop := !(void^)^ -> s64
sq_getstackobj := !(void^,s64,void^)^ -> void
sq_pushobject2 := !(void^,void^)^ -> void
sq_addref := !(void^,void^)^ -> void
sq_release := !(void^,void^)^ -> void

sq_newclosure := !(void^,void^,int)^ -> void
sq_newclass := !(void^,bool)^ -> void
sq_setparamscheck := !(void^,s64,char^)^ -> void
sq_setnativeclosurename := !(void^,s64,char^)^ -> void
sq_newslot := !(void^,s64,bool)^->s64

sq_get := !(void^,s64)^ -> s64
sq_createinstance := !(void^,s64)^ -> void
sq_setinstanceup := !(void^,s64,void^)^ -> void
sq_getinstanceup := !(void^,s64,void^^,s64)^ -> void
sq_pushinteger := !(void^,s64)^ -> void
sq_pushstring := !(void^,char^,s64)^ -> void
sq_getinteger := !(void^,s64,s64^)^ -> void
sq_getfloat := !(void^,s64,float^)^ -> void
sq_getthread := !(void^,s64,void^^)^ -> void
sq_pop := !(void^,s64)^ -> void

SqInit := !() -> void
{
	sLib := Library("libsquirrel3m.so","squirrel3m.dll")

	sq_open = sLib.Get("sq_open")
	sq_close = sLib.Get("sq_close")
	sq_getvmstate = sLib.Get("sq_getvmstate")
	sq_newthread = sLib.Get("sq_newthread")
	sq_compilebuffer = sLib.Get("sq_compilebuffer")
	sq_call = sLib.Get("sq_call")
	sq_resume = sLib.Get("sq_resume")
	sq_wakeupvm = sLib.Get("sq_wakeupvm")
	sq_suspendvm = sLib.Get("sq_suspendvm")

	sq_pushroottable = sLib.Get("sq_pushroottable")
	sq_pushregistrytable = sLib.Get("sq_pushregistrytable")
	sq_setprintfunc = sLib.Get("sq_setprintfunc")

	sq_gettop = sLib.Get("sq_gettop")
	sq_gettype = sLib.Get("sq_gettype")
	sq_move = sLib.Get("sq_move")
	sq_getstackobj = sLib.Get("sq_getstackobj")
	sq_pushobject2 = sLib.Get("sq_pushobject2")
	sq_addref = sLib.Get("sq_addref")
	sq_release = sLib.Get("sq_release")
	
	sq_newclosure = sLib.Get("sq_newclosure")
	sq_newclass = sLib.Get("sq_newclass")
	sq_setparamscheck = sLib.Get("sq_setparamscheck")
	sq_setnativeclosurename = sLib.Get("sq_setnativeclosurename")
	sq_newslot = sLib.Get("sq_newslot")
	
	sq_get = sLib.Get("sq_get")
	sq_createinstance = sLib.Get("sq_createinstance")
	sq_setinstanceup = sLib.Get("sq_setinstanceup")
	sq_getinstanceup = sLib.Get("sq_getinstanceup")
	sq_pushinteger = sLib.Get("sq_pushinteger")
	sq_pushstring = sLib.Get("sq_pushstring")
	sq_getinteger = sLib.Get("sq_getinteger")
	sq_getfloat = sLib.Get("sq_getfloat")
	sq_getthread = sLib.Get("sq_getthread")
	sq_pop = sLib.Get("sq_pop")

	sqstd_register_bloblib  = sLib.Get("sqstd_register_bloblib")
	sqstd_register_iolib  = sLib.Get("sqstd_register_iolib")
	sqstd_register_systemlib  = sLib.Get("sqstd_register_systemlib")
	sqstd_register_mathlib   = sLib.Get("sqstd_register_mathlib")
	sqstd_register_stringlib  = sLib.Get("sqstd_register_stringlib")
	sqstd_seterrorhandlers  = sLib.Get("sqstd_seterrorhandlers")
}

vprintf := !(char^ a,void^ b) -> int declare

PrintData := !(void^ vm, char^ frmt,...) -> void
{
	vars := int[32]
	va_start(vars&)
	vprintf(frmt,vars&)
	va_end(vars&)
}
gScriptErrorCount := 0
PrintDataError := !(void^ vm, char^ frmt,...) -> void
{
	gScriptErrorCount += 1
	vars := int[32]
	va_start(vars&)
	vprintf(frmt,vars&)
	va_end(vars&)
}

iRunScript := !(ScriptThread^ itScriptT) -> bool
{
	vm := itScriptT.thrdVM
	itProp := itScriptT.thrdProp
	vmState := sq_getvmstate(vm)
	switch(vmState->{int})
	{
		case 0: //IDLE
			ScriptSetProp(itScriptT,vm)
			//sq_pushroottable(vm)
			sq_call(vm,1,true,true)
		case 2: //SUSPENDED
			ScriptSetProp(itScriptT,gMainVM)
			sq_wakeupvm(vm,false,false,true,false)
	}
	vmState = sq_getvmstate(vm)
	if vmState == 0 itScriptT.Destroy()
	return vmState == 0
}

gsNowScript := ScriptThread^

CodeSpawn := !(void^ vm) -> int
{
	newNode := ref gsNowScript.thrdProp.sThreads.Emplace()
	newNode.thrdProp = gsNowScript.thrdProp
	
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

	if gHotload
		gsNowScript.thrdProp.modelPtr.hlRunThreads.Insert(newNode&)

	
	newEv := gUV.Once(0,newNode&,(x) => {
		asSc := x->{ScriptThread^}
		asSc.onceTimer = null
		asProp := asSc.thrdProp
		iRunScript(asSc)
	})
	newNode.onceTimer = newEv
	return 0
}
//The typemask consists in a zero teminated string that represent the expected parameter type. 
//The types are expressed as follows: 'o' null, 'i' integer, 'f' float, 'n' integer or float, 's' string, 't' table, 'a' array, 'u' userdata,
//'c' closure and nativeclosure, 'g' generator, 'p' userpointer, 'v' thread, 'x' instance(class instance), 'y' class, 'b' bool. and '.' any type. 
//The symbol '|' can be used as 'or' to accept multiple types on the same parameter. There isn't any limit on the number of 'or' that can be used. 
//Spaces are ignored so can be inserted between types to increase readbility. 
//For instance to check a function that espect a table as 'this' a string as first parameter and a number or a userpointer as second parameter, 
//the string would be "tsn|p" (table,string,number or userpointer). If the parameters mask is contains less parameters than 'nparamscheck' the remaining parameters will not be typechecked.

ScriptThread := class
{
	thrdObj := s64[16]
	thrdVM := void^
	thrdProp := vProp^

	onceTimer := uvTimer^

	this := !() -> void
	{
	}
	Destroy := !() -> void
	{
		if gHotload
		{
			thrdProp.modelPtr.hlRunThreads.Remove(this&)
		}
		if onceTimer != null
		{
			onceTimer.Stop()
		}
		
		sq_release(gMainVM,thrdObj&)
		//sq_close(thrdVM)

		i := 0
		for thrdProp.sThreads
		{
			if it& == this&
			{
				thrdProp.sThreads.DeleteAt(i)
				break
			}
			i += 1
		}
	}
}

gMainVM  := void^

ScriptSetProp := !(ScriptThread^ sThread,void^ vm) -> void
{
	itProp := sThread.thrdProp
	gsNowScript = sThread
	sq_pushroottable(vm)
	sq_pushstring(vm,"Prop",-1)
	sq_get(vm,-2)
	sq_pushstring(vm,"ThisProp",-1)
	sq_createinstance(vm,-2)
	sq_setinstanceup(vm,-1,itProp)
	sq_newslot(vm,-4,false)
	sq_pop(vm,2)
}

ScriptInit := !() -> void
{
	SqInit()

	gMainVM = sq_open(512)

	sq_setprintfunc(gMainVM,PrintData,PrintDataError)
	sq_pushroottable(gMainVM)
	sqstd_seterrorhandlers(gMainVM)	

	InitPropClass(gMainVM)
	ScriptInitGlobals(gMainVM)
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

PropSetPos := !(void^ vm) -> void
{
	itProp := vProp^
	sq_getinstanceup(vm,1,itProp&->{void^^},0)
	resPos := vec4f(0.0f,0.0f,0.0f,1.0f)
	midlObj := float
	sq_getfloat(vm,2,midlObj&)
	resPos.x = midlObj
	sq_getfloat(vm,3,midlObj&)
	resPos.y = midlObj
	sq_getfloat(vm,4,midlObj&)
	resPos.z = midlObj

	itProp.modelPos.pos = resPos
}


InitPropClass := !(void^ vm) -> void
{
	sq_pushroottable(vm)
	sq_pushstring(vm,"Prop",-1)
	sq_newclass(vm,false)

	sq_pushstring(vm,"SetPos",-1)
	sq_newclosure(vm,PropSetPos,0)
	sq_newslot(vm,-3,false)
	
	sq_pushstring(vm,"constructor",-1)
	sq_newclosure(vm,PropSetPos,0)
	sq_newslot(vm,-3,false)

	sq_newslot(vm,-3,false)
	sq_pop(vm,1)
}

ScriptUnit := class
{
	funcObj := s64[2]
}

ScriptCompile := !(vRepoFile^ itF) -> ScripObj^
{
	mp := itF.Map()
	defer itF.Unmap()
	
	res1 := sq_compilebuffer(gMainVM,mp,itF.Size(),itF.objName.Str(),true) ; $temp
	if res1 != 0
	{
		printf("failed compile\n")
		return null
	}
	scrp := new ScriptUnit
	sq_getstackobj(gMainVM,-1,scrp.funcObj&)
	sq_addref(gMainVM,scrp.funcObj&)
	sq_pop(gMainVM,1)

	return scrp
}
ScriptRun := !(ScriptUnit^ unit,vProp^ itProp) -> void
{
	newThread := ref itProp.sThreads.Emplace()
	newThread.thrdVM = sq_newthread(gMainVM,1024)
	newThread.thrdProp = itProp
	sq_getstackobj(gMainVM,-1,newThread.thrdObj&)
	sq_addref(gMainVM,newThread.thrdObj&)
	sq_pop(gMainVM,1)
	
	sq_pushobject2(newThread.thrdVM,unit.funcObj&)
	sq_pushroottable(newThread.thrdVM)
	iRunScript(newThread&)
}

iScriptASleep := !(void^ vm) -> s64
{
	waitTime := float
	sq_getfloat(vm,2,waitTime&)
	if(waitTime < 0) waitTime = 0
	newOnce := gUV.Once(waitTime,gsNowScript,(x) => {
		if gQuit return void //TODO: remove
		fO := int[2] //TODO: remove stack protection?
		asSc := x->{ScriptThread^}
		asSc.onceTimer = null
		iRunScript(asSc)
	})
	gsNowScript.onceTimer = newOnce
	resVal := sq_suspendvm(vm)
	return resVal
}
