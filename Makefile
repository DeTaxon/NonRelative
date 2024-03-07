TimeFlags := time -f "time results: real - %E , user - %U user,system - %S ,memory %M KiB"


Libs := -ldl -lpthread
Compiler := ./halfvoid
Prefix := -p posix
Result := engi
ifeq ($(OS),Windows_NT)
	Compiler := ./halfvoid.exe
	Prefix := -p win32
	Libs := 
	Result := wengi.exe
endif


CmpLL :=  clang++ -g Objs/engi.ll $(Libs) -o $(Result) -march=native

ifeq ($(cross),win32)
	Prefix := -p win32
	Result := wengi.exe
	Triplet := --target=x86_64-w64-mingw32-gnu 
	CmpLL := clang++ $(Triplet) -mf16c -c -g Objs/engi.ll -o ./Objs/engi.o; x86_64-w64-mingw32-g++ ./Objs/engi.o -o $(Result)
endif

en:
	$(TimeFlags) ./halfvoid -g -C0 StandardHVLibrary/ -C1 Source --vk ./vk.xml -o out.ll -cache /tmp/HVMecha.zip
	clang -gdwarf-4 -g out.ll -lm -o en
win.exe:
	$(TimeFlags) ./halfvoid -win32  -g -C0 StandardLibrary -C1 Source --vk ./vk.xml -o out.ll
	clang --target=x86_64-w64-mingw32-gnu  out.ll  -o win.exe
run:
	./halfvoid -g -C0 StandardHVLibrary/ -C1 Source --vk ./vk.xml -o out.ll -cache /tmp/HVMecha.zip -run main

WinCompiler := x86_64-w64-mingw32-gcc 

SLibs := -C0 "Libs/" -C0 "StandardHVLibrary/"

$(Result): Objs/engi.ll
	$(CmpLL)

Nuklear.so: $(wildcard ./Nuklear/*)
	gcc -shared -fPIC -fpermissive ./Nuklear/nuklear.cpp -o Nuklear.so

JIT: $(wildcard Source/*.hv)
	$(Compiler) $(SLibs) --vk vk.xml  Source/main.hv -C1 "Source/" --jit

Objs/engi.ll: 
	$(Compiler) -g $(Prefix) Source/main.hv $(SLibs) -C1 "Source/" --vk vk.xml -o Objs/engi.ll
	
clean: 
	rm -f out.ll WinObj.o a.exe a.out engig
grind:
	valgrind --log-file=grind.txt --leak-check=full --leak-resolution=med ./engi
soft:
	LIBGL_ALWAYS_SOFTWARE=1 __GLX_VENDOR_LIBRARY_NAME=mesa VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.i686.json:/usr/share/vulkan/icd.d/lvp_icd.x86_64.json ./en

.PHONY: clean engi Objs/engi.ll en
