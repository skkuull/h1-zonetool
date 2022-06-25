#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/dvars.hpp"

#include "command.hpp"

#include <utils/hook.hpp>

namespace fastfiles
{
	namespace
	{
		utils::hook::detour db_init_load_x_file_hook;

		void db_init_load_x_file(/*DBFile**/std::uintptr_t file, std::uint64_t offset)
		{
			printf("Loading xfile %s\n", reinterpret_cast<const char*>(file + 32));
			return db_init_load_x_file_hook.invoke<void>(file, offset);
		}

		void skip_extra_zones_stub(utils::hook::assembler& a)
		{
			const auto skip = a.newLabel();
			const auto original = a.newLabel();

			a.pushad64();
			a.test(esi, 0x400); // allocFlags
			a.jnz(skip);

			a.bind(original);
			a.popad64();
			a.mov(rdx, 0x8E2F80_b);
			a.mov(rcx, rbp);
			a.call(0x840A20_b);
			a.jmp(0x398070_b);

			a.bind(skip);
			a.popad64();
			a.mov(r14d, 0x400);
			a.not_(r14d);
			a.and_(esi, r14d);
			a.jmp(0x39814F_b);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
#ifdef DEBUG
			db_init_load_x_file_hook.create(0x3681E0_b, &db_init_load_x_file);
#endif

			// Allow loading of unsigned fastfiles
			utils::hook::set<uint8_t>(0x367B5B_b, 0xEB); // main function
			utils::hook::nop(0x368153_b, 2); // DB_AuthLoad_InflateInit

			// Don't load extra zones with loadzone
			utils::hook::nop(0x398061_b, 15);
			utils::hook::jump(0x398061_b, utils::hook::assemble(skip_extra_zones_stub), true);

			// Allow loading of mixed compressor types
			utils::hook::nop(0x3687A7_b, 2);
		}
	};
}

REGISTER_COMPONENT(fastfiles::component)
