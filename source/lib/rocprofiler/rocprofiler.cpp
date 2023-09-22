// MIT License
//
// Copyright (c) 2023 ROCm Developer Tools
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <rocprofiler/fwd.h>
#include <rocprofiler/rocprofiler.h>

#include "lib/common/utility.hpp"
#include "lib/rocprofiler/context/context.hpp"
#include "lib/rocprofiler/context/domain.hpp"
#include "lib/rocprofiler/hsa/agent.hpp"
#include "lib/rocprofiler/hsa/hsa.hpp"
#include "lib/rocprofiler/registration.hpp"

#include <atomic>
#include <vector>

namespace
{
template <typename... Tp>
auto
consume_args(Tp&&...)
{}
}  // namespace

extern "C" {
rocprofiler_status_t
rocprofiler_get_version(uint32_t* major, uint32_t* minor, uint32_t* patch)
{
    if(major) *major = ROCPROFILER_VERSION_MAJOR;
    if(minor) *minor = ROCPROFILER_VERSION_MINOR;
    if(patch) *patch = ROCPROFILER_VERSION_PATCH;
    return ROCPROFILER_STATUS_SUCCESS;
}

rocprofiler_status_t
rocprofiler_get_timestamp(rocprofiler_timestamp_t* ts)
{
    *ts = rocprofiler::common::timestamp_ns();
    return ROCPROFILER_STATUS_SUCCESS;
}
}
