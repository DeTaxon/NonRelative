
vCamera := class
{
	upDownAng := float
	leftRightAng := float
	camPos := vec4f

	perspConsts := float[4]

	this := !() -> void
	{
		upDownAng = 0.0f
		leftRightAng = 0.0f
		camPos.w = 1.0f
	}

	AddAngs := !(float lrA, float udA) -> void
	{
		upDownAng +=  udA
		leftRightAng += lrA

		fullSpin := 360deg
		almostSpin := 300deg
		if leftRightAng > almostSpin leftRightAng -= fullSpin
		if leftRightAng < -almostSpin leftRightAng += fullSpin

		limit := 89deg
		if upDownAng > limit upDownAng = limit
		if upDownAng < -limit upDownAng = -limit
	}
	SetPerspective := !(float w, float h, float near, float far, float pov) -> void
	{
		aspect := w / h
		fovTM := 1.0f / tanf(pov*0.5f)
		range := far - near

		perspConsts[0] = fovTM / aspect
		perspConsts[1] = fovTM
		perspConsts[2] = (near + far) / range
		perspConsts[3] =  -(2.0f * far*near/range)
	}

	addLocal := !(vec4f diffAdd) -> void
	{
		rotQ := quantfAt(0.0f,1.0f,0.0f,leftRightAng)<*>quantfAt(1.0f,0.0f,0.0f,upDownAng)

		oldW := camPos.w
		camPos += (rotQ*diffAdd)* vec4f(-1.0f,-1.0f,-1.0f,0.0)
		camPos.w = oldW
	}
	ApplyCamera := !(centf propPos, centf posRes) -> void
	{
		tempCent := centf
		tempCent.ang = quantfAt(0.0f,1.0f,0.0f,leftRightAng)<*>quantfAt(1.0f,0.0f,0.0f,upDownAng)
		tempCent.pos = camPos

		invC := tempCent.Inverse()
		posRes = invC<*>propPos
	}
}

