#pragma once

#include "Goal.h"

class TamableAnimal;

class WolfRetreatGoal : public Goal
{
private:
    TamableAnimal* tameAnimal;

    // Порог HP (0.0–1.0), ниже которого начинаем отступать
    float           retreatHealthFraction;

    // Множитель скорости при отступлении
    float           runSpeedMultiplier;

    // Счётчик тиков для переcчёта пути (не каждый тик)
    int             recalcPathTicks;

    // Гистерезис: восстановиться нужно чуть выше порога отступления,
    // чтобы не дёргаться туда-обратно
    static constexpr float HYSTERESIS = 0.15f;

    // Расстояние до хозяина, при котором считаем "добежали"
    static constexpr float SAFE_DIST_SQ = 4.0f * 4.0f;

    // Пересчитывать путь каждые N тиков
    static constexpr int   PATH_RECALC_INTERVAL = 10;

public:
    WolfRetreatGoal(TamableAnimal* tameAnimal,
        float retreatHealthFraction,
        float runSpeedMultiplier = 1.5f);

    bool canUse()           override;
    bool canContinueToUse() override;
    void start()            override;
    void stop()             override;
    void tick()             override;
};
