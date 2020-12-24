

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
	openAL.Open("libopenal.so","soft_oal.dll")

	alcOpenDevice = openAL.Get("alcOpenDevice")
	alcCreateContext = openAL.Get("alcCreateContext")
	alcMakeContextCurrent = openAL.Get("alcMakeContextCurrent")

	alGenBuffers = openAL.Get("alGenBuffers")
	alBufferData = openAL.Get("alBufferData")

	alGenSources = openAL.Get("alGenSources")
	alSourcei = openAL.Get("alSourcei")
	alSourcePlay = openAL.Get("alSourcePlay")

}

WAVHeader := class
{
	chunkId := u32
	chunkSize := u32
	format := u32
	subchunk1Id := u32
	subchunk1Size := u32
	audioFormat := u16
	numChannels := u16
	sampleRate := u32
	byteRate := u32
	blockAlign := u16
	bitsPerSample := u16
	subchunk2Id := u32
	subchunk2Size := u32
}

source := u32
VoidAudioInit := !() -> void
{
	OpenALInit()

	wav := MappedFile("Maps/Flat/yodl.wav")

	hed := wav.Get()->{WAVHeader^}
	wavdata := hed[1]&->{void^}

	alDev := alcOpenDevice(null)
	contx := alcCreateContext(alDev,null)
	alcMakeContextCurrent(contx)

	alGenSources(1,source&)

	buffer := u32
	alGenBuffers(1,buffer&)

	dataSize := wav.Size() - WAVHeader->TypeSize
	numberOfSamples := dataSize div (hed.numChannels * (hed.bitsPerSample div 8))
	alBufferData(buffer,AL_FORMAT_MONO16,wavdata,dataSize,hed.sampleRate)
	alSourcei(source,AL_LOOPING,1)
	alSourcei(source,AL_BUFFER,buffer)

}
StartTroll := !() -> void
{
	alSourcePlay(source)
}
