/*
    CASINO SIMULATOR
    Compatible with Dev C++ and Programiz
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STARTING_BALANCE 1000
#define VIP_THRESHOLD    50000

/* Global Variables */
char playerName[50];
int balance;
int luck;
int charms;
int goldBars;
int insurance;
int vipCharms;
int isVIP;
int totalEarned;

/* ---- UTILITY ---- */

void pauseScreen() {
    printf("\nPress Enter to continue...");
    fflush(stdin);
    getchar();
}

void showStatus() {
    printf("\n----------------------------------\n");
    printf("  Player  : %s\n", playerName);
    printf("  Balance : $%d\n", balance);
    printf("  Luck    : %d%%\n", luck);
    if (isVIP) printf("  Status  : ** VIP MEMBER **\n");
    printf("----------------------------------\n");
}

int getEffectiveLuck() {
    int eff = luck;
    if (charms > 0)    eff += 15;
    if (vipCharms > 0) eff += 25;
    if (eff > 90) eff = 90;
    return eff;
}

int luckyReroll() {
    return (rand() % 100) < getEffectiveLuck();
}

int getBet() {
    int bet;
    printf("Enter bet (1 to %d): $", balance);
    scanf("%d", &bet);
    if (bet < 1) bet = 1;
    if (bet > balance) bet = balance;
    return bet;
}

void applyWin(int amount) {
    balance += amount;
    totalEarned += amount;
    printf("You WON $%d!\n", amount);
}

void applyLoss(int amount) {
    if (insurance > 0) {
        insurance--;
        printf("Insurance used! Bet is protected.\n");
    } else {
        balance -= amount;
        printf("You LOST $%d!\n", amount);
    }
}

void checkVIP() {
    if (!isVIP && totalEarned >= VIP_THRESHOLD) {
        isVIP = 1;
        vipCharms++;
        printf("\n*** CONGRATULATIONS! ***\n");
        printf("You earned over $50,000 and unlocked VIP STATUS!\n");
        printf("VIP Charm added! (+25%% luck bonus)\n");
        pauseScreen();
    }
}

/* ---- SHOP ---- */

void shop() {
    int choice;
    printf("\n========== SHOP ==========\n");
    showStatus();
    printf("\n1. Lucky Charm    - $500  (have: %d)\n", charms);
    printf("2. Gold Bar       - $1000 (have: %d)\n", goldBars);
    printf("3. Insurance      - $300  (have: %d)\n", insurance);
    if (isVIP)
        printf("4. VIP Charm      - $2000 (have: %d)\n", vipCharms);
    printf("5. Sell Gold Bar  (+$1200)\n");
    printf("0. Back\n");
    printf("Choose: ");
    scanf("%d", &choice);

    if (choice == 1) {
        if (balance >= 500) { balance -= 500; charms++; luck += 5; printf("Lucky Charm bought!\n"); }
        else printf("Not enough balance!\n");
    } else if (choice == 2) {
        if (balance >= 1000) { balance -= 1000; goldBars++; printf("Gold Bar bought!\n"); }
        else printf("Not enough balance!\n");
    } else if (choice == 3) {
        if (balance >= 300) { balance -= 300; insurance++; printf("Insurance bought!\n"); }
        else printf("Not enough balance!\n");
    } else if (choice == 4 && isVIP) {
        if (balance >= 2000) { balance -= 2000; vipCharms++; printf("VIP Charm bought!\n"); }
        else printf("Not enough balance!\n");
    } else if (choice == 5) {
        if (goldBars > 0) { goldBars--; balance += 1200; totalEarned += 1200; printf("Sold for $1200!\n"); }
        else printf("No Gold Bars!\n");
    }

    pauseScreen();
}

/* ---- INVENTORY ---- */

void showInventory() {
    printf("\n========== INVENTORY ==========\n");
    printf("Lucky Charms : %d\n", charms);
    printf("Gold Bars    : %d\n", goldBars);
    printf("Insurance    : %d\n", insurance);
    if (isVIP) printf("VIP Charms   : %d\n", vipCharms);
    printf("Luck         : %d%%\n", getEffectiveLuck());
    printf("Total Earned : $%d\n", totalEarned);
    pauseScreen();
}

/* ---- GAME 1: ROLL DICE ---- */

void gameDice() {
    int choice, roll, won, bet;
    printf("\n========== ROLL THE DICE ==========\n");
    printf("Guess HIGH (4-6) or LOW (1-3). Win = 2x bet.\n");
    showStatus();

    if (balance <= 0) { printf("No balance!\n"); pauseScreen(); return; }

    bet = getBet();
    printf("1. High (4-6)\n2. Low  (1-3)\nChoose: ");
    scanf("%d", &choice);

    roll = (rand() % 6) + 1;
    printf("Dice rolled: %d\n", roll);

    won = (choice == 1 && roll >= 4) || (choice == 2 && roll <= 3);

    if (!won && luckyReroll()) {
        printf("Lucky Charm activated! Rerolling...\n");
        roll = (rand() % 6) + 1;
        printf("New roll: %d\n", roll);
        won = (choice == 1 && roll >= 4) || (choice == 2 && roll <= 3);
    }

    if (won) {
        if (roll == 6 && charms > 0) {
            printf("777 BONUS! Triple payout!\n");
            bet = bet * 3;
        }
        applyWin(bet);
    } else {
        applyLoss(bet);
    }

    checkVIP();
    pauseScreen();
}

/* ---- GAME 2: HEADS OR TAILS ---- */

void gameHeadsTails() {
    int choice, flip, won, bet;
    printf("\n========== HEADS OR TAILS ==========\n");
    printf("50/50 game. Win = 2x bet.\n");
    showStatus();

    if (balance <= 0) { printf("No balance!\n"); pauseScreen(); return; }

    bet = getBet();
    printf("1. Heads\n2. Tails\nChoose: ");
    scanf("%d", &choice);

    flip = rand() % 2;
    printf("Result: %s\n", flip == 0 ? "HEADS" : "TAILS");

    won = (choice == 1 && flip == 0) || (choice == 2 && flip == 1);

    if (!won && luckyReroll()) {
        printf("Lucky reroll!\n");
        flip = rand() % 2;
        printf("New result: %s\n", flip == 0 ? "HEADS" : "TAILS");
        won = (choice == 1 && flip == 0) || (choice == 2 && flip == 1);
    }

    if (won) applyWin(bet);
    else     applyLoss(bet);

    checkVIP();
    pauseScreen();
}

/* ---- GAME 3: GUESS THE NUMBER ---- */

void gameGuessNumber() {
    int secret, guess, tries, won, diff, bet;
    printf("\n========== GUESS THE NUMBER ==========\n");
    printf("Guess 1-100. You get 5 tries. Win = 5x bet!\n");
    showStatus();

    if (balance <= 0) { printf("No balance!\n"); pauseScreen(); return; }

    bet = getBet();
    secret = (rand() % 100) + 1;
    if (getEffectiveLuck() > 50)
        secret = secret / 2 + 25;

    tries = 5;
    won = 0;

    while (tries > 0) {
        printf("\nTries left: %d | Guess: ", tries);
        scanf("%d", &guess);

        if (guess == secret) { won = 1; break; }

        diff = guess < secret ? secret - guess : guess - secret;

        if (diff <= 5)       printf("VERY HOT! ");
        else if (diff <= 15) printf("HOT! ");
        else if (diff <= 30) printf("WARM. ");
        else                 printf("COLD. ");

        if (guess < secret) printf("Go higher!\n");
        else                printf("Go lower!\n");

        tries--;
    }

    printf("The number was: %d\n", secret);
    if (won) applyWin(bet * 5);
    else     applyLoss(bet);

    checkVIP();
    pauseScreen();
}

/* ---- GAME 4: NUMBER VAULT ---- */

void gameNumberVault() {
    int secret[4], guess[4], attempts, won;
    int correct_pos, correct_digit, i, j;
    int s_used[4], g_used[4];
    char input[10];
    int bet;

    printf("\n========== NUMBER VAULT ==========\n");
    printf("Crack the 4-digit code!\n");
    printf("(+) = right digit right place\n");
    printf("(?) = right digit wrong place\n");
    printf("(-) = wrong digit\n");
    printf("8 attempts. Win = 10x bet!\n");
    showStatus();

    if (balance <= 0) { printf("No balance!\n"); pauseScreen(); return; }

    bet = getBet();

    for (i = 0; i < 4; i++) {
        secret[i] = rand() % 10;
        if (getEffectiveLuck() > 60)
            secret[i] = rand() % 5;
    }

    attempts = 8;
    won = 0;

    while (attempts > 0) {
        printf("\nAttempts left: %d | Enter 4-digit code: ", attempts);
        scanf("%s", input);

        if ((int)strlen(input) != 4) { printf("Enter exactly 4 digits!\n"); continue; }

        won = 1;
        for (i = 0; i < 4; i++) {
            if (input[i] < '0' || input[i] > '9') { won = 0; break; }
            guess[i] = input[i] - '0';
        }
        if (!won) { printf("Digits only!\n"); won = 0; continue; }

        correct_pos = 0; correct_digit = 0;
        for (i = 0; i < 4; i++) s_used[i] = g_used[i] = 0;

        for (i = 0; i < 4; i++) {
            if (guess[i] == secret[i]) {
                correct_pos++;
                s_used[i] = g_used[i] = 1;
            }
        }
        for (i = 0; i < 4; i++) {
            if (g_used[i]) continue;
            for (j = 0; j < 4; j++) {
                if (!s_used[j] && guess[i] == secret[j]) {
                    correct_digit++;
                    s_used[j] = 1;
                    break;
                }
            }
        }

        printf("Result: ");
        for (i = 0; i < correct_pos;   i++) printf("(+) ");
        for (i = 0; i < correct_digit; i++) printf("(?) ");
        for (i = 0; i < 4 - correct_pos - correct_digit; i++) printf("(-) ");
        printf("\n");

        if (correct_pos == 4) { won = 1; break; }
        won = 0;
        attempts--;
    }

    printf("The code was: %d%d%d%d\n", secret[0], secret[1], secret[2], secret[3]);
    if (won) applyWin(bet * 10);
    else     applyLoss(bet);

    checkVIP();
    pauseScreen();
}

/* ---- GAME 5: LUCKY 7s ---- */

void gameLucky7s() {
    int s1, s2, s3, lk, bet, payout;
    printf("\n========== THE LUCKY 7s ==========\n");
    printf("Spin 3 numbers (1-9).\n");
    printf("2 same = 2x | 3 same = 3x | Three 7s = 10x JACKPOT!\n");
    showStatus();

    if (balance <= 0) { printf("No balance!\n"); pauseScreen(); return; }

    bet = getBet();

    s1 = (rand() % 9) + 1;
    s2 = (rand() % 9) + 1;
    s3 = (rand() % 9) + 1;

    lk = getEffectiveLuck();
    if (lk > 40 && (rand() % 100) < lk)        s2 = s1;
    if (lk > 70 && (rand() % 100) < (lk - 30)) s3 = s1;

    printf("\n[ %d ][ %d ][ %d ]\n", s1, s2, s3);

    payout = 0;
    if (s1 == 7 && s2 == 7 && s3 == 7) {
        payout = bet * 10;
        printf("JACKPOT! TRIPLE 7s!\n");
    } else if (s1 == s2 && s2 == s3) {
        payout = bet * 3;
        printf("THREE OF A KIND! 3x!\n");
    } else if (s1 == s2 || s2 == s3 || s1 == s3) {
        payout = bet * 2;
        printf("PAIR! 2x!\n");
    } else {
        printf("No match.\n");
    }

    if (payout > 0) applyWin(payout);
    else            applyLoss(bet);

    checkVIP();
    pauseScreen();
}

/* ---- MINI GAMES MENU ---- */

void miniGamesMenu() {
    int choice;
    do {
        printf("\n========== MINI GAMES ==========\n");
        showStatus();
        printf("\n1. Roll the Dice\n");
        printf("2. Heads or Tails\n");
        printf("3. Guess the Number\n");
        printf("4. Number Vault\n");
        printf("5. The Lucky 7s\n");
        printf("0. Back to Lobby\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if      (choice == 1) gameDice();
        else if (choice == 2) gameHeadsTails();
        else if (choice == 3) gameGuessNumber();
        else if (choice == 4) gameNumberVault();
        else if (choice == 5) gameLucky7s();
        else if (choice != 0) printf("Invalid choice.\n");

    } while (choice != 0);
}

/* ---- LOBBY ---- */

void lobby() {
    int choice;
    do {
        printf("\n====================================\n");
        printf("       CASINO SIMULATOR v1.0\n");
        printf("====================================\n");
        showStatus();
        printf("\n1. Play Mini Games\n");
        printf("2. Shop\n");
        printf("3. Inventory\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if      (choice == 1) miniGamesMenu();
        else if (choice == 2) shop();
        else if (choice == 3) showInventory();
        else if (choice == 0) {
            printf("\nThanks for playing!\n");
            printf("Final Balance: $%d\n", balance);
            printf("Goodbye, %s!\n", playerName);
        } else {
            printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

/* ---- MAIN ---- */

int main() {
    srand((unsigned int)time(NULL));

    balance     = STARTING_BALANCE;
    luck        = 10;
    charms      = 0;
    goldBars    = 0;
    insurance   = 0;
    vipCharms   = 0;
    isVIP       = 0;
    totalEarned = 0;

    printf("====================================\n");
    printf("       CASINO SIMULATOR v1.0\n");
    printf("====================================\n");
    printf("\nWelcome! Enter your name: ");
    scanf("%49s", playerName);
    printf("\nHello, %s! Starting balance: $%d\n", playerName, STARTING_BALANCE);
    pauseScreen();

    lobby();
    return 0;
}
