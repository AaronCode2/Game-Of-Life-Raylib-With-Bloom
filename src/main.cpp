#include <raylib.h>
#include <iostream>
#include "simulation.hpp"
#include "renderer/renderer.hpp"
#include "utility/assets.hpp"

int main() {

    Color GREY = {29, 29, 29, 255};
    int WINDOW_WIDTH = 750;
    int WINDOW_HEIGHT = 750;
    const int CELL_SIZE = 25;
    int FPS = 12;

    RendererDebugger::loadRenderDoc();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of Life");

    MaximizeWindow();

    WINDOW_HEIGHT = GetScreenHeight();
    WINDOW_WIDTH = GetScreenWidth();

    Shaders::compileShaders();

    Renderer renderer;

    renderer.setCurrentShader(ShaderIndex::BLOOM_FŬSI);

    SetTargetFPS(FPS);
    Simulation simulation{WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE};
    //Simulation Loop
    while(WindowShouldClose() == false) 
    {
        // 1. Event Handling

        if(IsKeyDown(KEY_T))
            Utils::bloomAdjust += 1.00f;
        if(IsKeyDown(KEY_G))
            Utils::bloomAdjust -= 1.00f;

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

            Vector2 mousePosition = GetMousePosition();
            int row = mousePosition.y / CELL_SIZE;
            int column = mousePosition.x / CELL_SIZE;
            simulation.ToggleCell(row, column, 1);
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {

            Vector2 mousePosition = GetMousePosition();
            int row = mousePosition.y / CELL_SIZE;
            int column = mousePosition.x / CELL_SIZE;
            simulation.ToggleCell(row, column, 0);
        }
        else if(IsKeyPressed(KEY_SPACE)){

            simulation.run = !simulation.run;
            
            if(simulation.run) {

                simulation.Start();
                SetWindowTitle("Game of Life has started.");
            } else {

                simulation.Stop();
                SetWindowTitle("Game of Life has stopped.");
            }
        }
        else if(IsKeyPressed(KEY_F))
        {
            FPS += 2;
            SetTargetFPS(FPS);
        }
        else if(IsKeyPressed(KEY_S))
        {
            if(FPS > 5)
            {
                FPS -= 2;
                SetTargetFPS(FPS);
            }
        }
        else if(IsKeyPressed(KEY_R))
        {
            simulation.CreateRandomState();
        }
        else if(IsKeyPressed(KEY_C))
        {
            simulation.ClearGrid();
        }

        // 2. Updating State
        simulation.Update(); 
        
        renderer.beginTargetRender();
        {
            ClearBackground(BLACK);
            simulation.Draw();
        }
        renderer.endTargetRender();

        renderer.applyShader();

        BeginDrawing();
        {
            ClearBackground(GREY);

            BeginBlendMode(BLEND_ADDITIVE);
            {
                renderer.drawResult();
            }
            EndBlendMode();
        }
        EndDrawing();
    }

    CloseWindow();
}