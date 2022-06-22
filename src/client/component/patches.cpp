#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/flags.hpp>

namespace patches
{
	namespace
	{
		void sync_gpu_stub()
		{
			std::this_thread::sleep_for(1ms);
		}

		void init_no_renderer()
		{
			static bool initialized = false;
			if (initialized) return;
			initialized = true;

			// R_LoadGraphicsAssets
			utils::hook::invoke<void>(0x686310_b);
		}

		void remove_renderer()
		{
			//printf("Renderer disabled...\n");

			// Hook R_SyncGpu
			utils::hook::jump(0x688620_b, sync_gpu_stub, true);

			utils::hook::jump(0x135600_b, init_no_renderer, true);

			// Disable VirtualLobby
			dvars::override::register_bool("virtualLobbyEnabled", false, game::DVAR_FLAG_READ);

			// Disable r_preloadShaders
			dvars::override::register_bool("r_preloadShaders", false, game::DVAR_FLAG_READ);

			utils::hook::nop(0x189514_b, 248); // don't load config file
			//utils::hook::nop(0x156C46_b, 5); // ^ ( causes the game to take long to bootup )
			utils::hook::set<uint8_t>(0x17F470_b, 0xC3); // don't save config file
			utils::hook::set<uint8_t>(0x351AA0_b, 0xC3); // disable self-registration
			utils::hook::set<uint8_t>(0x5BF4E0_b, 0xC3); // init sound system (1)
			utils::hook::set<uint8_t>(0x701820_b, 0xC3); // init sound system (2)
			utils::hook::set<uint8_t>(0x701850_b, 0xC3); // init sound system (3)
			utils::hook::set<uint8_t>(0x6C9B10_b, 0xC3); // render thread
			utils::hook::set<uint8_t>(0x343950_b, 0xC3); // called from Com_Frame, seems to do renderer stuff
			utils::hook::set<uint8_t>(0x12CCA0_b, 0xC3); // CL_CheckForResend, which tries to connect to the local server constantly
			utils::hook::set<uint8_t>(0x67ADCE_b, 0x00); // r_loadForRenderer default to 0
			utils::hook::set<uint8_t>(0x5B7AF0_b, 0xC3); // recommended settings check
			utils::hook::set<uint8_t>(0x5BE850_b, 0xC3); // some mixer-related function called on shutdown
			utils::hook::set<uint8_t>(0x4DEA50_b, 0xC3); // dont load ui gametype stuff

			utils::hook::nop(0x54ED81_b, 6); // unknown check in SV_ExecuteClientMessage
			utils::hook::nop(0x54E337_b, 4); // allow first slot to be occupied
			utils::hook::nop(0x13ABCB_b, 2); // properly shut down dedicated servers
			utils::hook::nop(0x13AB8E_b, 2); // ^
			utils::hook::nop(0x13ABF0_b, 5); // don't shutdown renderer

			utils::hook::set<uint8_t>(0xAA290_b, 0xC3); // something to do with blendShapeVertsView
			utils::hook::nop(0x70465D_b, 8); // sound thing

			// (COULD NOT FIND IN H1)
			utils::hook::set<uint8_t>(0x1D8A20_b, 0xC3); // cpu detection stuff?
			utils::hook::set<uint8_t>(0x690F30_b, 0xC3); // gfx stuff during fastfile loading
			utils::hook::set<uint8_t>(0x690E00_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x690ED0_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x39B980_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x690E50_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x651BA0_b, 0xC3); // directx stuff
			utils::hook::set<uint8_t>(0x681950_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x6CE390_b, 0xC3); // ^ - mutex
			utils::hook::set<uint8_t>(0x681ED0_b, 0xC3); // ^

			utils::hook::set<uint8_t>(0x0A3CD0_b, 0xC3); // rendering stuff
			utils::hook::set<uint8_t>(0x682150_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x682260_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x6829C0_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x6834A0_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x683B40_b, 0xC3); // ^ 

			// shaders
			utils::hook::set<uint8_t>(0x0AA090_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x0A9FE0_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x6C38D0_b, 0xC3); // ^ - mutex

			utils::hook::set<uint8_t>(0x5BFD10_b, 0xC3); // idk
			utils::hook::set<uint8_t>(0x652E10_b, 0xC3); // ^

			utils::hook::set<uint8_t>(0x687D20_b, 0xC3); // R_Shutdown
			utils::hook::set<uint8_t>(0x652BA0_b, 0xC3); // shutdown stuff
			utils::hook::set<uint8_t>(0x687DF0_b, 0xC3); // ^
			utils::hook::set<uint8_t>(0x686DE0_b, 0xC3); // ^

			// utils::hook::set<uint8_t>(0x1404B67E0, 0xC3); // sound crashes (H1 - questionable, function looks way different)

			utils::hook::set<uint8_t>(0x556250_b, 0xC3); // disable host migration

			utils::hook::set<uint8_t>(0x4F7C10_b, 0xC3); // render synchronization lock
			utils::hook::set<uint8_t>(0x4F7B40_b, 0xC3); // render synchronization unlock

			utils::hook::set<uint8_t>(0x27AA9D_b, 0xEB); // LUI: Unable to start the LUI system due to errors in main.lua
			utils::hook::set<uint8_t>(0x27AAC5_b, 0xEB); // LUI: Unable to start the LUI system due to errors in depot.lua
			utils::hook::set<uint8_t>(0x27AADC_b, 0xEB); // ^

			utils::hook::nop(0x5B25BE_b, 5); // Disable sound pak file loading
			utils::hook::nop(0x5B25C6_b, 2); // ^
			utils::hook::set<uint8_t>(0x3A0BA0_b, 0xC3); // Disable image pak file loading

			// Reduce min required memory
			utils::hook::set<uint64_t>(0x5B7F37_b, 0x80000000);

			utils::hook::set<uint8_t>(0x399E10_b, 0xC3); // some loop
			utils::hook::set<uint8_t>(0x1D48B0_b, 0xC3); // related to shader caching / techsets / fastfilesc
			utils::hook::set<uint8_t>(0x3A1940_b, 0xC3); // DB_ReadPackedLoadedSounds
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			remove_renderer();

			// disable splash
			utils::hook::set<uint8_t>(0x5BDF20_b, 0xC3);

			// disable demonware
			utils::hook::set<uint8_t>(0x5EDEC0_b, 0xC3); // dwNetStart
		}
	};
}

REGISTER_COMPONENT(patches::component)
