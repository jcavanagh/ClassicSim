
#include "OffhandAttackWarrior.h"
#include "Warrior.h"
#include "Flurry.h"
#include "Equipment.h"
#include "DeepWounds.h"

OffhandAttackWarrior::OffhandAttackWarrior(Engine* engine, Character* pchar, CombatRoll* roll) :
    OffhandAttack(engine,
                  pchar,
                  roll)
{
    this->pchar = dynamic_cast<Warrior*>(pchar);
    talent_ranks = {0.5, 0.525, 0.55, 0.575, 0.6, 0.625};
}

int OffhandAttackWarrior::spell_effect(const int) {
    complete_swing();
    engine->get_statistics()->increment("OH White Total Attempts");

    const int oh_wpn_skill = pchar->get_oh_wpn_skill();
    AttackResult* result = roll->get_melee_hit_result(oh_wpn_skill);

    if (result->is_miss()) {
        add_fail_stats("Miss");
        return 0;
    }
    // TODO: Apply Overpower
    if (result->is_dodge()) {
        add_fail_stats("Dodge");
        return 0;
    }
    if (result->is_parry()) {
        add_fail_stats("Parry");
        return 0;
    }

    float damage_dealt = pchar->get_random_non_normalized_oh_dmg() * talent_ranks[rank_talent];

    if (result->is_critical()) {
        damage_dealt = round(damage_dealt * 2);
        const int rage_gained = pchar->rage_gained_from_dd(round(damage_dealt));
        pchar->melee_oh_critical_effect();
        add_success_stats("Critical", round(damage_dealt), rage_gained);
        return rage_gained;
    }
    if (result->is_glancing()) {
        damage_dealt = round(roll->get_glancing_blow_dmg_penalty(oh_wpn_skill));
        const int rage_gained = pchar->rage_gained_from_dd(round(damage_dealt));
        pchar->melee_oh_hit_effect();
        add_success_stats("Glancing", damage_dealt, rage_gained);
        return rage_gained;
    }

    pchar->melee_oh_hit_effect();
    damage_dealt = round(damage_dealt);
    const int rage_gained = pchar->rage_gained_from_dd(damage_dealt);
    add_success_stats("Hit", damage_dealt, rage_gained);
    return rage_gained;
}