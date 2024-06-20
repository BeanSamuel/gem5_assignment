#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_HYBRID_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_HYBRID_RP_HH__

#include "mem/cache/replacement_policies/base.hh"
#include "mem/cache/replacement_policies/fifo_rp.hh"
#include "mem/cache/replacement_policies/lru_rp.hh"

struct HybridRPParams;

class HybridRP : public BaseReplacementPolicy {
protected:
    FIFORP replacementPolicyFIFO;
    LRURP replacementPolicyLRU;
    int threshold; // Threshold for choosing between FIFO and LRU

public:
    typedef HybridRPParams Params;

    HybridRP(const Params *p);

    virtual void invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const override;
    virtual void touch(const std::shared_ptr<ReplacementData>& replacement_data) const override;
    virtual void reset(const std::shared_ptr<ReplacementData>& replacement_data) const override;
    virtual ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const override;
    virtual std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_HYBRID_RP_HH__
