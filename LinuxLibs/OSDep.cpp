#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>

typedef void*(*talloc_t)(size_t itSize,size_t itAlign);

extern "C"
{
	int prvtGetFileInfo(char* fileName, char** fileFullName,uint64_t* fileSize,uint64_t* fileId,int* isFolder, void* tempAllocFunc)
	{
		talloc_t talloc = (talloc_t)tempAllocFunc;
		if(fileFullName != nullptr)
		{
			auto fulNameOut = realpath(fileName,nullptr);
			if(fulNameOut == nullptr) 
				return 0;
			auto fnSize = strlen(fulNameOut);
			fileFullName[0] = (char*)talloc(fnSize+1,1);
			memcpy(fileFullName[0],fulNameOut,fnSize);
			fileFullName[0][fnSize] = 0;
			free(fulNameOut);
		}
		if (fileId != nullptr || fileSize != nullptr || isFolder != nullptr)
		{
			struct stat resB;
			if(stat(fileName,&resB) != 0)
				return 0;
			if(fileId != nullptr)
				fileId[0] = resB.st_ino;
			if(fileSize != nullptr)
				fileSize[0] = resB.st_blocks*512;
			if(isFolder != nullptr)
				isFolder[0] = S_ISDIR(resB.st_mode) ? 1 : 0;
		}

		return 1;
	}

	
	struct dirIterType
	{
		DIR* ptrToAll;
		dirent* ptrToDir;
	};
	void* prvtStartFolderIter(char* fileName,void* tempAllocFunc) 
	{
		talloc_t talloc = (talloc_t)tempAllocFunc;

		auto newObj = (dirIterType*)talloc(sizeof(dirIterType),8);
		newObj->ptrToAll = opendir(fileName);
		if(newObj->ptrToAll == nullptr) 
			return nullptr;
		newObj->ptrToDir = readdir(newObj->ptrToAll);
		if(newObj->ptrToDir == nullptr)
			return nullptr;
		while(strcmp(newObj->ptrToDir->d_name,".") == 0 
		|| strcmp(newObj->ptrToDir->d_name,"..") == 0)
		{
			newObj->ptrToDir = readdir(newObj->ptrToAll);
		}
		return newObj;
	}
	char* prvtGetFolderIterItem(void*  prevItem,void* tempAllocFunc)
	{	
		auto newObj = (dirIterType*)prevItem;
		if (newObj->ptrToDir == nullptr)
			return nullptr;
		return newObj->ptrToDir->d_name;
	}
	void prvtGetNextFolderIter(void* prevItem)
	{
		auto newObj = (dirIterType*)prevItem;
		newObj->ptrToDir = readdir(newObj->ptrToAll);
	}
	int prvtIsEndFolderIter(void* prevItem)
	{
		auto newObj = (dirIterType*)prevItem;
		if(newObj->ptrToDir == nullptr)
		{
			closedir(newObj->ptrToAll);
			return 1;
		}
		return 0;
	}

}
