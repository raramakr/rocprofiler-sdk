// Copyright (c) 2018-2023 Advanced Micro Devices, Inc.
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

#include <rocprofiler/rocprofiler.h>

#include "lib/rocprofiler/hsa/queue.hpp"

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace rocprofiler
{
namespace hsa
{
// Tracks and manages HSA queues
class QueueController
{
public:
    QueueController() = default;
    // Initializes the QueueInterceptor. This must be delayed until
    // HSA has been inited.
    void init(CoreApiTable& core_table, AmdExtTable& ext_table);

    // Called to add a queue that was created by the user program
    void add_queue(hsa_queue_t*, std::unique_ptr<Queue>);
    void destory_queue(hsa_queue_t*);

    // Add callback to queues associated with the agent. Returns a client
    // id that can be used by callers to remove the callback.
    ClientID add_callback(const rocprofiler_agent_t&, Queue::QueueCB, Queue::CompletedCB);
    void     remove_callback(ClientID);

    const CoreApiTable& get_core_table() const { return _core_table; }
    const AmdExtTable&  get_ext_table() const { return _ext_table; }

    // Gets the list of supported HSA agents that can be intercepted
    const auto& get_supported_agents() const { return _supported_agents; }
    auto&       get_supported_agents() { return _supported_agents; }

private:
    using agent_callback_tuple_t =
        std::tuple<rocprofiler_agent_t, Queue::QueueCB, Queue::CompletedCB>;
    using queue_map_t       = std::unordered_map<hsa_queue_t*, std::unique_ptr<Queue>>;
    using client_id_map_t   = std::unordered_map<ClientID, agent_callback_tuple_t>;
    using agent_cache_map_t = std::unordered_map<uint32_t, AgentCache>;

    CoreApiTable                          _core_table       = {};
    AmdExtTable                           _ext_table        = {};
    common::Synchronized<queue_map_t>     _queues           = {};
    common::Synchronized<client_id_map_t> _callback_cache   = {};
    agent_cache_map_t                     _supported_agents = {};
};

QueueController&
get_queue_controller();

void
queue_controller_init(HsaApiTable* table);

}  // namespace hsa
}  // namespace rocprofiler