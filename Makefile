TimeFlags := -f "time results: real - %E , user - %U user,system - %S ,memory %M KiB"


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
	CmpLL := clang++ $(Triplet) -c -g Objs/engi.ll -o ./Objs/engi.o; x86_64-w64-mingw32-g++ ./Objs/engi.o -o $(Result)
endif

WinCompiler := x86_64-w64-mingw32-gcc 

SLibs := -C0 "Libs/$$" -C0 "../Lanq2_1/StandartLib/$$"

$(Result): Objs/engi.ll
	$(CmpLL)

Objs/engi.ll: 
	$(Compiler) -g $(Prefix) Source/main.cp $(SLibs) -C0 "FLibs/$$" -C1 "Source/$$" --vk vk.xml -o Objs/engi.ll
	
clean: 
	rm -f out.ll WinObj.o a.exe a.out engig

.PHONY: clean engi Objs/engi.ll
