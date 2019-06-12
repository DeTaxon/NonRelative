TimeFlags := -f "time results: real - %E , user - %U user,system - %S ,memory %M KiB"

Libs := -ldl -lpthread

glComp := /media/Docs/VulkanSDK/1.0.39.1/x86_64/bin/glslangValidator -V100 -e main

WinCompiler := x86_64-w64-mingw32-gcc 
WinCompiler := wine "c:/mingw/mingw64/bin/g++.exe"

FLibs := -f FLibs/glfw.cp  

engi: Objs/engi.o
	clang++ LinuxLibs/OSDep.cpp Objs/engi.o -IFLibs $(Libs) LinuxLibs/Deflate.cpp LinuxLibs/libz.a  -lglfw -o engi

wengi: Objs/wengi.o
	$(WinCompiler) WinLibs/OSDep.cpp  Objs/wengi.o -IFLibs LinuxLibs/Deflate.cpp WinLibs/libz.a -L WinLibs -lglfw3dll -o wengi

Objs/engi.o: Objs/engi.ll
	clang Objs/engi.ll -c -o Objs/engi.o
Objs/wengi.o: Objs/engi.ll
	clang Objs/engi.ll -c -o Objs/wengi.o --target=x86_64-win32-gnu

Objs/engi.ll: LearnVert
	./halfvoid  --rname result Source/main.cp -F "Libs/*.cp" -F "FLibs/*.cp" --vk vk.xml -o Objs/engi.ll


Shaders/% : ShadersSource/%
	$(glComp) $< -o $@

LearnVert: Shaders/LearnVert.vert Shaders/LearnFrag.frag 
	
clean: 
	rm -f out.ll WinObj.o a.exe a.out engig

.PHONY: clean gdbc cycle repair  LexTest engi LearnVert
