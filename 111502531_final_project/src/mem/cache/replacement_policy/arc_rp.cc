#include "mem/cache/replacement_policies/arc_rp.hh"



ArcRP::ArcRP(const Params *p)

    : BaseReplacementPolicy(p), threshold(p->threshold)

{

}



void ArcRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const

{

    auto data = std::static_pointer_cast<ArcReplData>(replacement_data);

    data->lastTouchTick = Tick(0);

    data->priority = 0;

}



void ArcRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const

{

    auto data = std::static_pointer_cast<ArcReplData>(replacement_data);

    data->lastTouchTick = curTick();

    updatePriority(data, false); // Assume read access for touch

}



void ArcRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const

{

    auto data = std::static_pointer_cast<ArcReplData>(replacement_data);

    data->lastTouchTick = curTick();

    data->priority = 1; // Initialize priority for new entries

}



ReplaceableEntry* ArcRP::getVictim(const ReplacementCandidates& candidates) const

{

    assert(candidates.size() > 0);



    ReplaceableEntry* victim = candidates[0];

    for (const auto& candidate : candidates) {

        auto candidate_data = std::static_pointer_cast<ArcReplData>(candidate->replacementData);

        auto victim_data = std::static_pointer_cast<ArcReplData>(victim->replacementData);

        

        if (candidate_data->priority < victim_data->priority ||

            (candidate_data->priority == victim_data->priority &&

             candidate_data->lastTouchTick < victim_data->lastTouchTick)) {

            victim = candidate;

        }

    }



    return victim;

}



std::shared_ptr<ReplacementData> ArcRP::instantiateEntry()

{

    return std::make_shared<ArcReplData>();

}



void ArcRP::updatePriority(const std::shared_ptr<ReplacementData>& replacement_data, bool is_write) const

{

    auto data = std::static_pointer_cast<ArcReplData>(replacement_data);

    if (is_write) {

        data->priority += 2;

    } else {

        data->priority += 1;

    }

}



ArcRP* ArcRP::Params::create() const

{

    return new ArcRP(this);

}

