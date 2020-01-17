
#include "UtttBoard.h"
//#include <vector>
#include <iostream>


using namespace std;

template<typename T>
vector<T> rotateMatrix90(vector<T> &board) {
    vector<T> temp(81);

    temp[0] = board[20];
    temp[1] = board[23];
    temp[2] = board[26];
    temp[9] = board[47];
    temp[10] = board[50];
    temp[11] = board[53];
    temp[18] = board[74];
    temp[19] = board[77];
    temp[20] = board[80];
    temp[3] = board[19];
    temp[4] = board[22];
    temp[5] = board[25];
    temp[12] = board[46];
    temp[13] = board[49];
    temp[14] = board[52];
    temp[21] = board[73];
    temp[22] = board[76];
    temp[23] = board[79];
    temp[6] = board[18];
    temp[7] = board[21];
    temp[8] = board[24];
    temp[15] = board[45];
    temp[16] = board[48];
    temp[17] = board[51];
    temp[24] = board[72];
    temp[25] = board[75];
    temp[26] = board[78];
    temp[27] = board[11];
    temp[28] = board[14];
    temp[29] = board[17];
    temp[36] = board[38];
    temp[37] = board[41];
    temp[38] = board[44];
    temp[45] = board[65];
    temp[46] = board[68];
    temp[47] = board[71];
    temp[30] = board[10];
    temp[31] = board[13];
    temp[32] = board[16];
    temp[39] = board[37];
    temp[40] = board[40];
    temp[41] = board[43];
    temp[48] = board[64];
    temp[49] = board[67];
    temp[50] = board[70];
    temp[33] = board[9];
    temp[34] = board[12];
    temp[35] = board[15];
    temp[42] = board[36];
    temp[43] = board[39];
    temp[44] = board[42];
    temp[51] = board[63];
    temp[52] = board[66];
    temp[53] = board[69];
    temp[54] = board[2];
    temp[55] = board[5];
    temp[56] = board[8];
    temp[63] = board[29];
    temp[64] = board[32];
    temp[65] = board[35];
    temp[72] = board[56];
    temp[73] = board[59];
    temp[74] = board[62];
    temp[57] = board[1];
    temp[58] = board[4];
    temp[59] = board[7];
    temp[66] = board[28];
    temp[67] = board[31];
    temp[68] = board[34];
    temp[75] = board[55];
    temp[76] = board[58];
    temp[77] = board[61];
    temp[60] = board[0];
    temp[61] = board[3];
    temp[62] = board[6];
    temp[69] = board[27];
    temp[70] = board[30];
    temp[71] = board[33];
    temp[78] = board[54];
    temp[79] = board[57];
    temp[80] = board[60];

    return temp;
}

template<typename T>
vector<T> reflectMatrix(vector<T> &board) {
    vector<T> temp(81);

    temp[0] = board[60];
    temp[1] = board[61];
    temp[2] = board[62];
    temp[9] = board[69];
    temp[10] = board[70];
    temp[11] = board[71];
    temp[18] = board[78];
    temp[19] = board[79];
    temp[20] = board[80];
    temp[3] = board[57];
    temp[4] = board[58];
    temp[5] = board[59];
    temp[12] = board[66];
    temp[13] = board[67];
    temp[14] = board[68];
    temp[21] = board[75];
    temp[22] = board[76];
    temp[23] = board[77];
    temp[6] = board[54];
    temp[7] = board[55];
    temp[8] = board[56];
    temp[15] = board[63];
    temp[16] = board[64];
    temp[17] = board[65];
    temp[24] = board[72];
    temp[25] = board[73];
    temp[26] = board[74];
    temp[27] = board[33];
    temp[28] = board[34];
    temp[29] = board[35];
    temp[36] = board[42];
    temp[37] = board[43];
    temp[38] = board[44];
    temp[45] = board[51];
    temp[46] = board[52];
    temp[47] = board[53];
    temp[30] = board[30];
    temp[31] = board[31];
    temp[32] = board[32];
    temp[39] = board[39];
    temp[40] = board[40];
    temp[41] = board[41];
    temp[48] = board[48];
    temp[49] = board[49];
    temp[50] = board[50];
    temp[33] = board[27];
    temp[34] = board[28];
    temp[35] = board[29];
    temp[42] = board[36];
    temp[43] = board[37];
    temp[44] = board[38];
    temp[51] = board[45];
    temp[52] = board[46];
    temp[53] = board[47];
    temp[54] = board[6];
    temp[55] = board[7];
    temp[56] = board[8];
    temp[63] = board[15];
    temp[64] = board[16];
    temp[65] = board[17];
    temp[72] = board[24];
    temp[73] = board[25];
    temp[74] = board[26];
    temp[57] = board[3];
    temp[58] = board[4];
    temp[59] = board[5];
    temp[66] = board[12];
    temp[67] = board[13];
    temp[68] = board[14];
    temp[75] = board[21];
    temp[76] = board[22];
    temp[77] = board[23];
    temp[60] = board[0];
    temp[61] = board[1];
    temp[62] = board[2];
    temp[69] = board[9];
    temp[70] = board[10];
    temp[71] = board[11];
    temp[78] = board[18];
    temp[79] = board[19];
    temp[80] = board[20];

    return temp;
}



bool UtttBoard::miniGetGameEnded(int miniBoard) {
    int offset = miniBoard * 9;
    if (miniGetWinResult(miniBoard) != 0) return true;

    for (int i = 0; i < 9; i++) {
        if (board[9 * miniBoard + i] == 0) return false;
    }
    return true;
}

int UtttBoard::miniGetWinResult(int miniBoard) {
    int offset = miniBoard * 9;

    for (int i = 0; i < 3; i++) {
        if (board[offset + i * 3] == board[offset + i * 3 + 1] && board[offset + i * 3] == board[offset + i * 3 + 2] && board[offset + i * 3] != 0) {
            return board[offset + i * 3];
        } 

        if (board[offset + i] == board[offset + 3 + i] && board[offset + i] == board[offset + 6 + i] && board[offset + i] != 0) {
            return board[offset + i];
        }
    }

    if (board[offset] == board[offset + 4] && board[offset + 4] == board[offset + 8] && board[offset] != 0) {
        return board[offset];
    }

    if (board[offset + 2] == board[offset + 4] && board[offset + 4] == board[offset + 6] && board[offset + 2] != 0) {
        return board[offset + 2];
    }

    return 0;
}

void UtttBoard::miniMarkValidMoves(int miniBoard, vector<int> &valid) {
    int offset = miniBoard * 9;
    if (miniGetGameEnded(miniBoard)) return;

    for (int i = 0; i < 9; i++) {
        valid[offset + i] = (board[offset + i] == 0 ? 1 : 0);
    }
}

vector<int> UtttBoard::getValidMoves() {
    vector<int> answer(81, 0);

    for (int i = 0; i < 9; i++) {
        if (i == nextSquare || nextSquare == -1) {
            miniMarkValidMoves(i, answer);
        }
    }

    return answer;
}

vector<int> UtttBoard::getValidMovesVector() {
    vector<int> validMoves = getValidMoves();
    vector<int> answer;

    for (int i = 0; i < 81; i++) {
        if (validMoves[i]) answer.push_back(i);
    }

    return answer;
}

bool UtttBoard::getGameEnded() {
    if (getWinResult() != 0) return true;

    for (int i = 0; i < 9; i++) {
        if (!miniGetGameEnded(i)) return false;
    }
    return true;
}

void UtttBoard::processMove(int position) {
    if (getGameEnded()) return;

    board[position] = curPlayer;

    nextSquare = position % 9;

    if (miniGetGameEnded(nextSquare)) {
        nextSquare = -1;
    }

    curPlayer = (curPlayer == 1 ? 2 : 1);
}

vector<int> UtttBoard::getSmallWinResults() {
    vector<int> answer(9, 0);

    for (int i = 0; i < 9; i++) {
        answer[i] = miniGetWinResult(i);
    }

    return answer;
}

int UtttBoard::getWinResult() {
    vector<int> winResults = getSmallWinResults();

    for (int i = 0; i < 3; i++) {
        if (winResults[i * 3] == winResults[i * 3 + 1] && winResults[i * 3] == winResults[i * 3 + 2] && winResults[i * 3] != 0) {
            return winResults[i * 3];
        } 

        if (winResults[i] == winResults[3 + i] && winResults[i] == winResults[6 + i] && winResults[i] != 0) {
            return winResults[i];
        }
    }

    if (winResults[0] == winResults[4] && winResults[4] == winResults[8] && winResults[0] != 0) {
        return winResults[0];
    }

    if (winResults[2] == winResults[4] && winResults[4] == winResults[6] && winResults[2] != 0) {
        return winResults[2];
    }

    return 0;
}

void UtttBoard::rotate90() {
    board = rotateMatrix90<int>(board);

    if (nextSquare == 0) {
        nextSquare = 6;
    } else if (nextSquare == 1) {
        nextSquare = 3;
    } else if (nextSquare == 2) {
        nextSquare = 0;
    } else if (nextSquare == 3) {
        nextSquare = 7;
    } else if (nextSquare == 5) {
        nextSquare = 1;
    } else if (nextSquare == 6) {
        nextSquare = 8;
    } else if (nextSquare == 7) {
        nextSquare = 5;
    } else if (nextSquare == 8) {
        nextSquare = 2;
    }
}

void UtttBoard::reflect() {
    board = reflectMatrix<int>(board);

    if (nextSquare == 6) {
        nextSquare = 0;
    } else if (nextSquare == 7) {
        nextSquare = 1;
    } else if (nextSquare == 8) {
        nextSquare = 2;
    } else if (nextSquare == 0) {
        nextSquare = 6;
    } else if (nextSquare == 1) {
        nextSquare = 7;
    } else if (nextSquare == 2) {
        nextSquare = 8;
    }
}
vector<pair<vector<float>, vector<float>>> UtttBoard::getSymmetries(vector<float> probabilityVector) {
    vector<pair<vector<float>, vector<float>>> answer;

    for (int i = 0; i < 4; i++) {
        rotate90();
        probabilityVector = rotateMatrix90<float>(probabilityVector);

        answer.push_back(make_pair(toNetInputVector(), probabilityVector));
    }

    reflect();
    probabilityVector = reflectMatrix<float>(probabilityVector);

    for (int i = 0; i < 4; i++) {

        rotate90();
        probabilityVector = rotateMatrix90<float>(probabilityVector);

        answer.push_back(make_pair(toNetInputVector(), probabilityVector));
    }

    reflect();

    return answer;
}

vector<float> UtttBoard::toNetInputVector() {
    vector<float> answer(189, 0.0f);

    for (int i = 0; i < 81; i++) {
        answer[i] = (board[i] == curPlayer ? 1.0f : 0.0f);
    }

    int opponent = (curPlayer == 1 ? 2 : 1);

    for (int i = 0; i < 81; i++) {
        answer[i + 81] = (board[i] == opponent ? 1.0f : 0.0f);
    }

    vector<int> winResults = getSmallWinResults();

    for (int i = 0; i < 9; i++) {
        answer[i + 162] = (winResults[i] == curPlayer ? 1.0f : 0.0f);
    }

    for (int i = 0; i < 9; i++) {
        answer[i + 171] = (winResults[i] == opponent ? 1.0f : 0.0f);
    }

    for (int i = 0; i < 9; i++) {
        answer[i + 180] = ((nextSquare == i || nextSquare == -1) && !miniGetGameEnded(i) ? 1.0f : 0.0f);
    }

    return answer;
}

string UtttBoard::toString() {
    vector<vector<int>> arrayHuman(9, vector<int>(9, 0));

    for (int i = 0; i < 81; i++) {
        int big = i / 9;
        int small = i % 9;

        int humanRow = (3 * (big / 3)) + (small / 3);
        int humanCol = (3 * (big % 3)) + (small % 3);

        arrayHuman[humanRow][humanCol] = board[i];
    }

    string answer = "";

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (arrayHuman[i][j] == 0) {
                answer += '.';
            } else if (arrayHuman[i][j] == 1) {
                answer += 'X';
            } else {
                answer += 'O';
            }

            if (j == 2 || j == 5) {
                answer += ' ';
            }
        }
        answer += '\n';

        if (i == 2 || i == 5) {
            answer += '\n';
        }
    }

    return answer;
}

long long UtttBoard::hash() {
    long long answer = nextSquare + 1;

    for (int i = 0; i < 81; i++) {
        answer = answer * 31 + board[i];
    }

    return answer;
}

long long UtttBoard::hashWithMove(int move) {
    return hash() * 83 + move; //83 is prime
}