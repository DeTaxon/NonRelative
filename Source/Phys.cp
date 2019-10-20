
PhysCommon := class
{
	System := centf
	Impulse := vec4f
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

//PhysBox := class extend PhysCommon
//{
//}


sqrta := !(float inp) -> float
{	
	if inp < 0
	{
		return -sqrtf(-inp)
	}
	return sqrtf(inp)
}
"*" := !(vec4f i1, float i2) -> vec4f
{
	preRes := i1
	preRes.x *= i2
	preRes.y *= i2
	preRes.z *= i2
	preRes.w *= i2
	return preRes
}

PhysCheckSvS := !(PhysSphere^ o1,PhysSphere^ o2) -> void
{
	distVec := o1.System.pos - o2.System.pos
	distVec.w = 0.0f
	distSq := distVec <+> distVec

	if distSq > (o1.Radius + o2.Radius)**2
		return void
	
	cas := distVec.Normal()

	imp1S := sqrta(o1.Impulse <+> cas)
	imp2S := sqrta(o2.Impulse <+> cas)

	hSum := (imp1S + imp2S) * 0.5f

	
	o1.Impulse = o1.Impulse + cas*(hSum - imp1S)
	o2.Impulse = o2.Impulse + cas*(hSum - imp2S)
	
}

