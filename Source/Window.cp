
startW := int
startH := int

glfwWindow := void^
buttons := bool[256]

glfwKeyPressRes := !(void^ win,int key, int scancode,int action) -> void
{
	asCh := 0

	switch(key)
	{
		case GLFW_KEY_0..GLFW_KEY_9
			asCh = '0' + key - GLFW_KEY_0
		case GLFW_KEY_A..GLFW_KEY_Z
			asCh = 'a' + key - GLFW_KEY_A
		case void
			return void
	}
	if action == GLFW_PRESS buttons[asCh] = true
	if action == GLFW_RELEASE buttons[asCh] = false

}

CreateWindow := !() -> void
{
	glfwInit()

	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API)
	glfwWindow = glfwCreateWindow(startW,startH,"Hi again",null,null)

	glfwSetKeyCallback(glfwWindow,glfwKeyPressRes)
}
StartLoop := !() -> void
{
}

DestroyWindow := !() -> void
{
	glfwTerminate()
}
