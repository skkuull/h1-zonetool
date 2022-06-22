#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/scheduler.hpp"

#include "zonetool/zonetool.hpp"

namespace zonetool
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			zonetool::initialize();
			scheduler::once([]()
			{
				zonetool::start();
			}, scheduler::pipeline::main);
		}
	};
}

REGISTER_COMPONENT(zonetool::component)
