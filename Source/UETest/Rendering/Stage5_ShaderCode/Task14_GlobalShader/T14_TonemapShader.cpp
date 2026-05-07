// T14 - Tonemap Global Shader 注册
//
// IMPLEMENT_GLOBAL_SHADER 把 C++ 类与 .usf 文件 + EntryPoint + Frequency 关联。
// shader 路径 "/UETest/Private/MyTonemap.usf" 由模块 Startup 中
// AddShaderSourceDirectoryMapping("/UETest", ...) 提供。

#include "T14_TonemapShader.h"

IMPLEMENT_GLOBAL_SHADER(FT14_TonemapVS,
	"/UETest/Private/MyTonemap.usf", "MainVS", SF_Vertex);

IMPLEMENT_GLOBAL_SHADER(FT14_TonemapPS,
	"/UETest/Private/MyTonemap.usf", "MainPS", SF_Pixel);
