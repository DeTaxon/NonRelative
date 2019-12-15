PhysCommon := class
{
	System := centf
	ImpulseV := vec4f
	ImpulseA := vec4f

	Weight := float
}

PhysSphere := class extend PhysCommon
{
	Radius := float

	this := !(float rad) -> void
	{
		Radius = rad
		System.ang = quantf(0.0f,0.0f,0.0f,1.0f)
		Weight = 1.0f
	}
}


PhysInfPlane := class extend PhysCommon
{
	Normal := vec4f
	Distance := float
	this := !(vec4f pos,vec4f norm) -> void
	{
		norm.w = 0.0f
		System.pos = pos
		Normal = norm.Normal()
		Distance = -(norm<+>pos)
	}
}
PhysPlayer := class extend PhysCommon
{
	Height := float
}
PhysHightSphere := class
{
	PosAndSize := vec4f
	SpheresCount := u8
	TriCount := u8
	CSpheres := u16[3]
	Triangles := u16[4]
}//total size 32 byte

PhysHeightMap := class
{
	Dots := vec4f^ //array
	Triangles := u16[3]^
	Spheres := PhysHightSphere^ //array
	
	CreateDots := !(RawModel^ mdlValue) -> void
	{
		vSize := mdlValue.GetVertSize()
		nDots := new vec4f[mdlValue.verts->len div vSize + 1] ; $temp
		
		for i,j : nDots
		{
			if j == 0 continue
			memcpy(i&,mdlValue.verts[(j-1)*vSize]&,vec4f->TypeSize)
		}
		
		triCount := mdlValue.inds->len div 3
		Triangles2 := new u16[3][triCount]
		
		for i : triCount
		{
			Triangles2[i][0] = mdlValue.inds[i*3    ] + 1
			Triangles2[i][1] = mdlValue.inds[i*3 + 1] + 1
			Triangles2[i][2] = mdlValue.inds[i*3 + 2] + 1
		}
		
		SpheresStart := new List.{PhysHightSphere^} ; $temp
		
		phase := new List.{Tuple.{PhysHightSphere^,bool}} ; $temp
		for tri,i : Triangles2
		{
			ns := new PhysHightSphere ; $temp
			a := nDots[tri[0]]
			b := nDots[tri[1]]
			c := nDots[tri[2]]
			center := (a + b + c) / 3.0
			aDs := Distance(a,center)
			bDs := Distance(b,center)
			cDs := Distance(c,center)
			center.w = aDs
			if center.w < bDs center.w = bDs
			if center.w < cDs center.w = cDs
			center.w *= 1.01
			//printf("wut %f %f\n",Distance(a.xyz,center.xyz),center.w)
			ns.PosAndSize = center
			ns.TriCount = 1
			ns.Triangles[0] = i
			
			phase.Emplace(ns,false)
		}
		
		fakeNodes := 0
		while phase.Size() != 1
		{
			newPhase := new List.{Tuple.{PhysHightSphere^,bool}} ; $temp
			
			for it,i : phase^
			{
				if it.1 continue
				
				cand := i
				candDist := 10000000000.0
				candPos := vec4f
				candBool := bool^
				
				for cn,j : phase^
				{
					if j <= i continue
					if cn.1 continue
					
					itDist := Distance(cn.0.PosAndSize.xyz,it.0.PosAndSize.xyz)
					itDist -= cn.0.PosAndSize.w + it.0.PosAndSize.w
					if candDist > itDist
					{
						cand = j
						candDist = itDist
						candPos = cn.0.PosAndSize
						candBool = cn.1&
					}
				}
				if cand == i
				{
					newPhase.Emplace(it.0,false)
					fakeNodes += 1
				}else{
					ns := new PhysHightSphere ; $temp
					cent := (it.0.PosAndSize + candPos) / 2
					cent.w = Distance(candPos.xyz,it.0.PosAndSize.xyz) + max(it.0.PosAndSize.w,candPos.w,it.0.PosAndSize.w)
					//printf("heh %f\n",it.0.PosAndSize.w)
					candBool^ = true
					ns.SpheresCount = 2
					ns.CSpheres[0] = i + SpheresStart.Size()
					ns.CSpheres[1] = cand + SpheresStart.Size()
					ns.PosAndSize = cent
					newPhase.Emplace(ns,false)
				}
			}
			
			for it : phase^
				SpheresStart.Push(it.0)
			phase = newPhase
		}
		
		allctdNodes := 0
		resSpheres := new PhysHightSphere[SpheresStart.Size() - fakeNodes] ; $temp
	
		addSphere := (int ind,u16^ resInt) ==> {
				resInt^ = allctdNodes
				nowSphere := ref resSpheres[allctdNodes]
				allctdNodes += 1

				fromSphere := ref SpheresStart^[ind]
				
				nowSphere.PosAndSize 
					= fromSphere.PosAndSize

				nowSphere.TriCount = fromSphere.TriCount
				for i : fromSphere.TriCount
				nowSphere.Triangles[i] = fromSphere.Triangles[i]

				nowSphere.SpheresCount = fromSphere.SpheresCount
				for i : nowSphere.SpheresCount
				{
					addSphere(fromSphere.CSpheres[i],nowSphere.CSpheres[i]&)
				}
		}
		fkNode := u16
		addSphere(SpheresStart.Size() - 1,fkNode&)
		
		resSize := nDots->len*vec4f->TypeSize + Triangles2->len*2*3 + resSpheres->len*PhysHightSphere->TypeSize
		
		asIntP := nDots->{u32^}
		asIntP[0] = nDots->len*vec4f->TypeSize
		asIntP[1] = resSpheres->len*PhysHightSphere->TypeSize
		asIntP[2] = Triangles2->len*2*3

		//TODO: 16 byte align alloc
		itMap := malloc(resSize)->{u8^}
		Dots = itMap->{void^}
		Spheres = itMap[asIntP[0]]&->{void^}
		Triangles = itMap[asIntP[0] + asIntP[1]]&->{void^}

		memcpy(Dots,nDots->{void^},asIntP[0])
		memcpy(Spheres,resSpheres->{void^},asIntP[1])
		memcpy(Triangles,Triangles2->{void^},asIntP[2])
	}
	GetTris := !(vec4f pos,u16[32]^ triBuf, int^ triSize,int sInd)-> void
	{
		itSphere := ref Spheres[sInd]


		if Distance(pos.xyz,itSphere.PosAndSize.xyz) > pos.w + itSphere.PosAndSize.w
			return void

		for i : itSphere.SpheresCount
			GetTris(pos,triBuf,triSize,itSphere.CSpheres[i])
		//printf("hh %f %f %f %i\n",Distance(pos.xyz,itSphere.PosAndSize.xyz),pos.w,itSphere.PosAndSize.w,itSphere.SpheresCount)

		for i : itSphere.TriCount
		{
			if triSize^ == 32 return void
			triBuf^[triSize^] = itSphere.Triangles[i]
			triSize^ += 1
		}
	}
	
	TriangleDistance := !(vec4f pos, u16 ind,vec4f^ res) -> bool
	{
		itTri := ref Triangles[ind]
		return TriangleDistance(pos,itTri[0],itTri[1],itTri[2],res)
	}
	GetNormal := !(int triInd,vec4f^ nId) -> void
	{
		itTri := ref Triangles[triInd]
		a := itTri[2] - itTri[0]
		b := itTri[1] - itTri[0]
		preRes := a <*> b
		nInd^ = preRes.xyz0.Normal()
	}
	TriangleDistance := !(vec4f pos, vec4f a, vec4f b, vec4f c,vec4f^ res) -> bool
	{
		ab := b.xyz - a.xyz
		ac := c.xyz - a.xyz
		ap := pos.xyz - a.xyz
		
		rb := (ab <+> ap) / ab.xyz.Length()
		rc := (ac <+> ap) / ac.xyz.Length()
		
		if rb < 0.0f or rc < 0.0f return false
		if rb + rc > 1.0f return false
		
		res^ = ab*rb + ac*rc
		
		return true
	}
	
	TriangleDistanceH := !(vec4f pos, u16 ind,vec4f^ res) -> bool
	{
		itTri := ref Triangles[ind]
		return TriangleDistanceH(pos,Dots[itTri[0]],Dots[itTri[1]],Dots[itTri[2]],res)
	}
	TriangleDistanceH := !(vec4f pos, vec4f a, vec4f b, vec4f c,vec4f^ res) -> bool
	{
		ab := b.xyz - a.xyz
		ac := c.xyz - a.xyz
		ap := pos.xyz - a.xyz
		
		rb := (ab.xy <+> ap.xy) / ab.xy.Length()
		rc := (ac.xy <+> ap.xy) / ac.xy.Length()
		
		printf("hu %f %f\n",rb,rc)
		if rb < 0.0f or rc < 0.0f return false
		if rb + rc > 1.0f return false
		
		res^ = (ab*rb + ac*rc).xyz0
		
		return true
	}
}



sqrta := !(float inp) -> float
{	
	if inp < 0
	{
		return -sqrtf(-inp)
	}
	return sqrtf(inp)
}

abc := int

PhysCheckPlayerVSHMap := !(PhysPlayer^ p,PhysHeightMap^ hMap) -> void
{
	triC := 0
	tris := u16[32]
	gotH := false
	minH := float
	maxH := float

	tHeight := p.Height*p.System.pos.w
	testPos := p.System.pos
	testPos.w = tHeight

	hMap.GetTris(p.System.pos,tris&,triC&,0)
	//printf("heh %i\n",triC)
	for i : triC
	{
		hV := vec4f
		nowT := hMap.TriangleDistanceH(p.System.pos,tris[i],hV&)
		if nowT
		{
			itH := p.System.pos.z - hV.z
			if itH > -tHeight
			{
				if gotH 
				{
					if minH > itH minH = itH
					if maxH < itH maxH = itH
				}else{
					gotH = true
					minH = itH
					maxH = itH
				}
			}
		}
	}
	if gotH
	{
		printf("step %i\n",abc)
		abc++
	}
}

PhysCheckPlayerVSSphere := !(PhysSphere^ p,PhysHeightMap^ hMap) -> void
{
	triC := int
	tris := u16[32]

	testPos := p.System.pos
	testPos.w = tHeight

	GetTris(p.System.pos,tris&,triC&,0)
	for i : triC
	{
		hV := vec4f
		nowT := TriangleDistance(p.System.pos,tris[i],hV&)
		if nowT
		{
			dist := Distance(hV.xyz,p.System.pos.xyz)
			if dist <= p.System.pos.w
			{
				norm := hMap.GetNormal(tris[i])
				nrm := hV.xyz - p.System.pos.xyz
				nrm = nrm.Normal()
			
				cas := nrm <+> (p.System.pos.xyz - hV.xyz)
				p.ImpulseV += nrm*(-cas*2.0f)
			}
		}
	}
}


PhysCheckSvIP := !(PhysSphere^ o1, PhysInfPlane^ o2) -> void
{
	dist := o2.Normal<+>o1.System.pos + o2.Distance

	if dist > o1.Radius
		return void
	
	diffImp := sqrta(o1.ImpulseV <+> o2.Normal)
	if diffImp > 0
		return void
	o1.ImpulseV += o2.Normal*(-diffImp*2.0f)
}
PhysCheckSvS := !(PhysSphere^ o1,PhysSphere^ o2) -> void
{
	distVec := o1.System.pos - o2.System.pos
	distVec.w = 0.0f
	distSq := distVec <+> distVec

	if distSq > (o1.Radius + o2.Radius)**2
		return void
	
	cas := distVec.Normal()

	imp1S := sqrta(o1.ImpulseV <+> cas)
	imp2S := sqrta(o2.ImpulseV <+> cas)

	massProc := o1.Weight / (o1.Weight + o2.Weight)

	mPass1 := massProc*2.0f - 1.0f
	mPass2 := -mPass1

	if imp1S > 0.0f
	{
		if imp2S > 0.0f
		{	
			resImp2  := -imp2S*mPass1
			o2.ImpulseV = o2.ImpulseV - cas*imp2S + cas*resImp2
			o1.ImpulseV = o1.ImpulseV - cas*imp1S + cas*(imp1S + imp2S - resImp2)
		}else{
			//dont care
		}
	}else{
		if imp2S > 0.0f
		{
			if imp2S > -imp1S
			{
				resImp2  := -imp2S*mPass1
				o2.ImpulseV = o2.ImpulseV - cas*imp2S + cas*resImp2
				o1.ImpulseV = o1.ImpulseV - cas*imp1S + cas*(imp1S + imp2S - resImp2)			
			}else{
				resImp1  := imp1S*mPass1
				o1.ImpulseV = o1.ImpulseV - cas*imp1S + cas*resImp1
				o2.ImpulseV = o2.ImpulseV - cas*imp2S + cas*(imp1S + imp2S - resImp1)			
			}
		}else{
			resImp1  := imp1S*mPass1
			o1.ImpulseV = o1.ImpulseV - cas*imp1S + cas*resImp1
			o2.ImpulseV = o2.ImpulseV - cas*imp2S + cas*(imp1S + imp2S - resImp1)
		}
	}
}

