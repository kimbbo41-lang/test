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
    constexpr int MAGE_HP = 80;
    constexpr int MAGE_MP = 110;
    constexpr int MAGE_ATK = 10;
    constexpr int MAGE_DEF = 5;
    constexpr int MAGE_AGI = 10;

    // 궁수 기본 스탯
    constexpr int ARCHER_HP = 100;
    constexpr int ARCHER_MP = 60;
    constexpr int ARCHER_ATK = 14;
    constexpr int ARCHER_DEF = 8;
    constexpr int ARCHER_AGI = 18;

    // --- 전투 ---
    constexpr double DAMAGE_MIN_MULTIPLIER = 0.8;   // 최소 데미지 배율
    constexpr double DAMAGE_MAX_MULTIPLIER = 1.2;   // 최대 데미지 배율
    constexpr double CRIT_RATE_PER_AGI = 0.02;      // 민첩 1당 크리티컬 확률 (2%)
    constexpr double MAX_CRIT_RATE = 0.40;           // 최대 크리티컬 확률 (40%)
    constexpr double CRIT_DAMAGE_MULTIPLIER = 1.5;   // 크리티컬 데미지 배율
    constexpr double ESCAPE_BASE_RATE = 0.5;         // 기본 도망 확률 (50%)
    constexpr double ESCAPE_AGI_BONUS = 0.02;        // 민첩 1당 도망 확률 보너스

    // --- 던전 ---
    constexpr int TOTAL_FLOORS = 10;                 // 총 던전 층수
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
    constexpr int LARGE_POTION_PRICE = 60;           // 큰 체력 물약: HP + 상태이상 해제
    constexpr int MP_POTION_PRICE = 30;
    constexpr int LARGE_MP_POTION_PRICE = 70;

    // --- 골드 ---
    constexpr int STARTING_GOLD = 100;               // 시작 골드

    // --- 엘리트 몬스터 ---
    constexpr int ELITE_APPEAR_FLOOR = 3;            // 엘리트 등장 최소 층
    constexpr int ELITE_CHANCE_PERCENT = 15;         // 일반 몬스터 방에서 엘리트 확률
    constexpr double ELITE_HP_MULT = 1.6;            // 엘리트 HP 배율
    constexpr double ELITE_ATK_MULT = 1.4;           // 엘리트 공격 배율
    constexpr double ELITE_AGI_MULT = 1.2;           // 엘리트 민첩 배율
    constexpr int ELITE_REWARD_MULT = 2;             // 엘리트 경험치/골드 배율
    constexpr int ELITE_STATUS_PROC_PERCENT = 20;    // 엘리트/보스 공격 시 상태이상 부여 확률

    // --- 저주받은 아이템 ---
    constexpr int CURSED_APPEAR_FLOOR = 3;           // 저주 아이템 최소 출현 층
    constexpr int CURSED_CHANCE_PERCENT = 12;        // 보물에서 장비가 저주일 확률
    constexpr double CURSED_BONUS_MULT = 1.6;        // 저주 장비 능력치 배율 (공/방)
    constexpr int CURSED_HP_PENALTY_PERCENT = 15;    // 저주 페널티: 최대 HP 감소율
    constexpr int CURSED_MP_PENALTY_PERCENT = 20;    // 저주 페널티: 최대 MP 감소율
    constexpr int CURSED_AGI_PENALTY = 5;            // 저주 페널티: 민첩 감소량

    // --- 상태이상 ---
    constexpr int POISON_DURATION = 3;               // 독 지속 턴
    constexpr int POISON_HP_PERCENT = 5;             // 독 턴당 최대 HP 데미지율
    constexpr int BURN_DURATION = 3;                 // 화상 지속 턴
    constexpr int BURN_DAMAGE_PER_FLOOR = 3;         // 화상 고정 데미지 (층수 × 이 값)
    constexpr int STUN_DURATION = 1;                 // 기절 지속 턴
    constexpr int TRAP_POISON_CHANCE_PERCENT = 25;   // 함정에서 독 부여 확률
}
