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
		if( isFolder != nullptr)
		{
			isFolder[0] = (GetFileAttributesW(wideChr) & FILE_ATTRIBUTE_DIRECTORY) != 0 ? 1 : 0;
		}
		if(fileSize != nullptr || fileId != nullptr)
		{
			auto anlFile = CreateFileW(wideChr,0,FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);

			if(anlFile == INVALID_HANDLE_VALUE)
				return 0;

			if(fileSize != nullptr || fileId != nullptr)
			{
				BY_HANDLE_FILE_INFORMATION flInf;
				if(!GetFileInformationByHandle(anlFile,&flInf))
				{
					CloseHandle(anlFile);
					return 0;
				}
				if(fileSize != nullptr)
					fileSize[0] = flInf.nFileSizeLow + (((uint64_t)flInf.nFileSizeHigh) << 32);
				if(fileId != nullptr)
					fileId[0] = flInf.nFileIndexLow + (((uint64_t)flInf.nFileIndexHigh) << 32);

			}
		}

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

	int prvtOpenFile(char* fileName,HANDLE* resId,int opnType,int flags,void* tall)
	{
		talloc_t talloc = (talloc_t)tall;
		auto wName = utf8ToWide(fileName,talloc);
		
		auto oType = GENERIC_READ;
		auto fFlg = OPEN_EXISTING;
		auto fS = FILE_SHARE_READ;

		if(opnType != 1)
		{
			fS |= FILE_SHARE_WRITE;
			oType |= GENERIC_WRITE;
		}
		if(opnType == 3)
		{
			fFlg = CREATE_NEW;
		}
		auto newFile = CreateFileW(wName,oType,fS,nullptr,fFlg,FILE_ATTRIBUTE_NORMAL,nullptr);
		if(newFile == INVALID_HANDLE_VALUE)
			return 0;
		resId[0] = newFile;
		return 1;
	}
	void prvtCloseFile(HANDLE fId) 
	{
		CloseHandle(fId);
	}
	
	int prvtMapFile(HANDLE fileId,HANDLE* mapId,void** mapPtr,uint64_t* fileSize,int fileFlags,void* tall)
	{
		union{
			DWORD vals[2];
			uint64_t fRSize;
		};
		vals[0] = GetFileSize(fileId,&vals[1]);
		auto oldSize = fileSize[0];
		if(fileSize[0] == 0)
		{
			fileSize[0] = fRSize;
		}
		fRSize = fileSize[0];

		auto pageOp = PAGE_READONLY;
		if(fileFlags != 1)
		{
			pageOp = PAGE_READWRITE;
		}
		auto mapHndl =CreateFileMappingW(fileId,nullptr,pageOp,vals[1],vals[0],nullptr);
		
		if(mapHndl == nullptr)
			return 0;

		auto mapValue = MapViewOfFile(mapHndl, fileFlags != 1 ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ,0,0,fRSize);
		if(mapValue == nullptr){
			CloseHandle(mapHndl);
			return 0;
		}
		mapPtr[0] = mapValue;
		return 1;
	}
	int prvtUnmapFile(HANDLE mapId,void* mapPtr,uint64_t leng)
	{
		UnmapViewOfFile(mapPtr);
		CloseHandle(mapId);
		return 1;
	}

	HMODULE OpenLib(char* fileName,void* tFunc) 
	{
		talloc_t talloc = (talloc_t)tFunc;
		auto wC = utf8ToWide(fileName,talloc);
		return LoadLibraryW(wC);
	}
	void* LoadFuncLib(HMODULE libId,char* loadName)
	{
		return (void*)GetProcAddress(libId,loadName);
	}
	void CloseLib(HMODULE libId)
	{
		FreeLibrary(libId);
	}
}
