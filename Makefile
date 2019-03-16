ForcedLibs := -f Libs/Lists.cp -f Libs/Memory.cp -f Libs/stdc.cp -f Libs/String.cp
TimeFlags := -f "time results: real - %E , user - %U user,system - %S ,memory %M KiB"

Libs := -ldl -lpthread


FLibs := -f FLibs/glfw.cp  

engi: 
	./halfvoid Source/main.cp $(FLibs) $(ForcedLibs) --vk vk.xml -o Objs/engi.ll; clang++ Objs/engi.ll $(Libs)  -lglfw -o engi

a.exe: out.ll WinMain.cpp
	clang out.ll -target x86_64-pc-windows-gnu -c -o WinObj.o ; x86_64-w64-mingw32-g++   WinMain.o  -mwindows -L.  -o a.exe

	
clean: 
	rm -f out.ll WinObj.o a.exe a.out

.PHONY: clean gdbc cycle repair test2 test2g LexTest engi
