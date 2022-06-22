#include <std_include.hpp>
#include "game.hpp"

namespace game
{
	uint64_t base_address;

	void load_base_address()
	{
		const auto module = GetModuleHandle(NULL);
		base_address = uint64_t(module);
	}

	int Cmd_Argc()
	{
		return cmd_args->argc[cmd_args->nesting];
	}

	const char* Cmd_Argv(const int index)
	{
		return cmd_args->argv[cmd_args->nesting][index];
	}

	int SV_Cmd_Argc()
	{
		return sv_cmd_args->argc[sv_cmd_args->nesting];
	}

	const char* SV_Cmd_Argv(const int index)
	{
		return sv_cmd_args->argv[sv_cmd_args->nesting][index];
	}

	void Cmd_TokenizeString(const char* text)
	{
		const auto limit = 512 - *reinterpret_cast<int*>(0x3516F40_b);
		Cmd_TokenizeStringWithLimit(text, limit);
	}

	void Cmd_EndTokenizeString()
	{
		const auto nesting = cmd_args->nesting;
		const auto argc = cmd_args->argc[nesting];
		--cmd_args->nesting;
		cmd_argsPrivate->totalUsedArgvPool -= argc;
		cmd_argsPrivate->totalUsedTextPool -= cmd_argsPrivate->usedTextPool[nesting];
	}

	void Cbuf_Execute(int localClientNum, int controllerIndex)
	{
		Sys_EnterCriticalSection(72);
		Cbuf_FuncExecute(localClientNum);
		Cbuf_ExecuteInternal(localClientNum, controllerIndex);
		Sys_LeaveCriticalSection(72);
	}
}

uintptr_t operator"" _b(const uintptr_t ptr)
{
	return game::base_address + ptr;
}