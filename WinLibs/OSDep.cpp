#include <stdint.h>
#include <windows.h>
#include <stdio.h>

typedef void*(*talloc_t)(size_t itSize,size_t itAlign);

extern "C"
{
	int prvtGetFileInfo(char* fileName, char** fileFullName,uint64_t* fileSize,uint64_t* fileId,int* isFolder, void* tempAllocFunc)
	{
		talloc_t talloc = (talloc_t)tempAllocFunc;
		wchar_t* wideChr = nullptr;
		auto outSize = MultiByteToWideChar(CP_UTF8,MB_PRECOMPOSED,fileName,-1,nullptr,0);
		wideChr = (wchar_t*)talloc((outSize+1)*2,2);
		MultiByteToWideChar(CP_UTF8,MB_PRECOMPOSED,fileName,-1,wideChr,outSize);
		wideChr[outSize] = 0;
		
		if(fileFullName != nullptr)
		{
			auto fileNameFW = (wchar_t*)talloc(MAX_PATH,2);
			auto nameSize = GetFullPathNameW(wideChr,MAX_PATH,fileNameFW,nullptr);
			fileNameFW[nameSize] = 0;
			auto retNSize = WideCharToMultiByte(CP_UTF8,WC_COMPOSITECHECK,fileNameFW,-1,nullptr,0,nullptr,nullptr);
			fileFullName[0] = (char*)talloc(retNSize+1,1);
			WideCharToMultiByte(CP_UTF8,WC_COMPOSITECHECK,fileNameFW,-1,fileFullName[0],retNSize,nullptr,nullptr);
			fileFullName[0][retNSize] = 0;
		}
		//if (fileId != nullptr || fileSize != nullptr || isFolder != nullptr)
		//{
		//	struct stat resB;
		//	if(stat(fileName,&resB) != 0)
		//		return 0;
		//	if(fileId != nullptr)
		//		fileId[0] = resB.st_ino;
		//	if(fileSize != nullptr)
		//		fileSize[0] = resB.st_blocks*512;
		//	if(isFolder != nullptr)
		//		isFolder[0] = S_ISDIR(resB.st_mode) ? 1 : 0;
		//}

		return 1;
	}
}
