
sq_open := !(s64)^ -> void^
sq_newthread := !(void^,s64)^ -> void^
sq_compilebuffer := !(void^,char^,u64,char^,bool)^ -> int
sq_call := !(void^,s64,bool,bool)^ -> int
sq_resume := !(void^,bool,bool)^ -> int
sq_wakeupvm := !(void^,bool,bool)^ -> int
sq_suspend := !(void^)^ -> void

sq_pushroottable := !(void^)^ -> int
sq_setprintfunc := !(void^,void^,void^)^-> int

sq_close := !(void^)^ -> void

sqstd_register_bloblib := !(void^)^->void
sqstd_register_iolib := !(void^)^->void
sqstd_register_systemlib := !(void^)^->void
sqstd_register_mathlib := !(void^)^->void
sqstd_register_stringlib := !(void^)^->void
sqstd_seterrorhandlers := !(void^)^->void

sq_getstackobj := !(void^,s64,void^)^ -> void
sq_pushobject := !(void^,void^)^ -> void
sq_addref := !(void^,void^)^ -> void

sq_newclosure := !(void^,void^,int)^ -> void
sq_setparamscheck := !(void^,s64,char^)^ -> void
sq_setnativeclosurename := !(void^,s64,char^)^ -> void
sq_newslot := !(void^,s64,bool)^->void

sq_pushinteger := !(void^,s64)^ -> void
sq_pushstring := !(void^,char^,s64)^ -> void
sq_getinteger := !(void^,s64,s64^)^ -> void
sq_getthread := !(void^,s64,void^^)^ -> void

ScriptInit := !() -> void
{
	sLib := Library("libsquirrel3.so")
	sq_open = sLib.Get("sq_open")
	sq_newthread = sLib.Get("sq_newthread")
	sq_compilebuffer = sLib.Get("sq_compilebuffer")
	sq_call = sLib.Get("sq_call")
	sq_resume = sLib.Get("sq_resume")
	sq_wakeupvm = sLib.Get("sq_wakeupvm")
	sq_pushroottable = sLib.Get("sq_pushroottable")
	sq_setprintfunc = sLib.Get("sq_setprintfunc")

	sq_close = sLib.Get("sq_close")

	sq_getstackobj = sLib.Get("sq_getstackobj")
	sq_pushobject = sLib.Get("sq_pushobject")
	sq_addref = sLib.Get("sq_addref")
	
	sq_newclosure = sLib.Get("sq_newclosure")
	sq_setparamscheck = sLib.Get("sq_setparamscheck")
	sq_setnativeclosurename = sLib.Get("sq_setnativeclosurename")
	sq_newslot = sLib.Get("sq_newslot")
	
	sq_pushinteger = sLib.Get("sq_pushinteger")
	sq_pushstring = sLib.Get("sq_pushstring")
	sq_getinteger = sLib.Get("sq_getinteger")
	sq_getthread = sLib.Get("sq_getthread")

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



hqObj := s64[32]
thrd := void^
CodeSpawn := !(void^ vm) -> int
{
	printf("Spawn:hello\n")
	sq_getstackobj(vm,2,hqObj&)
	sq_addref(vm,hqObj&)
	sq_getthread(vm,2,thrd&)
	return 0
}

// The typemask consists in a zero teminated string that represent the expected parameter
//type. The types are expressed as follows: ‘o’ null, ‘i’ integer, ‘f’ float, ‘n’ integer or float, ‘s’
//string, ‘t’ table, ‘a’ array, ‘u’ userdata, ‘c’ closure and nativeclosure, ‘g’ generator, ‘p’ user-
//pointer, ‘v’ thread, ‘x’ instance(class instance), ‘y’ class, ‘b’ bool. and ‘.’ any type. The symbol
//‘|’ can be used as ‘or’ to accept multiple types on the same parameter. There isn’t any limit
//on the number of ‘or’ that can be used. Spaces are ignored so can be inserted between types
//to increase readbility. For instance to check a function that espect a table as ‘this’ a string as
//first parameter and a number or a userpointer as second parameter, the string would be “tsn|p”
//(table,string,number or userpointer). If the parameters mask is contains less parameters than
//‘nparamscheck’ the remaining parameters will not be typechecked

ScriptTest := !() -> void
{
	try
	{
		ScriptInit()

		vm := sq_open(1024)
		sq_setprintfunc(vm,PrintData,PrintData)

		fil := MappedFile("test.nut")

		sq_pushroottable(vm)
		sqstd_seterrorhandlers(vm)
		
		sq_pushroottable(vm)
		sq_pushstring(vm,"Spawn",-1)
		sq_newclosure(vm,CodeSpawn,0)
		sq_setparamscheck(vm,2,".v")
		sq_setnativeclosurename(vm,2,"Spawn")
		sq_newslot(vm,-3,true)
		
		sq_compilebuffer(vm,fil.Get(),fil.Size(),"test",true)
		sq_pushroottable(vm)
		res := sq_call(vm,1,true,true)
		printf("heh %i\n",res)

		sq_pushroottable(thrd)
		sq_call(thrd,1,true,true)

		while true
		{
			sq_wakeupvm(thrd,true,true)
			//sq_pushobject(vm,hqObj)
			//sq_pushroottable(vm)
			//res2 := sq_call(vm,1,true,true)
			//printf("res2 %i\n",res2)
			TSleep(1)
		}

		printf("end\n")

	}catch(IException^ e)
	{
		printf("Exception: %s\n",e.Msg())
	}
}

