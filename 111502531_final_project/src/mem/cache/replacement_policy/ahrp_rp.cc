/**
 * Copyright (c) 2024
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: BeanSamuel
 */

#include "mem/cache/replacement_policies/ahrp_rp.hh"

#include <cassert>
#include <memory>

#include "params/AHRPRP.hh"

AHRPRP::AHRPRP(const Params *p)
    : BaseReplacementPolicy(p)
{
}

void
AHRPRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Reset last touch timestamp and priority
    auto data = std::static_pointer_cast<AHRPReplData>(replacement_data);
    data->lastTouchTick = Tick(0);
    data->priority = 0;
}

void
AHRPRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Update last touch timestamp and adjust priority based on access type
    auto data = std::static_pointer_cast<AHRPReplData>(replacement_data);
    data->lastTouchTick = curTick();
    updatePriority(data, false); // Assume read access for touch
}

void
AHRPRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Set last touch timestamp and initialize priority
    auto data = std::static_pointer_cast<AHRPReplData>(replacement_data);
    data->lastTouchTick = curTick();
    data->priority = 1; // Initialize priority for new entries
}

ReplaceableEntry*
AHRPRP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    // Visit all candidates to find victim
    ReplaceableEntry* victim = candidates[0];
    for (const auto& candidate : candidates) {
        auto candidate_data = std::static_pointer_cast<AHRPReplData>(candidate->replacementData);
        auto victim_data = std::static_pointer_cast<AHRPReplData>(victim->replacementData);
        
        // Compare based on priority first, then last touch timestamp
        if (candidate_data->priority < victim_data->priority ||
            (candidate_data->priority == victim_data->priority &&
             candidate_data->lastTouchTick < victim_data->lastTouchTick)) {
            victim = candidate;
        }
    }

    return victim;
}

std::shared_ptr<ReplacementData>
AHRPRP::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new AHRPReplData());
}

void
AHRPRP::updatePriority(const std::shared_ptr<ReplacementData>& replacement_data, bool is_write) const
{
    auto data = std::static_pointer_cast<AHRPReplData>(replacement_data);
    if (is_write) {
        // Increase priority for writes
        data->priority += 2;
    } else {
        // Increase priority for reads
        data->priority += 1;
    }
}

AHRPRP*
AHRPRPParams::create()
{
    return new AHRPRP(this);
}
