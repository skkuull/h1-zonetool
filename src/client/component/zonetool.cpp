#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "zonetool/zonetool.hpp"

namespace zonetool
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			zonetool::start();
		}
	};
}

REGISTER_COMPONENT(zonetool::component)
