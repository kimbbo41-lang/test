#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <memory>
#include <functional>
#include <limits>

// 직업 유형
enum class JobType
{
    Warrior,    // 전사
    Mage,       // 마법사
    Archer      // 궁수
};

// 아이템 유형
enum class ItemType
{
    Potion,     // 소비 아이템 (HP/MP 회복)
    Weapon,     // 무기
    Armor       // 방어구
};

// 스킬 유형
enum class SkillType
{
    Physical,   // 물리 스킬
    Magic,      // 마법 스킬
    Heal        // 회복 스킬
};

// 던전 이벤트 유형
enum class EventType
{
    Monster,    // 몬스터 조우
    Treasure,   // 보물 상자
    Trap,       // 함정
    Rest,       // 휴식처
    Shop        // 상점
};

// 전투 결과
enum class CombatResult
{
    Victory,    // 승리
    Defeat,     // 패배
    Escape      // 도망
};

// 전투 행동
enum class CombatAction
{
    Attack,     // 기본 공격
    Skill,      // 스킬 사용
    Item,       // 아이템 사용
    Escape      // 도망
};

// 상태이상 유형
enum class StatusEffect
{
    None,       // 없음
    Poison,     // 독 (최대 HP 비율 데미지)
    Burn,       // 화상 (고정 데미지, 방어 무시)
    Stun        // 기절 (다음 턴 행동 불가)
};

// 상태이상 인스턴스
struct StatusInstance
{
    StatusEffect type = StatusEffect::None;
    int remainingTurns = 0;
    int magnitude = 0;      // Burn: 턴당 고정 데미지, Poison: 최대 HP 비율(%), Stun: 미사용
};

// 저주 페널티 유형
enum class CursePenalty
{
    HpPercent,      // 최대 HP 감소
    MpPercent,      // 최대 MP 감소
    Agility         // 민첩 감소
};

// 캐릭터 기본 스탯 구조체
struct Stats
{
    int hp = 0;
    int maxHp = 0;
    int mp = 0;
    int maxMp = 0;
    int attack = 0;
    int defense = 0;
    int agility = 0;
};

// 전역 랜덤 엔진
inline std::mt19937& getRandomEngine()
{
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

// min~max 범위 랜덤 정수
inline int randomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(getRandomEngine());
}

// 0.0~1.0 범위 랜덤 실수
inline double randomDouble()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(getRandomEngine());
}
