
// including the necessary libaries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"

// define global vars
#define MAX_STRING 9
#define DECIMAL_BASE 10
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
const char *invalidOptionString = "Invalid option\n";

// declaring the necessary structs for the mission
typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

typedef struct Account
{
    unsigned int accountNumber;
    char *accountHolder;
    int balance;
} Account;

typedef struct Transaction
{
    unsigned int fromAccount;
    unsigned int toAccount;
    int amount;
} Transaction;

typedef struct Bank
{
    Node *accounts;
    Node *transactions;
} Bank;

// define handler function
typedef void (*Handler)(Bank *);

// define struct for menu item
typedef struct
{
    const char *name;
    Handler handler;
} MenuItem;

// declaring all the functions
void runMenu(MenuItem menu[], int menuSize, Bank *bank);
void exitProgram(Bank *bank);
MenuItem *createMenu(int *size);
void printMenu();
void createAccount(Bank *bank);
void deleteAccount(Bank *bank);
void updateAccountHolder(Bank *bank);
void changeBalance(Bank *bank);
void makeTransaction(Bank *bank);
void watchAccount(Bank *bank);

void robbery(Bank *bank, int isDemo);
void printTransactions(Bank *bank, unsigned int number);
void docuementTransaction(Bank *bank, unsigned int number1, unsigned int number2, int sum);
void doceumentDeposit(Bank *bank, unsigned int number, int sum);
void doceumentWithdraw(Bank *bank, unsigned int number, int sum);
void transferMoney(Bank *bank, char *holder);
void takeMoney(Bank *bank, unsigned int number);
void deposite(Bank *bank, unsigned int number);
void addAccountToList(Bank *bank, Account account, int isDemo);
void duplicate(Bank *bank, Bank *bankDemo);
void mallocCheckStringPlus1(char *check, char *toFree);
void mallocCheckStringPlus2(char *check, char *toFree1, char *toFree2);
void mallocCheckStringPlus3(char *check, char *toFree1, char *toFree2, char *toFree3);
void free3Strings(char *toFree1, char *toFree2, char *toFree3);
void reallocCheckString(char *holder);
void mallocCheckString(char *holder);
void changeBalanceHelper(char *holder, Bank *bank, unsigned int number);
int transferHelper(Bank *bank, unsigned int number1, unsigned int number2, int sum, int isDemo);
int transferMoney2(Bank *bank, char *holder);
int checkAccounts(Bank *bank, char *holder);
int checkFreeNumber(unsigned int number, Node *head);
int countSteps(unsigned int number, Node *head);
int makeNumber(char *holder, int base);
int isTransaction(char *holder);
int isActive(Bank *bank, unsigned int number);
int isNumber(char *holder);
int demo(Bank *bank, char *holder);
char *getString();

/**
 * @brief Main function
 *
 * This function asks the user for an option and calls the corresponding function.
 *
 * @return 0 on success
 */
int main()
{
    // declaring and intializing the bank and the choise
    Bank bank;
    bank.accounts = NULL;
    bank.transactions = NULL;
    int size;
    MenuItem *menu = createMenu(&size);
    runMenu(menu, size, &bank);
    robbery(&bank, 0);
    return 0;
}

/**
 * @brief function that frees the bank
 *
 * @param bank is the bank what needs to be freed
 * @param isDemo tells if its the real bank or the bank that was made for mission 5
 *
 * @return void
 */
void robbery(Bank *bank, int isDemo)
{
    // point the head of the accounts list and run through all of the nodes
    Node *accountNode = bank->accounts;
    while (accountNode != NULL)
    {
        Node *nextAccountNode = accountNode->next;
        Account *account = accountNode->data;
        // free the holder only if its not the demo and free the node anyway
        if (!isDemo)
        {
            char *holder = account->accountHolder;
            free(holder);
        }
        free(account);
        free(accountNode);
        // advance to the next node
        accountNode = nextAccountNode;
    }
    // stops the program if its not the real bank (because in the demo one there isnt transactions)
    if (isDemo)
    {
        return;
    }
    // point the head of the transactions list and run through all of the nodes
    Node *transactionNode = bank->transactions;
    while (transactionNode != NULL)
    {
        Node *nextTransactionNode = transactionNode->next;
        // free the node and the transaction
        free(transactionNode->data);
        free(transactionNode);
        // advance to the next node
        transactionNode = nextTransactionNode;
    }
}

/**
 * @brief function that exit the program if the memory alocation failed
 *
 * @param holder the string that "asked" the alocation
 *
 * @return void
 */
void mallocCheckString(char *holder)
{
    if (holder == NULL)
    {
        exit(1);
    }
}

/**
 * @brief function that exit the program if the memory realocation failed, and free the memory
 *
 * @param holder the string that "asked" the alocation
 *
 * @return void
 */
void reallocCheckString(char *holder)
{
    if (holder == NULL)
    {
        free(holder);
        exit(1);
    }
}

/**
 * @brief function that let the user deposite or withdraw money
 *
 * @param bank the bank
 *
 * @return void
 */
void changeBalance(Bank *bank)
{
    // takes the account number
    printf("Enter account number:\n");
    unsigned int number;
    scanf("%u", &number);
    scanf("%*c");
    // checks that this is a real account
    Node *head = bank->accounts;
    if (checkFreeNumber(number, head) == 1)
    {
        printError("Account not found\n");
        return;
    }
    // asks the user for the action he wants to make in the his account
    printf("Would you like to deposit or withdraw money?\n");
    char tempChar;
    int index = 1;
    // allocating memory for the first char
    char *holder = (char *)malloc(sizeof(char));
    mallocCheckString(holder);
    // takes the char one by one until the user press enter
    scanf("%c", &tempChar);
    while (tempChar != '\n')
    {
        holder[index - 1] = tempChar;
        // reallocating memory for the next char and puts it in the string
        holder = (char *)realloc(holder, (index + 1) * sizeof(char));
        reallocCheckString(holder);
        index++;
        scanf("%c", &tempChar);
    }
    // finish the string and make the action
    holder[index - 1] = '\0';
    changeBalanceHelper(holder, bank, number);
}

/**
 * @brief function that deposite or withdraw money in the account
 *
 * @param bank the bank
 * @param holder is the string that the user typed
 * @param number the account number
 *
 * @return void
 */
void changeBalanceHelper(char *holder, Bank *bank, unsigned int number)
{
    // declaring the only valid actions
    char withdraw[MAX_STRING] = "withdraw";
    char deposit[MAX_STRING] = "deposit";
    // checks that the action is valid
    if (strcmp(holder, withdraw) != 0 && strcmp(holder, deposit) != 0)
    {
        printError("Invalid action\n");
        free(holder);
        return;
        // deposit money if the user typed deposit
    }
    else if (strcmp(holder, withdraw) == 0)
    {
        takeMoney(bank, number);
        // withdraw money if the user typed withdraw
    }
    else if (strcmp(holder, deposit) == 0)
    {
        deposite(bank, number);
    }
    free(holder);
}

/**
 * @brief function that transfer money if the instructions are valid
 *
 * @param bank the bank
 *
 * @return void
 */
void makeTransaction(Bank *bank)
{
    // takes the instructions from the user
    printf("Enter instructions:\n");
    getchar();
    char *holder = getString();
    printf("The holder is:\n%s\n", holder);

    // checks that the syntax of the string is valid
    if (isTransaction(holder) == 0)
    {
        printError("Invalid instructions\n");
        free(holder);
        return;
        // checks that the accounts are exist
    }
    else if (checkAccounts(bank, holder) == 0)
    {
        printError("Invalid instructions\n");
        free(holder);
        return;
        // checks that all the transfers won't cause one of the accounts to be in overdraft
    }
    else if (demo(bank, holder) == 0)
    {
        printError("Invalid instructions\n");
        free(holder);
        return;
        // transfer the money if the instuction are valid
    }
    else
    {
        transferMoney(bank, holder);
        free(holder);
    }
}

/**
 * @brief function that simulates the transfer of the money in order to vheck if its valid
 *
 * @param bank the bank
 * @param holder the string with the instructions
 *
 * @return 1 if the istructions are valid, 0 else.
 */
int transferMoney2(Bank *bank, char *holder)
{
    // declaring consts for the digits and handle the first account
    char *temp = (char *)malloc(strlen(holder) + 1);
    mallocCheckStringPlus1(temp, holder);
    const char MINIMUM_DIGIT = '0', MAXIMUM_DIGIT = '9';
    int i = 0;
    char tempChar = holder[0];
    // takes the next char while the char is a digit
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT && tempChar != '\0')
    {
        temp[i] = tempChar;
        i++;
        tempChar = holder[i];
    }
    temp[i] = '\0';
    // convert the string to a number and advance to the next account
    unsigned int number1 = (unsigned int)makeNumber(temp, DECIMAL_BASE);
    if (tempChar != '\0')
    {
        i++;
        tempChar = holder[i];
    }
    // allocating memory for the next account
    char *temp2 = (char *)malloc(strlen(holder) + 1);
    mallocCheckStringPlus2(temp2, temp, holder);
    int j = 0;
    // takes the next char while the char is a digit
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT && tempChar != '\0')
    {
        temp2[j] = tempChar;
        i++;
        j++;
        tempChar = holder[i];
    }
    temp2[j] = '\0';
    // convert the string to a number and advance to the next account
    unsigned int number2 = (unsigned int)makeNumber(temp2, DECIMAL_BASE);
    // checks that the accounts arent the same
    if (number1 == number2)
    {
        return 0;
    }
    // advance to the sum
    if (tempChar != '\0')
    {
        i++;
        tempChar = holder[i];
    }
    // allocating memory for the sum of the transfer
    char *temp3 = (char *)malloc(strlen(holder) + 1);
    mallocCheckStringPlus3(temp3, temp2, temp, holder);
    int k = 0;
    // takes the next char while the char is a digit
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT && tempChar != '\0')
    {
        temp2[k] = tempChar;
        i++;
        k++;
        tempChar = holder[i];
    }
    temp2[k] = '\0';
    // convert the string to a number
    int sum = makeNumber(temp2, DECIMAL_BASE);
    // checks the sum is valid
    if (sum <= 0)
    {
        free3Strings(temp, temp2, temp3);
        return 0;
        // transfer the money and make sure it is valid
    }
    else if (transferHelper(bank, number1, number2, sum, 1) == 0)
    {
        free3Strings(temp, temp2, temp3);
        return 0;
    }
    // skips the commas
    while (tempChar == ',')
    {
        i++;
        tempChar = holder[i];
    }
    // continue recoursively with the next set of instructions
    if (tempChar != '\0')
    {
        char *newholder = &holder[i];
        free3Strings(temp, temp2, temp3);
        return transferMoney2(bank, newholder);
    }
    // free the strings and return 1 if all the transfers are valid
    free3Strings(temp, temp2, temp3);
    return 1;
}

/**
 * @brief function that simulates the bank and checks if the tranfers are valid
 *
 * @param bank the bank
 * @param holder the string with the instructions
 *
 * @return 1 if the istructions are valid, 0 else.
 */
int demo(Bank *bank, char *holder)
{
    // initializes the demo bank
    Bank bankDemo;
    bankDemo.accounts = NULL;
    bankDemo.transactions = NULL;
    // duplicates the bank
    duplicate(bank, &bankDemo);
    // makes the transactions and return 0 if they arent valid
    int check = transferMoney2(&bankDemo, holder);
    if (check == 0)
    {
        robbery(&bankDemo, 1);
        return 0;
    }
    // return 1 if all the transictions are valid
    robbery(&bankDemo, 1);
    return 1;
}

/**
 * @brief function that makes duplication of the original bank
 *
 * @param bank the original bank
 * @param bankDemo the duplicated bank
 *
 * @return void
 */
void duplicate(Bank *bank, Bank *bankDemo)
{
    // initializes pointer and run through the list
    Node *tmp1 = bank->accounts;
    while (tmp1 != NULL)
    {
        // initializes new account
        Account *account1 = (Account *)(tmp1->data);
        Account account2;
        // duplicate the data
        account2.accountNumber = account1->accountNumber;
        account2.balance = account1->balance;
        // add the duplicted account to the new list and advance to the next account
        addAccountToList(bankDemo, account2, 1);
        tmp1 = tmp1->next;
    }
}

/**
 * @brief function that checks if the account made at least one action
 *
 * @param bank the bank
 * @param number the account number
 *
 * @return 1 if the account was active, 0 else.
 */
int isActive(Bank *bank, unsigned int number)
{
    // initializes pointer and run through the list
    Node *head = bank->transactions;
    Node *tmp = head;
    while (tmp != NULL)
    {
        // declare the current transaction the accounts that were involved in it
        Transaction *current = (Transaction *)(tmp->data);
        unsigned int tmpTransaction = current->fromAccount;
        unsigned int tmpTransaction2 = current->toAccount;
        // return 1 if the account was involved in any transactions
        if (tmpTransaction == number || tmpTransaction2 == number)
        {
            return 1;
        }
        // advance to the next transactiopn
        tmp = tmp->next;
    }
    // return 0 if the account wasnt involved in any action
    return 0;
}

/**
 * @brief functionn that prints all the transactions of a certain client
 *
 * @param bank the bank
 *
 * @return void
 */
void watchAccount(Bank *bank)
{
    // asks for the account number
    printf("Enter account number:\n");
    unsigned int number;
    scanf("%u", &number);
    scanf("%*c");
    // checks that this is a real account
    Node *head = bank->accounts;
    if (checkFreeNumber(number, head) == 1 || number == 0)
    {
        printError("Account not found\n");
        return;
    }
    // searching for his account
    Account *current = (Account *)(head->data);
    while (current->accountNumber != number)
    {
        head = head->next;
        current = (Account *)(head->data);
    }
    // prints the basic details of the client
    printf("Account #%u (%s)\n", current->accountNumber, current->accountHolder);
    printf("Balance: %d\n", current->balance);
    // prints suitable massage if the user was not active
    if (!isActive(bank, number))
    {
        printf("No transactions\n");
        return;
    }
    // prints all his transactions if he ahd any
    printf("Transactions:\n");
    printTransactions(bank, number);
}

/**
 * @brief function that prints all the transactions of a certain client
 *
 * @param bank the bank
 * @param number the account number
 *
 * @return void.
 */
void printTransactions(Bank *bank, unsigned int number)
{
    // initializes pointer and run through the list
    Node *head2 = bank->transactions;
    Node *tmp = head2;
    while (tmp != NULL)
    {
        Transaction *transaction = (Transaction *)(tmp->data);
        // checks that the user was involved at that transaction
        if (transaction->toAccount == number || transaction->fromAccount == number)
        {
            // checks if the transaction was "negative" or "positive"
            if (transaction->fromAccount == number)
            {
                // checks if it was a withdraw or not
                if (transaction->toAccount != 0)
                {
                    printf("%d to %u\n", transaction->amount, transaction->toAccount);
                }
                else
                {
                    printf("Withdrew %d\n", transaction->amount);
                }
            }
            else
            {
                // checks if it was a deposite or not
                if (transaction->fromAccount != 0)
                {
                    printf("%d from %u\n", transaction->amount, transaction->fromAccount);
                }
                else
                {
                    printf("Deposited %d\n", transaction->amount);
                }
            }
        }
        else
        {
        }
        // advance to the next transaction
        tmp = tmp->next;
    }
}

/**
 * @brief function that exit the program if the memory alocation failed, and free another variable
 *
 * @param holder the variable that "asked" the alocation
 * @param toFree a variable that should be freed
 *
 * @return void
 */
void mallocCheckStringPlus1(char *check, char *toFree)
{
    if (check == NULL)
    {
        free(toFree);
        exit(1);
    }
}

/**
 * @brief function that exit the program if the memory alocation failed, and free another variables
 *
 * @param holder the variable that "asked" the alocation
 * @param toFree1 a variable that should be freed
 * @param toFree2 a variable that should be freed
 *
 * @return void
 */
void mallocCheckStringPlus2(char *check, char *toFree1, char *toFree2)
{
    if (check == NULL)
    {
        free(toFree1);
        free(toFree2);
        exit(1);
    }
}

/**
 * @brief function that exit the program if the memory alocation failed, and free another variables
 *
 * @param holder the variable that "asked" the alocation
 * @param toFree1 a variable that should be freed
 * @param toFree2 a variable that should be freed
 * @param toFree3 a variable that should be freed
 *
 * @return void
 */
void mallocCheckStringPlus3(char *check, char *toFree1, char *toFree2, char *toFree3)
{
    if (check == NULL)
    {
        free(toFree1);
        free(toFree2);
        free(toFree3);
        exit(1);
    }
}

/**
 * @brief function that transfer money
 *
 * @param bank the bank
 * @param holder the string with the instructions
 *
 * @return void
 */
void transferMoney(Bank *bank, char *holder)
{
    // declaring consts for the digits and handle the first account
    char *temp = (char *)malloc(strlen(holder) + 1);
    mallocCheckStringPlus1(temp, holder);
    const char MINIMUM_DIGIT = '0', MAXIMUM_DIGIT = '9';
    int i = 0;
    char tempChar = holder[0];
    // takes the next char while the char is a digit
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT && tempChar != '\0')
    {
        temp[i] = tempChar;
        i++;
        tempChar = holder[i];
    }
    temp[i] = '\0';
    // convert the string to a number and advance to the next account
    unsigned int number1 = (unsigned int)makeNumber(temp, DECIMAL_BASE);
    if (tempChar != '\0')
    {
        i++;
        tempChar = holder[i];
    }
    // allocating memory for the next account
    char *temp2 = (char *)malloc(strlen(holder) + 1);
    mallocCheckStringPlus2(temp2, temp, holder);
    int j = 0;
    // takes the next char while the char is a digit
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT && tempChar != '\0')
    {
        temp2[j] = tempChar;
        i++;
        j++;
        tempChar = holder[i];
    }
    temp2[j] = '\0';
    // convert the string to a number and advance to the next account
    unsigned int number2 = (unsigned int)makeNumber(temp2, DECIMAL_BASE);
    // advance to the sum
    if (tempChar != '\0')
    {
        i++;
        tempChar = holder[i];
    }
    // allocating memory for the sum of the transfer
    char *temp3 = (char *)malloc(strlen(holder) + 1);
    mallocCheckStringPlus3(temp3, temp2, temp, holder);
    int k = 0;
    // takes the next char while the char is a digit
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT && tempChar != '\0')
    {
        temp2[k] = tempChar;
        i++;
        k++;
        tempChar = holder[i];
    }
    temp2[k] = '\0';
    // convert the string to a number
    int sum = makeNumber(temp2, DECIMAL_BASE);
    // transfer the money
    transferHelper(bank, number1, number2, sum, 0);
    // skips the commas
    while (tempChar == ',')
    {
        i++;
        tempChar = holder[i];
    }
    // continue recoursively with the next set of instructions
    if (tempChar != '\0')
    {
        char *newholder = &holder[i];
        free3Strings(temp, temp2, temp3);
        return transferMoney(bank, newholder);
    }
    // free the strings and prints suitable massage
    printf("Instructions executed successfully\n");
    free3Strings(temp, temp2, temp3);
    return;
}

/**
 * @brief function that free 3 strings
 *
 * @param toFree1 a string that need to be freed
 * @param toFree2 a string that need to be freed
 * @param toFree3 a string that need to be freed
 *
 * @return void
 */
void free3Strings(char *toFree1, char *toFree2, char *toFree3)
{
    free(toFree1);
    free(toFree2);
    free(toFree3);
}

/**
 * @brief function that add docuement a withdraw
 *
 * @param bank the bank
 * @param number the account number
 * @param sum the ammount of money that withdrawn
 *
 * @return void
 */
void doceumentWithdraw(Bank *bank, unsigned int number, int sum)
{
    // creating the transaction
    Transaction transaction;
    transaction.amount = sum;
    transaction.fromAccount = number;
    transaction.toAccount = 0;
    // allocate memory for the node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        exit(1);
    }
    // allocate memory for the transaction
    newNode->data = malloc(sizeof(Transaction));
    if (newNode->data == NULL)
    {
        free(newNode);
        exit(1);
    }
    // initializes the next pointer of the transaction to NULL
    *(Transaction *)(newNode->data) = transaction;
    newNode->next = NULL;
    // adding it to the first place if the list is empty
    if (bank->transactions == NULL)
    {
        bank->transactions = newNode;
        // adding the transaction to the end of the list
    }
    else
    {
        Node *current = bank->transactions;
        // runs through the list
        while (current->next != NULL)
        {
            current = current->next;
        }
        // advance to the next node
        current->next = newNode;
    }
}

/**
 * @brief function that docuement a deposit
 *
 * @param bank the bank
 * @param number the account number
 * @param sum the amount of money that deposited
 *
 * @return void
 */
void doceumentDeposit(Bank *bank, unsigned int number, int sum)
{
    // creating the transaction
    Transaction transaction;
    transaction.amount = sum;
    transaction.fromAccount = 0;
    transaction.toAccount = number;
    // allocate memory for the node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        exit(1);
    }
    // allocate memory for the transaction
    newNode->data = malloc(sizeof(Transaction));
    if (newNode->data == NULL)
    {
        free(newNode);
        exit(1);
    }
    // initializes the next pointer of the transaction to NULL
    *(Transaction *)(newNode->data) = transaction;
    newNode->next = NULL;
    // adding it to the first place if the list is empty
    if (bank->transactions == NULL)
    {
        bank->transactions = newNode;
        // adding the transaction to the end of the list
    }
    else
    {
        Node *current = bank->transactions;
        // runs through the list
        while (current->next != NULL)
        {
            current = current->next;
        }
        // advance to the next node
        current->next = newNode;
    }
}

/**
 * @brief function that transfer money from one account to another
 *
 * @param bank the bank
 * @param number1 the account number that the money came out from
 * @param number2 the account number that the money was transfered to
 * @param sum the amount of money that trasfered
 * @param isDemo tells if that the real bank or the bank that duplicted for mission 5
 *
 * @return 1 if the transfer is valid, 0 else.
 */
int transferHelper(Bank *bank, unsigned int number1, unsigned int number2, int sum, int isDemo)
{
    // initializes pointers for the nodes and the account
    Node *head = bank->accounts;
    Node *tmp = head;
    Account *current = (Account *)(tmp->data);
    // search the account that delivered the money
    while (current->accountNumber != number1)
    {
        tmp = tmp->next;
        current = (Account *)(tmp->data);
    }
    // substruct the amount of money from his account
    current->balance = current->balance - sum;
    // checks that the transfer is valid
    if (current->balance < 0)
    {
        return 0;
    }
    // initializes pointers for the nodes and the account
    Node *head2 = bank->accounts;
    Node *tmp2 = head2;
    Account *current2 = (Account *)(tmp2->data);
    // search the account that recieved the money
    while (current2->accountNumber != (unsigned int)number2)
    {
        tmp2 = tmp2->next;
        current2 = (Account *)(tmp2->data);
    }
    // add the money to his account
    current2->balance = current2->balance + sum;
    // docuement the Transaction if its the real bank
    if (!isDemo)
    {
        docuementTransaction(bank, number1, number2, sum);
    }
    // return 1 if the transfer was valid
    return 1;
}

/**
 * @brief function that docuement Transaction by adding it to the Transactions list
 *
 * @param bank the bank
 * @param number1 the account number that the money came out from
 * @param number2 the account number that the money was transfered to
 * @param sum the amount of money that trasfered
 *
 * @return void
 */
void docuementTransaction(Bank *bank, unsigned int number1, unsigned int number2, int sum)
{
    // create the transaction
    Transaction transaction;
    transaction.amount = sum;
    transaction.fromAccount = number1;
    transaction.toAccount = number2;
    // allocating memory for the node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        exit(1);
    }
    // allocating memory for the Transaction
    newNode->data = malloc(sizeof(Transaction));
    if (newNode->data == NULL)
    {
        free(newNode);
        exit(1);
    }
    // initializes the next pointer of the transaction to NULL
    *(Transaction *)(newNode->data) = transaction;
    newNode->next = NULL;
    // adding it to the first place if the list is empty
    if (bank->transactions == NULL)
    {
        bank->transactions = newNode;
        // adding the transaction to the end of the list
    }
    else
    {
        Node *current = bank->transactions;
        // runs through the list
        while (current->next != NULL)
        {
            current = current->next;
        }
        // advance to the next node
        current->next = newNode;
    }
}

/**
 * @brief function that checks if the account at the instructions are exist
 *
 * @param bank the bank
 * @param holder the string of the instructions
 *
 * @return 1 if all the accounts are exist, 0 else.
 */
int checkAccounts(Bank *bank, char *holder)
{
    // allocating memory for the first account and define consts for the digits
    char *temp = (char *)malloc(strlen(holder) + 1);
    mallocCheckString(temp);
    const char MINIMUM_DIGIT = '0', MAXIMUM_DIGIT = '9';
    int i = 0;
    char tempChar = holder[0];
    // takes all the digits
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT)
    {
        temp[i] = tempChar;
        i++;
        tempChar = holder[i];
    }
    temp[i] = '\0';
    // convert the string to a number
    unsigned int number = (unsigned int)makeNumber(temp, DECIMAL_BASE);
    Node *head = bank->accounts;
    // checks if the account is exist
    if (checkFreeNumber(number, head) == 1 || number == 0)
    {
        free(temp);
        return 0;
    }
    // recoursively advance to the next account on the string of the instructions
    if (tempChar == '-')
    {
        char *newholder = &holder[i + 1];
        free(temp);
        return checkAccounts(bank, newholder);
    }
    // skips all the chars until the next account
    if (tempChar == ':')
    {
        i++;
        tempChar = holder[i];
        // skips the sum
        while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT)
        {
            i++;
            tempChar = holder[i];
        }
        // skips the commas
        while (tempChar == ',')
        {
            i++;
            tempChar = holder[i];
        }
    }
    // running recoursively with the next account
    if (tempChar != '\0')
    {
        char *newholder = &holder[i];
        free(temp);
        return checkAccounts(bank, newholder);
        // checks the string didn't end with comma
    }
    else if (holder[i - 1] == ',')
    {
        free(temp);
        return 0;
    }
    // return 1 if all the accounts are valid
    free(temp);
    return 1;
}

/**
 * @brief function that checks if a string is in the form of valid set of instructions
 *
 * @param holder the string that contain the set of the instructions
 *
 * @return 1 if it's in the the form of valid set of instructions, 0 else.
 */
int isTransaction(char *holder)
{
    // define consts for the digits
    const char MINIMUM_DIGIT = '0', MAXIMUM_DIGIT = '9';
    int i = 0;
    char tempChar = holder[0];
    // confirm that the first char is a digit
    if (tempChar < MINIMUM_DIGIT || tempChar > MAXIMUM_DIGIT)
    {
        return 0;
    }
    // going through the first account
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT)
    {
        i++;
        tempChar = holder[i];
    }
    // confirm the next char is '-'
    if (tempChar != '-')
    {
        return 0;
    }
    i++;
    tempChar = holder[i];
    // going through the second account
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT)
    {
        i++;
        tempChar = holder[i];
    }
    // confirm the next char is ':'
    if (tempChar != ':')
    {
        return 0;
    }
    i++;
    tempChar = holder[i];
    // going through the sum
    while (tempChar >= MINIMUM_DIGIT && tempChar <= MAXIMUM_DIGIT)
    {
        i++;
        tempChar = holder[i];
    }
    // confirm the string didn't end with comma
    if (tempChar != '\0')
    {
        if (tempChar != ',')
        {
            return 0;
        }
        i++;
        tempChar = holder[i];
        // skipping the commas
        while (tempChar == ',')
        {
            i++;
            tempChar = holder[i];
        }
        // recoursively countinue the checking with the next instruction
        if (tempChar != '\0')
        {
            return isTransaction(&holder[i]);
        }
    }
    // return 1 if it's in the the form of valid set of instructions
    return 1;
}

/**
 * @brief function that makes a withdraw
 *
 * @param bank the bank
 * @param number the account number
 *
 * @return void
 */
void takeMoney(Bank *bank, unsigned int number)
{
    // initializes pointers for the nodes and the account
    Node *head = bank->accounts;
    Node *tmp = head;
    Account *current = (Account *)(tmp->data);
    // search the account
    while (current->accountNumber != number)
    {
        tmp = tmp->next;
        current = (Account *)(tmp->data);
    }
    // takes the sum from the user and confirm it's valid number
    printf("How much money would you like to withdraw?\n");
    char *holder = getString();
    if (isNumber(holder) == 0)
    {
        printError("Invalid amount\n");
        free(holder);
        return;
    }
    // converts the string to a number
    int sum = makeNumber(holder, DECIMAL_BASE);
    // checks the sum is positive
    if (sum <= 0)
    {
        printError("Invalid amount\n");
        free(holder);
        return;
    }
    // checks the client has enough money
    if (sum > current->balance)
    {
        printError("Not enough money\n");
        free(holder);
        return;
    }
    // substruct the sum from his balance, prints suitable massage and doceument the Withdraw
    current->balance = current->balance - sum;
    printf("Money withdrawn successfully; your new balance is %d\n", current->balance);
    doceumentWithdraw(bank, number, sum);
    free(holder);
}

/**
 * @brief function that get a string in unknown length from the user
 *
 * @return string that the user typed
 */
char *getString()
{
    char tempChar;
    int index = 1;
    // initializes memory for the first char and takes it from the user
    char *holder = (char *)malloc(sizeof(char));
    if (holder == NULL)
    {
        exit(1);
    }
    scanf("%c", &tempChar);
    // runs until the user press enter
    while (tempChar != '\n')
    {
        // add the char to the string and reallocte memory for another char
        holder[index - 1] = tempChar;
        holder = (char *)realloc(holder, (index + 1) * sizeof(char));
        if (holder == NULL)
        {
            free(holder);
            exit(1);
        }
        // takes the next char and increment the index
        index++;
        scanf("%c", &tempChar);
    }
    // finish the string and return it
    holder[index - 1] = '\0';
    return holder;
}

/**
 * @brief function that checks if a string is a number
 *
 * @param holder the string
 *
 * @return 1 if its a number, 0 else
 */
int isNumber(char *holder)
{
    // define consts for the digits
    const char MINIMUM_DIGIT = '0';
    const char MAXINUM_DIGIT = '9';
    int i = 0;
    char tempChar = holder[0];
    // run through the string
    while (tempChar != '\0')
    {
        // checks if the char is not a digit
        if (tempChar < MINIMUM_DIGIT || tempChar > MAXINUM_DIGIT)
        {
            return 0;
        }
        // increment the index and checks the next char in the string
        i++;
        tempChar = holder[i];
    }
    // return 1 if the string is a number
    return 1;
}

/**
 * @brief function that converts a string to a number
 *
 * @param holder the string
 * @param base the base of the number (decimal)
 *
 * @return the number itself as an integer
 */
int makeNumber(char *holder, int base)
{
    // declaring variables for the index, the current sum etc.
    int i = 0;
    int sum = 0;
    int minusFlag = 0;
    int tempDigit;
    char tempChar = holder[0];
    // running until the user press enter
    while (tempChar != '\0')
    {
        // checks if its a negative number
        if (i == 0)
        {
            if (tempChar == '-')
            {
                minusFlag++;
                i++;
                tempChar = holder[i];
                continue;
            }
        }
        // converts the char to a number and adding it to the sum
        tempDigit = tempChar - '0';
        sum = (sum * base) + tempDigit;
        i++;
        tempChar = holder[i];
    }
    // handle negative numbers
    if (minusFlag)
    {
        sum = sum * (-1);
    }
    // return the nummber itself
    return sum;
}

/**
 * @brief function that makes deposit
 *
 * @param bank the bank
 * @param number the account number
 *
 * @return void
 */
void deposite(Bank *bank, unsigned int number)
{
    // initializes pointers for the nodes and the account
    Node *head = bank->accounts;
    Node *tmp = head;
    Account *current = (Account *)(tmp->data);
    // search the account
    while (current->accountNumber != number)
    {
        tmp = tmp->next;
        current = (Account *)(tmp->data);
    }
    // takes the sum from the user and confirm it's valid number
    printf("How much money would you like to deposit?\n");
    char *holder = getString();
    if (holder[0] == '\n')
    {
    }
    if (isNumber(holder) == 0)
    {
        printError("Invalid amount\n");
        free(holder);
        return;
    }
    // converts the string to a number
    int sum = makeNumber(holder, DECIMAL_BASE);
    // checks the sum is positive
    if (sum <= 0)
    {
        printError("Invalid amount\n");
        free(holder);
        return;
    }
    // add the sum to his balance, prints suitable massage and doceument the deposit
    current->balance = current->balance + sum;
    printf("Money deposited successfully; your new balance is %d\n", current->balance);
    doceumentDeposit(bank, number, sum);
    free(holder);
}

/**
 * @brief function that update the name of the Account Holder
 *
 * @param bank the bank
 *
 * @return void
 */
void updateAccountHolder(Bank *bank)
{
    // asks for the account number
    printf("Enter account number:\n");
    unsigned int number;
    scanf("%u", &number);
    scanf("%*c");
    Node *head = bank->accounts;
    // checks it is a real account
    if (checkFreeNumber(number, head) == 1)
    {
        printError("Account not found\n");
        return;
    }
    head = bank->accounts;
    // asks for the new name
    printf("Enter new account holder:\n");
    char tempChar;
    int index = 1;
    // allocating memory for the first char and takes it from the user
    char *holder = (char *)malloc(sizeof(char));
    if (holder == NULL)
    {
        exit(1);
    }
    scanf(" %c", &tempChar);
    // runs until the user press enter
    while (tempChar != '\n')
    {
        // add the char to the string and reallocate memory for the next char
        holder[index - 1] = tempChar;
        holder = (char *)realloc(holder, (index + 1) * sizeof(char));
        if (holder == NULL)
        {
            free(holder);
            exit(1);
        }
        // increment the index and takes the next char
        index++;
        scanf("%c", &tempChar);
    }
    // finish the string
    holder[index - 1] = '\0';
    Account *current = (Account *)(head->data);
    // search for the account
    while (current->accountNumber != number)
    {
        head = head->next;
        current = (Account *)(head->data);
    }
    // updates the name
    free(current->accountHolder);
    current->accountHolder = holder;
}

/**
 * @brief function that delete account from the list
 *
 * @param bank the bank
 *
 * @return void
 */
void deleteAccount(Bank *bank)
{
    // asks fot the account number
    printf("Enter account number:\n");
    unsigned int number;
    scanf("%u", &number);
    scanf("%*c");
    Node *head = bank->accounts;
    // checks it is a real account
    if (checkFreeNumber(number, head) == 1)
    {
        printError("Account not found\n");
        return;
    }
    else
    {
        // checks the place of the account in the list
        head = bank->accounts;
        int steps = countSteps(number, head);
        Node *tmp = head;
        Node *tmp2 = head;
        // handle edge case
        if (countSteps(number, head) == 0)
        {
            bank->accounts = head->next;
            head->next = NULL;
            printf("Account deleted successfully\n");
            Account *account = tmp2->data;
            char *holder = account->accountHolder;
            free(holder);
            free(account);
            free(tmp2);
            return;
        }
        // finds the node that comes before the account
        for (int i = 0; i < steps - 1; i++)
        {
            tmp = tmp->next;
        }
        // saving a pointer to the account and connect the previous node to the next one
        Node *tmp3 = tmp->next;
        tmp->next = tmp->next->next;
        Account *account2 = tmp3->data;
        // free all the data of his account and prints a suitable massage
        char *holder = account2->accountHolder;
        free(holder);
        free(account2);
        free(tmp3);
        printf("Account deleted successfully\n");
    }
}

/**
 * @brief function that count the number of nodes that comes before a certain node
 *
 * @param number the account number
 * @param head pointer to the first node in the list
 *
 * @return the number of steps that was made to get to the node
 */
int countSteps(unsigned int number, Node *head)
{
    // initialzes the counter and the necessary pointers
    int counter = 0;
    Node *tmp = head;
    Account *current = (Account *)(tmp->data);
    // search the account
    while (current->accountNumber != number)
    {
        tmp = tmp->next;
        current = (Account *)(tmp->data);
        // increments the counter
        counter++;
    }
    // return the number of steps
    return counter;
}

/**
 * @brief function that checks if a account number is already taken
 *
 * @param number the account number
 * @param head pointer to the first node in the list
 *
 * @return 1 if the number is free (wasn't taken), 0 else.
 */
int checkFreeNumber(unsigned int number, Node *head)
{
    Node *tmp = head;
    // run through the list
    while (tmp != NULL)
    {
        Account *account = (Account *)(tmp->data);
        unsigned int tmpNum = account->accountNumber;
        // return 0 if the number is taken
        if (tmpNum == number)
        {
            return 0;
        }
        tmp = tmp->next;
    }
    // return 1 if the number is free
    return 1;
}

/**
 * @brief function that create a new account
 *
 * @param bank the bank
 *
 * @return void
 */
void createAccount(Bank *bank)
{
    // declaring new account and initialize its balance to 0
    Account new;
    new.balance = 0;
    // asks for the wanted account number
    printf("Enter account number:\n");
    scanf("%u", &new.accountNumber);
    Node *head = bank->accounts;
    // checks that the number is valid and not taken
    if (new.accountNumber == 0 || checkFreeNumber(new.accountNumber, head) == 0)
    {
        printError("Account number already exists\n");
        scanf("%*c");
        return;
    }
    // asks for his name
    printf("Enter account holder:\n");
    char tempChar;
    int index = 1;
    // allocate memory for the first char and takes it from the user
    char *holder = (char *)malloc(sizeof(char));
    if (holder == NULL)
    {
        exit(1);
    }
    scanf(" %c", &tempChar);
    // run until the user press enter
    while (tempChar != '\n')
    {
        // reallocating memory for the next char and add the previous char to the string
        holder = (char *)realloc(holder, (index + 1) * sizeof(char));
        if (holder == NULL)
        {
            free(holder);
            exit(1);
        }
        holder[index - 1] = tempChar;
        // increment the index and takes the next char
        index++;
        scanf("%c", &tempChar);
    }
    // finish the string, implement it as the name of account holder and add the account to the list
    holder[index - 1] = '\0';
    new.accountHolder = holder;
    addAccountToList(bank, new, 0);
}

/**
 * @brief function that add Account To the account List
 *
 * @param bank the bank
 * @param account the account that will add to the list
 * @param isDemo tells if it's the real bank or the demo one for mission 5
 *
 * @return void
 */
void addAccountToList(Bank *bank, Account account, int isDemo)
{
    // alocate memory for the node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        exit(1);
    }
    // alocate memory for the account
    newNode->data = malloc(sizeof(Account));
    if (newNode->data == NULL)
    {
        free(newNode);
        exit(1);
    }
    // initializes the next pointer of the account to NULL
    *(Account *)(newNode->data) = account;
    newNode->next = NULL;
    // adding it to the first place if the list is empty
    if (bank->accounts == NULL)
    {
        bank->accounts = newNode;
        // adding the account to the end of the list
    }
    else
    {
        Node *current = bank->accounts;
        // runs through the list
        while (current->next != NULL)
        {
            current = current->next;
        }
        // advance to the next node
        current->next = newNode;
    }
    // prints a suitable massage if it's not the demo bank
    if (!isDemo)
    {
        printf("Account created successfully\n");
    }
}

/**
 * @brief - function that creates the menu
 *
 * @return - a new menu
 */
MenuItem *createMenu(int *size)
{
    // create the menu
    static MenuItem menu[] = {
        {"Exit", exitProgram},
        {"Create account", createAccount},
        {"Delete account", deleteAccount},
        {"Update account", updateAccountHolder},
        {"Deposit / Withdraw money", changeBalance},
        {"Transferring", makeTransaction},
        {"View account", watchAccount}};

    // update the size and return
    *size = sizeof(menu) / sizeof(menu[0]);
    return menu;
}

/**
 * @brief - function that exits the program
 *
 * @return - void
 */
void exitProgram(Bank *bank)
{
    printf("Exiting program...\n");
    exit(0);
}

/**
 * @brief - runs the menu loop, prints the menu and executes handler according to user choice
 *
 * @param menu - array of MenuItem
 * @param menuSize - number of items in the menu
 * @return void
 */
void runMenu(MenuItem menu[], int menuSize, Bank *bank)
{
    // sets the choice to be diffrent from 0
    int choice = 1;
    // let the user choose an option, until he finished
    while (1)
    {
        // print the menu
        printMenu(menu, menuSize);
        // Try to read an integer choice from the user.
        // If input is invalid (not a number), clear the input buffer, show an error, and prompt again.
        if (scanf("%d", &choice) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printError(invalidOptionString);
            continue;
        }

        // If the choice is within the valid range, call the corresponding handler function.
        // Otherwise, notify the user that the choice is invalid.
        if (choice > 0 && choice < menuSize)
        {
            menu[choice].handler(bank);
        }
        else if (choice == 0)
        {
            break;
        }
        else
        {
            printError(invalidOptionString);
        }
    }
}

/**
 * @brief - print the menu options to the terminal
 *
 * @return - void
 */
void printMenu(MenuItem *menu, int size)
{
    // print the menu
    printf(GREEN "Choose an option:\n" RESET);
    for (int i = 0; i < size; i++)
    {
        printf("%d. %s\n", i, menu[i].name);
    }
}