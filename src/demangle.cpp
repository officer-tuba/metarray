#include "demangle.hpp"

namespace diagnostic {

std::string demangle(const std::type_info& info) noexcept
{
	using buffer_t = std::unique_ptr<char, decltype(&std::free)>;

	int status;
	buffer_t buffer{abi::__cxa_demangle(info.name(), nullptr, nullptr, &status), &std::free};
	return 0 == status ? buffer.get() : "";
}

}//diagnostic
