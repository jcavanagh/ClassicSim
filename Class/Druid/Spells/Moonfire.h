#pragma once

#include "SpellPeriodic.h"
#include "TalentRequirer.h"

#include <QVector>

class Buff;
class Druid;
class DruidSpells;
class Random;

class Moonfire: public SpellPeriodic, public TalentRequirer {
public:
    Moonfire(Druid* pchar, DruidSpells* druid_spells, const int spell_rank);
    ~Moonfire() override;

    bool is_rank_learned() const override;

    static int duration_for_spell_rank(const unsigned spell_rank);

private:
    DruidSpells* druid_spells;
    Random* instant_dmg;

    double instant_spell_dmg_coefficient;
    double dot_spell_dmg_coefficient;
    unsigned base_resource_cost;
    unsigned base_instant_damage_min;
    unsigned base_instant_damage_max;
    unsigned full_duration_damage;
    int duration;

    double num_ticks;
    double reported_resource_cost;
    double reported_execution_time;
    double tick_rest {0.0};

    double imp_moonfire_damage_bonus {1.0};
    unsigned imp_moonfire_crit_bonus {0};
    const QVector<QPair<double, unsigned>> improved_moonfire_ranks {
        {1.0, 0}, {1.02, 200}, {1.04, 400}, {1.06, 600}, {1.08, 800}, {1.10, 1000},
    };

    bool check_application_success() override;
    void new_application_effect() override;
    void refresh_effect() override;
    void reset_effect() override;
    void tick_effect() override;

    void increase_talent_rank_effect(const QString& talent_name, const int curr) override;
    void decrease_talent_rank_effect(const QString& talent_name, const int curr) override;
};