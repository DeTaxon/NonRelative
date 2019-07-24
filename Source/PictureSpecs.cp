
BMPMainHeader := packed_class
{
	bmpMagic := u16
	fileSize := u32
	reserved := u32
	offsetToImg := u32
}
BMPImgHeaderOld := packed_class
{
	itSize := u32
	itW := u32
	itH := u32
	colorPanels := u16
	bitsPerPixel := u16
	compression := u32
	imgSize := u32
	horizRez := u32
	vertRez := u32
	numOfColors := u32
	colorsUsed := u32
}
