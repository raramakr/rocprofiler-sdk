// MIT License
//
// Copyright (c) 2023 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <rocprofiler-sdk/version.h>

#include "lib/common/mpl.hpp"

#include "fmt/core.h"
#include "fmt/ranges.h"

#include <hsa/hsa.h>
#include <hsa/hsa_ext_amd.h>

#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#if !defined(ROCPROFILER_HSA_RUNTIME_EXT_AMD_VERSION)
#    define ROCPROFILER_HSA_RUNTIME_EXT_AMD_VERSION                                                \
        ((10000 * HSA_AMD_INTERFACE_VERSION_MAJOR) + (100 * HSA_AMD_INTERFACE_VERSION_MINOR))
#endif

namespace rocprofiler
{
namespace hsa
{
namespace utils
{
template <typename Tp>
auto
stringize_impl(const Tp& _v)
{
    using nonpointer_type = typename std::remove_pointer_t<Tp>;

    if constexpr(common::mpl::is_pair<Tp>::value)
    {
        return std::make_pair(stringize_impl(_v.first), stringize_impl(_v.second));
    }
    else if constexpr(std::is_constructible<std::string_view, Tp>::value)
    {
        auto _ss = std::stringstream{};
        _ss << _v;
        return _ss.str();
    }
    else if constexpr(fmt::is_formattable<Tp>::value && !std::is_pointer<Tp>::value)
    {
        return fmt::format("{}", _v);
    }
    else if constexpr(std::is_pointer<Tp>::value && !std::is_pointer<nonpointer_type>::value &&
                      common::mpl::is_type_complete_v<nonpointer_type> &&
                      !std::is_void<nonpointer_type>::value)
    {
        if(_v)
        {
            return stringize_impl(*_v);
        }
        else
        {
            auto _ss = std::stringstream{};
            _ss << _v;
            return _ss.str();
        }
    }
    else
    {
        auto _ss = std::stringstream{};
        _ss << _v;
        return _ss.str();
    }
}

template <typename... Args>
auto
stringize(Args... args)
{
    return std::vector<std::pair<std::string, std::string>>{stringize_impl(args)...};
}

template <typename Tp>
struct handle_formatter
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename Ctx>
    auto format(const Tp& v, Ctx& ctx) const
    {
        return fmt::format_to(ctx.out(), "handle={}", v.handle);
    }
};

template <typename Tp>
struct handle_formatter<const Tp> : handle_formatter<Tp>
{};
}  // namespace utils
}  // namespace hsa
}  // namespace rocprofiler

#if ROCPROFILER_HSA_RUNTIME_EXT_AMD_VERSION >= 10300
namespace fmt
{
template <>
struct formatter<hsa_agent_t> : rocprofiler::hsa::utils::handle_formatter<hsa_agent_t>
{};

template <>
struct formatter<hsa_amd_memory_pool_t>
: rocprofiler::hsa::utils::handle_formatter<hsa_amd_memory_pool_t>
{};

template <>
struct formatter<hsa_amd_vmem_alloc_handle_t>
: rocprofiler::hsa::utils::handle_formatter<hsa_amd_vmem_alloc_handle_t>
{};

template <>
struct formatter<hsa_access_permission_t>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename Ctx>
    auto format(hsa_access_permission_t v, Ctx& ctx) const
    {
        auto label = [v]() -> std::string_view {
            switch(v)
            {
                case HSA_ACCESS_PERMISSION_NONE: return "NONE";
                case HSA_ACCESS_PERMISSION_RO: return "READ_ONLY";
                case HSA_ACCESS_PERMISSION_WO: return "WRITE_ONLY";
                case HSA_ACCESS_PERMISSION_RW: return "READ_WRITE";
            }
            return "NONE";
        }();
        return fmt::format_to(ctx.out(), "{}", label);
    }
};

template <>
struct formatter<hsa_amd_memory_access_desc_t>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename Ctx>
    auto format(const hsa_amd_memory_access_desc_t& v, Ctx& ctx) const
    {
        return fmt::format_to(
            ctx.out(), "permissions={}, agent_handle={}", v.permissions, v.agent_handle);
    }
};
}  // namespace fmt
#endif
