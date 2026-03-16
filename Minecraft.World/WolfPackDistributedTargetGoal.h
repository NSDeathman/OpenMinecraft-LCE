#pragma once

#include "Goal.h"
#include "TargetGoal.h"
#include "LivingEntity.h"

class TamableAnimal;

// Заменяет TameAnimalDefendGoal для волков в стае.
// Вместо того чтобы вся стая фокусировалась на одной цели,
// каждый волк выбирает наименее "занятого" врага по формуле:
//   score = assignedWolves * ASSIGNMENT_PENALTY_SQ + distanceSq
// Меньший score = более приоритетная цель.
class PackDistributedTargetGoal : public TargetGoal
{
private:
    TamableAnimal* tameAnimal;
    weak_ptr<LivingEntity> assignedTarget;
    float                  maxChaseDistFromOwner;

    // Штраф за каждого уже назначенного волка на цель.
    // Эквивалентен "виртуальному удалению" цели на 12 блоков за каждого волка.
    // Чем выше — тем равномернее распределение.
    // Чем ниже — тем сильнее влияет дистанция.
    static constexpr double ASSIGNMENT_PENALTY_SQ = 12.0 * 12.0;

    // Жёсткий лимит волков на одну цель при наличии альтернативы.
    // Если все цели заняты сверх лимита — лимит игнорируется
    // (волки не простаивают).
    static constexpr int    MAX_WOLVES_PER_TARGET = 2;

public:
    PackDistributedTargetGoal(TamableAnimal* tameAnimal,
        bool mustSee,
        float maxChaseDistFromOwner = 20.0f);

    bool canUse()           override;
    bool canContinueToUse() override;
    void start()            override;

private:
    // Собрать всех прирученных волков того же хозяина в радиусе range
    vector<shared_ptr<LivingEntity>> getPackmates(double range);

    // Посчитать сколько волков из packmates уже атакуют target
    int countAssignments(
        const vector<shared_ptr<LivingEntity>>& packmates,
        shared_ptr<LivingEntity> target);

    // Выбрать лучшую цель из candidates с учётом распределения.
    // respectLimit: если true — не назначать на цели с >= MAX_WOLVES_PER_TARGET
    shared_ptr<LivingEntity> pickBestTarget(
        const vector<shared_ptr<Entity>>* candidates,
        const vector<shared_ptr<LivingEntity>>& packmates,
        bool respectLimit);
};
