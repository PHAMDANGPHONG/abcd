#include <iostream>
#include <vector>
using namespace std;

typedef vector<vector<char> > State;
const int SIZE = 3;

void renderGame(State state)
{
    cout << " -------------" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << " | " << state[i][j];
        }
        cout << " |" << endl << " -------------" << endl;
    }
}

char NextPlayer(State s)
{
    int countX = 0, countO = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == 'X') countX++;
            else if (s[i][j] == 'O') countO++;
        }
    }
    if (countX == countO) return 'X';
    else return 'O';
}

State play(State s, int i, int j, char player)
{
    State newState = State(3, vector<char>(3, ' '));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            newState[i][j] = s[i][j];
        }
    }
    newState[i][j] = player;
    return newState;
}

vector<State> getNextStates(State s)
{
    char nextPlayer = NextPlayer(s);
    vector<State> states;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == ' ') {
                State nextState = play(s, i, j, nextPlayer);
                states.push_back(nextState);
            }
        }
    }
    return states;
}

bool isFinalState(State s)
{
    for (int i = 0; i < SIZE; i++)
        if (s[i][0] == s[i][1] && s[i][0] == s[i][2] && s[i][0] != ' ') return true;
    for (int i = 0; i < SIZE; i++)
        if (s[0][i] == s[1][i] && s[0][i] == s[2][i] && s[0][i] != ' ') return true;
    if (s[0][0] == s[1][1] && s[0][0] == s[2][2] && s[0][0] != ' ') return true;
    if (s[0][2] == s[1][1] && s[0][2] == s[2][0] && s[0][2] != ' ') return true;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == ' ') return false;
        }
    }
    return true;
}

int ScoreFinalState(State s)
{
    for (int i = 0; i < SIZE; i++)
        if (s[i][0] == s[i][1] && s[i][0] == s[i][2] && s[i][0] != ' ')
            return s[i][0] == 'X' ? 1 : -1;
    for (int i = 0; i < SIZE; i++)
        if (s[0][i] == s[1][i] && s[0][i] == s[2][i] && s[0][i] != ' ')
            return s[0][i] == 'X' ? 1 : -1;
    if (s[0][0] == s[1][1] && s[0][0] == s[2][2] && s[0][0] != ' ')
        return s[0][0] == 'X' ? 1 : -1;
    if (s[0][2] == s[1][1] && s[0][2] == s[2][0] && s[0][2] != ' ')
        return s[0][2] == 'X' ? 1 : -1;;
    return 0;
}

pair<int, State> getScore(State s)
{
    if (isFinalState(s)) {
        return make_pair(ScoreFinalState(s), s);
    }
    char player = NextPlayer(s);
    vector<State> states = getNextStates(s);
    int bestScore = -10;
    State bestState;
    for (State ns: states) {
        pair<int, State> p = getScore(ns);
        int score = p.first;
        if (player == 'X') {
            if (bestScore < score || bestScore == -10) {
                bestScore = score;
                bestState = ns;
            }
        } else {
            if (bestScore > score || bestScore == -10) {
                bestScore = score;
                bestState = ns;
            }
        }
    }
    return make_pair(bestScore, bestState);
}

pair<int, int> getComputerPlay(State s)
{
    pair<int, State> p = getScore(s);
    int score = p.first;
    State newState = p.second;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (newState[i][j] != s[i][j]) return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}

int main()
{
    int test=0;
    State s = State(3, vector<char>(3, ' '));
    char humanPlayer = ' ', currentPlayer = 'X';
    cout << "Choose your player (X or O): ";
    cin >> humanPlayer;
    if(humanPlayer!='X'&&humanPlayer!='O') cout<<"Error!!!"<<endl;
    else{
        cout << "You are " << humanPlayer << " player" << endl;
    renderGame(s);
    while (true) {
        int i = -1, j = -1;
        if (humanPlayer == currentPlayer) {
            cout << "Enter your play: ";
            cin >> i >> j;
            if(i<0||j<0||i>2||j>2||s[i][j]!=' ')
            {
                test=1;
                cout<<"Error, ";
            }
            else{
                cout << "You play (" << i << ", " << j <<")"<< endl;
                test=0;
            }
        } else {
            pair<int, int> move = getComputerPlay(s);
            i = move.first;
            j = move.second;
            cout << "I play (" << i << ", " << j << ")" << endl;
        }
        if(test==0)
        {
            s = play(s, i, j, currentPlayer);
        renderGame(s);

        currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
        if (isFinalState(s)) {
            int score = ScoreFinalState(s);
            if (score > 0) {
                cout << "X won" << endl;
            } else if (score < 0) {
                cout << "X lost" << endl;
            } else {
                cout << "A draw" << endl;
            }
            break;
        }
        }
        else cout<<"Enter again!!!"<<endl;
    }
    }
    return 0;
}
