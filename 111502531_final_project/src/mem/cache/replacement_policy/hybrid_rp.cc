#include "mem/cache/replacement_policies/hybrid_rp.hh"
#include "mem/cache/replacement_policies/fifo_rp.hh"
#include "mem/cache/replacement_policies/lru_rp.hh"

HybridRP::HybridRP(const Params *p)
    : BaseReplacementPolicy(p),
      replacementPolicyFIFO(nullptr), // Adjust as per FIFORP constructor requirements
      replacementPolicyLRU(nullptr)   // Adjust as per LRURP constructor requirements
{
    threshold = p->threshold;
    replacementPolicyFIFO = new FIFORP(nullptr);
    replacementPolicyLRU = new LRURP(nullptr);
}

void HybridRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    if (threshold >= 5) {
        replacementPolicyFIFO->invalidate(replacement_data);
    } else {
        replacementPolicyLRU->invalidate(replacement_data);
    }
}

void HybridRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    if (threshold >= 5) {
        replacementPolicyFIFO->touch(replacement_data);
    } else {
        replacementPolicyLRU->touch(replacement_data);
    }
}

void HybridRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    if (threshold >= 5) {
        replacementPolicyFIFO->reset(replacement_data);
    } else {
        replacementPolicyLRU->reset(replacement_data);
    }
}

ReplaceableEntry* HybridRP::getVictim(const ReplacementCandidates& candidates) const
{
    if (threshold >= 5) {
        return replacementPolicyFIFO->getVictim(candidates);
    } else {
        return replacementPolicyLRU->getVictim(candidates);
    }
}

std::shared_ptr<ReplacementData> HybridRP::instantiateEntry()
{
    // Always instantiate with the LRU policy
    return replacementPolicyLRU->instantiateEntry();
}
