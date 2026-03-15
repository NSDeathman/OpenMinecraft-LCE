#include "stdafx.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.phys.h"
#include "TameAnimalDefendGoal.h"
#include "Level.h"

TameAnimalDefendGoal::TameAnimalDefendGoal(TamableAnimal* tameAnimal, bool mustSee)
    : TargetGoal(tameAnimal, mustSee)
{
    this->tameAnimal = tameAnimal;
    setRequiredControlFlags(TargetGoal::TargetFlag);
}

bool TameAnimalDefendGoal::canUse()
{
    // Только прирученный и не сидящий питомец ищет угрозы сам
    if (!tameAnimal->isTame())    return false;
    if (tameAnimal->isSitting())  return false;

    // Если уже есть цель — не перебиваем (приоритет у реактивных целей)
    if (mob->getTarget() != nullptr && mob->getTarget()->isAlive()) return false;

    shared_ptr<LivingEntity> owner =
        dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());
    // owner может быть nullptr (оффлайн-игрок) — допускаем, но wantsToAttack
    // ниже обработает это корректно

    double range = getFollowDistance();

    // Ищем все Monster-сущности в радиусе
    vector<shared_ptr<Entity>>* candidates = mob->level->getEntitiesOfClass(
        typeid(Monster),
        mob->bb->grow(range, 4.0, range),
        nullptr     // без дополнительного фильтра — canAttack сделает за нас
    );

    if (candidates == nullptr || candidates->empty())
    {
        delete candidates;
        return false;
    }

    shared_ptr<LivingEntity> best = nullptr;
    double                   bestSq = DBL_MAX;

    for (auto& e : *candidates)
    {
        shared_ptr<LivingEntity> le = dynamic_pointer_cast<LivingEntity>(e);
        if (!le) continue;

        // Стандартная проверка дистанции, видимости, ограничений
        if (!canAttack(le, false)) continue;

        // Специфичная для питомца фильтрация
        // (не атаковать крипера, своих питомцев владельца и т.д.)
        if (owner != nullptr && !tameAnimal->wantsToAttack(le, owner)) continue;

        double distSq = mob->distanceToSqr(le);
        if (distSq < bestSq)
        {
            bestSq = distSq;
            best = le;
        }
    }

    delete candidates;

    if (!best) return false;

    nearestHostile = weak_ptr<LivingEntity>(best);
    return true;
}

void TameAnimalDefendGoal::start()
{
    shared_ptr<LivingEntity> target = nearestHostile.lock();
    mob->setTarget(target);
    tameAnimal->onTameAggroStarted(target);
    TargetGoal::start();
}

void TameAnimalDefendGoal::stop()
{
    tameAnimal->onTameAggroStopped();
    TargetGoal::stop();
}
