
vMapLink := class
{
	renderCost := int
	relativePos := centf
	linkFrom := vMap^
	linkTo := vMap^
}

vMap := class extend ScriptBox
{
	mapName := char^
	mapProps := List.{vProp^}

	AddProp := !(char^ modelName) -> vProp^
	{
		itMod := vGetModel(modelName)
		if itMod == null return null
		return vAddProp(itMod,this&)
	}
}

linksToMap := AVLMap.{vMap^,List.{vMapLink^}}
linksFromMap := AVLMap.{vMap^,List.{vMapLink^}}
