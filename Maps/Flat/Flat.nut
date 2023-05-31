a <- SpawnProp("Boys")
b <- SpawnProp("BoysB")

step <- 0.4

function animation()
{
while(true)
{
	a.SetPos(0.0,0.0,0.0)
	b.SetPos(0.0,0.0,-100.0)
	ASleep(step)
	a.SetPos(0.0,0.0,-100.0)
	b.SetPos(0.0,0.0,0.0)
	ASleep(step)
}
}
Spawn(animation)
