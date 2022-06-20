#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	/***************************************************************
	 * Functions
	 **************************************************************/

	WEAK symbol<void(int localClientNum, int controllerIndex, const char* text)> Cbuf_AddText_{0x1CF480};
#define Cbuf_AddText(localClientNum, text) \
	Cbuf_AddText_(localClientNum, 0, text);

	WEAK symbol<void(int localClientNum, int controllerIndex, const char* buffer,
		void (int, int, const char*))> Cbuf_ExecuteBufferInternal{0x155BC0};
	WEAK symbol<void(const char* message)> Conbuf_AppendText{0x0};
	WEAK symbol<char*(int start)> ConcatArgs{0x413050};
	WEAK symbol<void(int localClientNum, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{0x156E90};
	WEAK symbol<void(const char* cmdName, void(), cmd_function_s* allocedCmd)> Cmd_AddCommandInternal{0x156880};
	WEAK symbol<void(const char*)> Cmd_RemoveCommand{0x157690};

	WEAK symbol<void(const char* text_in, int limit)> Cmd_TokenizeStringWithLimit{0x157A40};

	WEAK symbol<void(errorParm code, const char* message, ...)> Com_Error{0x159860};
	WEAK symbol<void()> Com_Quit_f{0x1F9280};

	WEAK symbol<void()> Quit{0x17CF50};

	WEAK symbol<void(int hash, const char* name, const char* buffer)> Dvar_SetCommand{0x1857D0};
	WEAK symbol<dvar_t*(const char* name)> Dvar_FindVar{0x183EB0};
	WEAK symbol<dvar_t*(int hash)> Dvar_FindMalleableVar{0x183E20};
	WEAK symbol<void(const dvar_t* dvar)> Dvar_ClearModified{0x183D50};
	WEAK symbol<void(char* buffer, int index)> Dvar_GetCombinedString{0x4EA020};
	WEAK symbol<const char*(dvar_t* dvar, bool is_hashed, dvar_value value)> Dvar_ValueToString{0x187000};
	WEAK symbol<void(dvar_t* dvar, DvarSetSource source)> Dvar_Reset{0x185390};
	WEAK symbol<void(const char*, const char*, 
		DvarSetSource)> Dvar_SetFromStringByNameFromSource{0x185BD0};

	WEAK symbol<dvar_t*(int hash, const char* name, bool value, 
		unsigned int flags)> Dvar_RegisterBool{0x182340};
	WEAK symbol<dvar_t*(int hash, const char* name, int value, int min, int max, 
		unsigned int flags)> Dvar_RegisterInt{0x182A10};
	WEAK symbol<dvar_t*(int hash, const char* dvarName, float value, float min, 
		float max, unsigned int flags)> Dvar_RegisterFloat{0x1827F0};
	WEAK symbol<dvar_t*(int hash, const char* dvarName, float value, float min, 
		float max, unsigned int flags)> Dvar_RegisterFloatHashed{0x182900};
	WEAK symbol<dvar_t*(int hash, const char* dvarName, const char* value, 
		unsigned int flags)> Dvar_RegisterString{0x182BD0};
	WEAK symbol<dvar_t*(int dvarName, const char* a2, float x, float y, float z, 
		float w, float min, float max, unsigned int flags)> Dvar_RegisterVec4{0x183010};

	WEAK symbol<long long(const char* qpath, char** buffer)> FS_ReadFile{0x1EC690};
	WEAK symbol<void(void* buffer)> FS_FreeFile{0x59E2F0};
	WEAK symbol<void(const char* gameName)> FS_Startup{0x0};
	WEAK symbol<void(const char* path, const char* dir)> FS_AddLocalizedGameDirectory{0x1878F0};

	WEAK symbol<int(const char* fname)> generateHashValue{0x183F80};

	WEAK symbol<void()> G_Glass_Update{0x417940};

	WEAK symbol<char*(char* string)> I_CleanStr{0x5AF2E0};

	WEAK symbol<const char*(int, int, int)> Key_KeynumToString{0x199990};

	WEAK symbol<unsigned int(int)> Live_SyncOnlineDataFlags{0x1A5C10};

	WEAK symbol<void(int arg, char* buffer, int bufferLength)> SV_Cmd_ArgvBuffer{0x1CAC60};
	WEAK symbol<void(const char* text_in)> SV_Cmd_TokenizeString{0x1CACE0};
	WEAK symbol<void()> SV_Cmd_EndTokenizedString{0x1CACA0};

	WEAK symbol<void(const char* error, ...)> Sys_Error{0x1D8710};
	WEAK symbol<int()> Sys_Milliseconds{0x5BDDF0};
	WEAK symbol<bool()> Sys_IsDatabaseReady2{0x4F79C0};

	/***************************************************************
	 * Variables
	 **************************************************************/

	WEAK symbol<CmdArgs> sv_cmd_args{0x2ED1EB0};

	WEAK symbol<const char*> command_whitelist{0x10ACB70};
	WEAK symbol<cmd_function_s*> cmd_functions{0x344DF18};
	WEAK symbol<CmdArgs> cmd_args{0x2ED1E00};
	WEAK symbol<CmdArgsPrivate> cmd_argsPrivate{0x3513F20};

	WEAK symbol<int> dvarCount{0x2999C34};
	WEAK symbol<dvar_t> dvarPool{0x344DF20};

	WEAK symbol<DWORD> threadIds{0xAC80740};
}
