
gMetMapPhys := List.{Tuple.{vMap^,centf}}^ ; $temp
vPhysStage := !(double deltaTime) -> void
{
	gMetMapPhys = new List.{Tuple.{vMap^,centf}} ; $temp
	oldPlayerImpz := gPlayer.ImpulseV.z
	forw := 0.0f
	lft := 0.0f
	if buttons['w'] forw += 2.0f
	if buttons['s'] forw -= 2.0f
	if buttons['a'] lft += 2.0f
	if buttons['d'] lft -= 2.0f
	if buttons['S'] forw *= 3.0f
	gPlayer.ImpulseV = quantfAt(0.0f,0.0f,-1.0f,gCam.leftRightAng)*vec4f(-forw,-lft,0.0f,0.0f)
	gPlayer.ImpulseV.z = oldPlayerImpz - 9.8f*deltaTime
	if gPlayer.ImpulseV.z < -20.0
		gPlayer.ImpulseV.z = -20
	zeroOffset := centf()
	vPhysStagePlayer(deltaTime,gPlayerMap,zeroOffset,2)
	gPlayer.System.pos += gPlayer.ImpulseV*deltaTime
	vPhysStagePlayer(deltaTime,gPlayerMap,zeroOffset,2)

	vPhysCheckMoves()

	gCam.camPos = gPlayer.System.pos + vec4f(0.0f,0.0f,1.7f,0.0f)
}
vPhysCheckMoves := !() -> void
{
	if gMetMapPhys^.Size() != 0
	{
		tst := ref gMetMapPhys^[0]
		if gPlayerMap != tst.0 or tst.1.Size() != 0 
		{
			gPlayer.System.pos = tst.1.Inverse().Apply(gPlayer.System.pos)
		}
	}
}
vPhysStagePlayer := !(double deltaTime,vMap^ someMap,centf offset,int depth) -> void
{
	if depth <= 0
		return void

	for it : someMap.mapProps
	{
		if it.physObj != null
		{
	 		fixes := PhysVsPhys(gPlayer->{PhysCommon^},it.physObj,offset)
			if it.physObj.PhysType == PhysType_hmap //TODO: prop with map checker
			{
				if fixes != 0
					gMetMapPhys.Emplace(someMap,offset)
			}
		}
	}

	for it : linksFromMap[someMap]
	{
		newPos := offset<*>it.relativePos
		vPhysStagePlayer(deltaTime,it.linkTo,newPos,depth-1)
	}
	for it : linksToMap[someMap]
	{	
		newPos := offset<*>it.relativePos.Inverse()
		vPhysStagePlayer(deltaTime,it.linkFrom,newPos,depth-1)
	}

}
