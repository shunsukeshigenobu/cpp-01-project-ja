#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 車の状態を表す変数
double carSpeed = 0.0;
double carGas = 100.0;
pair<int, int> carPosition = {0, 0};

// ランドマークの情報を表す変数
// std::vector landmarks = {
//     {"Park", {10, 10},"This is park"}},
//     {"Museum", {25, 5},"This is Museum"},
//     {"gasstation", {40, -3},"This is gas station"}
// };
// 左折
void turnLeft() {
    std::cout << "Turning left." << endl;
    carPosition.first -= 5;
}
// 右折
void turnRight() {
    std::cout << "Turning right." << endl;
    carPosition.first += 5;
}
// 前進
void continueDriving() {
    std::cout << "Continuing forward." << endl;
    carPosition.second += static_cast<int>(carSpeed / 10);
}
// 加速
void accelerate() {
    std::cout << "Accelerating." << endl;
    carSpeed += 10;
}

// 減速
void decelerate() {
    std::cout << "Decelerating." << endl;
    carSpeed -= 10;
}
// 停車
void stop() {
    std::cout << "stop" << endl;
    carSpeed = 0;
}

// ガソリンの消費処理
void consumeGas() {
    if (carSpeed == 0) {
        return;
    }
    double consumption = carSpeed / 20.0;
    carGas -= consumption;
    if (carGas <= 0) {
       std::cout << "Out of gas! Stopping the game." << endl;
        stop();
    }
}
// ランドマークの情報を表示する関数
// void displayLandmarkInfo() {
//     for (const auto& landmark : landmarks) {
//         if (landmark.second == carPosition) {
//             std::cout << "You are at " << landmark.first << "." << endl;
//             return;
//         }
//     }
// }

// 車の状態を表示する関数
void displayStatus() {
    std::cout << "Current Status" << endl;
    std::cout << "Position: (" << carPosition.first << ", " << carPosition.second << ")" << endl;
    std::cout << "Speed: " << carSpeed << " Km/h" << endl;
    std::cout << "Gas: " << carGas << " L" << endl;

    // displayLandmarkInfo();

    std::cout << endl;
}

int main() {
    std::cout << "Welcome to the Driving Game!" << endl;

    while (true) {
        displayStatus();

        string command;
        std::cout << "Enter a command (turn left,turn right, continue, accelerate, decelerate, stop): ";
        getline(cin, command);
        if (command == "turn left") {
            turnLeft();
        } else if (command == "turn right") {
            turnRight();
        } else if (command == "continue") {
            continueDriving();
        } else if (command == "accelerate") {
            accelerate();
        } else if (command == "decelerate") {
            decelerate();
        } else if (command == "stop") {
            stop();
            break;
        } else {
            std::cout << "Invalid command. Try again." << endl;
        }
        consumeGas();
    }
    return 0;
}