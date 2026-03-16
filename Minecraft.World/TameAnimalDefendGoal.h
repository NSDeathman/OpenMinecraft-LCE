#pragma once

#include "TargetGoal.h"

class TamableAnimal;

// Прирученный питомец проактивно атакует ближайшего враждебного моба в радиусе
class TameAnimalDefendGoal : public TargetGoal
{
private:
    TamableAnimal* tameAnimal;
    weak_ptr<LivingEntity>  nearestHostile;

public:
    // mustSee=true: видеть цель перед атакой
    // scanRadius: если 0, используется FOLLOW_RANGE атрибут
    TameAnimalDefendGoal(TamableAnimal* tameAnimal, bool mustSee);

    bool canUse()  override;
    void start()   override;
    void stop()   override;
};
