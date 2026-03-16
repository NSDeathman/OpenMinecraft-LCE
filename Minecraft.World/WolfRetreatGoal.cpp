#include "stdafx.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "WolfRetreatGoal.h"

WolfRetreatGoal::WolfRetreatGoal(TamableAnimal* tameAnimal,
    float retreatHealthFraction,
    float runSpeedMultiplier)
{
    this->tameAnimal = tameAnimal;
    this->retreatHealthFraction = retreatHealthFraction;
    this->runSpeedMultiplier = runSpeedMultiplier;
    this->recalcPathTicks = 0;

    // Цель использует флаг движения — блокирует FollowOwnerGoal,
    // RandomStrollGoal и т.д. пока активна
    setRequiredControlFlags(0);
}

// ---------------------------------------------------------------------------
bool WolfRetreatGoal::canUse()
{
    // Только прирученный и не сидящий
    if (!tameAnimal->isTame())    return false;
    if (tameAnimal->isSitting())  return false;

    // Проверяем порог здоровья
    float hpFraction = tameAnimal->getHealth() / tameAnimal->getMaxHealth();
    if (hpFraction > retreatHealthFraction)   return false;

    // Есть живой хозяин
    shared_ptr<Entity> owner = tameAnimal->getOwner();
    if (owner == nullptr) return false;

    shared_ptr<LivingEntity> ownerLiving =
        dynamic_pointer_cast<LivingEntity>(owner);
    return ownerLiving != nullptr && ownerLiving->isAlive();
}

// ---------------------------------------------------------------------------
bool WolfRetreatGoal::canContinueToUse()
{
    // Прекратить если сел по команде
    if (tameAnimal->isSitting()) return false;

    // Прекратить если достаточно вылечился (с гистерезисом)
    float hpFraction = tameAnimal->getHealth() / tameAnimal->getMaxHealth();
    if (hpFraction >= retreatHealthFraction + HYSTERESIS) return false;

    // Прекратить если хозяин исчез или умер
    shared_ptr<Entity> owner = tameAnimal->getOwner();
    if (owner == nullptr) return false;
    shared_ptr<LivingEntity> ownerLiving =
        dynamic_pointer_cast<LivingEntity>(owner);
    if (ownerLiving == nullptr || !ownerLiving->isAlive()) return false;

    // Прекратить если уже добежали до хозяина
    if (tameAnimal->distanceToSqr(owner) <= SAFE_DIST_SQ) return false;

    return true;
}

// ---------------------------------------------------------------------------
void WolfRetreatGoal::start()
{
    recalcPathTicks = 0;

    // Бросить текущую боевую цель и сбросить злость
    tameAnimal->setTarget(nullptr);
    tameAnimal->onTameAggroStopped();   // Wolf: setAngry(false)

    // Сразу двигаемся к хозяину
    shared_ptr<Entity> owner = tameAnimal->getOwner();
    if (owner != nullptr)
    {
        tameAnimal->getNavigation()->moveTo(owner, runSpeedMultiplier);
    }
}

// ---------------------------------------------------------------------------
void WolfRetreatGoal::stop()
{
    tameAnimal->getNavigation()->stop();
}

// ---------------------------------------------------------------------------
void WolfRetreatGoal::tick()
{
    // Пересчитываем путь не каждый тик — хозяин движется,
    // путь устаревает
    if (--recalcPathTicks <= 0)
    {
        recalcPathTicks = PATH_RECALC_INTERVAL;

        shared_ptr<Entity> owner = tameAnimal->getOwner();
        if (owner != nullptr)
        {
            tameAnimal->getNavigation()->moveTo(owner, runSpeedMultiplier);
        }
    }
}
