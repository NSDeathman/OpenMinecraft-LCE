#include "stdafx.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.phys.h"
#include "Level.h"
#include "Wolf.h"
#include "WolfPackDistributedTargetGoal.h"

// ---------------------------------------------------------------------------
PackDistributedTargetGoal::PackDistributedTargetGoal(
    TamableAnimal* tameAnimal,
    bool mustSee,
    float maxChaseDistFromOwner)
    : TargetGoal(tameAnimal, mustSee)
{
    this->tameAnimal = tameAnimal;
    this->maxChaseDistFromOwner = maxChaseDistFromOwner;
    setRequiredControlFlags(TargetGoal::TargetFlag);
}

// ---------------------------------------------------------------------------
vector<shared_ptr<LivingEntity>> PackDistributedTargetGoal::getPackmates(
    double range)
{
    vector<shared_ptr<LivingEntity>> result;

    wstring ownerUUID = tameAnimal->getOwnerUUID();
    if (ownerUUID.empty()) return result;

    // Ищем всех волков в радиусе
    vector<shared_ptr<Entity>>* nearby = mob->level->getEntitiesOfClass(
        typeid(Wolf),
        mob->bb->grow(range, 4.0, range),
        nullptr);

    if (nearby == nullptr)
        return result;

    for (auto& e : *nearby)
    {
        // Пропустить самого себя
        if (e.get() == mob) continue;

        shared_ptr<Wolf> wolf = dynamic_pointer_cast<Wolf>(e);
        if (wolf == nullptr)     continue;
        if (!wolf->isTame())     continue;

        // Только волки того же хозяина
        if (wolf->getOwnerUUID() != ownerUUID) continue;

        result.push_back(wolf);
    }

    delete nearby;
    return result;
}

// ---------------------------------------------------------------------------
int PackDistributedTargetGoal::countAssignments(
    const vector<shared_ptr<LivingEntity>>& packmates,
    shared_ptr<LivingEntity> target)
{
    if (target == nullptr) return 0;

    int count = 0;
    for (auto& packmate : packmates)
    {
        shared_ptr<LivingEntity> packTarget =
            dynamic_pointer_cast<LivingEntity>(
                dynamic_pointer_cast<Wolf>(packmate) != nullptr
                ? dynamic_pointer_cast<Wolf>(packmate)->getTarget()
                : nullptr);

        if (packTarget != nullptr && packTarget.get() == target.get())
            count++;
    }
    return count;
}

// ---------------------------------------------------------------------------
shared_ptr<LivingEntity> PackDistributedTargetGoal::pickBestTarget(
    const vector<shared_ptr<Entity>>* candidates,
    const vector<shared_ptr<LivingEntity>>& packmates,
    bool respectLimit)
{
    shared_ptr<LivingEntity> best = nullptr;
    double                   bestScore = DBL_MAX;

    shared_ptr<LivingEntity> owner =
        dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());

    for (auto& e : *candidates)
    {
        shared_ptr<LivingEntity> le = dynamic_pointer_cast<LivingEntity>(e);
        if (le == nullptr)                   continue;
        if (!canAttack(le, false))           continue;
        if (owner != nullptr && !tameAnimal->wantsToAttack(le, owner)) continue;

        int assigned = countAssignments(packmates, le);

        // Фаза 1: уважаем жёсткий лимит
        if (respectLimit && assigned >= MAX_WOLVES_PER_TARGET)
            continue;

        // score = штраф за назначенных волков + дистанция²
        // Это позволяет предпочесть менее занятую цель,
        // но при равном числе назначений — выбрать ближайшую
        double distSq = mob->distanceToSqr(le);
        double score = assigned * ASSIGNMENT_PENALTY_SQ + distSq;

        if (score < bestScore)
        {
            bestScore = score;
            best = le;
        }
    }

    return best;
}

// ---------------------------------------------------------------------------
bool PackDistributedTargetGoal::canUse()
{
    if (!tameAnimal->isTame())    return false;
    if (tameAnimal->isSitting())  return false;
    if (mob->getTarget() != nullptr && mob->getTarget()->isAlive())
        return false;

    double range = getFollowDistance();

    vector<shared_ptr<Entity>>* candidates = mob->level->getEntitiesOfClass(
        typeid(Monster),
        mob->bb->grow(range, 4.0, range),
        nullptr);

    if (candidates == nullptr || candidates->empty())
    {
        delete candidates;
        return false;
    }

    vector<shared_ptr<LivingEntity>> packmates = getPackmates(range);

    // Фаза 1: ищем цель с учётом лимита MAX_WOLVES_PER_TARGET
    shared_ptr<LivingEntity> target = pickBestTarget(candidates, packmates, true);

    // Фаза 2: если все цели заняты сверх лимита — игнорируем лимит,
    //         чтобы волк не простаивал
    if (target == nullptr)
        target = pickBestTarget(candidates, packmates, false);

    delete candidates;

    if (target == nullptr) return false;

    assignedTarget = weak_ptr<LivingEntity>(target);
    return true;
}

// ---------------------------------------------------------------------------
bool PackDistributedTargetGoal::canContinueToUse()
{
    // Базовые проверки TargetGoal (цель жива, в радиусе follow_range и т.д.)
    if (!TargetGoal::canContinueToUse()) return false;

    // Не удаляться от хозяина дальше поводка
    shared_ptr<Entity> owner = tameAnimal->getOwner();
    if (owner == nullptr) return false;

    return mob->distanceToSqr(owner) <=
        maxChaseDistFromOwner * maxChaseDistFromOwner;
}

// ---------------------------------------------------------------------------
void PackDistributedTargetGoal::start()
{
    shared_ptr<LivingEntity> target = assignedTarget.lock();
    mob->setTarget(target);
    tameAnimal->onTameAggroStarted(target);
    TargetGoal::start();
}
