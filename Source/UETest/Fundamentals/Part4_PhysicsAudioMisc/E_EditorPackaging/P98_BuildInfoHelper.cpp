#include "P98_BuildInfoHelper.h"

EP98_BuildConfig UP98_BuildInfoHelper::GetBuildConfiguration()
{
#if UE_BUILD_DEBUG
	return EP98_BuildConfig::Debug;
#elif UE_BUILD_DEVELOPMENT
	return EP98_BuildConfig::Development;
#elif UE_BUILD_TEST
	return EP98_BuildConfig::Test;
#elif UE_BUILD_SHIPPING
	return EP98_BuildConfig::Shipping;
#else
	return EP98_BuildConfig::Unknown;
#endif
}

bool UP98_BuildInfoHelper::IsEditorBuild()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

bool UP98_BuildInfoHelper::IsShippingBuild()
{
#if UE_BUILD_SHIPPING
	return true;
#else
	return false;
#endif
}

bool UP98_BuildInfoHelper::HasDebugOutput()
{
#if !UE_BUILD_SHIPPING
	return true;
#else
	return false;
#endif
}
