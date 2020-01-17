#pragma once

#include <string>
#include <vector>

struct UtttBoard {
    bool miniGetGameEnded(int miniBoard);
    int miniGetWinResult(int miniBoard);
    void miniMarkValidMoves(int miniBoard, std::vector<int> &valid);

    std::vector<int> board = std::vector<int>(81, 0);
    int nextSquare = -1, curPlayer = 1;
    bool invalid = false;

    UtttBoard(bool inv = false) { invalid = inv; }

    std::vector<int> getValidMoves();
    std::vector<int> getValidMovesVector();
    bool getGameEnded();
    void processMove(int position);
    std::vector<int> getSmallWinResults();
    int getWinResult();
    void rotate90();
    void reflect();
    std::vector<std::pair<std::vector<float>, std::vector<float>>>
    getSymmetries(std::vector<float> probabilityVector);
    std::vector<float> toNetInputVector();
    std::string toString();
    long long hash();
    long long hashWithMove(int move);
};
