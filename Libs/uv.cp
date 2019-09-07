
uv_default_loop := !()^ -> void^
uv_run := !(void^,int)^ -> void 

uv_timer_init := !(void^,void^)^ -> void
uv_timer_start := !(void^,!(void^)^->void,int,int)^ -> void
uv_timer_stop := !(void^)^ -> void


uvInit := !() -> void
{
	uv := Library("libuv.so.1")

	uv_default_loop = uv.Get("uv_default_loop")
	uv_run = uv.Get("uv_run")

	uv_timer_init = uv.Get("uv_timer_init")
	uv_timer_start = uv.Get("uv_timer_start")
	uv_timer_stop = uv.Get("uv_timer_stop")
}

uvLoop := class
{
	loopPtr := void^
	this := !() -> void
	{
		loopPtr = uv_default_loop()
	}
	Run := !() -> void
	{
		uv_run(loopPtr,0)
	}
	Get := !() -> void^
	{
		return loopPtr
	}
	Timer := !(double timeout,double repeat,!(uvTimer^)^-> void callb) -> uvTimer^
	{
		toRet := new uvTimer
		uv_timer_init(loopPtr,toRet)
		uv_timer_start(toRet,callb,timeout*1000.0,repeat*1000.0)
		return toRet
	}
}

uvTimer := class
{
	buffer := char[1024]
	Stop := !() -> void
	{
		uv_timer_stop(buffer[0]&)
	}
	
}
