#include "stdafx.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.animal.h"
#include "PrioritizeOwnerThreatGoal.h"

PrioritizeOwnerThreatGoal::PrioritizeOwnerThreatGoal(TamableAnimal* tameAnimal)
    : TargetGoal(tameAnimal, false)
{
    this->tameAnimal = tameAnimal;
    this->timestamp = 0;
    setRequiredControlFlags(TargetGoal::TargetFlag);
}

// ---------------------------------------------------------------------------
bool PrioritizeOwnerThreatGoal::canUse()
{
    // Только прирученный и не сидящий
    if (!tameAnimal->isTame())    return false;
    if (tameAnimal->isSitting())  return false;

    // Этот goal — именно ПЕРЕКЛЮЧАТЕЛЬ, а не первичный выбор цели.
    // Если текущей цели нет — пусть этим занимаются
    // HurtByTargetGoal / TameAnimalDefendGoal
    shared_ptr<LivingEntity> currentTarget = mob->getTarget();
    if (currentTarget == nullptr || !currentTarget->isAlive()) return false;

    // Нужен живой хозяин
    shared_ptr<LivingEntity> owner =
        dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());
    if (owner == nullptr || !owner->isAlive()) return false;

    // Кто последний ударил хозяина?
    shared_ptr<LivingEntity> ownerAttacker = owner->getLastHurtByMob();
    int ts = owner->getLastHurtByMobTimestamp();

    // Ничего нового с момента последней обработки
    if (ts == timestamp)          return false;

    // Уже атакуем именно эту угрозу — незачем переключаться
    if (ownerAttacker == currentTarget) return false;

    // Стандартные проверки: цель жива, не союзник, видна и т.д.
    if (!canAttack(ownerAttacker, false)) return false;

    // Питомец-специфичный фильтр (Wolf: не атаковать Creeper,
    // питомцев того же хозяина, PvP-флаг и т.д.)
    if (!tameAnimal->wantsToAttack(ownerAttacker, owner)) return false;

    // Переключаемся только если новая угроза ощутимо ближе к хозяину,
    // чем текущая цель — иначе не стоит бросать начатое
    double distToOwnerSq = owner->distanceToSqr(ownerAttacker);
    double currentDistToOwnerSq = owner->distanceToSqr(currentTarget);

    if (distToOwnerSq >= currentDistToOwnerSq - SWITCH_DIST_ADVANTAGE_SQ)
        return false;

    newThreat = weak_ptr<LivingEntity>(ownerAttacker);
    return true;
}

// ---------------------------------------------------------------------------
void PrioritizeOwnerThreatGoal::start()
{
    shared_ptr<LivingEntity> target = newThreat.lock();
    mob->setTarget(target);
    tameAnimal->onTameAggroStarted(target); // Wolf: setAngry(true)

    // Запомнить timestamp чтобы не обрабатывать это событие повторно
    shared_ptr<LivingEntity> owner =
        dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());
    if (owner != nullptr)
        timestamp = owner->getLastHurtByMobTimestamp();

    TargetGoal::start();
}
