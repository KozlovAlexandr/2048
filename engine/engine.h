//
// Created by aboo on 14.10.2019.
//

#ifndef SRC_ENGINE_H
#define SRC_ENGINE_H


#include "logic.h"

class TGameEngine {
public:
    void Show();
    void RenderAll(const std::vector<TGameLogic::TChange> &changes);
private:
    void DrawChange(TGameLogic::TChange change, double init);
    void DrawTiles();
    void DrawAff();
    TGameLogic Logic;
    TDisplay Display;
    EKey KeyPressed();
};


#endif //SRC_ENGINE_H
