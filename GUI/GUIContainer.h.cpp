#include <iostream>
#include <string>
#include <vector>
#include <Utilities.h>
#include "GUIContainer.h"
using namespace std;

GUIContainer::GUIContainer(string name)
	: Object(name, Vector2D(0, 0), Vector2D(DEFAULT_RESOLUTION.first, DEFAULT_RESOLUTION.second)) {}

GUIContainer::GUIContainer(string name, Vector2D position, Vector2D size, Color backgroundColor)
	: Object(name, position, size){
	this->backgroundColor = backgroundColor;
}

GUIContainer::GUIContainer(const GUIContainer& guiContainer)
	: Object(guiContainer) {
	this->backgroundColor = guiContainer.backgroundColor;
	TextureHandler::CopyTexture(this->backgroundTexture, guiContainer.backgroundTexture);
}

GUIContainer::~GUIContainer() {
	TextureHandler::ReleaseTexture(this->backgroundTexture);
}

Color GUIContainer::getBackgroundColor() const { return backgroundColor; }
Color GUIContainer::getColorTint() const { return colorTint; }
Texture2D* GUIContainer::getBackgroundTexture() const { return backgroundTexture; }

void GUIContainer::setBackgroundTexture(Texture2D* backgroundTexture) {
	this->backgroundTexture = TextureHandler::CloneTexture(backgroundTexture);
}
void GUIContainer::setBackgroundTexture(const string& filepath) {
	this->backgroundTexture = TextureHandler::AddTexture(filepath);
}

void GUIContainer::setBackgroundColor(Color backgroundColor) { this->backgroundColor = backgroundColor; }
void GUIContainer::setBackgroundColor(int a, int b, int c, int alph) {
	this->backgroundColor = Color(a, b, c, alph);
}

void GUIContainer::setColorTint(Color colorTint) { this->colorTint = colorTint; }

//void GUIContainer::Destroy() { delete this; }

void GUIContainer::UpdateSecluded() {}

void GUIContainer::Update() {
	if (!isActive || stop_update) return;
	UpdateSecluded();
	Object::Update();
}

void GUIContainer::DrawSecluded() {
	float scale = Game::Main->GetScaleFactor();
	Rectangle dest;
	dest.x = position[0] * scale; dest.y = position[1] * scale; dest.width = size[0] * scale; dest.height = size[1] * scale;
	DrawRectangleRec(dest, backgroundColor);

	if (!IsTextureValid(backgroundTexture)) return;
	Rectangle src;
	src.y = 0; src.height = backgroundTexture->height;
	src.x = 0; src.width = backgroundTexture->width;
	DrawTexturePro(*backgroundTexture, src, dest, Vector2(0, 0), 0, colorTint);
}
void GUIContainer::Draw() {
	if (!isActive) return;
	DrawSecluded();
	Object::Draw();
}


