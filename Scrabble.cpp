//Scrabble in C++
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    int sump1 = 0;
    int sump2 = 0;
    int limitp1 = 1;
    int limitp2 = 1;
    string p1;
    string p2;
    map<char, int> alphscore = {
        {'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1},
        {'F', 4}, {'G', 2}, {'H', 4}, {'I', 1}, {'J', 8},
        {'K', 5}, {'L', 1}, {'M', 3}, {'N', 1}, {'O', 1},
        {'P', 3}, {'Q', 10}, {'R', 1}, {'S', 1}, {'T', 1},
        {'U', 1}, {'V', 4}, {'W', 4}, {'X', 8}, {'Y', 4},
        {'Z', 10}, {'!', 0}, {'?', 0}
    };
    cout << "Player 1: ";
    cin >> p1;
    cout << "Player 2: ";
    cin >> p2;
    transform(p1.begin(), p1.end(), p1.begin(), ::toupper);
    transform(p2.begin(), p2.end(), p2.begin(), ::toupper);
    vector<char> playero(p1.begin(), p1.end());
    vector<char> playert(p2.begin(), p2.end());
    limitp1 = limitp1 + playero.size();
    limitp2 = limitp2 + playert.size();
    for (char i; i < limitp1 - 1; i++) {
       sump1 = sump1 + alphscore[playero.at(i)];
    }

    for (char t; t < limitp2 - 1; t++) {
        sump2 = sump2 + alphscore[playert.at(t)];
    }

    if (sump1 > sump2) {
        cout << "Player 1 wins!" << endl;
    }

    if (sump2 > sump1) {
        cout << "Player 2 wins!" << endl;
    }


    if (playero == playert) {
        cout << "Tie!";
    }


}
