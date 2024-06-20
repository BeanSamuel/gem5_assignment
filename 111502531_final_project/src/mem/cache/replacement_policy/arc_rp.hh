#ifndef __ARC_RP_HH__
#define __ARC_RP_HH__

#include "mem/cache/replacement_policies/base.hh"
#include "base/types.hh"

class ArcReplData : public ReplacementData {
public:
    Tick lastTouchTick;
    uint32_t priority;

    ArcReplData() : lastTouchTick(Tick(0)), priority(0) {}
};

class ArcRP : public BaseReplacementPolicy {
public:
    struct Params : public BaseReplacementPolicy::Params {
        int threshold;

        Params() : threshold(10) {}

        ArcRP* create() const;
    };

    ArcRP(const Params *p);

    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const override;
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const override;
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const override;
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const override;
    std::shared_ptr<ReplacementData> instantiateEntry() override;

private:
    void updatePriority(const std::shared_ptr<ReplacementData>& replacement_data, bool is_write) const;

    int threshold;
};

#endif // __ARC_RP_HH__
