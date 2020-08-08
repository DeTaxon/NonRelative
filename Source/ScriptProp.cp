
PropSetPos := !(void^ vm) -> void
{
	itProp := vProp^
	sq_getinstanceup(vm,1,itProp&->{void^^},0)
	resPos := vec4f(0.0f,0.0f,0.0f,1.0f)
	midlObj := float
	sq_getfloat(vm,2,midlObj&)
	resPos.x = midlObj
	sq_getfloat(vm,3,midlObj&)
	resPos.y = midlObj
	sq_getfloat(vm,4,midlObj&)
	resPos.z = midlObj

	itProp.modelPos.pos = resPos
}

PropSetAngDegAt := !(void^ vm) -> void
{
	itProp := vProp^
	sq_getinstanceup(vm,1,itProp&->{void^^},0)
	x := float
	sq_getfloat(vm,2,x&)
	y := float
	sq_getfloat(vm,3,y&)
	z := float
	sq_getfloat(vm,4,z&)
	ang := float
	sq_getfloat(vm,5,ang&)
	ang /= 180
	ang *= 1pi

	itProp.modelPos.ang = quantfAt(x,y,z,ang)
}

InitPropClass := !(void^ vm) -> void
{
	sq_pushroottable(vm)
	sq_pushstring(vm,"Prop",-1)
	sq_newclass(vm,false)

	gsClassProp.Get(vm,-1)
	gsClassProp.IncRef(vm)

	sq_pushstring(vm,"SetPos",-1)
	sq_newclosure(vm,PropSetPos,0)
	sq_newslot(vm,-3,false)

	sq_pushstring(vm,"SetAngDegAt",-1)
	sq_newclosure(vm,PropSetAngDegAt,0)
	sq_newslot(vm,-3,false)
	
	sq_pushstring(vm,"constructor",-1)
	sq_newclosure(vm,PropSetPos,0)
	sq_newslot(vm,-3,false)

	sq_newslot(vm,-3,false)
	sq_pop(vm,1)
}
