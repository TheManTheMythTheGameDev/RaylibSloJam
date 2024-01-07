/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 
#include <iostream>

#include "Scene.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "GoalComponent.h"
#include "PhysicsHandler.h"
#include "ControllerComponent.h"
#include "DestructibleWallComponent.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE, 
    SCREEN_GAMEPLAY, 
    SCREEN_ENDING
} GameScreen;

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;

static RenderTexture2D target = { 0 };  // Render texture to render our game

// TODO: Define global variables here, recommended to make them static

static Scene sampleScene;
static Texture2D defaultTexture;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib gamejam template");
    
    // TODO: Load resources / Initialize variables at this point
    sampleScene = Scene();
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    constexpr int imageSize = 500;
    Image circleImage = GenImageColor(imageSize, imageSize, Color{ 1, 1, 1, 0 });
    ImageDrawCircle(&circleImage, imageSize / 2, imageSize / 2, imageSize / 2, WHITE);
    defaultTexture = /*LoadTexture("src/vecteezy_white-circle-png_21115771_475.png")*/ LoadTextureFromImage(circleImage);
    UnloadImage(circleImage);
    static Texture2D wallTexture = LoadTexture("src/Solid_white_bordered.png");
    static Texture2D endScreenTexture = LoadTexture("src/EndScreen.png");

    ControllerComponent* playerController =  new ControllerComponent();
    Entity examplePlayer = Entity(Vector2{ 100.0f, 100.0f }, 0.0f, 0, new GraphicsComponent(defaultTexture, 30.0f, 30.0f), new PhysicsComponent(sampleScene.physicsHandler, Shape(Shape::ShapeType::Circle, Shape::ShapeData{ 30.0f }), true), playerController);
    Entity exampleGolfBall = Entity(Vector2{ 500.0f, 300.0f }, 0.0f, 1, new GraphicsComponent(defaultTexture, 30.0f, 30.0f), new PhysicsComponent(sampleScene.physicsHandler, Shape(Shape::ShapeType::Circle, Shape::ShapeData{ 30.0f }), true));
    Entity exampleGoal = Entity(Vector2{ 1180.0f, 620.0f }, 0.0f, 0, new GraphicsComponent(defaultTexture, 100.0f, 100.0f), new PhysicsComponent(sampleScene.physicsHandler, Shape(Shape::ShapeType::Circle, Shape::ShapeData{ 100.0f })), new GoalComponent(1, playerController, &sampleScene, endScreenTexture));
    Shape::ShapeData rectangleDimensions = Shape::ShapeData{ 0.0f };
    rectangleDimensions.rectangleData = Shape::RectangleDimensions{ 30.0f, 400.0f };
    Entity exampleWall = Entity(Vector2{400.0f, 200.0f}, PI / 4, 0,  new GraphicsComponent(wallTexture, 15.0f, 200.0f), new PhysicsComponent(sampleScene.physicsHandler, Shape(Shape::ShapeType::Rectangle, rectangleDimensions)));
    Entity exampleDestructibleWall = Entity(Vector2{ 200.0f, 500.0f }, 0.0f, 0, new DestructibleWallComponent(sampleScene, defaultTexture, Vector2{ 100.0f, 150.0f }), new GraphicsComponent(wallTexture, 100.0f, 150.0f));
    sampleScene.AddEntity(examplePlayer);
    sampleScene.AddEntity(exampleGolfBall);
    sampleScene.AddEntity(exampleWall);
    sampleScene.AddEntity(exampleGoal);
    sampleScene.AddEntity(exampleDestructibleWall);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    UnloadTexture(defaultTexture);
    UnloadTexture(wallTexture);
    UnloadTexture(endScreenTexture);
    
    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Entity newEntity = Entity(GetMousePosition(), 0.0f, 0, new GraphicsComponent(defaultTexture), new PhysicsComponent(sampleScene.physicsHandler, Shape(Shape::ShapeType::Circle, Shape::ShapeData{ (20.0f) }), 1.0f));
        sampleScene.AddEntity(newEntity);
    }

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture, 
    // it could be useful for scaling or further sahder postprocessing
    BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        
        // TODO: Draw your game screen here
        DrawRectangle(10, 10, screenWidth - 20, screenHeight - 20, SKYBLUE);
        
    EndTextureMode();
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, Rectangle{ 0, 0, (float)target.texture.width, -(float)target.texture.height }, Rectangle{ 0, 0, (float)target.texture.width, (float)target.texture.height }, Vector2{ 0, 0 }, 0.0f, WHITE);

        // TODO: Draw everything that requires to be drawn at this point, maybe UI?
        sampleScene.Update();

    EndDrawing();
    //----------------------------------------------------------------------------------  
}
