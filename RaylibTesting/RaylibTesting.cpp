#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <raypoly.h>
#include <viewport.h>
#include <tiledpp.h>
int main() {
	// Define window dimensions
	const int window_width = 800;
	const int window_height = 450;

	// Create window
	InitWindow(window_width, window_height, "Raylib");
	SetTargetFPS(60);

	// Init viewport
	Viewport viewport;
	viewport.offset = { 400,225 };
	viewport.rotation = 0.0f;
	viewport.target = { 0.0f,0.0f };
	viewport.zoom = 6.0f;

	// Create test pentagon
	Polygon pentagon = CreateRegularPolygon(5, 10.0f);
	pentagon.center = { 0,0 }; // Position in center of screen
	
	//Main loop
	while (!WindowShouldClose()) {
		viewport.update();
		Vector2 mpos = GetMousePosition();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(viewport);
		DrawPolygon(pentagon, RED);
		EndMode2D();
		EndDrawing();
	}

	// Destroy window and gl context
	CloseWindow();
}