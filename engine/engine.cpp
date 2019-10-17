//
// Created by aboo on 14.10.2019.
//

#include "engine.h"
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
void TGameEngine::Show() {
    static EKey prev = EKey::KEY_DUMMY;
    while (!Display.Closed()) {

        Display.ProcessEvents();
        DrawTiles();
        EKey key = KeyPressed();
        if (key == EKey::KEY_DUMMY || !Display.IsKeyPressed(prev)) {
            Logic.Move(key);
            prev = key;
            RenderAll(Logic.GetChanges());
        }
        DrawTiles();
        Display.Render();
    }

}

void TGameEngine::DrawTiles() {
    for (int i = 0; i < TGameLogic::N; ++i) {
        for (int j = 0; j < TGameLogic::N; ++j) {
            if (Logic.GetTile(i, j) != ETileType::TILE_0) {
                Display.DrawTile(i, j, Logic.GetTile(i, j));
            }
        }
    }
}

void TGameEngine::DrawAff() {
    for (int i = 0; i < TGameLogic::N; ++i) {
        for (int j = 0; j < TGameLogic::N; ++j) {
            if (Logic.Affected[i][j] != ETileType::TILE_0) {
                Display.DrawTile(i, j, Logic.Affected[i][j]);
            }
        }
    }
}


EKey TGameEngine::KeyPressed() {

    if (Display.IsKeyPressed(EKey::KEY_UP)) {
        return EKey::KEY_UP;
    } else if (Display.IsKeyPressed(EKey::KEY_DOWN)) {
        return EKey::KEY_DOWN;
    } else if (Display.IsKeyPressed(EKey::KEY_LEFT)) {
        return EKey::KEY_LEFT;
    } else if (Display.IsKeyPressed(EKey::KEY_RIGHT)) {
        return EKey::KEY_RIGHT;
    }
    return EKey::KEY_DUMMY;
}

void TGameEngine::DrawChange(TGameLogic::TChange change, double diff) {

    double TIME = 0.1;
    switch (change.key) {
        case EKey::KEY_RIGHT :
            Display.DrawTile(change.i, change.j + diff / TIME * change.len, change.tile);
            break;
        case EKey::KEY_LEFT :
            Display.DrawTile(change.i, change.j - diff / TIME * change.len, change.tile);
            break;
        case EKey::KEY_UP :
            Display.DrawTile(change.i - diff / TIME * change.len, change.j, change.tile);
            break;
        case EKey::KEY_DOWN :
            Display.DrawTile(change.i + diff / TIME * change.len, change.j, change.tile);
    }
}

void TGameEngine::RenderAll(const std::vector<TGameLogic::TChange>& v) {

    const double TIME = 0.1;
    if (v.empty()) {
        return;
    }

    double init = Display.GetTime(), diff;

    while ((diff = Display.GetTime() - init) < TIME) {
        for (auto &change : v) {
            if (!change.len) {
                continue;
            }
            DrawChange(change, diff);
        }
        DrawAff();
        Display.Render();
        //Display.Render();
    }

    DrawTiles();
    Display.Render();



/*    for (auto &change : v) {
        if (change.len == 0) {
            continue;
        }
        double init = Display.GetTime();
        while (!Display.Closed()) {
            double diff = Display.GetTime() - init;
            if (diff > TIME) {
                break;
            }
            switch (change.key) {
                case EKey::KEY_RIGHT :
                    Display.DrawTile(change.i, change.j + diff / TIME * change.len, change.tile);
                    break;
                case EKey::KEY_LEFT :
                    Display.DrawTile(change.i, change.j - diff / TIME * change.len, change.tile);
                    break;
                case EKey::KEY_UP :
                    Display.DrawTile(change.i - diff / TIME * change.len, change.j, change.tile);
                    break;
                case EKey::KEY_DOWN :
                    Display.DrawTile(change.i + diff / TIME * change.len, change.j, change.tile);

            }

            DrawAff();
            Display.Render();
        }
    }
    DrawTiles();
    Display.Render();*/
}
