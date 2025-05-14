// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RL_EnemyConfig.h"

FEnemySkills URL_EnemyConfig::FindSkillsByTag(FGameplayTag SkillTag) const
{
    for (const FEnemySkills&  Skill : EnemySkills)
    {
        if (Skill.AbilityTag.MatchesTagExact(SkillTag))
        {
            return Skill;
        }
    }
    return FEnemySkills();
}
