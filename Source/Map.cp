
vMapLink := class
{
	relativePos := centf
	linkOne := vMap^
	linkTwo := vMap^
}

vMap := class extend ScriptBox
{
	mapProps := List.{vProp^}

	AddProp := !(char^ modelName) -> vProp^
	{
		itMod := vGetModel(modelName)
		if itMod == null return null
		return vAddProp(itMod,null)
	}
}

linksToMap := AVLMap.{vMap^,List.{vMap^}}
linksFromMap := AVLMap.{vMap^,List.{vMap^}}
