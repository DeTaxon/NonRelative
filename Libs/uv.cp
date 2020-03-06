
uv_default_loop := !()^ -> void^
uv_run := !(void^,int)^ -> void 

uv_timer_init := !(void^,void^)^ -> void
uv_timer_start := !(void^,!(void^)^->void,int,int)^ -> void
uv_timer_stop := !(void^)^ -> void

uv_idle_init := !(void^,void^)^ -> void
uv_idle_start := !(void^,!(void^)^->void)^ -> void
uv_idle_stop := !(void^)^ -> void

uv_ip4_addr := !(char^,int,void^)^ -> void

uv_udp_init := !(void^,void^)^ -> void
uv_udp_bind := !(void^,void^,int)^ -> void
uv_udp_recv_start := !(void^,!(uvUDP^,u64,uv_buf_t^)^ ->void^, !(void^,int,uv_buf_t^,uvAddr^,int)^-> void)^ -> void
uv_udp_send := !(void^,void^,void^,int,void^,!(void^,int)^->void)^ -> void

uv_tcp_init := !(void^,void^)^ -> void
uv_tcp_bind := !(void^,uvAddr^,int)^ -> void
uv_tcp_connect := !(void^,void^,uvAddr^,!(void^,int)^->void)^->void

uv_close := !(void^,!(void^)^->void)^ -> void

uv_listen := !(void^,int,!(void^,int)^->void)^ -> void
uv_accept := !(void^,void^)^ -> void

uv_read_start := !(void^,!(void^,u64,uv_buf_t)^->void,!(void^,u64,uv_buf_t^)^->void)^-> void
uv_write := !(void^,void^,uv_buf_t^,int,!(void^,int)^->void)^->void

uv_queue_work := !(void^,void^,!(void^)^-> void,!(void^,int)^-> void)^ -> void

uv_thread_create := !(void^,!(void^)^->void,void^)^ -> void
uv_thread_join := !(void^)^ -> void

uv_async_init := !(void^,void^,!(void^)^->void)^ -> void
uv_async_send := !(void^)^ -> void

uv_fs_event_init := !(void^,void^)^ -> void
uv_fs_event_start := !(void^,!(void^,char^,int,int)^->void,char^,int)^ -> void
uv_fs_event_stop := !(void^)^ -> void

uv_barrier_init := !(void^,int)^->void
uv_barrier_wait := !(void^)^-> void
uv_barrier_destroy := !(void^)^ -> void

uv_mutex_init := !(void^)^ -> void
uv_mutex_lock := !(void^)^ -> void
uv_mutex_trylock := !(void^)^ -> int
uv_mutex_unlock := !(void^)^ -> void
uv_mutex_destroy := !(void^)^-> void

uv_cond_init := !(void^)^ -> void
uv_cond_signal := !(void^)^ -> void
uv_cond_broadcast := !(void^)^ -> void
uv_cond_wait := !(void^,void^)^-> void
uv_cond_timedwait := !(void^,void^,u64)^ -> int
uv_cond_destroy  := !(void^)^ -> void

uv_sem_init := !(void^,int)^ -> void
uv_sem_post := !(void^)^ -> void
uv_sem_wait := !(void^)^ -> int
uv_sem_trywait := !(void^)^ -> int
uv_sem_destroy := !(void^)^ -> void

uv_fs_stat := !(void^,void^,char^,!(void^)^->void)^ -> void

//sizeof(uv_tcp_t) = 248
//sizeof(sockaddr) = 16
//sizeof(uv_udp_t) = 216
//uv_buf_t = 16

uv_timespec_t := class
{
	tv_sec := c_long
	tv_nsec := c_long

	ToDouble := !() -> double
	{
		return tv_sec->{double} + tv_nsec->{double}*0.0000000001
	}
}

uv_stat_t := class
{
	st_dev := u64
	st_mode := u64
	st_nlink := u64
	st_uid := u64
	st_gid := u64
	st_rdev := u64
	st_ino := u64
	st_size := u64
	st_blksize := u64
	st_blocks := u64
	st_flags := u64
	st_gen := u64
	st_atim := uv_timespec_t
	st_mtim := uv_timespec_t
	st_ctim := uv_timespec_t
	st_birthtim := uv_timespec_t
}

uv_buf_t := class
{
	ptr := void^
	len := u64
}

uvInit := !() -> void
{
	uv := Library("libuv.so.1","libuv-1.dll")

	uv_tcp_init = uv.Get("uv_tcp_init")
	uv_tcp_bind = uv.Get("uv_tcp_bind")
	uv_tcp_connect = uv.Get("uv_tcp_connect")

	uv_queue_work = uv.Get("uv_queue_work")

	uv_listen = uv.Get("uv_listen")
	uv_accept = uv.Get("uv_accept")

	uv_read_start = uv.Get("uv_read_start")
	uv_write = uv.Get("uv_write")

	uv_default_loop = uv.Get("uv_default_loop")
	uv_run = uv.Get("uv_run")

	uv_timer_init = uv.Get("uv_timer_init")
	uv_timer_start = uv.Get("uv_timer_start")
	uv_timer_stop = uv.Get("uv_timer_stop")

	uv_idle_init = uv.Get("uv_idle_init")
	uv_idle_start = uv.Get("uv_idle_start")
	uv_idle_stop = uv.Get("uv_idle_stop")

	uv_ip4_addr = uv.Get("uv_ip4_addr")

	uv_udp_init = uv.Get("uv_udp_init")
	uv_udp_bind = uv.Get("uv_udp_bind")
	uv_udp_recv_start = uv.Get("uv_udp_recv_start")
	uv_udp_send = uv.Get("uv_udp_send")

	uv_close = uv.Get("uv_close")

	uv_thread_create = uv.Get("uv_thread_create")
	uv_thread_join = uv.Get("uv_thread_join")

	uv_async_init = uv.Get("uv_async_init")
	uv_async_send = uv.Get("uv_async_send")

	uv_fs_event_init = uv.Get("uv_fs_event_init")
	uv_fs_event_start = uv.Get("uv_fs_event_start")
	uv_fs_event_stop = uv.Get("uv_fs_event_stop")

	uv_barrier_init = uv.Get("uv_barrier_init")
	uv_barrier_wait = uv.Get("uv_barrier_wait")
	uv_barrier_destroy = uv.Get("uv_barrier_destroy")

	uv_mutex_init = uv.Get("uv_mutex_init") 
	uv_mutex_lock = uv.Get("uv_mutex_lock")
	uv_mutex_trylock = uv.Get("uv_mutex_trylock")
	uv_mutex_unlock = uv.Get("uv_mutex_unlock")
	uv_mutex_destroy = uv.Get("uv_mutex_destroy")

	uv_cond_init = uv.Get("uv_cond_init")
	uv_cond_signal = uv.Get("uv_cond_signal")
	uv_cond_broadcast = uv.Get("uv_cond_broadcast")
	uv_cond_wait = uv.Get("uv_cond_wait")
	uv_cond_timedwait = uv.Get("uv_cond_timedwait")
	uv_cond_destroy = uv.Get("uv_cond_destroy")

	uv_sem_init 	= uv.Get("uv_sem_init")
	uv_sem_post 	= uv.Get("uv_sem_post")
	uv_sem_wait 	= uv.Get("uv_sem_wait")
	uv_sem_trywait 	= uv.Get("uv_sem_trywait")
	uv_sem_destroy 	= uv.Get("uv_sem_destroy")

	uv_fs_stat 	= uv.Get("uv_fs_stat")
}

uvSem := class
{
	uvBuffer := char[1024] //TODO check size
	"this" := !(int count) -> void
	{
		uv_sem_init(this&,count)
	}
	Lock := !() -> void
	{
		uv_sem_wait(this&)
	}
	TryLock := !() -> bool
	{
		return uv_sem_trywait(this&) == 0
	}
	Unlock := !() -> void
	{
		uv_sem_post(this&)
	}
	Destroy := !() -> void
	{
		uv_sem_destroy(this&)
	}
}

uvCond := class
{
	uvBuffer := char[1024] // TODO: check size
	"this" := !() -> void
	{
		uv_cond_init(this&)
	}
	Wait := !(uvMutex mtx) -> void
	{
		uv_cond_wait(this&,mtx&)
	}
	Wait := !(uvMutex mtx,double time) -> bool
	{
		itTm := time*1000.0
		return uv_cond_timedwait(this&,mtx&,itTm->{s32}->{u32}->{u64}) == 0
	}
	Notify := !() -> void
	{
		uv_cond_signal(this&)
	}
	NotifyAll := !() -> void
	{
		uv_cond_broadcast(this&)
	}
}

uvMutex := class
{
	uvBuffer := char[1024] //TODO: check size
	"this" := !() -> void
	{
		uv_mutex_init(this&)
	}
	Lock := !() -> void
	{
		uv_mutex_lock(this&)
	}
	TryLock := !() -> bool
	{
		return uv_mutex_trylock(this&) == 0
	}
	Unlock := !() -> void
	{
		uv_mutex_unlock(this&)
	}
	Destroy := !() -> void
	{
		uv_mutex_destroy(this&)
	}
}


uvThread := class
{
	uvBuffer := char[1000] // TODO: check size
	itLambda := !()&->void
	"this" := !(void^ arg,!(void^)^->void thrBuf) -> void
	{
		uv_thread_create(this&,thrBuf,arg)
	}
	"this" := !(!()&->void thrd) -> void
	{
		itLambda = thrd
		uv_thread_create(this&,x => {
			x->{uvThread^}.itLambda()
		},this&)
	}
	Join := !() -> void
	{
		uv_thread_join(this&)
	}
}
uvEvent := class
{
	uvBuffer := char[1000]
	loopP := void^
	itLambda := !()& -> void

	"this" := !(void^ lp, !()&->void thrd) -> void
	{
		itLambda = thrd
		uv_async_init(lp,this&,x => {
			x->{uvEvent^}.itLambda()
		})
	}
	Emit := !() -> void
	{
		uv_async_send(this&)
	}
	Close := !() -> void
	{
		uv_close(this&,null)
	}
	
}
uvWork := class
{
	uvBuffer := char[256]
}
uvTCP := class
{	
	buffer := char[256]
	accpCallb := !(uvTCP^,uvTCP^)&->void
	connectCallb := !(uvTCP^)& ->void
	readCallb := !(uvTCP^,int)& -> void
	loopPtr := uvLoop^
	recvBuf := uv_buf_t
	bgh := char[256]

	Loop := !() -> uvLoop^
	{
		return loopPtr
	}
	Recv := !(@InTyp[] inBuf, !(uvTCP^,int)&->void callb) -> void {
		this.Recv(inBuf[0]&,InTyp->TypeSize*inBuf->len,callb)
	}	
	Recv := !(@InTyp[@inSiz] inBuf, !(uvTCP^,int)&->void callb) -> void {
		this.Recv(inBuf[0]&,InTyp->TypeSize*inSiz,callb)
	}	
	Recv := !(void^ bufferIn,int siz, !(uvTCP^,int)&->void callb) -> void
	{
		recvBuf.ptr = bufferIn
		recvBuf.len = siz
		readCallb = callb.Capture()
		uv_read_start(buffer[0]&,(x,y,z) => {
			z.ptr = x->{uvTCP^}.recvBuf.ptr
			z.len = x->{uvTCP^}.recvBuf.len
		},(x,y,z) => {
			x->{uvTCP^}.readCallb(x->{uvTCP^},z[0].len)
		})
	}
	Send := !(void^ bufferIn,int size) -> void
	{	
		recvBuf.ptr = bufferIn
		recvBuf.len = size
		wrt := new char[192]
		uv_write(wrt,this&,recvBuf&,1,(x,y)=>{})
	}
	Listen := !(char^ addr, int port,int backLog, !(uvTCP^,uvTCP^)&->void callb) -> void 
	{
		itAD := loopPtr.IP4(addr,port)
		accpCallb = callb.Capture()
		uv_tcp_bind(this&,itAD&,0)
		uv_listen(this&,backLog, (x,y) => {
			newTcp := x->{uvTCP^}.loopPtr.TCP()
			uv_accept(x,newTcp)
			x->{uvTCP^}.accpCallb(x->{uvTCP^},newTcp)
		})
	}
	Connect := !(char^ addr, int port, !(uvTCP^)&-> void callb) -> void 
	{
		itAD := loopPtr.IP4(addr,port)
		cnct := new uvConnect
		cnct.lpPtr = this&
		connectCallb = callb.Capture()
		uv_tcp_connect(cnct,this&,itAD&,(x,y) => {
			lambPtr := ref x->{uvConnect^}.lpPtr.connectCallb
			x->{uvConnect^}.lpPtr.connectCallb(x->{uvConnect^}.lpPtr)
			//delete x
			lambPtr.Delete()
			//lambPtr = null
			
		})
	}
	Destroy := !() -> void
	{
		//TODO: implement
	}
}

uvUDP := class
{
	buffer := char[256]
	sndBufs := uv_buf_t[16]
	udpSendT := char[320]
	loopPtr := uvLoop^

	Loop := !() -> uvLoop^
	{
		return loopPtr
	}
	Send := !(uvAddr^ somAddr,void^ toSend,u64 sndSize, !(void^,int)^ -> void callb) -> void
	{
		sndBufs[0].ptr = toSend,
		sndBufs[0].len = sndSize
		uv_udp_send(udpSendT[0]&,buffer[0]&,sndBufs[0]&,1,somAddr,callb)
	}
	Recv := !(char^ addr, int port,@At[] toSet, !(uvUDP^,int,uvAddr^,int)& -> void callb) -> uvUDP^ {
		return UDPRecv(addr,port,toSet[0]&,toSet->len*At->TypeSize,callb)
	}
	Recv := !(char^ addr, int port,@At[@Bt] toSet, !(uvUDP^,int,uvAddr^,int)& -> void callb) -> uvUDP^ {
		return UDPRecv(addr,port,toSet[0]&,Bt*At->TypeSize,callb)
	}
	Recv := !(char^ addr, int port,void^ toSet,u64 bufSize, !(uvUDP^,int,uvAddr^,int)& -> void callb) -> uvUDP^
	{
		sndBufs[0].ptr = toSet
		sndBufs[0].len = bufSize
		itAddr := this.IP4(addr,port)
		uv_udp_bind(this&,itAddr&,0)
		uv_udp_recv_start(this&,(x,y,z) => 
		{
			z.ptr = x.sndBufs[0].ptr
			z.len = x.sndBufs[0].len
		},callb)

		return toRet
	}
}

uvIdle := class
{
	buffer := char[120] //  sizeof(uv_idle_t) = 120
	Stop := !() -> void
	{
		uv_idle_stop(buffer)
	}
}

uvAddr := class
{
	buffer := char[128]

	"=" := !(uvAddr toGet) -> void
	{
		memcpy(this&,toGet&,uvAddr->TypeSize)
	}
}

uvConnect := class
{	
	uvBuffer := char[96]
	lpPtr := uvTCP^
}

uvLoop := class
{
	loopPtr := void^
	
	//stAlloc := StaticSizeAllocator.{1024,16}

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
	Event := !(!()&->void callb) -> uvEvent^
	{
		cal := callb.Capture()
		return new uvEvent(loopPtr,cal)
	}
	Timer := !(double timeout,double repeat,!(uvTimer^)&-> void callb) -> uvTimer^
	{
		toRet := new uvTimer
		toRet.timerCallb = callb.Capture()
		uv_timer_init(loopPtr,toRet)
		if (repeat*1000.0)->{int} == 0
		{
			uv_timer_start(toRet,x => {
				x->{uvTimer^}.timerCallb(x->{uvTimer^})
				//TODO: Destroy
				x->{uvTimer^}.timerCallb.Delete()
			},timeout*1000.0,repeat*1000.0)
		}else{
			uv_timer_start(toRet,x => {
				x->{uvTimer^}.timerCallb(x->{uvTimer^})
			},timeout*1000.0,repeat*1000.0)
		}
		return toRet
	}
	Idle := !(!(uvIdle^)^->void callb) -> uvIdle^
	{
		toRet := new uvIdle
		uv_idle_init(loopPtr,toRet)
		uv_idle_start(toRet,callb)
		return toRet
	}
	IP4 := !(char^ addr, int port) -> uvAddr
	{
		uv_ip4_addr(addr,port,result&)
	}
	TCP := !() -> uvTCP^
	{
		toRet := new uvTCP
		uv_tcp_init(loopPtr,toRet)
		toRet.loopPtr = this&
		return toRet
	}
	UDP := !() -> uvUDP^
	{
		toRet := new uvUDP
		toRet.loopPtr = this&
		uv_udp_init(loopPtr,toRet)
		return toRet
	}
	Work := !(!(uvWork^)^-> void callb) -> uvWork^
	{
		itWork := malloc(uvWork->TypeSize)
		uv_queue_work(loopPtr,itWork,callb,(x,y) => {
			free(x)
		})
	}
	Once := !(double timeout,void^ passV,!(void^)^-> void callb) -> uvTimer^
	{
		toRet := new uvTimer
		uv_timer_init(loopPtr,toRet)
		toRet.timerPassValue = passV
		toRet.timerCallbFunc = callb

		uv_timer_start(toRet,x => {
			tmr := x->{uvTimer^}
			if not tmr.stopped
				tmr.timerCallbFunc(x->{uvTimer^}.timerPassValue)
			delete x
		},timeout*1000.0,0)
		
		return toRet
	}
	DirectoryMonitor := !(char^ fileName,!(uvFsEvent^,char^)& ->void clb) -> uvFsEvent^
	{
		preRes := new uvFsEvent
		uv_fs_event_init(loopPtr,preRes)
		preRes.callb = clb.Capture()
		uv_fs_event_start(preRes,(hndl,flName,ev,status) => {
			if ev != 2 return void
			asRealHndl := hndl->{uvFsEvent^}
			asRealHndl.callb(asRealHndl,flName)
		}
		,fileName,0)
		//4 -recursive, not working on linux

		return preRes
	}
	//Stat := !(char^ pth) -> uv_stat_t
	//{
	//	uv_fs_stat(loopPtr,result&,pth,null)
	//}
}
uvTimer := class
{
	buffer := char[1024]
	timerCallb := !(uvTimer^)&-> void
	timerCallbFunc := !(uvTimer^)^-> void
	timerPassValue := void^
	stopped := bool
	Stop := !() -> void
	{
		stopped = true
		uv_timer_stop(buffer[0]&)
	}
	
}
uvFsEvent := class
{
	buf := char[1000]
	callb := !(uvFsEvent^,char^)& -> void

	Stop := !() -> void
	{
		uv_fs_event_stop(this&)
	}

}
