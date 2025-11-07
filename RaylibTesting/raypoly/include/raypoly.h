#pragma once
#include <vector>
struct Polygon {
	std::vector<Vector2> points;

	Vector2 center = { 0.0f,0.0f };
	float rotation = 0.0f;

	Vector2& operator[](int i) {
		return points[i];
	}
	void push_back(Vector2 point) {
		points.push_back(point);
	}
	void pop_back() {
		points.pop_back();
	}
	size_t size() {
		return points.size();
	}
};
using Polygons = std::vector<Polygon>;

Polygon PolygonScale(Polygon poly, float scale) {
	for (size_t i = 0; i < poly.size(); i++) {
		poly[i] = Vector2Scale(poly[i], scale);
	}
	return poly;
}

Polygon CreateRectanglePolygon(Rectangle rect) {
	Polygon rectPoly = {
		{
			{rect.width, rect.height},
			{rect.width, 0},
			{0, 0},
			{0, rect.height}
		},
		{
			rect.x,
			rect.y,
		}
	};
	return rectPoly;
}
Polygon CreateRegularPolygon(int n_sides, float radius = 1.0f) {
	Polygon output;
	for (size_t i = 0; i < n_sides; i++) {
		float t = 2 * PI * i / (float)n_sides;
		Vector2 point = {
			-cos(t) * radius,
			sin(t) * radius
		};
		output.push_back(point);
	}
	return output;
}

void DrawPolygonLines(Polygon poly, Color color, float line_thickness = 1.0f) {
	for (size_t i = 0; i < poly.size(); i++) {
		Vector2 A = Vector2Rotate(poly[i], poly.rotation) + poly.center;
		Vector2 B = Vector2Rotate(poly[(i + 1) % poly.size()], poly.rotation) + poly.center;
		DrawLineEx(A, B, line_thickness, RED);
	}
}
void DrawPolygon(Polygon poly, Color color) { 
	// Basic filled polygon function.
	// Does not work correctly if:
	// - center of polygon is not inside points
	// - polygon is concave
	for (size_t i = 0; i < poly.size(); i++) {
		Vector2 A = Vector2Rotate(poly[i], poly.rotation) + poly.center;
		Vector2 B = Vector2Rotate(poly[(i + 1) % poly.size()], poly.rotation) + poly.center;
		DrawTriangle(poly.center, A, B, color);
		DrawTriangle(poly.center, B, A, color);
	}
}