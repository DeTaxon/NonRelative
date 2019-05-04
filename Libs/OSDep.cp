
prvtGetFileInfo := !(char^ fileName,char^^ fullPath,u64^ fileSize, u64^ fileId,int^ isFolder,void^ funcPtr) -> int declare


prvtStartFolderIter := !(char^ fileName,void^ funcPtr) -> void^ declare
prvtGetFolderIterItem := !(void^ itrItm,void^ funcPtr) -> char^ declare
prvtGetNextFolderIter := !(void^ itrItm) -> void declare
prvtIsEndFolderIter := !(void^ itrItm) -> int declare
