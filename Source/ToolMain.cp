
ToolMain := !(int argc, char^^ argv) -> bool
{
	if argc >= 4 and argv[1] == "mfcreate"
	{
		rf2 := new RawModel ; $temp
		rf2.LoadFromFile(argv[2])
		rf2.SaveAsMF1(argv[3])
		return true
	}
	return false
}
