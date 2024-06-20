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
 * Authors: [Your Name]
 */

#ifndef __AHRP_RP_HH__
#define __AHRP_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

class AHRPRP : public BaseReplacementPolicy
{
  public:
    AHRPRP(const Params *p);

    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const override;

    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const override;

    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const override;

    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const override;

    std::shared_ptr<ReplacementData> instantiateEntry() override;

  private:
    void updatePriority(const std::shared_ptr<ReplacementData>& replacement_data, bool is_write) const;
};

class AHRPReplData : public ReplacementData
{
  public:
    Tick lastTouchTick;
    uint32_t priority;

    AHRPReplData() : lastTouchTick(Tick(0)), priority(0) {}
};

#endif // __AHRP_RP_HH__
