#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "SerialPort.h"
#include "SerialPort.cpp"
using namespace std;

char arduinoOutput[MAX_DATA_LENGTH];
char *port;

void screenPrint(int posTarget){
    int line = 0;
    for(int t = 0; t < 3; t++) { // Blocks
        for (int r = 0; r < 5; r++){ // Lines
            for (int i = 0; i < 3; i++) { // Spaces (1 de 9)
                if (posTarget != line+i){
                    cout << "--------------- ";
                } else {
                    cout << "+++++++++++++++ ";
                }
            }
            cout << endl;
        }
        line += 3;
        cout << endl;
    }
}

int main() {
    // Variable declaration
    int score = 0;
    int lives = 3;
    string directions[4] = {"UP", "RIGHT", "DOWN", "LEFT"};
    int screenPosition[4] = {1, 5, 7, 3};
    string input;   

    // Connect SerialMonitor
    port = (char *)(new string("\\\\.\\COM5"))->c_str();
    SerialPort arduino(port);
    if(arduino.isConnected()) {
        cout << "Connection is Established" << endl;
    } else {
        cout << "Error in port name";
    }
    
    Sleep(2000);
    system("cls");

    // Start of game
    cout << "Welcome!" << endl;
    cout << endl;
    cout << "Instructions:" << endl;
    cout << "The screen will show the position you should point to" << endl;
    cout << "There will be 5 seconds to point that direction" << endl;
    cout << "You will have 3 lives to score the most amount of points" << endl;
    cout << "Good Luck!" << endl;

    system("pause");
    system("cls");
    cout << "Starts in.. 3";
    Sleep(1000);
    cout << "  2";
    Sleep(1000);
    cout << "  1";
    Sleep(1000);
    system("cls");
    screenPrint(8);

    while(arduino.isConnected()) {
        int lastSkipLine = 0;
        int positionX = 0;
        int positionY = 0;
        int positionZ = 0;
        int intX, intY, intZ = 0;
        srand (time(NULL));
        int randPos = rand()%4;

        cout << "CURRENT SCORE: " << score << endl;
        cout << endl;
        cout << "THERE IS AN ENEMY!!" << endl;
        cout << "POINT " << directions[randPos] << endl;
        cout << endl;
        screenPrint(screenPosition[randPos]);
        cout << "You have.. 5";
        Sleep(1000);
        cout << "  4";
        Sleep(1000);
        cout << "  3";
        Sleep(1000);
        cout << "  2";
        Sleep(1000);
        cout << "  1" << endl;
        Sleep(1000);

        arduino.readSerialPort(arduinoOutput, MAX_DATA_LENGTH);


        // Look for the end of last line
        for(int pos = 200; pos >= 0; pos--){
            if (arduinoOutput[pos] == '\n'){
                lastSkipLine = pos;
                break;
            }
        }
        // Look for Z position
        for(int pz = lastSkipLine-6; pz >= 0; pz--){
            if (arduinoOutput[pz] == 'Z'){
                positionZ = pz;
                break;
            }
        }
        // Look for Y position
        for(int py = positionZ-7; py >= 0; py--){
            if (arduinoOutput[py] == 'Y'){
                positionY = py;
                break;
            }
        }
        // Look for X position
        for(int px = positionY-7; px >= 0; px--){
            if (arduinoOutput[px] == 'X'){
                positionX = px;
                break;
            }
        }


        // Check if X is negative, and turn it into an integer
        if (arduinoOutput[positionX+2] == '-'){
            if(arduinoOutput[positionX+4] == '.'){
                intX = -1*(arduinoOutput[positionX+3] - '0');
            } else {
                intX = -1*(((arduinoOutput[positionX+3] - '0')*10) + arduinoOutput[positionX+4] - '0');
            }
        } else {
            if(arduinoOutput[positionX+3] == '.'){
                intX = arduinoOutput[positionX+2] - '0';
            } else {
                intX = ((arduinoOutput[positionX+2] - '0')*10) + arduinoOutput[positionX+3] - '0';
            }
        }
        
        // Check if Y is negative, and turn it into an integer
        if (arduinoOutput[positionY+2] == '-'){
            if(arduinoOutput[positionY+4] == '.'){
                intY = -1*(arduinoOutput[positionY+3] - '0');
            } else {
                intY = -1*(((arduinoOutput[positionY+3] - '0')*10) + arduinoOutput[positionY+4] - '0');
            }
        } else {
            if(arduinoOutput[positionY+3] == '.'){
                intY = arduinoOutput[positionY+2] - '0';
            } else {
                intY = ((arduinoOutput[positionY+2] - '0')*10) + arduinoOutput[positionY+3] - '0';
            }
        }

        // Check if Z is negative, and turn it into an integer
        if (arduinoOutput[positionZ+2] == '-'){
            if(arduinoOutput[positionZ+4] == '.'){
                intZ = -1*(arduinoOutput[positionZ+3] - '0');
            } else {
                intZ = -1*(((arduinoOutput[positionZ+3] - '0')*10) + arduinoOutput[positionZ+4] - '0');
            }
        } else {
            if(arduinoOutput[positionZ+3] == '.'){
                intZ = arduinoOutput[positionZ+2] - '0';
            } else {
                intZ = ((arduinoOutput[positionZ+2] - '0')*10) + arduinoOutput[positionZ+3] - '0';
            }
        }
        
        cout << intX << " " << intY << " " << intZ << endl;

        // Increase or decrease score
        if(randPos == 0 && intY > 4) {
            score += 1;
            cout << "NICE" << endl;
            cout << "Score + 1" << endl;
        } else if(randPos == 1 && intX < -4) {
            score += 1;
            cout << "NICE" << endl;
            cout << "Score + 1" << endl;
        } else if(randPos == 2 && intZ > 4) {
            score += 1;
            cout << "NICE" << endl;
            cout << "Score + 1" << endl;
        } else if(randPos == 3 && intX > 4) {
            score += 1;
            cout << "NICE" << endl;
            cout << "Score + 1" << endl;
        } else {
            cout << "YOU MISSED" << endl;
            if(lives > 1){
                lives -= 1;
                cout << "Lives left: " << lives << endl;
            } else {
                break;
            }
        }

        Sleep(2000);
    }
    cout << "Your score is: " << score << endl;
    cout << endl;
    cout << "THANKS FOR PLAYING!!" << endl;
    system("cls");
    return 0;
}