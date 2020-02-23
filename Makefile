TimeFlags := -f "time results: real - %E , user - %U user,system - %S ,memory %M KiB"

Libs := -ldl -lpthread

glComp := /media/Docs/VulkanSDK/1.0.39.1/x86_64/bin/glslangValidator -V100 -e main

WinCompiler := x86_64-w64-mingw32-gcc 
#WinCompiler := wine "c:/mingw/mingw64/bin/g++.exe"

FLibs := -C0 FLibs/glfw.cp 
SLibs := -C0 "Libs/*" -Z0 "SLib.zip"

engi: Objs/engi.o
	clang++ -g Objs/engi.o -IFLibs $(Libs) -o engi
engig: Objs/engig.o
	clang++ Objs/engig.o -IFLibs $(Libs) -o engi

wengi.exe: Objs/wengi.o
	$(WinCompiler) Objs/wengi.o -g -IFLibs -L WinLibs -o wengi.exe

Objs/engi.o: Objs/engi.ll
	clang -g Objs/engi.ll -c -o Objs/engi.o
Objs/engig.o: Objs/engig.ll
	clang Objs/engig.ll -g -c -o Objs/engig.o
Objs/wengi.o: Objs/wengi.ll
	clang Objs/wengi.ll -c -o Objs/wengi.o --target=x86_64-win32-gnu

Objs/engi.ll: LearnVert
	./halfvoid -g -p posix  --rname result Source/main.cp $(SLibs) -C0 "FLibs/*" -C1 "Source/*" --vk vk.xml -o Objs/engi.ll
Objs/engig.ll: LearnVert
	nemiver ./halfvoid -g -p posix  --rname result Source/main.cp $(SLibs) -C0 "FLibs/*" -C1 "Source/*" --vk vk.xml -o Objs/engi.ll
Objs/wengi.ll: LearnVert
	./halfvoid  --rname result -p win32 Source/main.cp $(SLibs) -C0 "FLibs/*" -C1 "Source/*" --vk vk.xml -o Objs/wengi.ll

test2: main2.cp
	./halfvoid --rname result -g main2.cp $(SLibs) -o test2.ll; clang test2.ll -g $(Libs) -o test2
test2w: main2.cp
	./halfvoid --rname result -p win32  -g main2.cp $(SLibs) -o test2.ll; clang --target=x86_64-win32-gnu -c  test2.ll -g $(Libs) -o test2.o; $(WinCompiler) test2.o -o test2.exe

Shaders/% : ShadersSource/%
	$(glComp) $< -o $@

LearnVert: Shaders/LearnVert.vert Shaders/LearnFrag.frag 
	
clean: 
	rm -f out.ll WinObj.o a.exe a.out engig
nvidia:
	ENABLE_PRIMUS_LAYER=1 optirun -b primus ./engi

.PHONY: clean gdbc cycle repair  LexTest engi LearnVert engig test2
