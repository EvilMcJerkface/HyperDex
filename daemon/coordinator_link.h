// Copyright (c) 2012, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef hyperdex_daemon_coordinator_link_h_
#define hyperdex_daemon_coordinator_link_h_

// STL
#include <map>
#include <tr1/memory>

// Replicant
#include <replicant.h>

// HyperDex
#include "common/configuration.h"
#include "common/ids.h"

namespace hyperdex
{
class daemon;

class coordinator_link
{
    public:
        coordinator_link(daemon*);
        ~coordinator_link() throw ();

    public:
        void set_coordinator_address(const char* host, uint16_t port);
        int register_id(server_id us, const po6::net::location& bind_to);
        bool wait_for_config(configuration* config);
        void transfer_go_live(const transfer_id& id);
        void transfer_complete(const transfer_id& id);
        void report_tcp_disconnect(const server_id& id);

    private:
        bool initiate_wait_for_config();
        bool initiate_get_config();

    private:
        daemon* m_daemon;
        std::auto_ptr<replicant_client> m_repl;
        int64_t m_wait_config_id;
        replicant_returncode m_wait_config_status;
        int64_t m_get_config_id;
        replicant_returncode m_get_config_status;
        const char* m_get_config_output;
        size_t m_get_config_output_sz;
        std::map<int64_t, std::pair<transfer_id, std::tr1::shared_ptr<replicant_returncode> > > m_transfers_go_live;
        std::map<int64_t, std::pair<transfer_id, std::tr1::shared_ptr<replicant_returncode> > > m_transfers_complete;
        std::map<int64_t, std::pair<server_id, std::tr1::shared_ptr<replicant_returncode> > > m_tcp_disconnects;

    private:
        coordinator_link(const coordinator_link&);
        coordinator_link& operator = (const coordinator_link&);
};

} // namespace hyperdex

#endif // hyperdex_daemon_coordinator_link_h_
