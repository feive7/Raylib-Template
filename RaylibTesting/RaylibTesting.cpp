#include <iostream>
#include <raylib.h>
int main() {
	// Define window dimensions
	const int window_width = 800;
	const int window_height = 450;

	// Create window
	InitWindow(window_width, window_height, "Raylib");
	SetTargetFPS(60);

	//Main loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Hello World!", 5, 5, 20, BLACK);
		EndDrawing();
	}

	// Destroy window and gl context
	CloseWindow();
}