//
// Created by aboo on 13.10.2019.
//

#ifndef LOGIC_LOGIC_H
#define LOGIC_LOGIC_H
#include <display/display.h>

#include <vector>

class TGameLogic {
public:
    TGameLogic();
    void Move(EKey key);
    ETileType GetTile(int i, int j) const;
    static int N;
    struct TChange {
        int i, j, len;
        EKey key;
        ETileType tile;
        TChange(int i, int j, int len, EKey key, ETileType tile);
        TChange() = default;
    };
    const std::vector<TChange>& GetChanges() const;
    std::vector<std::vector<ETileType>> Affected;
private:

    std::vector<TChange> Changes;
    std::vector<std::vector<ETileType>> Field;
    void FlipDiag();
    void FlipHorizontal();
    void GenNewCell();
    unsigned OccupiedCounter;

    void UnionTiles(int row, int leftCol, int rightCol, ETileType oldTile, EKey key);
};


#endif //LOGIC_LOGIC_H