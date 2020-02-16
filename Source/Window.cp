
gWindowW := int
gWindowH := int
gLastTimeResized := double

glfwWindow := void^
buttons := bool[256]
mouseX := double
mouseY := double

glfwKeyPressRes := !(void^ win,int key, int scancode,int action) -> void
{
	asCh := 0

	//printf("heh %i\n",key)
	switch(key)
	{
		case GLFW_KEY_0..GLFW_KEY_9
			asCh = '0' + key - GLFW_KEY_0
		case GLFW_KEY_A..GLFW_KEY_Z
			asCh = 'a' + key - GLFW_KEY_A
		case 340
			asCh = 'S'
		case 32
			asCh = ' '
		case void
			return void
	}
	if action == GLFW_PRESS or action == GLFW_RELEASE
	{
		buttons[asCh] = action == GLFW_PRESS

		//KeyPress(asCh,action == GLFW_PRESS)
	}

}
glfwWinResized := !(void^ window, int newW, int newH) -> void
{
	gWindowW = newW
	gWindowH = newH
	gLastTimeResized = glfwGetTime()
}
glfwMouseMove := !(void^ w, double itX, double itY) -> void
{
	mouseX = itX
	mouseY = itY
}

CreateWindow := !(int newW,int newH) -> void
{
	gWindowW = newW
	gWindowH = newH
	glfwInit()

	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API)
	glfwWindow = glfwCreateWindow(gWindowW,gWindowH,"Hi again",null,null)

	glfwSetKeyCallback(glfwWindow,glfwKeyPressRes)
	glfwSetWindowSizeCallback(glfwWindow,glfwWinResized)

	glfwSetCursorPosCallback(glfwWindow,glfwMouseMove)
	glfwSetInputMode(glfwWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED)
}

DestroyWindow := !() -> void
{
	glfwTerminate()
}
