%Vec4f = type <4 x float>
%OpaqType = type {i1}
%RangeTypeFloat = type {float,float}
%RangeTypeInt = type {i32,i32}
declare float     @llvm.pow.f32(float  %Val, float %Power)
declare double    @llvm.pow.f64(double %Val, double %Power)
declare float @llvm.experimental.vector.reduce.fadd.f32.v4f32(float %acc, <4 x float> %a)
target triple="x86_64-pc-linux-gnu"
attributes #0 = { nounwind "target-cpu"="x86-64"  }
@Str40 = constant [7 x i8] c"delete\00"
@Str37 = constant [4 x i8] c"new\00"
@Str124 = constant [39 x i8] c"vkEnumerateInstanceExtensionProperties\00"
@Str464 = constant [11 x i8] c"vkLoadAddr\00"
@Str32 = constant [5 x i8] c"~For\00"
@Str43 = constant [6 x i8] c"~this\00"
@Str575 = constant [4 x i8] c"win\00"
@Str120 = constant [22 x i8] c"vkGetInstanceProcAddr\00"
@Str128 = constant [35 x i8] c"vkEnumerateInstanceLayerProperties\00"
@Str131 = constant [17 x i8] c"vkCreateInstance\00"
@Str455 = constant [12 x i8] c"vkDllHandle\00"
@Str473 = constant [7 x i8] c"printf\00"
@Str102 = constant [5 x i8] c"this\00"
@Str481 = constant [8 x i8] c"newFunc\00"
@Str557 = constant [14 x i8] c"glfwTerminate\00"
@Str50 = constant [3 x i8] c"in\00"
@Str25 = constant [7 x i8] c"lib.cp\00"
@Str118 = constant [15 x i8] c"libvulkan.so.1\00"
@Str591 = constant [12 x i8] c"itBusyPages\00"
@Str595 = constant [13 x i8] c"itFreedPages\00"
@Str566 = constant [15 x i8] c"glfwWindowHint\00"
@Str578 = constant [22 x i8] c"glfwWindowShouldClose\00"
@Str461 = constant [6 x i8] c"dlsym\00"
@Str552 = constant [9 x i8] c"glfwInit\00"
@Str582 = constant [15 x i8] c"glfwPollEvents\00"
@Str569 = constant [17 x i8] c"glfwCreateWindow\00"
@Str452 = constant [7 x i8] c"dlopen\00"
@Str548 = constant [8 x i8] c"dlclose\00"
@Str22 = constant [9 x i8] c"Hi again\00"
@Str60 = constant [22 x i8] c"_(){}[]!@#$%^&*~\22\5C/.,\00"
@Str126 = constant [21 x i8] c"cant get func props\0A\00"
@Str122 = constant [28 x i8] c"cant get function ProcAddr\0A\00"
@Str63 = constant [8 x i8] c"arrs.cp\00"
@Str20 = constant [14 x i8] c"VulkanCore.cp\00"
@Str28 = constant [2 x i8] c"^\00"
@Str35 = constant [14 x i8] c"MemoryPool.cp\00"
@Str450 = constant [11 x i8] c"InitVulkan\00"
@Str30 = constant [3 x i8] c"<<\00"
@Str73 = constant [3 x i8] c"==\00"
@Str560 = constant [16 x i8] c"GLFW_CLIENT_API\00"
@Str563 = constant [12 x i8] c"GLFW_NO_API\00"
@Str544 = constant [14 x i8] c"DestroyVulkan\00"
@Str69 = constant [4 x i8] c"<=>\00"
@Str65 = constant [3 x i8] c"%i\00"
@Str71 = constant [2 x i8] c"<\00"
@Str77 = constant [2 x i8] c"+\00"
@Str448 = constant [6 x i8] c"$keep\00"
@Str67 = constant [3 x i8] c"%f\00"
@Str75 = constant [3 x i8] c"!=\00"
%Class83 = type {i32,i8*,i32,i8*,i32,i8*,i32,%Class84*,i32,%Class84*,i32,%Class85*}
%Class84 = type {i32,i8*,i8*,[4 x float]}
%Class85 = type {i32,i8*,i32,i64,i8*}
%Class90 = type {i8*,i32}
%Class99 = type {i32,i32}
%Class143 = type {[256 x i8],i32}
%Class145 = type {[256 x i8],i32,i32,[256 x i8]}
%Class147 = type {i32,i8*,i32,%Class148*,i32,i8**,i32,i8**}
%Class148 = type {i32,i8*,i8*,i32,i8*,i32,i32}
%Class149 = type {i8*,i8*(i8* , i64 , i64 , i32)*,i8*(i8* , i8* , i64 , i64 , i32)*,void(i8* , i8*)*,void(i8* , i64 , i32 , i32)*,void(i8* , i64 , i32 , i32)*}
%Class188 = type {i8*,i32,%Class195,%Class195}
%Class195 = type {%Class197*,i32,%Class197*}
%Class197 = type {i8*,%Class197*}
%Class221 = type {%Class197*}
%AllocClass158 = type {i8* , i8** , i32}
define dso_local void @hiddenFunc542(i8* %HiddenName) #0  ; hiddenFunc
{
%ItHiddenName542 = bitcast i8* %HiddenName to %AllocClass158*
%T571 = getelementptr %AllocClass158 , %AllocClass158* %ItHiddenName542 , i32 0, i32 0
%T160 = getelementptr %AllocClass158 , %AllocClass158* %ItHiddenName542 , i32 0, i32 1
%T159 = getelementptr %AllocClass158 , %AllocClass158* %ItHiddenName542 , i32 0, i32 2
call void()@func156(); Line: 7 File: Source/main.cp
br label %ContPath541id0in0
br label %LastContPath541
ContPath541id0in0:
br label %LastContPath541
LastContPath541:
br label %OutLabel542
OutLabel542:
ret void
}
define dso_local void @hiddenFunc555(i8* %HiddenName) #0  ; hiddenFunc
{
%ItHiddenName555 = bitcast i8* %HiddenName to %AllocClass158*
%T571 = getelementptr %AllocClass158 , %AllocClass158* %ItHiddenName555 , i32 0, i32 0
%T160 = getelementptr %AllocClass158 , %AllocClass158* %ItHiddenName555 , i32 0, i32 1
%T159 = getelementptr %AllocClass158 , %AllocClass158* %ItHiddenName555 , i32 0, i32 2
call void()@glfwTerminate(); Line: 11 File: Source/main.cp
br label %ContPath554id0in0
br label %LastContPath554
ContPath554id0in0:
br label %LastContPath554
LastContPath554:
br label %OutLabel555
OutLabel555:
ret void
}
define dso_local i32 @main(i32 %argc , i8** %argv) #0  ; main
{
%AllocItem158 = alloca %AllocClass158
%T571 = getelementptr %AllocClass158 , %AllocClass158* %AllocItem158 , i32 0, i32 0
%T160 = getelementptr %AllocClass158 , %AllocClass158* %AllocItem158 , i32 0, i32 1
%T159 = getelementptr %AllocClass158 , %AllocClass158* %AllocItem158 , i32 0, i32 2
store i32 %argc , i32* %T159
store i8** %argv , i8*** %T160
%Result = alloca i32
store i32 0 , i32* %Result
%T540 = call i1()@func153(); Line: 6 File: Source/main.cp
store i32 0 , i32* %Result
br label %RetPath161in1
%T553 = call i32()@glfwInit(); Line: 10 File: Source/main.cp
%T561 = load i32 , i32* @T183
%T564 = load i32 , i32* @T184
call void(i32 , i32)@glfwWindowHint(i32 %T561 , i32 %T564); Line: 13 File: Source/main.cp
%T162 = getelementptr [9 x i8] , [9 x i8]* @Str22, i32 0,i32 0
%T570 = call i8*(i32 , i32 , i8* , i8* , i8*)@glfwCreateWindow(i32 640 , i32 480 , i8* %T162 , i8* null , i8* null); Line: 14 File: Source/main.cp
store i8* %T570 , i8** %T571
br label %Check572
Check572:
%T576 = load i8* , i8** %T571
%T579 = call i1(i8*)@glfwWindowShouldClose(i8* %T576); Line: 18 File: Source/main.cp
%T580 = xor i1 %T579,1
br i1 %T580, label %OnTrue572 , label %End572
OnTrue572:
call void()@glfwPollEvents(); Line: 22 File: Source/main.cp
br label %ContPath573id0in0
br label %LastContPath573
ContPath573id0in0:
br label %LastContPath573
LastContPath573:

br label %Check572
End572:
store i32 0 , i32* %Result
br label %RetPath161in7
br label %ContPath161id0in8
br label %LastContPath161
RetPath161in0:
br label %OutLabel158
RetPath161in1:
%T598 = bitcast %AllocClass158* %AllocItem158 to i8*
call void(i8*)@hiddenFunc542(i8* %T598)
br label %RetPath161in0
RetPath161in2:
br label %RetPath161in1
RetPath161in3:
br label %RetPath161in2
RetPath161in4:
%T599 = bitcast %AllocClass158* %AllocItem158 to i8*
call void(i8*)@hiddenFunc555(i8* %T599)
br label %RetPath161in3
RetPath161in5:
br label %RetPath161in4
RetPath161in6:
br label %RetPath161in5
RetPath161in7:
br label %RetPath161in6
RetPath161in8:
br label %RetPath161in7
ContPath161id0in0:
br label %LastContPath161
ContPath161id0in1:
%T600 = bitcast %AllocClass158* %AllocItem158 to i8*
call void(i8*)@hiddenFunc542(i8* %T600)
br label %ContPath161id0in0
ContPath161id0in2:
br label %ContPath161id0in1
ContPath161id0in3:
br label %ContPath161id0in2
ContPath161id0in4:
%T601 = bitcast %AllocClass158* %AllocItem158 to i8*
call void(i8*)@hiddenFunc555(i8* %T601)
br label %ContPath161id0in3
ContPath161id0in5:
br label %ContPath161id0in4
ContPath161id0in6:
br label %ContPath161id0in5
ContPath161id0in7:
br label %ContPath161id0in6
ContPath161id0in8:
br label %ContPath161id0in7
LastContPath161:
br label %OutLabel158
OutLabel158:
%ResultItem = load i32 , i32* %Result
ret i32 %ResultItem
}
declare i32 @glfwInit()
declare void @glfwTerminate()
declare void @glfwSetErrorCallback(i8*)
declare void @glfwSetKeyCallback(i8* , i8*)
declare i8* @glfwCreateWindow(i32 , i32 , i8* , i8* , i8*)
declare void @glfwWindowHint(i32 , i32)
declare void @glfwDestroyWindow(i8*)
declare void @glfwMakeContextCurrent(i8*)
declare i8* @glfwGetProcAddress(i8*)
declare i1 @glfwWindowShouldClose(i8*)
declare void @glfwPollEvents()
declare void @glfwSwapBuffers(i8*)
declare void @glfwSwapInterval(i32)
@T163 = dso_local global i32 48
@T164 = dso_local global i32 57
@T165 = dso_local global i32 65
@T166 = dso_local global i32 90
@T167 = dso_local global i32 91
@T168 = dso_local global i32 92
@T169 = dso_local global i32 93
@T170 = dso_local global i32 96
@T171 = dso_local global i32 32
@T172 = dso_local global i32 39
@T173 = dso_local global i32 44
@T174 = dso_local global i32 45
@T175 = dso_local global i32 46
@T176 = dso_local global i32 47
@T177 = dso_local global i32 59
@T178 = dso_local global i32 61
@T179 = dso_local global i32 0
@T180 = dso_local global i32 0
@T181 = dso_local global i32 1
@T182 = dso_local global i32 2
@T183 = dso_local global i32 139265
@T184 = dso_local global i32 0
define void @ClassExtraConstructor197(%Class197* %this)
{
ret void
}
%AllocClass427 = type {%Class197*}
%AllocClass198 = type {%Class197*}
%AllocClass202 = type {i8* , %Class197*}
%AllocClass207 = type {%Class197* , i8* , %Class197*}
define void @ClassExtraConstructor221(%Class221* %this)
{
ret void
}
%AllocClass423 = type {%Class221*}
%AllocClass224 = type {%Class197* , %Class221*}
%AllocClass229 = type {%Class221*}
%AllocClass233 = type {%Class221*}
%AllocClass237 = type {%Class221*}
define void @ClassExtraConstructor195(%Class195* %this)
{
ret void
}
%AllocClass431 = type {%Class195*}
define dso_local void @func432(%Class195* %this) #0  ; ~this
{
%AllocItem431 = alloca %AllocClass431
%T433 = getelementptr %AllocClass431 , %AllocClass431* %AllocItem431 , i32 0, i32 0
store %Class195* %this , %Class195** %T433
br label %OutLabel431
OutLabel431:
ret void
}
%AllocClass213 = type {%Class195*}
%AllocClass217 = type {%Class195*}
@T189 = dso_local thread_local global %Class188 zeroinitializer
declare i32 @printf(i8* , ...)
declare i8* @calloc(i32 , i32)
declare i8* @malloc(i32)
declare void @free(i8*)
declare i8* @dlopen(i8* , i32)
declare i8* @dlsym(i8* , i8*)
declare i32 @dlclose(i8*)
declare float @tanf(float)
declare float @cosf(float)
declare float @sinf(float)
declare float @sqrtf(float)
declare i32 @time()
declare void @srand(i32)
declare i32 @rand()
declare void @memset(i8* , i8 , i32)
declare void @memcpy(i8* , i8* , i32)
%AllocClass285 = type {%RangeTypeInt , i32}
define void @ClassExtraConstructor99(%Class99* %this)
{
ret void
}
%AllocClass439 = type {%Class99*}
%AllocClass267 = type {i32 , i32 , %Class99*}
%AllocClass273 = type {%Class99*}
%AllocClass277 = type {%Class99*}
%AllocClass281 = type {%Class99*}
%AllocClass290 = type {i32}
%AllocClass294 = type {%RangeTypeInt}
%AllocClass298 = type {i8* , i32}
%AllocClass303 = type {i32}
%AllocClass307 = type {i8*}
@T328 = dso_local global [4096 x i8] zeroinitializer
declare i32 @sprintf(i8* , i8* , ...)
declare i32 @strcmp(i8* , i8*)
%AllocClass332 = type {i32}
%AllocClass336 = type {float}
%AllocClass340 = type {i8* , i8*}
%AllocClass345 = type {i8* , i8*}
%AllocClass350 = type {i8* , i8*}
%AllocClass355 = type {i8* , i8*}
%AllocClass360 = type {i8* , i8*}
%AllocClass365 = type {i8* , i8*}
%AllocClass370 = type {i32 , i8*}
%AllocClass375 = type {float , i8*}
%AllocClass380 = type {i8*}
%AllocClass384 = type {i8*}
%AllocClass388 = type {i8*}
%AllocClass392 = type {i8*}
%AllocClass396 = type {i8 , i8*}
%AllocClass401 = type {i8* , i32}
%AllocClass406 = type {i8* , i32}
%AllocClass411 = type {i8*}
%AllocClass415 = type {i8*}
define void @ClassExtraConstructor90(%Class90* %this)
{
ret void
}
%AllocClass443 = type {%Class90*}
%AllocClass311 = type {i8* , %Class90*}
%AllocClass316 = type {%Class90*}
%AllocClass320 = type {%Class90*}
%AllocClass324 = type {%Class90*}
%AllocClass419 = type {i8*}
define void @ClassExtraConstructor188(%Class188* %this)
{
ret void
}
%AllocClass435 = type {%Class188*}
define dso_local void @func436(%Class188* %this) #0  ; ~this
{
%AllocItem435 = alloca %AllocClass435
%T437 = getelementptr %AllocClass435 , %AllocClass435* %AllocItem435 , i32 0, i32 0
store %Class188* %this , %Class188** %T437
%T592 = getelementptr %Class188 , %Class188* %this, i32 0, i32 2
call void(%Class195*)@func432(%Class195* %T592)
%T596 = getelementptr %Class188 , %Class188* %this, i32 0, i32 3
call void(%Class195*)@func432(%Class195* %T596)
br label %OutLabel435
OutLabel435:
ret void
}
%AllocClass241 = type {%Class188*}
%AllocClass245 = type {%Class188*}
%AllocClass249 = type {i32 , i32 , %Class188*}
%AllocClass255 = type {i8* , %Class188*}
%AllocClass260 = type {%Class188*}
@T142 = dso_local global void()*(i8* , i8*)* zeroinitializer
@T144 = dso_local global i32(i8* , i32* , %Class143*)* zeroinitializer
@T146 = dso_local global i32(i32* , %Class145*)* zeroinitializer
@T150 = dso_local global i32(%Class147* , %Class149* , i8**)* zeroinitializer
@T151 = dso_local global i8* zeroinitializer
%AllocClass152 = type {i32 , i8*}
define dso_local i1 @func153() #0  ; InitVulkan
{
%AllocItem152 = alloca %AllocClass152
%T538 = getelementptr %AllocClass152 , %AllocClass152* %AllocItem152 , i32 0, i32 0
%T476 = getelementptr %AllocClass152 , %AllocClass152* %AllocItem152 , i32 0, i32 1
%Result = alloca i1
store i1 0,i1* %Result
%T133 = getelementptr [15 x i8] , [15 x i8]* @Str118, i32 0,i32 0
%T453 = call i8*(i8* , i32)@dlopen(i8* %T133 , i32 2); Line: 11 File: /media/Normal/Mecha/Source/VulkanCore.cp
store i8* %T453, i8** @T151
%T459 = load i8* , i8** @T151
%T134 = getelementptr [22 x i8] , [22 x i8]* @Str120, i32 0,i32 0
%T462 = call i8*(i8* , i8*)@dlsym(i8* %T459 , i8* %T134); Line: 13 File: /media/Normal/Mecha/Source/VulkanCore.cp
%TPre466 = bitcast i8* %T462 to void()*(i8* , i8*)*
store void()*(i8* , i8*)* %TPre466, void()*(i8* , i8*)** @T142
%T470 = load void()*(i8* , i8*)* , void()*(i8* , i8*)** @T142
%PreOne471 = bitcast void()*(i8* , i8*)* %T470 to i8*
%PreTwo471 = bitcast i8* null to i8*
%T471 = icmp eq i8* %PreOne471,%PreTwo471
br i1 %T471, label %OnTrue467 , label %End467
OnTrue467:
%T135 = getelementptr [28 x i8] , [28 x i8]* @Str122, i32 0,i32 0
%T474 = call i32(i8* , ...)@printf(i8* %T135); Line: 16 File: /media/Normal/Mecha/Source/VulkanCore.cp
%T475 = icmp ne i32 0 ,0
store i1 %T475 , i1* %Result
br label %RetPath468in0
br label %ContPath468id0in1
br label %LastContPath468
RetPath468in0:
br label %RetPath154in1
RetPath468in1:
br label %RetPath468in0
ContPath468id0in0:
br label %LastContPath468
ContPath468id0in1:
br label %ContPath468id0in0
LastContPath468:
br label %End467
End467:
%T136 = getelementptr [39 x i8] , [39 x i8]* @Str124, i32 0,i32 0
%T478 = load void()*(i8* , i8*)* , void()*(i8* , i8*)** @T142
%T479 = call void()*(i8* , i8*)%T478(i8* null , i8* %T136)
%TPre483 = bitcast void()* %T479 to i8*
store i8* %TPre483, i8** %T476
%T487 = load i8* , i8** %T476
%PreOne488 = bitcast i8* %T487 to i8*
%PreTwo488 = bitcast i8* null to i8*
%T488 = icmp eq i8* %PreOne488,%PreTwo488
br i1 %T488, label %OnTrue484 , label %End484
OnTrue484:
%T137 = getelementptr [21 x i8] , [21 x i8]* @Str126, i32 0,i32 0
%T490 = call i32(i8* , ...)@printf(i8* %T137); Line: 22 File: /media/Normal/Mecha/Source/VulkanCore.cp
%T491 = icmp ne i32 0 ,0
store i1 %T491 , i1* %Result
br label %RetPath485in0
br label %ContPath485id0in1
br label %LastContPath485
RetPath485in0:
br label %RetPath154in4
RetPath485in1:
br label %RetPath485in0
ContPath485id0in0:
br label %LastContPath485
ContPath485id0in1:
br label %ContPath485id0in0
LastContPath485:
br label %End484
End484:
%T493 = load i8* , i8** %T476
%T494 = bitcast i8* %T493 to i32(i8* , i32* , %Class143*)*
store i32(i8* , i32* , %Class143*)* %T494, i32(i8* , i32* , %Class143*)** @T144
%T138 = getelementptr [35 x i8] , [35 x i8]* @Str128, i32 0,i32 0
%T499 = load void()*(i8* , i8*)* , void()*(i8* , i8*)** @T142
%T500 = call void()*(i8* , i8*)%T499(i8* null , i8* %T138)
%TPre503 = bitcast void()* %T500 to i8*
store i8* %TPre503, i8** %T476
%T507 = load i8* , i8** %T476
%PreOne508 = bitcast i8* %T507 to i8*
%PreTwo508 = bitcast i8* null to i8*
%T508 = icmp eq i8* %PreOne508,%PreTwo508
br i1 %T508, label %OnTrue504 , label %End504
OnTrue504:
%T139 = getelementptr [21 x i8] , [21 x i8]* @Str126, i32 0,i32 0
%T510 = call i32(i8* , ...)@printf(i8* %T139); Line: 26 File: /media/Normal/Mecha/Source/VulkanCore.cp
%T511 = icmp ne i32 0 ,0
store i1 %T511 , i1* %Result
br label %RetPath505in0
br label %ContPath505id0in1
br label %LastContPath505
RetPath505in0:
br label %RetPath154in7
RetPath505in1:
br label %RetPath505in0
ContPath505id0in0:
br label %LastContPath505
ContPath505id0in1:
br label %ContPath505id0in0
LastContPath505:
br label %End504
End504:
%T513 = load i8* , i8** %T476
%T514 = bitcast i8* %T513 to i32(i32* , %Class145*)*
store i32(i32* , %Class145*)* %T514, i32(i32* , %Class145*)** @T146
%T140 = getelementptr [17 x i8] , [17 x i8]* @Str131, i32 0,i32 0
%T519 = load void()*(i8* , i8*)* , void()*(i8* , i8*)** @T142
%T520 = call void()*(i8* , i8*)%T519(i8* null , i8* %T140)
%TPre523 = bitcast void()* %T520 to i8*
store i8* %TPre523, i8** %T476
%T527 = load i8* , i8** %T476
%PreOne528 = bitcast i8* %T527 to i8*
%PreTwo528 = bitcast i8* null to i8*
%T528 = icmp eq i8* %PreOne528,%PreTwo528
br i1 %T528, label %OnTrue524 , label %End524
OnTrue524:
%T141 = getelementptr [21 x i8] , [21 x i8]* @Str126, i32 0,i32 0
%T530 = call i32(i8* , ...)@printf(i8* %T141); Line: 30 File: /media/Normal/Mecha/Source/VulkanCore.cp
%T531 = icmp ne i32 0 ,0
store i1 %T531 , i1* %Result
br label %RetPath525in0
br label %ContPath525id0in1
br label %LastContPath525
RetPath525in0:
br label %RetPath154in10
RetPath525in1:
br label %RetPath525in0
ContPath525id0in0:
br label %LastContPath525
ContPath525id0in1:
br label %ContPath525id0in0
LastContPath525:
br label %End524
End524:
%T533 = load i8* , i8** %T476
%T534 = bitcast i8* %T533 to i32(%Class147* , %Class149* , i8**)*
store i32(%Class147* , %Class149* , i8**)* %T534, i32(%Class147* , %Class149* , i8**)** @T150
%T539 = icmp ne i32 0 ,0
store i1 %T539 , i1* %Result
br label %RetPath154in13
br label %ContPath154id0in14
br label %LastContPath154
RetPath154in0:
br label %OutLabel152
RetPath154in1:
br label %RetPath154in0
RetPath154in2:
br label %RetPath154in1
RetPath154in3:
br label %RetPath154in2
RetPath154in4:
br label %RetPath154in3
RetPath154in5:
br label %RetPath154in4
RetPath154in6:
br label %RetPath154in5
RetPath154in7:
br label %RetPath154in6
RetPath154in8:
br label %RetPath154in7
RetPath154in9:
br label %RetPath154in8
RetPath154in10:
br label %RetPath154in9
RetPath154in11:
br label %RetPath154in10
RetPath154in12:
br label %RetPath154in11
RetPath154in13:
br label %RetPath154in12
RetPath154in14:
br label %RetPath154in13
ContPath154id0in0:
br label %LastContPath154
ContPath154id0in1:
br label %ContPath154id0in0
ContPath154id0in2:
br label %ContPath154id0in1
ContPath154id0in3:
br label %ContPath154id0in2
ContPath154id0in4:
br label %ContPath154id0in3
ContPath154id0in5:
br label %ContPath154id0in4
ContPath154id0in6:
br label %ContPath154id0in5
ContPath154id0in7:
br label %ContPath154id0in6
ContPath154id0in8:
br label %ContPath154id0in7
ContPath154id0in9:
br label %ContPath154id0in8
ContPath154id0in10:
br label %ContPath154id0in9
ContPath154id0in11:
br label %ContPath154id0in10
ContPath154id0in12:
br label %ContPath154id0in11
ContPath154id0in13:
br label %ContPath154id0in12
ContPath154id0in14:
br label %ContPath154id0in13
LastContPath154:
br label %OutLabel152
OutLabel152:
%ResultItem = load i1 , i1* %Result
ret i1 %ResultItem
}
define dso_local void @func156() #0  ; DestroyVulkan
{
%T546 = load i8* , i8** @T151
%T549 = call i32(i8*)@dlclose(i8* %T546); Line: 61 File: /media/Normal/Mecha/Source/VulkanCore.cp
br label %ContPath157id0in0
br label %LastContPath157
ContPath157id0in0:
br label %LastContPath157
LastContPath157:
br label %OutLabel155
OutLabel155:
ret void
}
