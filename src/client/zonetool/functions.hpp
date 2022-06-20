#pragma once

namespace zonetool
{
	extern const char* get_asset_name(XAssetType type, void* pointer);
	extern const char* get_asset_name(XAsset* asset);
	extern const char* type_to_string(XAssetType type);

	extern bool zone_exists(const std::string& zone);

	extern XAssetHeader DB_FindXAssetHeader_Safe(XAssetType type, const std::string& name);
	extern void DB_EnumXAssets(const XAssetType type, const std::function<void(XAssetHeader)>& callback, const bool includeOverride);

	WEAK game::symbol<void(XAssetType type, void(__cdecl* func)(XAssetHeader, void*), const void* inData, bool includeOverride)> DB_EnumXAssets_Internal{0x394C60};
	WEAK game::symbol<const char* (const XAsset* asset)> DB_GetXAssetName{0x366140};
	WEAK game::symbol<XAssetHeader(XAssetType type, const char* name, int createDefault)> DB_FindXAssetHeader{0x3950C0};
	WEAK game::symbol<XAssetEntry* (XAssetType type, const char* name)> DB_FindXAssetEntry{0x394FF0};
	WEAK game::symbol<void(XZoneInfo* zoneInfo, unsigned int zoneCount, DBSyncMode syncMode)> DB_LoadXAssets{0x397500};
	WEAK game::symbol<bool(const char* zone, int source)> DB_FileExists{0x394DC0};
	WEAK game::symbol<size_t(XAssetType type)> DB_GetXAssetTypeSize{0x366180};
}