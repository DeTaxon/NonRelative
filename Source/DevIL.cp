ilInit := !()^ -> void
iluInit := !()^ -> void

ilGenImages := !(int num,int^ imgs)^ -> bool
ilBindImage := !(int num)^ -> bool
ilDeleteImages := !(int num,int^ imgs)^ -> bool
ilLoadL := !(int fileType,void^ src, u32 fileSize)^ -> int
ilGetData := !()^ -> void^
ilCopyPixels := !(int xOff,int yOff,int zOff,int w, int h, int d,int format,int fType,void^ destin)^ -> void
ilActiveMipmap := !(int mipnum)^ -> void
ilActiveImage := !(int imgNum)^ -> void
ilGetInteger := !(int param)^ -> int
iluBuildMipmaps := !()^ -> void

IL_RGB := 0x1907
IL_RGBA := 0x1908
IL_BGR := 0x80E0
IL_BGRA := 0x80E1

IL_DXTC_FORMAT := 0x0705
IL_DXT1 := 0x0706

IL_BYTE := 0x1400
IL_UNSIGNED_BYTE := 0x1400
IL_SHORT:= 0x1400
IL_UNSIGNED_SHORT := 0x1400
IL_INT := 0x1400
IL_UNSIGNED_INT := 0x1400
IL_FLOAT := 0x1400
IL_DOUBLE := 0x1400
IL_HALF := 0x1400

IL_JPG := 0x0425
IL_PNG := 0x042A
IL_TGA := 0x042D
IL_GIF := 0x0436
IL_MP3 := 0x0452

IL_IMAGE_WIDTH := 0x0DE4
IL_IMAGE_HEIGHT := 0x0DE5
IL_IMAGE_DEPTH := 0x0DE6
IL_IMAGE_FORMAT := 0x0DEA
IL_NUM_FACES := 0x0DE1
IL_NUM_IMAGES := 0x0DF1
IL_NUM_MIPMAPS := 0x0DF2
IL_NUM_LAYERS := 0x0DF3
IL_IMAGE_DURATION := 0x0DF8
IL_CUR_IMAGE := 0x0DF7
IL_IMAGE_CHANNELS := 0x0DFF

DevIL := Library
DevILU := Library

DevILInit := !() -> void
{
	DevIL.Open("DevIL.dll")
	DevILU.Open("ILU.dll")

	ilInit = DevIL.Get("ilInit")
	iluInit = DevILU.Get("iluInit")

	ilGenImages = DevIL.Get("ilGenImages")
	ilBindImage = DevIL.Get("ilBindImage")
	ilDeleteImages = DevIL.Get("ilDeleteImages")
	ilLoadL = DevIL.Get("ilLoadL")
	ilGetData = DevIL.Get("ilGetData")
	ilActiveMipmap = DevIL.Get("ilActiveMipmap")
	ilCopyPixels = DevIL.Get("ilCopyPixels")
	ilActiveImage = DevIL.Get("ilActiveImage")
	ilGetInteger = DevIL.Get("ilGetInteger")
	iluBuildMipmaps = DevILU.Get("iluBuildMipmaps")
}
