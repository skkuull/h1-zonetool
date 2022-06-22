#include <std_include.hpp>
#include "loader/loader.hpp"
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/string.hpp>
#include <utils/flags.hpp>
#include <utils/io.hpp>

#define GAME_BINARY "h1_mp64_ship.exe"

namespace aslr
{
	bool apply_patch(std::string* data)
	{
		// mp binary size
		if (data->size() != 0x1B97788)
		{
			return false;
		}

		auto* dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(&data->at(0));
		auto* nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(&data->at(dos_header->e_lfanew));
		auto* optional_header = &nt_headers->OptionalHeader;

		if (optional_header->DllCharacteristics & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE)
		{
			optional_header->DllCharacteristics &= ~(IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE);
		}

		return true;
	}

	void get_patched_binary(std::string* binary, std::string* data)
	{
		std::string patched_binary = "zonetool\\" + *binary;

		if (!apply_patch(data) ||
			(!utils::io::file_exists(patched_binary) &&
				!utils::io::write_file(patched_binary, *data, false)))
		{
			throw std::runtime_error(utils::string::va(
				"Could not create aslr patched binary!\n(%s)",
				binary->data()
			));
		}

		*binary = patched_binary;
	}
}

DECLSPEC_NORETURN void WINAPI exit_hook(const int code)
{
	component_loader::pre_destroy();
	exit(code);
}

BOOL WINAPI system_parameters_info_a(const UINT uiAction, const UINT uiParam, const PVOID pvParam, const UINT fWinIni)
{
	component_loader::post_unpack();
	return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);
}

FARPROC load_binary(std::string binary, uint64_t* base_address)
{
	loader loader;
	utils::nt::library self;

	loader.set_import_resolver([self](const std::string& library, const std::string& function) -> void*
	{
		if (library == "steam_api64.dll"
			&& function != "SteamAPI_Shutdown")
		{
			return self.get_proc<FARPROC>(function);
		}
		else if (function == "ExitProcess")
		{
			return exit_hook;
		}
		else if (function == "SystemParametersInfoA")
		{
			return system_parameters_info_a;
		}

		return component_loader::load_import(library, function);
	});

	std::string data;
	if (!utils::io::read_file(binary, &data))
	{
		throw std::runtime_error(utils::string::va(
			"Failed to read game binary (%s)!\nPlease copy the h1-zonetool.exe into your Call of Duty: Modern Warfare Remastered installation folder and run it from there.",
			binary.data()));
	}

	aslr::get_patched_binary(&binary, &data);

#ifdef INJECT_HOST_AS_LIB
	return loader.load_library(binary, base_address);
#else
	* base_address = 0x140000000;
	return loader.load(self, data);
#endif
}

void remove_crash_file()
{
	utils::io::remove_file("__h1Exe");
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	FARPROC entry_point;

	srand(uint32_t(time(nullptr)));
	remove_crash_file();

	{
		auto premature_shutdown = true;
		const auto _ = gsl::finally([&premature_shutdown]()
		{
			if (premature_shutdown)
			{
				component_loader::pre_destroy();
			}
		});

		try
		{
			if (!component_loader::post_start()) return 0;

			uint64_t base_address{};
			entry_point = load_binary(GAME_BINARY, &base_address);
			if (!entry_point)
			{
				throw std::runtime_error("Unable to load binary into memory");
			}

			game::base_address = base_address;

			if (!component_loader::post_load()) return 0;

			premature_shutdown = false;
		}
		catch (std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "ERROR", MB_ICONERROR);
			return 1;
		}
	}

	return static_cast<int>(entry_point());
}

int __stdcall WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	return main();
}
