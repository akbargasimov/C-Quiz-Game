//
//  main.m
//  AkbarGasimov2388163Assignment2
//
//  Created by Akber Gasimov on 31.08.23.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_QUESTIONS 15
#define MAX_LENGTH 1000
#define MAX_ANSWER_CHOICES 4
#define MAX_ANSWER_LENGTH 1000
#define CORRECT 100
#define MAX_LINES 5
#define MAX_LINE_LENGTH 100

char getUserInput(void) {// This function help us to get user input
    fflush(stdin);
    char input;
    scanf("%c", &input);
    return input;
}

void viewHighScores(char lines[MAX_LINES][MAX_LINE_LENGTH]) {// This function help us to view score in menu
    int lineCount = 5;
    printf("\nTop 5 High Score:\n");
    for(int i = 0;i < lineCount; ++i){
        printf("%s",lines[i]);
    }
    printf("\n");
}
void saveHighScore(int correct){// This function help us to save high score of previous(or current) player
    FILE *outFile;
    char name[20];
    char buffer[200];
    
    char temp[MAX_LINES][MAX_LENGTH];
    
    outFile = fopen("high_scores.txt", "r");
    
    printf("\nEnter your name : ");
    fflush(stdin);
    gets(name);
    
    if(outFile == NULL){
        outFile = fopen("high_scores.txt", "w");
        fprintf(outFile, "%d.%s - %d/10\n",1,name,correct);
        fclose(outFile);
        exit(1);
    }else{
        outFile = fopen("high_scores.txt", "a");
        for (int i = 0; i < MAX_LINES; i++) {
            if (fgets(temp[i], MAX_LINE_LENGTH,outFile) == NULL) {
                fprintf(outFile, "%d.%s - %d/10\n",i,name,correct);
            }
            }
    }
}
void loadHighScores(void){//Loading high scores to array the sending it to viewhighscore
    FILE *outFile;
    
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    
    outFile = fopen("high_scores.txt", "r");
    
    if(outFile == NULL){
        printf("Unable to open the File!");
        exit(1);
    }
    
    for (int i = 0; i < MAX_LINES; i++) {
            if (fgets(lines[i], MAX_LINE_LENGTH,outFile) == NULL) {
                break;
            }
        }
    fclose(outFile);
    viewHighScores(lines);
}
void askQuestion(char question[][MAX_LENGTH], char answer[][MAX_ANSWER_CHOICES][MAX_ANSWER_LENGTH], char correctAnswer[][CORRECT], int *questionCount, int *correctCount) {// This function helps us to print questions one by one after getting input
    char choice;
    int correct = 0;
    char scoreChoice;
    for (int i = 1; i < *questionCount+1; i++) {
        printf("\n");
        printf("%d. %s",i, question[i-1]);
        for (int j = 0; j < MAX_ANSWER_CHOICES; j++) {
            printf("%s", answer[i][j]);
        }
        
        printf("Enter your choice: ");

        choice = getUserInput();
        
        choice = (char)toupper((char)choice);

        if (choice == correctAnswer[i][16]) {
            printf("\nCorrect!\n");
            ++correct;
        } else {
            printf("\nIncorrect.\n");
        }
    }
    printf("\nYou completed the quiz! Your score: %d/10\n",correct);
    printf("\nDo you want to save your score? (Y/N): ");
    scoreChoice = getUserInput();
    if(scoreChoice == 'Y' || scoreChoice == 'y'){
        saveHighScore(correct);
    }
    else if(scoreChoice == 'N' || scoreChoice == 'n'){
        //displayMenu(question, answer, correctAnswer, questionCount, correctCount); Can not call this function I guess it is because of compiler
    }
    else{
        printf("\nWrong input\n");
        exit(1);
    }
}


void loadQuestions(char question[][MAX_LENGTH], char answer[][MAX_ANSWER_CHOICES][MAX_ANSWER_LENGTH], char correctAnswer[][CORRECT], int *questionCount, int *correctCount) {// this  function help us to load questions to array
    FILE *inFile = fopen("questions.txt.txt", "r");
    if (inFile == NULL) {
        perror("Error opening file");
        exit(1);
    }

    int questionRead = 0;

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), inFile) != NULL) {
        if (strstr(line, "Question") != NULL) {
            if (questionRead == 0) {
                strcpy(question[(*questionCount)], line);
                (*questionCount)++;
                questionRead = 1;
            }
        } else if (strstr(line, "Correct") != NULL) {
            strcpy(correctAnswer[*questionCount], line); // Store correct answers
            (*correctCount)++;
        } else if (questionRead == 1) {
            strcpy(answer[*questionCount][0], line);// Store all multiple choice answer
            for (int i = 1; i < MAX_ANSWER_CHOICES; i++) {
                if (fgets(line, sizeof(line), inFile) == NULL) {
                    break;
                }
                strcpy(answer[*questionCount][i], line);
            }
            questionRead = 0; // Reset for the next question
        }
    }
    fclose(inFile);
}
void startQuiz(char question[][MAX_LENGTH], char answer[][MAX_ANSWER_CHOICES][MAX_ANSWER_LENGTH], char correctAnswer[][CORRECT], int *questionCount, int *correctCount) {// Startquiz function to handle all logic
    loadQuestions(question, answer, correctAnswer, questionCount, correctCount);
    printf("Loading the Quiz...");
    askQuestion(question, answer, correctAnswer, questionCount, correctCount);
}

int isValidChoice(char choice) {// checking characters as integers
    return (choice == '1' || choice == '2' || choice == '3');
}

void displayMenu(char question[][MAX_LENGTH], char answer[][MAX_ANSWER_CHOICES][MAX_ANSWER_LENGTH], char correctAnswer[][CORRECT], int *questionCount, int *correctCount) {//displaying the menu
    char choice;

    printf("Menu:\n1. Start Quiz\n2. View High Scores\n3. Quit\n");
    printf("Enter your choice: ");

    choice = getUserInput();
    fflush(stdin);

    while (!isValidChoice(choice)) {
        printf("Wrong selection. Please enter your choice again: ");
        fflush(stdin);
        choice = getUserInput();
    }

    switch (choice) {
        case '1':
            startQuiz(question, answer, correctAnswer, questionCount, correctCount);
            break;
        case '2':
            loadHighScores();
            break;
        case '3':
            exit(0);
            break;
    }
}

int main(void) {//main function
    char question[MAX_QUESTIONS][MAX_LENGTH];
    char answer[MAX_QUESTIONS][MAX_ANSWER_CHOICES][MAX_ANSWER_LENGTH];
    char correctAnswer[MAX_QUESTIONS][CORRECT];
    int questionCount = 0;
    int correctCount = 0;

    displayMenu(question, answer, correctAnswer, &questionCount, &correctCount);

    return 0;
}

