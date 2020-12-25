
ToolMain := !(int argc, char^^ argv) -> bool
{
	if argc >= 4 and argv[1] == "mfcreate"
	{
		rf2 := new RawModel ; $temp
		rf2.LoadFromFile(argv[2])
		rf2.IndsRefit()
		rf2.ReVert(x ==> {
			i := 0
			while i < argc
			{
				setType := VType_Void
				if argv[i] in !["All","Pos","Normal","UV"]
				{
					switch argv[i+1]
					{
						case "Void"
						case "Half"
							setType = VType_Half
						case "Float"
							setType = VType_Float
						case void
							throw new Exception("Incorrect input")
					}
					switch argv[i]
					{
						case "All"
							x[^].BaseType = setType
						case "Pos"
							x[0].BaseType = setType
						case "Normal"
							x[1].BaseType = setType
						case "UV"
							x[2].BaseType = setType
					}
					i += 2
				}else{
					i += 1
				}
			}
		})
		rf2.SaveAsMF1(argv[3])
		return true
	}
	return false
}
