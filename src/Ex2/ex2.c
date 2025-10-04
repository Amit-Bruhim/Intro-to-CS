// (89110, Spring 2023, Assignment #2, amit bruhim, 211548045, amitbru)
#include <stdio.h>

/**
 * @ brief - main function that gives the user 6 options of functions
 * @ return - 0 if the program runs successfully
 */
int main() {
  /*sets the choise to be diffrent from 0 and declare variabe
   *that helps clean the buffer
   */
  char choise = '1';
  // show the menu and let the user choose an option, until he finished
  while (1) {
    // flags for special missions, in which the buffer cleans alone
    int mission45 = 0;
    int mission0 = 0;
    printf("Choose an option:\n");
    printf("0. Exit\n");
    printf("1. Hourglass\n");
    printf("2. Reverse bits\n");
    printf("3. Reverse digits\n");
    printf("4. Convert to Decimal\n");
    printf("5. Base64 to Decimal\n");
    printf("6. Swap bits\n");
    scanf("%c", &choise);
    char clean;
    scanf("%c", &clean);
    // activate the case that the user chose
    switch (choise) {
    // stop the program when the user decide
    case '0': {
      mission0++;
      break;
    }

    // let the user print hourglass by his paramaters
    case '1': {
      // takes the size of the hourglass from the user
      int size;
      const int MINIMUMSIZE = 3;
      printf("Enter size:\n");
      scanf("%d", &size);
      // edge case
      char clean;
      if (size < MINIMUMSIZE) {
        scanf("%c", &clean);
        printf("Invalid paramaters\n");
        continue;
      }

      /* let the user to decide if the sand will be at the top or the
       * buttom of the hour glass
       */
      const int SANDUP = 1, SANDDOWN = 0;
      int flag;
      printf("Enter flag:\n");
      scanf("%d", &flag);
      // edge case
      if ((flag != SANDDOWN) && (flag != SANDUP)) {
        printf("Invalid paramaters\n");
        scanf("%c", &clean);
        continue;
      }

      // printing the upper triangle of the hourglass
      for (int a = 0; a < size; a++) {
        printf("-");
      }

      printf("\n");
      for (int j = 0; j < size / 2; j++) {
        for (int k = 0; k < j; k++) {
          printf(" ");
        }

        printf("\\");
        for (int i = 0; i < size - 2 - j * 2; i++) {
          // choosing the place of the sand according to the user will
          if (flag == SANDUP) {
            printf(".");
          } else {
            printf(" ");
          }
        }
        printf("/\n");
      }

      // adding X in the middle of the hourglass in case the size is not even
      if (size % 2 == 1) {
        for (int p = 0; p < size / 2; p++) {
          printf(" ");
        }

        printf("X\n");
      }

      // printing the lower triangle of the hourglass
      for (int t = size / 2; t > 0; t--) {
        for (int q = 0; q < t - 1; q++) {
          printf(" ");
        }

        printf("/");
        for (int w = 0; w <= size - 1 - t * 2; w++) {
          // choosing the place of the sand according to the user will
          if (flag == SANDUP) {
            printf(" ");
          } else {
            printf(".");
          }
        }

        printf("\\\n");
      }

      for (int b = 0; b < size; b++) {
        printf("-");
      }

      printf("\n");
      break;
    }

    // reverse the bits of a number
    case '2': {
      // takes number from the user
      printf("Enter a number:\n");
      int num, revers = 0;
      scanf("%d", &num);
      // reverse the bits of the user number
      const int SIZE = sizeof(int) * 8;
      for (int i = 0; i < SIZE; i++) {
        revers = revers << 1;
        revers = revers | (1 & num);
        num = num >> 1;
      }

      // printing the result
      printf("The reversed number is %d\n", revers);
      break;
    }

    // reverse a number
    case '3': {
      // taking the number from the user
      int num;
      printf("Enter a number:\n");
      scanf(" %d", &num);
      // edge case
      char clean;
      if (num == 0) {
        scanf("%c", &clean);
        printf("The reversed number is 0\n");
        continue;
      }

      // reverse the number until it ran out of digits
      const int BASE = 10;
      int digit = num % BASE;
      while (digit == 0) {
        num = num / BASE;
        digit = num % BASE;
      }

      printf("The reversed number is ");
      while (num != 0) {
        printf("%d", digit);
        num = num / BASE;
        digit = num % BASE;
      }

      printf("\n");
      break;
    }

    // convert any number in 2-9 bases to decimal
    case '4': {
      int base;
      char num;
      // takes the base from the user
      printf("Enter base:\n");
      scanf("%d", &base);
      char clean;
      scanf("%c", &clean);
      const int MINBASE = 2, MAXBASE = 9;
      if ((base < MINBASE) || (base > MAXBASE)) {
        printf("Invalid base\n");
        continue;
      }

      // takes the number from the user
      printf("Enter a number:\n");
      scanf("%c", &num);
      int sum = 0, error = 0;
      // runs until the user press enter
      while (num != '\n') {
        // edge cases
        if ((num < '0' || (num >= base + '0'))) {
          printf("Invalid character %c in base %d\n", num, base);
          error++;
          break;
        }

        // calculates the result
        sum = (num - '0') + sum * base;
        scanf("%c", &num);
      }

      // edge cases - cleans the buffer
      if (error) {
        while (num != '\n') {
          scanf("%c", &num);
        }

        continue;
      }

      // printing the result
      printf("The converted number is %d\n", sum);
      mission45++;
      break;
    }

    // convert a number in 64 base to decimal
    case '5': {
      mission45++;
      const int BASE = 64;
      const int NUMBERSOFLETTERS = 26, DIFF = ('a' - NUMBERSOFLETTERS),
                ZERO64 = 52;
      const int DIFF2 = (ZERO64 - '0');
      int tempDigit = 0, error = 0;
      long long int sum = 0;
      // taking the number from the user
      printf("Enter a number:\n");
      char number;
      scanf("%c", &number);
      // runs until the user press enter
      while (number != '\n') {
        // convert the digit to decimal
        if (number >= '0' && number <= '0' + 9) {
          tempDigit = number + DIFF2;
        } else if (number >= 'A' && number < 'A' + NUMBERSOFLETTERS) {
          tempDigit = number - 'A';
        } else if (number >= 'a' && number < 'a' + NUMBERSOFLETTERS) {
          tempDigit = number - DIFF;
        } else if (number == '+') {
          tempDigit = 62;
        } else if (number == '/') {
          tempDigit = 63;
        }

        // edge cases
        else {
          printf("Invalid character %c in base 64 \n", number);
          mission45 = 0;
          error++;
          break;
        }

        // calculates the result and taking the next digit
        sum = sum * BASE + (tempDigit);
        scanf("%c", &number);
      }

      // printing the result
      if (error == 0) {
        printf("The converted number is %lld\n", sum);
      }

      // edge cases and cleans the buffer
      break;
    }

    // swiping specific bits in a given number, according to the user choises
    case '6': {
      // taking the number and the bits
      int index1, index2;
      unsigned int number, result = 0;
      printf("Enter a number:\n");
      scanf("%d,", &number);
      printf("Enter indexes:\n");
      scanf("%d", &index1);
      scanf("%d", &index2);
      result = number;
      int bitIndex1 = 0, bitIndex2 = 0;
      // edge cases
      const int SIZE = sizeof(int) * 8;
      char clean;
      if (index1 < 0 || index2 < 0 || index1 >= SIZE || index2 >= SIZE) {
        printf("Invalid indexes\n");
        scanf("%c", &clean);
        continue;
      }

      // checks if the bit is 0 or 1
      if (number & (1 << index1)) {
        bitIndex1 = 1;
      }

      if (number & (1 << index2)) {
        bitIndex2 = 1;
      }

      // swiping the bits
      if (bitIndex1 != bitIndex2) {
        if (bitIndex1 == 1) {
          result = (number & (~(1 << index1)));
          result = (result | (1 << index2));
          printf("the result is %u\n", result);
        } else {
          result = (number & (~(1 << index2)));
          result = (result | (1 << index1));
          printf("The result is %u\n", result);
        }

        // printing the same number if those specific bits are the same
      } else {
        printf("The result is %u\n", result);
      }

      break;
    }

    // edge cases
    default: {
      printf("Invalid option\n");
      continue;
    }
    }

    // edge missionds that cleans the buffer alone
    if (mission45) {
      continue;
    }

    // cleans the buffer
    clean = choise;
    if (!mission0) {
      while (clean != '\n') {
        scanf("%c", &clean);
      }
    }

    // stops the program when the user dedides
    if (choise == '0') {
      break;
    }
  }
  return 0;
}