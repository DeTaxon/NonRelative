//printf := !(char^ a,...) -> int declare



main := !(int argc, char^^ argv)
{
	tst := RBSet.{int}() ; $keep
	arr := int[1024]
	k := 40
	while true
	{	
		k--
		if k < 0 return 0
			
		ab := rand() % 1024
		if ab > 256 or tst.Size() == 0
		{
			l := rand() % 1024
			tst << l
		}else{
			arr[i] = tst[^i]
			c := rand() % tst.Size()
			cc := arr[c]
			tst.Remove(cc)
		}
	}

	return 0
}

//uv_default_loop := !()^ -> void^
//uv_run := !(void^,int)^ -> void 
//
//uv_timer_init := !(void^,void^)^ -> void
//uv_timer_start := !(void^,void^,int,int)^ -> void
//uv_timer_stop := !(void^)^ -> void
//
//c := 14
//
//deLoop := !(void^ hndl) -> void
//{
//	printf("heh\n")
//	c-- 
//	if c < 0
//		uv_timer_stop(hndl)
//}
//main := !() -> int
//{
//	uv := Library("libuv.so.1")
//
//	uv_default_loop = uv.Get("uv_default_loop")
//	uv_run = uv.Get("uv_run")
//
//	uv_timer_init = uv.Get("uv_timer_init")
//	uv_timer_start = uv.Get("uv_timer_start")
//	uv_timer_stop = uv.Get("uv_timer_stop")
//	
//	loop := uv_default_loop()
//
//	dat := char[1024]
//	uv_timer_init(loop,dat[0]&)
//	uv_timer_start(dat[0]&,deLoop,1000,1000)
//
//	uv_run(loop,0)
//
//
//	return 0
//}

