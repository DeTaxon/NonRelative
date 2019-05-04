#include <stdint.h>
#include <windows.h>
#include <stdio.h>

typedef void*(*talloc_t)(size_t itSize,size_t itAlign);

extern "C"
{
	wchar_t* utf8ToWide(char* utf, talloc_t talloc) 
	{
		wchar_t* wideChr = nullptr;
		auto outSize = MultiByteToWideChar(CP_UTF8,MB_PRECOMPOSED,utf,-1,nullptr,0);
		wideChr = (wchar_t*)talloc((outSize+1)*2,2);
		MultiByteToWideChar(CP_UTF8,MB_PRECOMPOSED,utf,-1,wideChr,outSize);
		wideChr[outSize] = 0;
		return wideChr;	
	}
	char* wideToUtf8(wchar_t* wd, talloc_t talloc)
	{
		auto retNSize = WideCharToMultiByte(CP_UTF8,WC_COMPOSITECHECK,wd,-1,nullptr,0,nullptr,nullptr);
		auto retRes = (char*)talloc(retNSize+1,1);
		WideCharToMultiByte(CP_UTF8,WC_COMPOSITECHECK,wd,-1,retRes,retNSize,nullptr,nullptr);
		retRes[retNSize] = 0;
		return retRes;
	}
	int prvtGetFileInfo(char* fileName, char** fileFullName,uint64_t* fileSize,uint64_t* fileId,int* isFolder, void* tempAllocFunc)
	{
		talloc_t talloc = (talloc_t)tempAllocFunc;
		wchar_t* wideChr = utf8ToWide(fileName,talloc);
		
		if(fileFullName != nullptr)
		{
			auto fileNameFW = (wchar_t*)talloc(MAX_PATH,2);
			auto nameSize = GetFullPathNameW(wideChr,MAX_PATH,fileNameFW,nullptr);
			fileNameFW[nameSize] = 0;
			fileFullName[0] = wideToUtf8(fileNameFW,talloc);
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

	struct dirIterType
	{	
		HANDLE ptrToAll;
		WIN32_FIND_DATAW ptrToDir;
		bool isEnd;
		wchar_t* newObjName;
	};
	void* prvtStartFolderIter(char* fileName,void* tempAllocFunc) 
	{
		talloc_t talloc = (talloc_t)tempAllocFunc;

		auto inpSize = strlen(fileName);
		auto newStr = (char*)talloc(inpSize+3,1);
		memcpy(newStr,fileName,inpSize);
		memcpy(newStr + inpSize, "/*\0",3);

		auto newObj = (dirIterType*)talloc(sizeof(dirIterType),16);
		auto itW = utf8ToWide(newStr,talloc);
		newObj->ptrToAll = FindFirstFileW(itW,&(newObj->ptrToDir));
		newObj->isEnd = newObj->ptrToAll == INVALID_HANDLE_VALUE;

		while(wcscmp(newObj->ptrToDir.cFileName,L".") == 0 
		|| wcscmp(newObj->ptrToDir.cFileName,L"..") == 0)
		{
			newObj->isEnd = FindNextFileW(newObj->ptrToAll,&(newObj->ptrToDir)) == 0;
		}
		return newObj;
	}
	char* prvtGetFolderIterItem(void*  prevItem,void* tempAllocFunc)
	{	
		auto newObj = (dirIterType*)prevItem;
		//return newObj->ptrToDir->d_name;
		return wideToUtf8(newObj->ptrToDir.cFileName,(talloc_t)tempAllocFunc);
	}
	void prvtGetNextFolderIter(void* prevItem)
	{
		auto newObj = (dirIterType*)prevItem;
		//newObj->ptrToDir = readdir(newObj->ptrToAll);
		newObj->isEnd = FindNextFileW(newObj->ptrToAll,&(newObj->ptrToDir)) == 0;
	}
	int prvtIsEndFolderIter(void* prevItem)
	{
		auto newObj = (dirIterType*)prevItem;
		return newObj->isEnd ? 1 : 0;
	}

}
