#pragma once

namespace zonetool
{
	WEAK game::symbol<const char*> g_assetNames{ 0x10B30D0 };

	WEAK game::symbol<void*> DB_XAssetPool{ 0x10B4460 };
	WEAK game::symbol<int> g_poolSize{ 0x10B3C80 };

	WEAK game::symbol<XZoneInfoInternal> g_zoneInfo{ 0x5F5A370 };
	WEAK game::symbol<unsigned int> g_zoneCount{ 0x3D10088 };
	WEAK game::symbol<unsigned int> g_zoneIndex{ 0x3D1008C };
}