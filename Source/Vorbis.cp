

v_DataOgg := class
{
    fPtr := void^
    fileSize := s64
    filePtr := s64

    this := !(void^ data,s64 size) -> void
    {
        fPtr = data
        fileSize = size
        filePtr = 0
    }
}
v_ReadOgg := !(void^ dst ,size_t siz1,size_t siz2,void^ fh) -> size_t
{
    dat := fh->{v_DataOgg^}
    len := siz1*siz2
    memcpy(dst,dat.fPtr + dat.filePtr,len)
    dat.filePtr += len
    return len
}
v_SeekOgg := !(void^ fh, s64 to, int types) -> int
{
    dat := fh->{v_DataOgg^}
    switch types
    {
        case 0 //set
            dat.filePtr = to
        case -1 //cur
            dat.filePtr += to
        case -2 //end
            dat.filePtr = dat.fileSize - to
    }

    if dat.filePtr < 0
    {
        dat.filePtr = 0
        return -1
    }
    if dat.filePtr > dat.fileSize
    {
        dat.filePtr = dat.fileSize
        return -1
    }
    return 0
}
v_closeOgg := !(void^ fh) -> int
{
    return 0
}
v_tellOgg := !(void^ fh) -> c_long
{
    dat := fh->{v_DataOgg^}
    return dat.filePtr
}

VorbisLoadFromMem := !(void^ data,int size) -> void
{
    libVorbisInit()

    callbacks := void^[4]
    callbacks[0] = v_ReadOgg
    callbacks[1] = v_SeekOgg
    callbacks[2] = v_closeOgg
    callbacks[3] = v_tellOgg
    dat := v_DataOgg(data,size)
    ov := new char[4024] ; $temp
    ov_open_callbacks(dat&,ov->{void^},null,-1,callbacks[0]&)
    inf := ov_info(ov->{void^},-1)
    printf("tst %p\n",inf)
}