// T15 - Compute Shader 注册

#include "T15_ParticleSimShader.h"

IMPLEMENT_GLOBAL_SHADER(FT15_ParticleSimCS,
	"/UETest/Private/ParticleSim.usf", "MainCS", SF_Compute);
