#pragma once

// ============================================
// 게임 밸런스 설정값
// 모든 에이전트는 이 파일의 상수를 참조하여
// 일관된 게임 밸런스를 유지한다.
// ============================================

namespace Config
{
    // --- 캐릭터 ---
    constexpr int MAX_LEVEL = 10;
    constexpr int EXP_PER_LEVEL = 50;           // 레벨업 필요 경험치 = 레벨 * EXP_PER_LEVEL
    constexpr int STAT_GROWTH_PER_LEVEL = 3;    // 레벨업 시 주요 스탯 증가량

    // 전사 기본 스탯
    constexpr int WARRIOR_HP = 120;
    constexpr int WARRIOR_MP = 30;
    constexpr int WARRIOR_ATK = 15;
    constexpr int WARRIOR_DEF = 12;
    constexpr int WARRIOR_AGI = 8;

    // 마법사 기본 스탯
    constexpr int MAGE_HP = 70;
    constexpr int MAGE_MP = 100;
    constexpr int MAGE_ATK = 8;
    constexpr int MAGE_DEF = 5;
    constexpr int MAGE_AGI = 10;

    // 궁수 기본 스탯
    constexpr int ARCHER_HP = 90;
    constexpr int ARCHER_MP = 50;
    constexpr int ARCHER_ATK = 12;
    constexpr int ARCHER_DEF = 7;
    constexpr int ARCHER_AGI = 15;

    // --- 전투 ---
    constexpr double DAMAGE_MIN_MULTIPLIER = 0.8;   // 최소 데미지 배율
    constexpr double DAMAGE_MAX_MULTIPLIER = 1.2;   // 최대 데미지 배율
    constexpr double CRIT_RATE_PER_AGI = 0.02;      // 민첩 1당 크리티컬 확률 (2%)
    constexpr double MAX_CRIT_RATE = 0.30;           // 최대 크리티컬 확률 (30%)
    constexpr double CRIT_DAMAGE_MULTIPLIER = 1.5;   // 크리티컬 데미지 배율
    constexpr double ESCAPE_BASE_RATE = 0.5;         // 기본 도망 확률 (50%)
    constexpr double ESCAPE_AGI_BONUS = 0.02;        // 민첩 1당 도망 확률 보너스

    // --- 던전 ---
    constexpr int TOTAL_FLOORS = 5;                  // 총 던전 층수
    constexpr int MIN_ROOMS_PER_FLOOR = 3;           // 층당 최소 방 수
    constexpr int MAX_ROOMS_PER_FLOOR = 5;           // 층당 최대 방 수

    // 이벤트 확률 (합계 100)
    constexpr int EVENT_MONSTER_WEIGHT = 40;
    constexpr int EVENT_TREASURE_WEIGHT = 20;
    constexpr int EVENT_TRAP_WEIGHT = 15;
    constexpr int EVENT_REST_WEIGHT = 15;
    constexpr int EVENT_SHOP_WEIGHT = 10;

    // 함정 데미지
    constexpr int TRAP_MIN_DAMAGE = 5;
    constexpr int TRAP_MAX_DAMAGE = 20;

    // 휴식 회복량 (최대 HP/MP의 비율)
    constexpr double REST_HP_RECOVERY = 0.3;         // 30%
    constexpr double REST_MP_RECOVERY = 0.3;         // 30%

    // --- 인벤토리 ---
    constexpr int MAX_INVENTORY_SIZE = 10;           // 최대 인벤토리 슬롯

    // --- 아이템 가격 ---
    constexpr int SMALL_POTION_PRICE = 20;
    constexpr int LARGE_POTION_PRICE = 50;
    constexpr int MP_POTION_PRICE = 30;

    // --- 골드 ---
    constexpr int STARTING_GOLD = 100;               // 시작 골드
}
