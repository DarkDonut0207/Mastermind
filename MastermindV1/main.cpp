/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Drake
 *
 * Created on April 17, 2021, 8:30 PM
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <set>
using namespace std;
const int maxMvs = 10; // Global constant for max moves
void menu();
void game();
void setting(int&, int&, bool&);
/* Header for genCode function
 * ----------------------------------------------------------------------------
 * Receives an integer representing selection of numbers code will contain, an
 * integer representing the length of the code, and a bool representing whether
 * duplicates can appear or not. Returns the code to be used.
 */
string genCode(int, int, bool);
string getGuess(int, int, bool);
bool checkGuess(string[maxMvs], string, int, int[maxMvs], 
                int[maxMvs], int, int, bool);
/*
 * 
 */
int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    menu();
    return 0;
}
void game() {
    int curMv = 0;
    int letC; 
    int cdeLng; // Length of the code (4-8)
    bool duplic; // Stores whether duplicates are allowed
    string code; // Code to be solved for
    string guess[maxMvs]; // Guess inputted by user
    int RLWS[maxMvs]; // Array to store right letter wrong spot info
    int RLRS[maxMvs]; // Array to store right letter right spot info
    bool win = false;
    setting(letC, cdeLng, duplic);
    // Assign initial values to RLWS, RLRS, and guess
    for (int i=0; i<maxMvs; i++) {
        RLWS[i] = 0;
        RLRS[i] = 0;
        switch (cdeLng) {
            case 4: 
                guess[i] = "----";
                break;
            case 5:
                guess[i] = "-----";
                break;
            case 6:
                guess[i] = "------";
                break;
            case 7:
                guess[i] = "-------";
                break;
            case 8:
                guess[i] = "-------";
                break;
            default:
                guess[i] = "----";
        }
    }
    code = genCode(letC, cdeLng, duplic); // Generate code
    while (curMv < maxMvs && !win) {
        cout << "Code:";
        for (int i=0; i<cdeLng; i++)
            cout << " -";
        for (int i=maxMvs-1; i>=0; i--) {
            cout << "\n| ";
            for (int j=0; j<cdeLng; j++) {
                cout << guess[i][j] << " ";
            }
            cout << "| RLWS: " << RLWS[i] << " RLRS: " << RLRS[i];
        }
        guess[curMv] = getGuess(letC, cdeLng, duplic);
        win = checkGuess(guess, code, curMv, RLWS, RLRS, letC, cdeLng, duplic);
        curMv++;
    }
    if (win) cout << "You have cracked the code!\n";
    else cout << "You ran out of moves.\n";
    cout << "Code:";
    for (int i=0; i<cdeLng; i++)
        cout << " " << code[i];
    for (int i=maxMvs-1; i>=0; i--) {
        cout << "\n| ";
        for (int j=0; j<cdeLng; j++) {
            cout << guess[i][j] << " ";
        }
        cout << "| RLWS: " << RLWS[i] << " RLRS: " << RLRS[i];
    }
}
// Definition for setting function
void setting(int& letC, int& cdeLng, bool& duplic) {
    char dupCho = 'n';
    cout << "Input a code length from 4-8: ";
    cin >> cdeLng;
    cout << "Input the number of unique letters that may appear in the code " <<
            "(6-8, can't be less than code length): ";
    cin >> letC;
    cout << "Allow duplicates? (y/n): ";
    cin >> dupCho;
    // Error prevention
    if (cdeLng < 4) cdeLng = 4;
    else if (cdeLng > 8) cdeLng = 8;
    if (letC<cdeLng) letC = cdeLng;
    if (letC<6) letC = 6;
    else if (letC>8) letC = 8;
    switch (dupCho) {
        case 'y':
            duplic = true;
            break;
        case 'n':
            duplic = false;
            break;
        default:
            duplic = false;
    }
}
// Definition for genCode function
string genCode(int letC, int cdeLng, bool duplic) {
    string code = "\0"; // Code to be solved for
    
    char tmpLet; // Temporarily stores letter to add to code
    // Generate code
    if (duplic) { // If duplicates are allowed
        for (int i=0; i<cdeLng; i++) { // Loop for code length
            code += 65 + rand()%letC; // Add a random letter to the code
        }   
    } else { // If duplicates are not allowed
        for (int i=0; i<cdeLng; i++) { // Loop for code length
            set<char, greater<char>> s1; // declare set
            set<char, greater<char> >::iterator itr; // declare set iterator
            char usdLet[i]; // A char array to store used letters
            tmpLet = 65 + rand()%(letC-i); // Random letter
            for (int j=0; j<i; j++) {
                s1.insert(code[j]); // Store the letters
            }
            set<char> s2(s1.begin(), s1.end()); // Copy set over to new set
            itr = s2.begin(); // reset iterator
            for (int j=0; j<i; j++) {
                if (tmpLet>=*itr) {
                    tmpLet += 1;
                }
                itr++;
            }
            code += tmpLet;
        }
    }
    return code;
}

string getGuess(int letC, int cdeLng, bool duplic) {
    string guess = "\0";
    char guessC[cdeLng];
    char letter;
    cout << "\nInput Guess as " << cdeLng << " uppercase letters from";
    cout << " selection: ";
    for (int i=0; i<letC; i++) {
        cout << char('A' + i) << " ";
    }
    cout << "\nNote that duplicates may ";
    if(!duplic) cout << "not ";
    cout << "appear in the code.\n";
    cout << "Guess: ";
    for (int i=0; i<cdeLng; i++) {
        cin >> guessC[i];
        if (guessC[i]<'A') guessC[i] = 'A';
        else if (guessC[i]>char('A' + letC-1)) guessC[i] = char('A' + letC-1);
        guess += string(1, guessC[i]);
    }
    return guess;
}

bool checkGuess(string guess[maxMvs], string code, 
                int move, int RLWS[maxMvs], int RLRS[maxMvs],
                int letC, int cdeLng, bool duplic) {
    bool usdSlt[cdeLng]; // will store used slots for keeping track of info
    for (int i=0; i<cdeLng; i++) usdSlt[i] = false; // Initialize values
    // First check for right slot right letter
    for (int i=0; i<cdeLng; i++) { // Loop through code
        if (guess[move][i] == code[i] && !usdSlt[i]) {
            RLRS[move]++; // Increment right letter right slot counter
            usdSlt[i] = true; // Set that the slot being used is true
        }
    }
    // Now check for right letter wrong slot
    for (int i=0; i<cdeLng; i++) { // Loop through the guess
        for (int j=0; j<cdeLng; j++) { // Loop through the code
            if (guess[move][i] == code[j] && i!=j && !usdSlt[j]) {
                RLWS[move]++; // Increment right letter right slot counter
                usdSlt[j] = true; // Set that the slot being used is true
            }
        }
    }
    if (guess[move]==code) return true;
    else return false;
}

void menu() {
    char choice = 'n';
    cout << "---Mastermind---\nCoded by Drake Fafard\nGame Information:\n" <<
            "RLWS stands for right letter, but wrong spot.\n" <<
            "RLRS stands for right letter and right spot.\n" <<
            "The goal is to reach the max amount of RLRS by using the" <<
            "\ninformation that these two counters provide on your guess.\n" <<
            "Reaching the max amount of RLRS means you have cracked the code.";
    cout << "\nStart a new game? (y/n): ";
    cin >> choice;
    while (choice=='y' || choice == 'Y') {
        game(); // Start a new game
        cout << "\nStart a new game? (y/n): ";
        cin >> choice;
    }
    cout << "\nEnding program. Thank you for playing Mastermind.";
}