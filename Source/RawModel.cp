MODEL_NONE := 0
MODEL_LOADED := 1
MODEL_INVRAM := 2

MODEL_POSITION := 1
MODEL_NORMAL := 2
MODEL_UV := 4

RawModel := class
{
	verts := float[]
	vertItems := int

	inds := int[]

	GetVertSize := !() -> int
	{
		preRet := 0
		if (vertItems and_b MODEL_POSITION) != 0 preRet += 3
		if (vertItems and_b MODEL_NORMAL) != 0 preRet += 3
		if (vertItems and_b MODEL_UV) != 0 preRet += 2

		return preRet
	}
	this := !() -> void
	{
		verts = null
		inds = null
		vertItems = 0
	}
	Clear := !() -> void
	{
		if inds != null delete inds
		if verts != null delete verts
	}

	SaveAsMF1 := !(char^ flName) -> void
	{
		fileSize := verts->len*float->TypeSize + inds->len*int->TypeSize + 4*int->TypeSize

		outFile := MappedFile(flName,FILE_CREATE,fileSize)
		defer outFile.Close()

		outData := outFile.Get()->{int^}
		outData[0] = VType_Float
		outData[1] = verts->len
		outData[2] = VType_UInt32
		outData[3] = inds->len

		vertSize := verts->len*4
		toWr := outData[4]&->{u8^}
		memcpy(toWr,verts->{void^},vertSize)

		toWr = toWr[vertSize]&
		indSize := inds->len*4
		memcpy(toWr,inds->{void^},indSize)
	}
	MapFromMF1 := !(void^ flP,u64 itSize) -> bool
	{
		itData := flP->{int^}
		verts = new float[itData[1]] ; $temp
		memcpy(verts->{void^},flP + 4*4,verts->len*4)
		inds = new int[itData[3]] ; $temp
		memcpy(inds->{void^},flP + (4*4 + verts->len*4),inds->len*4)

		vertItems = MODEL_POSITION + MODEL_NORMAL + MODEL_UV

		return true
	}
	LoadFromFile := !(vRepoFile^ fl) -> bool
	{
		ext := fl.GetName()[-3..0]
		ptr := fl.Map()
		defer fl.Unmap()
		if ext == "ply" return MapFromPLY(ptr,fl.Size())
		if ext == "mf1" return MapFromMF1(ptr,fl.Size())
		return false
	}
	LoadFromFile := !(char^ flName) -> bool
	{
		fl := MappedFile(flName)
		defer fl.Close()

		if flName[-3..0] == "ply"
		{
			return MapFromPLY(fl.Get(),fl.Size()->{u64})
		}else if flName[-3..0] == "mf1"{
			return MapFromMF1(fl.Get(),fl.Size()->{u64})
		}
		return false
	}
	MapFromPLY := !(void^ flP,u64 itSize) -> bool
	{
		daFile := flP->{u8^}
		keyWords := string[3]
		keyWords[0] = "element vertex !" 
		keyWords[1] = "end_header!"
		keyWords[2] = "property float !"

		poses := int[3]
		poses[0] = 0
		poses[1] = 0
		poses[2] = 0

		pos := 0
		working := true

		vertsCount := 0

		while working and pos < itSize
		{
			nowVal := flP->{u8^}[pos]

			for i : keyWords->len
			{
				if keyWords[i][poses[i]] == nowVal
				{
					poses[i] += 1
					if keyWords[i][poses[i] ] == '!'
					{
						if i == 0
						{
							pos += 1

							while flP->{u8^}[pos] in '0'..'9'
							{
								vertsCount = vertsCount*10 + daFile[pos] - '0'
								pos += 1
							}
							pos -= 1
						}
						if i == 1 working = false
						if i == 2{	
							pos += 1
							if daFile[pos] == 'x' vertItems = vertItems or_b MODEL_POSITION
							if daFile[pos] == 'n' vertItems = vertItems or_b MODEL_NORMAL
							if daFile[pos] == 's' vertItems = vertItems or_b MODEL_UV
						}
						poses[i] = 0
					}
				}else{
					poses[i] = 0
				}
			}			
			pos += 1
		}

		vertSize := 0

		if vertItems and_b MODEL_POSITION vertSize += 3
		if vertItems and_b MODEL_NORMAL vertSize += 3
		if vertItems and_b MODEL_UV vertSize += 2

		if vertItems != 7 return false //TODO: normalless support

		totalVerts := vertsCount*vertSize
		verts = new float[totalVerts] ; $temp

		posInVerts := 0

		isNeg := false
		hiValue := 0
		lowValue := 1 // value = hi / lo

		vertFileState := 0

		while posInVerts < totalVerts and pos < itSize
		{
			if vertFileState == 0
			{
				switch daFile[pos]
				{
					case '-'
						isNeg = true
					case '0'..'9'
						hiValue = hiValue*10 + daFile[pos] - '0'
					case '.'
						vertFileState = 1
						pos += 1 //TODO: why?
					case void
						isNeg = false
						lowValue = 1
						hiValue = 0
				}
			}
			if vertFileState == 1
			{
				if daFile[pos] in '0'..'9'
				{
					hiValue = hiValue*10 + daFile[pos] - '0'
					lowValue *= 10
				}else{
					verts[posInVerts] = hiValue / lowValue
					if isNeg  {
						verts[posInVerts] *= -1.0f
					}
					isNeg = false
					lowValue = 1
					hiValue = 0
					posInVerts += 1
					vertFileState = 0
				}
			}
			pos += 1
		}


		preInd := List.{int}() ; $temp
		miniBox := int[16]
		miniPos := 0
		nowValue := 0
		nowState := 0
		intVal := 0

		while pos < itSize
		{
			switch nowState
			{
				case 0 //start
					switch daFile[pos]
					{
						case '0'..'9'
							nowValue = nowValue*10 + daFile[pos] - '0'
						case ' '
							nowState = 1
							intVal = 0
					}

				case 1 //space
					switch daFile[pos]
					{
						case '0'..'9'
							intVal = intVal*10 + daFile[pos] - '0'
						case ' '
							miniBox[miniPos] = intVal
							intVal = 0
							miniPos += 1
						case '\n'
							nowState = 0
							miniBox[miniPos] = intVal
							intVal = 0
							miniPos += 1

							switch nowValue
							{
								case 4
									preInd.Push(miniBox[0])
									preInd.Push(miniBox[1])
									preInd.Push(miniBox[2])
									preInd.Push(miniBox[0])
									preInd.Push(miniBox[2])
									preInd.Push(miniBox[3])
								case 3
									preInd.Push(miniBox[0])
									preInd.Push(miniBox[1])
									preInd.Push(miniBox[2])
							}
							miniPos = 0
							nowValue = 0
					}

			}
			pos += 1
		}
		inds = preInd.ToArray()

		return true
	}
}




