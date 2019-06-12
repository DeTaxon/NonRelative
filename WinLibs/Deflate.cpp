#include <zlib.h>

extern "C"
{
	int prvtDeflateInflate(void* inpData, unsigned long long inpSize, void* outData,unsigned long long outSize) 
	{
		z_stream strmItm = {};
		strmItm.next_in = (Bytef*)inpData;
		strmItm.next_out = (Bytef*)outData;
		strmItm.avail_in = inpSize;
		strmItm.avail_out = outSize;
		inflateInit2_(&strmItm,-15,ZLIB_VERSION,sizeof(z_stream));
		inflate(&strmItm,0);
		inflateEnd(&strmItm);
	
		return 1;
	}
}
