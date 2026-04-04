# 코딩 컨벤션

## 네이밍 규칙

| 대상 | 스타일 | 예시 |
|------|--------|------|
| 클래스 / 구조체 | PascalCase | `Character`, `CombatEngine` |
| 함수 / 메서드 | camelCase | `takeDamage()`, `getHp()` |
| 변수 | camelCase | `maxHp`, `playerName` |
| 멤버 변수 | camelCase | `hp`, `attackPower` |
| 상수 / 매크로 | UPPER_SNAKE_CASE | `MAX_LEVEL`, `DEFAULT_HP` |
| 열거형 타입 | PascalCase | `JobType`, `EventType` |
| 열거형 값 | PascalCase | `Warrior`, `FireBall` |
| 파일명 | PascalCase | `Character.h`, `Combat.cpp` |

## 파일 구조

### 헤더 파일 (.h)
```cpp
#pragma once

#include <필요한 표준 헤더>
#include "필요한 프로젝트 헤더"

class ClassName {
public:
    // 생성자 / 소멸자
    // public 메서드
    // getter / setter

private:
    // 멤버 변수
    // private 메서드
};
```

### 구현 파일 (.cpp)
```cpp
#include "ClassName.h"

// 생성자 구현
// 메서드 구현 (헤더에 선언된 순서대로)
```

## 코드 스타일

### 들여쓰기
- **4칸 스페이스** (탭 사용 금지)

### 중괄호
- 클래스, 함수: 다음 줄에 배치
```cpp
class Character
{
public:
    void attack()
    {
        // ...
    }
};
```

### 포인터 / 참조
```cpp
int* ptr;          // * 는 타입 쪽에 붙임
const std::string& name;  // & 도 타입 쪽에 붙임
```

### include 순서
```cpp
#include "자기 자신 헤더"    // 1순위
#include "프로젝트 헤더"     // 2순위
#include <표준 라이브러리>   // 3순위
```

## 메모리 관리
- `new` / `delete` 직접 사용 금지
- `std::unique_ptr` 또는 `std::shared_ptr` 사용
- 가능하면 스택 할당 또는 `std::vector` 활용

## 문자열
- `std::string` 사용 (C 스타일 `char[]` 금지)
- 한글 문자열 리터럴: `"한글 텍스트"` (UTF-8)

## 입력 처리
- `cin >>` 사용 후 반드시 유효성 검사
```cpp
int choice;
std::cin >> choice;
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // 재입력 요청
}
```

## 주석
- 클래스 / 함수 위에 간단한 한글 주석
- 복잡한 로직에만 인라인 주석 추가
- 불필요한 주석은 작성하지 않음
