#pragma once

#include "TargetGoal.h"

class TamableAnimal;

// Если волк уже дерётся с мобом A, но хозяина начал бить моб B —
// переключиться на B. Работает только при наличии активной цели,
// иначе уступает TameAnimalDefendGoal и HurtByTargetGoal.
class PrioritizeOwnerThreatGoal : public TargetGoal
{
private:
    TamableAnimal* tameAnimal;
    weak_ptr<LivingEntity> newThreat;

    // Запоминаем timestamp последней обработанной атаки на хозяина,
    // чтобы не переключаться на одну и ту же угрозу дважды
    int                    timestamp;

    // Минимальная разница дистанций (в блоках²), при которой
    // новая угроза считается "ближе" и стоит переключиться.
    // Защищает от дёрганья между двумя равноудалёнными целями.
    static constexpr double SWITCH_DIST_ADVANTAGE_SQ = 4.0 * 4.0;

public:
    PrioritizeOwnerThreatGoal(TamableAnimal* tameAnimal);

    bool canUse()  override;
    void start()   override;
};

