TimeFlags := -f "time results: real - %E , user - %U user,system - %S ,memory %M KiB"

Libs := -ldl -lpthread

glComp := /media/Docs/VulkanSDK/1.0.39.1/x86_64/bin/glslangValidator -V100 -e main

WinCompiler := x86_64-w64-mingw32-gcc 
WinCompiler := wine "c:/mingw/mingw64/bin/g++.exe"

FLibs := -C0 FLibs/glfw.cp  

engi: Objs/engi.o
	clang++ LinuxLibs/OSDep.cpp Objs/engi.o -IFLibs $(Libs) LinuxLibs/Deflate.cpp LinuxLibs/libz.a  -lglfw -o engi
engig: Objs/engig.o
	clang++ LinuxLibs/OSDep.cpp Objs/engig.o -IFLibs $(Libs) LinuxLibs/Deflate.cpp LinuxLibs/libz.a  -lglfw -o engi

wengi: Objs/wengi.o
	$(WinCompiler) WinLibs/OSDep.cpp  Objs/wengi.o -IFLibs LinuxLibs/Deflate.cpp WinLibs/libz.a -L WinLibs -lglfw3 -lgdi32 -o wengi

Objs/engi.o: Objs/engi.ll
	clang Objs/engi.ll -c -o Objs/engi.o
Objs/engig.o: Objs/engig.ll
	clang Objs/engig.ll -g -c -o Objs/engig.o
Objs/wengi.o: Objs/wengi.ll
	clang Objs/wengi.ll -c -o Objs/wengi.o --target=x86_64-win32-gnu

Objs/engi.ll: LearnVert
	./halfvoid  --rname result Source/main.cp -C0 "Libs/*" -C0 "FLibs/*" -C1 "Source/*" --vk vk.xml -o Objs/engi.ll
Objs/engig.ll: LearnVert
	./halfvoid  --rname result -g Source/main.cp -C0 "Libs/*" -C0 "FLibs/*" -C1 "Source/*" --vk vk.xml -o Objs/engig.ll
Objs/wengi.ll: LearnVert
	./halfvoid  --rname result -p win32 Source/main.cp -C0 "Libs/*" -C0 "FLibs/*" -C1 "Source/*" --vk vk.xml -o Objs/wengi.ll


Shaders/% : ShadersSource/%
	$(glComp) $< -o $@

LearnVert: Shaders/LearnVert.vert Shaders/LearnFrag.frag 
	
clean: 
	rm -f out.ll WinObj.o a.exe a.out engig

.PHONY: clean gdbc cycle repair  LexTest engi LearnVert engig
