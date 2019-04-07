ForcedLibs := -f Libs/Lists.cp -f Libs/Memory.cp -f Libs/stdc.cp -f Libs/String.cp -f Libs/AVLMap.cp -f Libs/AVLTree.cp -f Libs/AVLSet.cp -f Libs/FatArray.cp
TimeFlags := -f "time results: real - %E , user - %U user,system - %S ,memory %M KiB"

Libs := -ldl -lpthread

glComp := glslangValidator -V100 -e main


FLibs := -f FLibs/glfw.cp  

engi: LearnVert
	./halfvoid Source/main.cp $(FLibs) -F "Libs/*.cp" --vk vk.xml -o Objs/engi.ll; clang++ Objs/engi.ll $(Libs)  -lglfw -o engi
engig: 
	gdb --args ./halfvoid --work Source/main.cp $(FLibs) -F "Libs/*.cp" --vk vk.xml -o Objs/engi.ll; clang++ Objs/engi.ll $(Libs)  -lglfw -o engi


Shaders/% : ShadersSource/%
	$(glComp) $< -o $@

LearnVert: Shaders/LearnVert.vert Shaders/LearnFrag.frag 
	
clean: 
	rm -f out.ll WinObj.o a.exe a.out engig

.PHONY: clean gdbc cycle repair  LexTest engi LearnVert
