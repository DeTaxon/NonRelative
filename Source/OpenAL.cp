
openAL := Library
alcOpenDevice := !(char^ name)^ -> void^ 
alcCreateContext := !(void^ device,...)^ -> void^
alcMakeContextCurrent := !(void^ conxt)^ -> void

alGenBuffers := !(int count, u32^ num)^ -> void
alBufferData := !(u32 buf,int format,void^ data, int size, int freq)^ -> void

alGenSources := !(int count,u32^ sources)^ -> void
alSourcei := !(u32 source,int param,int value)^ -> void

alSourcePlay := !(u32 source)^ -> void

AL_LOOPING := 0x1007
AL_BUFFER := 0x1009

AL_FORMAT_MONO16 := 0x1101

OpenALInit := !() -> void
{
	openAL.Open("libopenal.so","libopenal-1.dll","soft_oal.dll")

	alcOpenDevice = openAL.Get("alcOpenDevice")
	alcCreateContext = openAL.Get("alcCreateContext")
	alcMakeContextCurrent = openAL.Get("alcMakeContextCurrent")

	alGenBuffers = openAL.Get("alGenBuffers")
	alBufferData = openAL.Get("alBufferData")

	alGenSources = openAL.Get("alGenSources")
	alSourcei = openAL.Get("alSourcei")
	alSourcePlay = openAL.Get("alSourcePlay")

}
