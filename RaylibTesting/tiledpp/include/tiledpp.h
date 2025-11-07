#pragma once
#include <fstream>
#include <json.hpp>
#include <variant>
using namespace nlohmann;
namespace Tiled {
	enum DrawOrder {
		TopDown,
		Manual,
	};
	enum Orientation {
		Orthogonal,
		Isometric,
		Isometric_Staggered,
		Hexagonal_Staggered,
	};
	enum TileRenderOrder {
		Right_Down,
		Right_Up,
		Left_Down,
		Left_Up,
	};
	struct Point {
		float x;
		float y;
	};
	struct Property {
		std::string name = "";
		std::string type = "";
		std::variant<int, float, bool, std::string> value;
	};
	using Properties = std::vector<Property>;
	using Polygon = std::vector<Point>;
	struct Object {
		float x;
		float y;
		float width;
		float height;
		float rotation;
		int id;
		std::string name;
		std::string type;
		bool visible;
		bool ellipse;
		Polygon polygon;
		Properties properties;
	};
	struct ObjectLayer {
		float x;
		float y;
		DrawOrder draw_order;
		int id;
		std::string name;
		float opacity;
		bool visible;
		std::vector<Object> objects;
	};
	struct Map {
		int compression_level;
		int width;
		int height;
		bool infinite;
		int next_layer_id;
		int next_object_id;
		Orientation orientation;
		TileRenderOrder tile_render_order;
		std::string tiled_version;
		int tile_width;
		int tile_height;
		//??? tilesets;
		std::string version;
		std::vector<ObjectLayer> object_layers;
	};
	Polygon ParsePolygon(json jsonObject) {
		Polygon output;
		for (size_t i = 0; i < jsonObject.size(); i++) {
			Point p = {
				jsonObject[i]["x"],
				jsonObject[i]["y"],
			};
			output.push_back(p);
		};
		return output;
	}
	Properties ParseProperties(json jsonObject) {
		Properties output;
		for (size_t i = 0; i < jsonObject.size(); i++) {
			Property prop;
			prop.name = jsonObject[i]["name"];
			prop.type = jsonObject[i]["type"];
			json value = jsonObject[i]["value"];
			if (value.is_boolean()) prop.value = value.get<bool>();
			else if (value.is_number_float()) prop.value = value.get<float>();
			else if (value.is_number_integer()) prop.value = value.get<int>();
			else if (value.is_string()) prop.value = value.get<std::string>();
		}
		return output;
	}
	Object ParseObject(json jsonObject) {
		Object output;
		output.height = jsonObject["height"];
		output.id = jsonObject["id"];
		output.name = jsonObject["name"];
		output.rotation = jsonObject["rotation"];
		output.type = jsonObject["type"];
		output.visible = jsonObject["visible"];
		output.width = jsonObject["width"];
		output.x = jsonObject["x"];
		output.y = jsonObject["y"];
		output.ellipse = (jsonObject.contains("ellipse") ? (bool)jsonObject["ellipse"] : false);
		output.polygon = ParsePolygon(jsonObject["polygon"]);
		output.properties = ParseProperties(jsonObject["properties"]);
		return output;
	}
	ObjectLayer ParseObjectLayer(json jsonObject) {
		ObjectLayer output;
		if (jsonObject["draworder"] == "topdown") output.draw_order = TopDown;
		else if (jsonObject["draworder"] == "index") output.draw_order = Manual;
		output.id = jsonObject["id"];
		output.name = jsonObject["name"];
		output.opacity = jsonObject["opacity"];
		output.visible = jsonObject["visible"];
		output.x = jsonObject["x"];
		output.y = jsonObject["y"];
		for (size_t i = 0; i < jsonObject["objects"].size(); i++) {
			output.objects.push_back(ParseObject(jsonObject["objects"][i]));
		}
		return output;
	}
	Map ParseMap(const char* filename) {
		Map output;

		std::ifstream stream(filename);
		json mapJson = json::parse(stream);

		output.compression_level = mapJson["compressionlevel"];
		output.height = mapJson["height"];
		output.infinite = mapJson["infinite"];
		output.next_layer_id = mapJson["nextlayerid"];
		output.next_object_id = mapJson["nextobjectid"];
		if (mapJson["orientation"] == "orthogonal") output.orientation = Orthogonal;
		if (mapJson["orientation"] == "isometric") output.orientation = Isometric;
		if (mapJson["orientation"] == "staggered") output.orientation = Isometric_Staggered;
		if (mapJson["orientation"] == "hexagonal") output.orientation = Hexagonal_Staggered;
		if (mapJson["renderorder"] == "right-down") output.tile_render_order = Right_Down;
		if (mapJson["renderorder"] == "right-up") output.tile_render_order = Right_Up;
		if (mapJson["renderorder"] == "left-down") output.tile_render_order = Left_Down;
		if (mapJson["renderorder"] == "left-up") output.tile_render_order = Left_Up;
		output.tiled_version = mapJson["tiledversion"];
		output.tile_height = mapJson["tileheight"];
		//output.tilesets = mapJson["tilsets"];
		output.tile_width = mapJson["tilewidth"];
		output.version = mapJson["version"];
		output.width = mapJson["width"];
		for (size_t i = 0; i < mapJson["layers"].size(); i++) {
			output.object_layers.push_back(ParseObjectLayer(mapJson["layers"][i]));
		}

		stream.close();

		return output;
	}
}