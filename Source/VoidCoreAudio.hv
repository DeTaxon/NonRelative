


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

globalAudioSource := u32
VoidAudioInit := !() -> void
{
	OpenALInit()

	alDev := alcOpenDevice(null)
	contx := alcCreateContext(alDev,null)
	alcMakeContextCurrent(contx)

	alGenSources(1,globalAudioSource&)
	alSourcei(globalAudioSource,AL_LOOPING,1)
}


AudioObject := class
{
	bufferId := int

	Gen := !() -> void
	{
		alGenBuffers(1,bufferId&)
	}
	LoadFromFile := !(vRepoFile^ rep) -> void
	{
		wav := rep.Map()
		defer rep.Unmap()

		hed := wav->{WAVHeader^}
		wavdata := hed[1]&->{void^}

		Gen()

		dataSize := rep.Size() - WAVHeader->TypeSize
		numberOfSamples := dataSize div (hed.numChannels * (hed.bitsPerSample div 8))
		alBufferData(bufferId,AL_FORMAT_MONO16,wavdata,dataSize,hed.sampleRate)
	}
}

audios := AVLMap.{vRepoFile^,AudioObject^}

vLoadAudio := !(vRepoFile^ rep) -> AudioObject^
{
	if rep in audios
		return audios[rep]
	
	preRet := new AudioObject
	preRet.LoadFromFile(rep)

	audios[rep] = preRet

	return preRet
}

vPlayAudioGlobal := !(AudioObject^ toPlay) -> void
{
	alSourcei(globalAudioSource,AL_BUFFER,toPlay.bufferId)
	alSourcePlay(globalAudioSource)
}
