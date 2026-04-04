#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

// 콘솔 색상 설정
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 색상 코드
#define WHITE   15
#define YELLOW  14
#define CYAN    11
#define GREEN   10
#define RED      12
#define MAGENTA 13
#define GRAY     8

// 타이틀 출력
void printTitle() {
    system("cls");
    setColor(CYAN);
    printf("╔══════════════════════════════════════╗\n");
    printf("║      🎮  가위바위보 게임 2022        ║\n");
    printf("╚══════════════════════════════════════╝\n");
    setColor(WHITE);
}

// 점수판 출력
void printScore(int win, int lose, int draw) {
    setColor(YELLOW);
    printf("\n[ 점수판 ]  ");
    setColor(GREEN);  printf("승리: %d  ", win);
    setColor(RED);    printf("패배: %d  ", lose);
    setColor(GRAY);   printf("무승부: %d\n", draw);
    setColor(WHITE);
}

// 손 모양 출력
void printHand(int choice) {
    if (choice == 1) {
        printf("  ✌  가위\n");
    }
    else if (choice == 2) {
        printf("  ✊  바위\n");
    }
    else {
        printf("  🖐  보\n");
    }
}

int main() {
    // 한글 출력 설정
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    srand((unsigned int)time(NULL));

    int user, computer;
    int win = 0, lose = 0, draw = 0;
    int totalGame = 0;
    char again;

    while (1) {
        printTitle();
        printScore(win, lose, draw);

        setColor(YELLOW);
        printf("\n  ┌─────────────────────┐\n");
        printf("  │  1. ✌  가위         │\n");
        printf("  │  2. ✊  바위         │\n");
        printf("  │  3. 🖐  보           │\n");
        printf("  │  0. 🚪  종료         │\n");
        printf("  └─────────────────────┘\n");
        setColor(WHITE);

        printf("\n  선택 >> ");
        scanf_s("%d", &user);

        if (user == 0) break;

        if (user < 1 || user > 3) {
            setColor(RED);
            printf("\n  ⚠ 잘못된 입력! 0~3 사이로 입력하세요.\n");
            setColor(WHITE);
            Sleep(1200);
            continue;
        }

        computer = rand() % 3 + 1;
        totalGame++;

        // 대결 화면
        printTitle();
        setColor(CYAN);
        printf("\n  ═══════ 대결 결과 ═══════\n\n");

        setColor(GREEN);
        printf("  👤 나   : ");
        setColor(WHITE);
        printHand(user);

        setColor(RED);
        printf("  🤖 컴퓨터: ");
        setColor(WHITE);
        printHand(computer);

        printf("\n");

        // 승패 판정
        if (user == computer) {
            draw++;
            setColor(YELLOW);
            printf("  ══════════════════════\n");
            printf("  ⚖  비겼습니다!\n");
            printf("  ══════════════════════\n");
        }
        else if ((user == 1 && computer == 3) ||
            (user == 2 && computer == 1) ||
            (user == 3 && computer == 2)) {
            win++;
            setColor(GREEN);
            printf("  ══════════════════════\n");
            printf("  🎉 당신이 이겼습니다!\n");
            printf("  ══════════════════════\n");
        }
        else {
            lose++;
            setColor(RED);
            printf("  ══════════════════════\n");
            printf("  😢 컴퓨터가 이겼습니다!\n");
            printf("  ══════════════════════\n");
        }

        printScore(win, lose, draw);

        setColor(MAGENTA);
        printf("\n  총 %d판 | 승률: %.1f%%\n",
            totalGame,
            totalGame > 0 ? (float)win / totalGame * 100 : 0);

        setColor(WHITE);
        printf("\n  계속하려면 아무 키나 누르세요...");
        _getch();
    }

    // 최종 결과
    printTitle();
    setColor(YELLOW);
    printf("\n  ╔══════════════════════════╗\n");
    printf("  ║       최종 결과          ║\n");
    printf("  ╠══════════════════════════╣\n");
    printf("  ║  총 게임 수 : %3d판      ║\n", totalGame);
    setColor(GREEN);
    printf("  ║  승    리   : %3d판      ║\n", win);
    setColor(RED);
    printf("  ║  패    배   : %3d판      ║\n", lose);
    setColor(GRAY);
    printf("  ║  무 승 부   : %3d판      ║\n", draw);
    setColor(CYAN);
    printf("  ║  승    률   : %5.1f%%     ║\n",
        totalGame > 0 ? (float)win / totalGame * 100 : 0);
    setColor(YELLOW);
    printf("  ╚══════════════════════════╝\n");

    setColor(WHITE);
    printf("\n  이용해 주셔서 감사합니다! 👋\n\n");
    system("pause");

    return 0;
}