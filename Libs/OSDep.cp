
prvtGetFileInfo := !(char^ fileName,char^^ fullPath,u64^ fileSize, u64^ fileId,int^ isFolder,void^ funcPtr) -> int declare


prvtStartFolderIter := !(char^ fileName,void^ funcPtr) -> void^ declare
prvtGetFolderIterItem := !(void^ itrItm,void^ funcPtr) -> char^ declare
prvtGetNextFolderIter := !(void^ itrItm) -> void declare
prvtIsEndFolderIter := !(void^ itrItm) -> int declare

prvtOpenFile := !(char^ fileName, u64^ resId,int opnType,int flags, void^ funcPtr) -> int declare
prvtCloseFile := !(u64 fileId) -> void declare
prvtMapFile := !(u64 fileId,u64^ mapId, u8^^ mapPtr,u64^ fileSize,int flags,void^ funcPtr) -> int declare
prvtUnmapFile := !(u64 mapId,void^ mapPtr,u64 mapSize) -> int declare

OpenLib := !(char^ fileName,void^ talloc) -> u64 declare
LoadFuncLib := !(u64 libId,char^ loadName) -> void^ declare
CloseLib := !(u64 libId) -> void declare
