#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accNumber;
    char name[50];
    float balance;
};

void createAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "ab");

    printf("Enter Account Number: ");
    scanf("%d", &acc.accNumber);
    printf("Enter Name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("Account created successfully.\n");
}

void viewAccounts() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb");

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- All Accounts ---\n");
    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("Account No: %d\nName: %s\nBalance: %.2f\n\n", acc.accNumber, acc.name, acc.balance);
    }
    fclose(fp);
}

void searchAccount() {
    struct Account acc;
    int searchNo, found = 0;
    FILE *fp = fopen("accounts.dat", "rb");

    if (!fp) {
        printf("No accounts available.\n");
        return;
    }

    printf("Enter Account Number to Search: ");
    scanf("%d", &searchNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == searchNo) {
            printf("Account Found:\nName: %s\nBalance: %.2f\n", acc.name, acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found.\n");
    fclose(fp);
}

void deposit() {
    struct Account acc;
    int accNo, found = 0;
    float amount;
    FILE *fp = fopen("accounts.dat", "rb+");

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNo) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;

            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            found = 1;
            printf("Amount deposited successfully.\n");
            break;
        }
    }

    if (!found) printf("Account not found.\n");
    fclose(fp);
}

void withdraw() {
    struct Account acc;
    int accNo, found = 0;
    float amount;
    FILE *fp = fopen("accounts.dat", "rb+");

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNo) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount <= acc.balance) {
                acc.balance -= amount;
                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);
                printf("Withdrawal successful.\n");
            } else {
                printf("Insufficient balance.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found.\n");
    fclose(fp);
}

void deleteAccount() {
    struct Account acc;
    int accNo;
    FILE *fp = fopen("accounts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    printf("Enter Account Number to Delete: ");
    scanf("%d", &accNo);

    int deleted = 0;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber != accNo) {
            fwrite(&acc, sizeof(acc), 1, temp);
        } else {
            deleted = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (deleted)
        printf("Account deleted successfully.\n");
    else
        printf("Account not found.\n");
}

int main() {
    int choice;

    do {
        printf("\n--- Bank Management System ---\n");
        printf("1. Create Account\n");
        printf("2. View All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Deposit\n");
        printf("5. Withdraw\n");
        printf("6. Delete Account\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccounts(); break;
            case 3: searchAccount(); break;
            case 4: deposit(); break;
            case 5: withdraw(); break;
            case 6: deleteAccount(); break;
            case 0: printf("Thank you!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}
