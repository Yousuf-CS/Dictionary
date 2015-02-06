#include<stdio.h>
#pragma pack(6)
#include<string.h>
#include<ctype.h>
#include<windows.h>
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

typedef struct {
    char string[150];
} String;



int levenshtein(char *s1, char *s2);         // find levenshtein distance
int isStringAlpha(char input[]);             // returns 0 if string contains any numeric value otherwise returns 1
int search(char input[]);                    // returns the meaning of the input word
void stringCaseChanger(char input[]);        // returns string with first letter uppercase and others lowercase
int checkSpace(char input[]);                // returns 0 if any space character is found in the input string
void suggest(char input[]);                // suggest closest strings




int main( void ) {
    int i = 0, flag = 0, selection = 0;
    char input[150];


    while(1 > 0) {
        puts("1. To find the meaning of word.  2. To exit");
        printf("\n>>> "); // just for UI
        scanf("%d", &selection);

        fseek(stdin, 0, SEEK_END); // for stopping while loop from running twice. Only works on windows platform

        switch(selection) {
        case 1:
            do {
                printf("\nEnter a word to find its meanings : ");
                gets(input);
                if (isStringAlpha(input) != 1) {
                    puts("Please enter a word with only alphabets!!");
                }
                if (checkSpace(input) == 0) {
                    puts("Please enter word without spaces!!");
                }
               } while ((isStringAlpha(input) != 1) || (checkSpace(input) == 0));

               stringCaseChanger(input); // changes the case of input string

               if (strlen(input) == 1) {
                   puts("This is an alphabet.");
               } else {
                   flag = search(input);
               }

               if (flag != 1) {
                   puts("Sorry, the word you entered was not found!");
                   suggest(input);
                   flag = 0; // initializing flag variable to 0 again
               }
               break;
        case 2:
            exit(0);
            break;
        default:
            puts("\nPlease select only 1 or 2\n");
        }
    }




	getchar();
	return 0;

}


int isStringAlpha(char input[]) {
    int flag = 1, i = 0;
    for (i = 0; i < strlen(input); i++) {
        if (!(isalpha(input[i]))) {
            flag = 0;
        }
    }
    return flag;
}

int search(char input[]) {
    int flag = 0;
    char word[150], meaning[10000];
    FILE *fp = fopen("wordmeanings.txt", "r");
    if (!fp) {
        puts("Error opening wordmeanings.txt..! Program will now terminate..");
        exit(0);
    }
    while(!feof(fp)) {
        fscanf(fp, "%s  ", word);  // read the word
        fgets(meaning, 10000, fp); // read its meaning
        if (strcmp(input, word) == 0) {
            printf("\n");
            puts(meaning);
            flag = 1;
            break;
        }
    }
    return flag;
}

void stringCaseChanger(char input[]) {
    int i = 0;
    input[0] = toupper(input[0]); // capitalizing first letter of string
    for (i = 1; i < strlen(input); i++) {
        input[i] = tolower(input[i]);
    }
}

int checkSpace(char input[]) {
    int i = 0;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            return 0;
        }
    }
}

void suggest(char input[]) {
    int count = 0, f = -1, i = 0, selection = 0;
    char word[150];
    String suggestedWord[6]; // array of structure "String"
    FILE *fp = fopen("predict.txt", "r");
    if (!fp) {
        puts("Error opening predict.txt..! Program will now terminate..");
        exit(0);
    }
    while(!feof(fp)) {
        fscanf(fp, "%s\n", word);
        if (word[0] == input[0]) {
            if ((levenshtein(word, input)) <= 2) {
                if (count <= 5) {
                    strcpy(suggestedWord[count].string, word);
                    f = count;
                }
                count++;
            }
        } else {
            continue;
        }
    } //  end while
    if (f != -1) {
        printf("\nDid you mean :  ");
        for (i = 0; i <= f; i++) {
            printf("%d.%s  ", (i+1), suggestedWord[i].string);
        }
        printf("\n\n"); // just for UI
        printf("Enter the adjacent number to find the meaning :  ");
        selection = getchar();
        switch(selection) {
        case '1':
            search(suggestedWord[0].string);
            break;
        case '2':
            search(suggestedWord[1].string);
            break;
        case '3':
            search(suggestedWord[2].string);
            break;
        case '4':
            search(suggestedWord[3].string);
            break;
        case '5':
            search(suggestedWord[4].string);
            break;
        case '6':
            search(suggestedWord[5].string);
            break;
        default:
            puts("Please enter the correct number.");
        }
    } else {
        puts("Sorry!! Word not found in dictionary.");
    }

}



int levenshtein(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}









