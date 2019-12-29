
glfwInit := !() -> int
{
	lib := Library("glfw3.dll","libglfw.so.3")

	glfwInitReal = lib.Get("glfwInit")
	glfwTerminate = lib.Get("glfwTerminate")
	glfwSetErrorCallback = lib.Get("glfwSetErrorCallback")
	glfwSetKeyCallback = lib.Get("glfwSetKeyCallback")
	glfwCreateWindow = lib.Get("glfwCreateWindow")
	glfwWindowHint = lib.Get("glfwWindowHint")
	glfwDestroyWindow = lib.Get("glfwDestroyWindow")
	glfwMakeContextCurrent = lib.Get("glfwMakeContextCurrent")
	glfwGetProcAddress = lib.Get("glfwGetProcAddress")
	glfwWindowShouldClose = lib.Get("glfwWindowShouldClose")
	glfwPollEvents = lib.Get("glfwPollEvents")
	glfwSwapBuffers = lib.Get("glfwSwapBuffers")
	glfwSwapInterval = lib.Get("glfwSwapInterval")

	glfwGetTime = lib.Get("glfwGetTime")
	glfwCreateWindowSurface = lib.Get("glfwCreateWindowSurface")
	glfwSetWindowSizeCallback = lib.Get("glfwSetWindowSizeCallback")
	glfwSetWindowTitle = lib.Get("glfwSetWindowTitle")
	
	glfwSetCursorPosCallback = lib.Get("glfwSetCursorPosCallback") 
	glfwSetInputMode = lib.Get("glfwSetInputMode")

	glfwInitReal()
}

glfwInitReal := !()^ -> int
glfwTerminate := !()^ -> void 

glfwSetErrorCallback := !(void^)^ -> void 
glfwSetKeyCallback := !(void^, void^)^ ->void 

glfwSetCursorPosCallback := !(void^, void^)^ ->void 
glfwSetInputMode := !(void^,int,int)^ -> void

glfwCreateWindow := !(int,int, char^,void^, void^)^ -> void^
glfwWindowHint := !(int, int)^ -> void 
glfwDestroyWindow := !(void^)^ -> void 

glfwMakeContextCurrent := !(void^)^ -> void 
glfwGetProcAddress := !(char^)^ -> void^ 

glfwWindowShouldClose := !(void^)^ -> bool 
glfwPollEvents := !()^ -> void 

glfwSwapBuffers := !(void^)^ -> void 
glfwSwapInterval := !(int)^ -> void 

glfwGetTime := !()^ -> double 

glfwCreateWindowSurface := !(void^,void^, void^, void^)^ -> int 

glfwSetWindowSizeCallback := !(void^,void^)^ -> void 
glfwSetWindowTitle := !(void^,char^)^ -> void 

GLFW_KEY_0               :=    48
GLFW_KEY_9               :=    57
GLFW_KEY_A               :=    65
GLFW_KEY_Z               :=    90
GLFW_KEY_LEFT_BRACKET    :=    91  ///* [ */
GLFW_KEY_BACKSLASH       :=    92  ///* \ */
GLFW_KEY_RIGHT_BRACKET   :=    93  ///* ] */
GLFW_KEY_GRAVE_ACCENT    :=    96  ///* ` */
GLFW_KEY_SPACE           :=    32
GLFW_KEY_APOSTROPHE      :=    39  ///* ' */
GLFW_KEY_COMMA           :=    44  ///* , */
GLFW_KEY_MINUS           :=    45  ///* - */
GLFW_KEY_PERIOD          :=    46  ///* . */
GLFW_KEY_SLASH           :=    47  ///* / */
GLFW_KEY_SEMICOLON       :=    59  ///* ; */
GLFW_KEY_EQUAL           :=    61  ///* = */

GLFW_FALSE    := 	0
GLFW_RELEASE  := 	0
GLFW_PRESS    := 	1
GLFW_REPEAT   := 	2

GLFW_CLIENT_API := 0x22001
GLFW_NO_API := 0

GLFW_CURSOR := 0x33001
GLFW_CURSOR_NORMAL := 0x34001
GLFW_CURSOR_HIDDEN := 0x34002
GLFW_CURSOR_DISABLED := 0x34003
