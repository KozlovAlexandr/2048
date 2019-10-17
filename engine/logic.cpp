//
// Created by aboo on 13.10.2019.
//

#include <queue>
#include <stack>
#include "logic.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

int TGameLogic::N = 4;

TGameLogic::TChange::TChange(int i, int j, int len, EKey key, ETileType tile) : i(i), j(j), len(len), key(key),
                        tile(tile) {}

void TGameLogic::FlipDiag() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < i; ++j) {
            std::swap(Field[i][j], Field[j][i]);
            std::swap(Affected[i][j], Affected[j][i]);
        }
    }

    for (auto &ch : Changes) {
        std::swap(ch.i, ch.j);
    }
}

void TGameLogic::FlipHorizontal() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N / 2; ++j) {
            std::swap(Field[i][j], Field[i][N - 1 - j]);
            std::swap(Affected[i][j], Affected[i][N - 1 - j]);
        }
    }

    for (auto &ch : Changes) {
        ch.j = N - 1 - ch.j;
    }
}

void TGameLogic::Move(EKey key) {

    Changes.clear();
    Affected = Field;

    if (key == EKey::KEY_DUMMY) {
        return;
    }

    if (key == EKey::KEY_UP || key == EKey::KEY_DOWN) {
        FlipDiag();
    }

    if (key == EKey::KEY_LEFT || key == EKey::KEY_UP) {
        FlipHorizontal();
    }

    bool fieldChanged = false;
    for (int i = 0; i < N; ++i) {
        std::stack<std::pair<int, ETileType>> st;

        for (int j = 0; j < N; ++j) {
            if (Field[i][j] != ETileType::TILE_0) {
                st.push(std::make_pair(j, Field[i][j]));
            }
        }

        while (!st.empty()) {
            auto p1 = st.top();
            st.pop();
            int k;

            if (!st.empty() && st.top().second == p1.second) {

                UnionTiles(i, st.top().first, p1.first, p1.second, key);
/*                ETileType prev1 = Field[i][p1.first], prev2 = Field[i][st.top().first];
                Field[i][p1.first] = Field[i][st.top().first] = ETileType::TILE_0;

                Affected[i][p1.first] = Affected[i][st.top().first] = ETileType::TILE_0;

                for (k = N - 1; Field[i][k] != ETileType::TILE_0; --k);
                Field[i][k] = static_cast<ETileType>(static_cast<int>(p1.second) + 1);

                if (k == p1.first) {
                    Affected[i][k] = p1.second;
                }

                Changes.emplace_back(i, p1.first, k - p1.first, key, prev1);
                Changes.emplace_back(i, st.top().first, k - st.top().first, key, prev2);

                --OccupiedCounter;*/
                st.pop();
                fieldChanged = true;

            } else {

                for (k = N - 1; Field[i][k] != ETileType::TILE_0 && k >= 0; --k);

                if (p1.first < k) {
                    fieldChanged = true;
                    Changes.emplace_back(i, p1.first, k - p1.first, key, Field[i][p1.first]);

                    Field[i][p1.first] = Affected[i][p1.first] = ETileType::TILE_0;
                    Field[i][k] = p1.second;
                }
            }
        }
    }

    if (fieldChanged) {
        GenNewCell();
    }

    if (key == EKey::KEY_LEFT || key == EKey::KEY_UP) {
        FlipHorizontal();
    }

    if (key == EKey::KEY_UP || key == EKey::KEY_DOWN) {
        FlipDiag();
    }
}

TGameLogic::TGameLogic() : Affected(N, std::vector<ETileType>(N, ETileType::TILE_0)), Field(N, std::vector<ETileType>(N, ETileType::TILE_0)) {

    srand(time(nullptr));
    OccupiedCounter = 0;

    GenNewCell(), GenNewCell();
}

void TGameLogic::GenNewCell() {
    unsigned idx = rand() % (N * N - OccupiedCounter);
    ETileType tileToInsert = (rand() % 10) == 9 ? ETileType::TILE_1 : ETileType::TILE_2;
    int k = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (Field[i][j] == ETileType::TILE_0) {
                if (k == idx) {
                    Field[i][j] = tileToInsert;
                    ++OccupiedCounter;
                    return;
                }
                ++k;
            }
        }
    }
}

const std::vector<TGameLogic::TChange>& TGameLogic::GetChanges() const {
    return Changes;
}

ETileType TGameLogic::GetTile(const int i, const int j) const {
    return Field[i][j];
}

void TGameLogic::UnionTiles(int row, int leftCol, int rightCol, ETileType oldTile, EKey key) {
    // i -> row, p1.first -> rightCol, st.top().first = leftCol,
    int k;
    ETileType prev1 = Field[row][rightCol], prev2 = Field[row][leftCol];
    Field[row][rightCol] = Field[row][leftCol] = ETileType::TILE_0;

    Affected[row][rightCol] = Affected[row][leftCol] = ETileType::TILE_0;

    for (k = N - 1; Field[row][k] != ETileType::TILE_0; --k);
    Field[row][k] = static_cast<ETileType>(static_cast<int>(oldTile) + 1);

    if (k == rightCol) {
        Affected[row][k] = oldTile;
    }

    Changes.emplace_back(row, rightCol, k - rightCol, key, prev1);
    Changes.emplace_back(row, leftCol, k - leftCol, key, prev2);

    --OccupiedCounter;
}
