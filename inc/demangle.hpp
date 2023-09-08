#pragma once

#include <string>
#include <typeinfo>
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace diagnostic {

std::string demangle(const std::type_info& info) noexcept;

template <typename T>
std::string demangle() noexcept
{
	return demangle(typeid(T));
}

}//diagnostic
