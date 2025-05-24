#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <Utilities.h>


#include "GUIObject.h"
using namespace std;

/*----------------------------------------------------------------------------------------------------------*/
/*
 * GUIObject Definitions
 */
/*----------------------------------------------------------------------------------------------------------*/

GUIObject::GUIObject() {
    roundness = 0;
    backgroundColor = BLANK;
}

GUIObject::GUIObject(const string& name, Vector2D position, Vector2D size, Color backgroundColor) :
        Object(name, position, size)
{ this->backgroundColor = backgroundColor; roundness = 0; }

GUIObject::GUIObject(const GUIObject &object) : Object(object) {
    this->roundness = object.roundness;
    this->backgroundColor = object.backgroundColor;
}

GUIObject::~GUIObject() = default;

Color GUIObject::getBackgroundColor() const { return backgroundColor; }
float GUIObject::getRoundness() const { return roundness; }

void GUIObject::setBackgroundColor(Color backgroundColor) { this->backgroundColor = backgroundColor; }
void GUIObject::setBackgroundColor(int r, int g, int b, int a) {
    this->backgroundColor = Color(r, g, b, a);
}

void GUIObject::setRoundness(float roundness) { this->roundness = roundness; }

GUIObject& GUIObject::operator=(GUIObject& object) { return object; }

void GUIObject::UpdateSecluded() {}
void GUIObject::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}
void GUIObject::DrawSecluded() {
    Rectangle dest;

    float scale = Game::Main->GetScaleFactor();

    dest.x = scale * position[0];
    dest.y = scale * position[1];
    dest.width = scale * size[0];
    dest.height = scale * size[1];

    DrawRectangleRounded(dest, this->roundness, 5.0f, backgroundColor);
}
void GUIObject::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

/*----------------------------------------------------------------------------------------------------------*/
/*
 * ImageLabel Definitions
 */
/*----------------------------------------------------------------------------------------------------------*/

class ImageTextureNull : public exception {
    public:
        void what() {
            cout << "Accesing texture variables when texture is nullptr";
        }
};

ImageLabel::ImageLabel(std::string name, Vector2D position, Vector2D size, Color backgroundColor)
    : GUIObject(name, position, size, backgroundColor), anim_handler(this, nullptr) {}

ImageLabel::ImageLabel(std::string name, Texture2D* texture, Vector2D position, Vector2D size, Color backgroundColor)
        : GUIObject(name, position, size, backgroundColor), anim_handler(this, texture)
{
    this->imageTexture = TextureHandler::CloneTexture(texture);
}

ImageLabel::ImageLabel(std::string name, std::string filepath, Vector2D position, Vector2D size, Color backgroundColor)
        : GUIObject(name, position, size, backgroundColor), anim_handler(this, filepath)
{
    this->imageTexture = TextureHandler::AddTexture(filepath);
}


ImageLabel::ImageLabel(const ImageLabel& label) : GUIObject(label), anim_handler(label.anim_handler) {
    TextureHandler::CopyTexture(this->imageTexture, label.imageTexture);
    this->colorTint = label.colorTint;
    this->transparency = label.transparency;
};

ImageLabel::~ImageLabel() {
    TextureHandler::ReleaseTexture(this->imageTexture);
}

float ImageLabel::getTransparency() const { return this->transparency; }
Texture2D* ImageLabel::getImageTexture() const { return this->imageTexture; }
Color ImageLabel::getColorTint() const { return this->colorTint; }
AnimationHandler &ImageLabel::getAnimationHandler() { return anim_handler; }


void ImageLabel::setColorTint(Color colorTint) { this->colorTint = colorTint; }
void ImageLabel::setColorTint(int r, int g, int b, int a) { this->colorTint = Color(r, g, b, a); }

void ImageLabel::setTransparency(float transparent) { this->transparency = transparent; }


void ImageLabel::setImageTexture(Texture2D* imageTexture) {
    TextureHandler::CopyTexture(this->imageTexture, imageTexture);
    anim_handler.SetBaseTexture(this->imageTexture);
}

void ImageLabel::setImageTexture(const string &filepath) {
    TextureHandler::ReleaseTexture(this->imageTexture);
    this->imageTexture = TextureHandler::AddTexture(filepath);
    anim_handler.SetBaseTexture(this->imageTexture);
}

// MAI TREBUIE GANDITI ASTIA
ImageLabel& ImageLabel::operator=(ImageLabel& label) { return label; }

void ImageLabel::UpdateSecluded() {
    GUIObject::UpdateSecluded();
    anim_handler.Update();
}

void ImageLabel::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

void ImageLabel::DrawSecluded() {
    GUIObject::DrawSecluded();

    if (!IsTextureValid(this->imageTexture)) {
        cout << "WARNING: Trying to draw invalid texture (ImageLabel): " << name << endl;
        return;
    }

    if (anim_handler.GetPlayingAnimationTexture() != nullptr) {
        int alphaValue = (1 - this->transparency) * 255;
        colorTint.a = alphaValue;
        anim_handler.Draw(position, size, colorTint);
    }
    else {
        float scale = Game::Main->GetScaleFactor();
        Rectangle dest;
        dest.x = scale * position[0];
        dest.y = scale * position[1];
        dest.width = scale * size[0];
        dest.height = scale * size[1];
        Rectangle src;
        src.y = 0; src.height = imageTexture->height;
        src.x = 0; src.width = imageTexture->width;
        int alphaValue = (1 - this->transparency) * 255;
        colorTint.a = alphaValue;
        DrawTexturePro(*imageTexture, src, dest, Vector2(0, 0), 0, colorTint);
    }
}

void ImageLabel::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

/*----------------------------------------------------------------------------------------------------------*/
/*
 * TextLabel Definitions
 */
/*----------------------------------------------------------------------------------------------------------*/

TextLabel::TextLabel(string name, string text, Vector2D position, Vector2D size) :
    GUIObject(name, position, size) {
    this->text = text;
}

TextLabel::TextLabel(const TextLabel &label) : GUIObject(label) {
    this->text = label.text;
    this->textChanged = label.textChanged;
    this->fontColor = label.fontColor;
    this->fontSize = label.fontSize;
    this->fontSpacing = label.fontSpacing;
    this->shrink_to_fit = label.shrink_to_fit;
    this->font = label.font;

};

TextLabel::~TextLabel() {}

string TextLabel::getText() const { return text; }
Font* TextLabel::getFont() const { return font; }
Color TextLabel::getFontColor() const { return fontColor; }
float TextLabel::getFontSize() const { return fontSize; }
float TextLabel::getFontSpacing() const { return fontSpacing; }


void TextLabel::setText(string text) { this->text = text; textChanged = true; }
void TextLabel::setFont(Font* font) { this->font = font; textChanged = true; }
void TextLabel::setFontColor(Color color) { this->fontColor = color; }
void TextLabel::setFontColor(int r, int g, int b, int alpha) { this->fontColor = Color(r, g, b, alpha); }
void TextLabel::setFontSpacing(float spacing) { this->fontSpacing = spacing; textChanged = true; }
void TextLabel::setFontSize(float fontSize) { this->fontSize = fontSize; fontSpacing = fontSize / 10; textChanged = true; }

void TextLabel::setSize(float x, float y) { this->setSize({x, y}); }
void TextLabel::setSize(Vector2D size) { Object::setSize(size); textChanged = true; }

TextLabel& TextLabel::operator=(TextLabel& label) { return label; }
void TextLabel::UpdateSecluded() {
    GUIObject::UpdateSecluded();

    float scale = Game::Main->GetScaleFactor();
    if (textChanged) {
        DefFontFormatString(text, size, font, fontSize, fontSpacing);
        size = GetTextBoxSize(text, font, fontSize, fontSpacing);
        textChanged = false;
    }
}

void TextLabel::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

void TextLabel::DrawSecluded() {
    GUIObject::DrawSecluded();
    float scale = Game::Main->GetScaleFactor();
    DrawTextEx(*font, text.c_str(),
        Vector2(scale * position[0], scale * position[1]),
        scale * fontSize,
        scale * fontSpacing,
        fontColor);
}
void TextLabel::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

/*----------------------------------------------------------------------------------------------------------*/
/*
 * Button Definitions
 */
/*----------------------------------------------------------------------------------------------------------*/

Button::Button() : ImageLabel("Button", Vector2D(0, 0), Vector2D(128, 64), WHITE) {
    roundness = 0.5f;
    normalColor = backgroundColor;

    if (text != nullptr)
        delete text;
    text = new ::TextLabel(name + "Text", "Click Me!");
}

Button::Button(string name, Vector2D position, Vector2D size, Color backgroundColor) :
    ImageLabel(name, position, size, backgroundColor) {
    roundness = 0.5f;
    normalColor = backgroundColor;

    if (text != nullptr)
        delete text;
    text = new ::TextLabel(name + "Text", "Click Me!");
}

Button::Button(const Button& object) : ImageLabel(object) {
    lastState = ButtonVoidState;
    alignment = object.alignment;

    normalColor = object.normalColor;
    TextureHandler::CopyTexture(normalTexture, object.normalTexture);

    onHoverColor = object.onHoverColor;
    TextureHandler::CopyTexture(onHoverTexture, object.onHoverTexture);

    onPressColor = object.onPressColor;
    TextureHandler::CopyTexture(onPressTexture, object.onPressTexture);

    pressCooldown = DEFAULT_BUTTON_PRESS_COOLDOWN;
    lastTimePressed = -1;
    canBePressed = true;

    buttonState = Normal;
    onClickCallbacks = vector<void(*)(Button*)> ();
    if (text != nullptr)
        delete text;
    text = new ::TextLabel(*object.text);

}

Button::~Button() {
    TextureHandler::ReleaseTexture(normalTexture);
    TextureHandler::ReleaseTexture(onHoverTexture);
    TextureHandler::ReleaseTexture(onPressTexture);
    if (text != nullptr)
        delete text;
    text = nullptr;
}

Alignment Button::getTextAlignment() const { return alignment; }
void Button::setTextAlignment(Alignment alignment) { this->alignment = alignment; }
ButtonState Button::getButtonState() const { return buttonState; }
void Button::setButtonState(ButtonState new_state) { buttonState = new_state; }
TextLabel* Button::TextLabel() { return text; }

float Button::getPressCooldown() const { return pressCooldown; }
bool Button::getCanBePressed() const { return canBePressed; }
vector<void(*)(Button*)>& Button::getOnClickCallbacks() { return onClickCallbacks; }

void Button::setHoverColor(Color color) { this->onHoverColor = color; }
void Button::setHoverColor(int r, int g, int b, int a) { this->onHoverColor = Color(r, g, b, a); }
void Button::setPressColor(Color color) { this->onPressColor = color; }
void Button::setPressColor(int r, int g, int b, int a) { this->onPressColor = Color(r, g, b, a); }
void Button::setPressCooldown(float cooldown) { this->pressCooldown = cooldown; }
void Button::setCanBePressed(bool canBePressed) { this->canBePressed = canBePressed; }
void Button::setPressTexture(Texture2D* texture) {
    TextureHandler::CopyTexture(normalTexture, texture);
}
void Button::setPressTexture(const string& filepath) {
    TextureHandler::ReleaseTexture(onPressTexture);
    onPressTexture = TextureHandler::AddTexture(filepath);
}
void Button::setHoverTexture(Texture2D* texture) {
    TextureHandler::CopyTexture(onHoverTexture, texture);
}
void Button::setHoverTexture(const string& filepath) {
    TextureHandler::ReleaseTexture(onHoverTexture);
    onHoverTexture = TextureHandler::AddTexture(filepath);
}

void Button::setNormalTexture(const string& filepath) {
    TextureHandler::ReleaseTexture(normalTexture);
    TextureHandler::ReleaseTexture(onHoverTexture);
    TextureHandler::ReleaseTexture(onPressTexture);
    normalTexture = TextureHandler::AddTexture(filepath);
    onHoverTexture = TextureHandler::CloneTexture(normalTexture);
    onPressTexture = TextureHandler::CloneTexture(normalTexture);
}
void Button::setNormalTexture(Texture2D* texture) {
    TextureHandler::ReleaseTexture(normalTexture);
    TextureHandler::ReleaseTexture(onHoverTexture);
    TextureHandler::ReleaseTexture(onPressTexture);
    this->normalTexture = TextureHandler::CloneTexture(texture);
    this->onHoverTexture = TextureHandler::CloneTexture(texture);
    this->onPressTexture = TextureHandler::CloneTexture(texture);
}

void Button::setNormalColor(Color color) {
    normalColor = color;
    onPressColor = ColorTint(normalColor, DEFAULT_PRESS_COLOR);
    onHoverColor = ColorTint(onHoverColor, DEFAULT_HOVER_COLOR);
}

void Button::setNormalColor(int r, int g, int b, int a) {
    setNormalColor(Color(r, g, b, a));
}

void Button::addClickEvent(void (*callback)(Button*)) { onClickCallbacks.push_back(callback); }
void Button::removeClickEvent(void (*callback)(Button*)) {
    for (int i = 0; i < onClickCallbacks.size(); i++) {
        if (onClickCallbacks[i] == callback) {
            onClickCallbacks.erase(onClickCallbacks.begin() + i);
        }
    }
}
void Button::clearEvents() { onClickCallbacks.clear(); }

void Button::setPosition(Vector2D position) {
    text->setPosition(position + (text->position - this->position));
    Object::setPosition(position);
}

void Button::setPosition(float x, float y) {
    this->setPosition(Vector2D(x, y));
}

Button& Button::operator=(Button& button) { return button; }
void Button::UpdateSecluded() {
    ImageLabel::UpdateSecluded();
    float scale = Game::Main->GetScaleFactor();
    if (text != nullptr)
        text->Update();
    Vector2 mousePosition(GetMouseX() / scale, GetMouseY() / scale);
    Rectangle buttonRec(position[0], position[1], size[0], size[1]);

    if (text != nullptr)
        switch (alignment) {
            case Centered: {
                AlignCenterText(buttonRec, text->position, text->size);
                break;
            }
            case Left: {
                AlignLeftText(buttonRec, text->position, text->size);
                break;
            }
            case Right: {
                AlignRightText(buttonRec, text->position, text->size);
                break;
            }
            default: break;
        }

    switch (buttonState) {
        case Normal: {
            if (buttonState != lastState) {
                backgroundColor = normalColor;
                this->setImageTexture(normalTexture);
                colorTint = normalColor;
                lastState = buttonState;
            }
            // if mouse on button -> Selected
            if (CheckCollisionPointRec(mousePosition, buttonRec))
                buttonState = Selected;

            break;
        }
        case Selected: {
            if (lastState != buttonState) {
                backgroundColor = onHoverColor;
                this->setImageTexture(onHoverTexture);
                colorTint = onHoverColor;
                lastState = buttonState;
            }
            // if mouse off button -> Normal
            if (!CheckCollisionPointRec(mousePosition, buttonRec)) {
                buttonState = Normal;
                break;
            }

            // if mouse clicked on button -> Clicked
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                buttonState = Clicked;

            break;
        }
        case Clicked: {
            if (buttonState != lastState) {
                backgroundColor = onPressColor;
                this->setImageTexture(onPressTexture);
                colorTint = onPressColor;
                lastState = buttonState;
            }
            // while mouse is clikced on button we keep press colors

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                // if mouse released on button -> Pressed
                // if mouse released off button -> Normal
                if (CheckCollisionPointRec(mousePosition, buttonRec) && canBePressed)
                    buttonState = Pressed;
                else
                    buttonState = Normal;

            }
            break;
        }
        case Pressed: {
            // intra direct in wait si asteapta press cooldown -> Wait
            if (buttonState != lastState) {
                backgroundColor = onPressColor;
                this->setImageTexture(onPressTexture);
                colorTint = onPressColor;
                lastState = buttonState;
            }
            lastTimePressed = 0;

            for (int i = 0; i < onClickCallbacks.size(); i++) {
                if (onClickCallbacks[i] != nullptr)
                    onClickCallbacks[i](this);
            }
            buttonState = Wait;
            break;
        }
        case Wait: {
            lastTimePressed += GetFrameTime();

            if (lastTimePressed > pressCooldown) {
                // dupa press cooldown
                lastTimePressed = -1;

                // intra in selected daca mouse-ul e pe button -> Selected
                if (CheckCollisionPointRec(mousePosition, buttonRec))
                    buttonState = Selected;
                else
                    buttonState = Normal;
            }
            break;
        }
        default: break;
    }
}

void Button::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

void Button::DrawSecluded() {
    if (imageTexture != nullptr) {
        backgroundColor = BLANK;
        ImageLabel::DrawSecluded();
    }
    else
        GUIObject::DrawSecluded();

    if (text != nullptr)
        text->Draw();
}
void Button::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

/*----------------------------------------------------------------------------------------------------------*/
/*
 * Slider Definitions
 */
/*----------------------------------------------------------------------------------------------------------*/

Slider::Slider(string name, double minValue, double maxValue, Vector2D position, Vector2D size, Color backgroundColor)
    : GUIObject(name, position, size, backgroundColor),
minValue(minValue), maxValue(maxValue), maxSize(size) {
    value = minValue + maxValue / 2;
    roundness = 1;
}

Slider::Slider(const Slider &object)
    : GUIObject(object) {
    this->minValue = object.minValue;
    this->maxValue = object.maxValue;
    this->value = object.value;
    this->maxSize = object.maxSize;
    this->startDragging = false;
}
Slider::~Slider() = default;

double Slider::getMinValue() const { return minValue; }
double Slider::getMaxValue() const { return maxValue; }
double Slider::getValue() const { return value; }
Vector2D Slider::getMaxSize() const { return maxSize; }

void Slider::setMinValue(double minValue) {
    this->minValue = minValue;
    if (this->minValue > maxValue) this->minValue = maxValue;
    if (this->minValue < minValue) this->minValue = minValue;
}
void Slider::setMaxValue(double maxValue) {
    this->maxValue = maxValue;
    if (this->maxValue < minValue) this->maxValue = minValue;
    if (this->value > maxValue) this->value = maxValue;
}
void Slider::setValue(double value) {
    this->value = value;
    if (this->value > maxValue) this->value = maxValue;
    if (this->value < minValue) this->value = minValue;
}
void Slider::setMaxSize(Vector2D maxSize) { this->maxSize = maxSize;}

Slider &Slider::operator=(Slider &slider) { return slider; }
void Slider::UpdateSecluded() {
    // aicea fac toate verificarile in legatura cu schimbare original size-ului si a value-ului
    // plus verificare de mouse down si location

    double percentage = (value - minValue) / (maxValue - minValue);
    Vector2D newSize(maxSize[0] * percentage, maxSize[1]);
    size = newSize;

    float scale = Game::Main->GetScaleFactor();
    Vector2D mousePosition(GetMouseX() / scale, GetMouseY() / scale);

    Vector2D circleCoord(position[0] + size[0], position[1] + size[1] / 2);
    float circleRadius = size[1] * 1.5;

    if (CheckCollisionPointRec(Vector2(mousePosition[0], mousePosition[1]),
        Rectangle(position[0], position[1], maxSize[0], maxSize[1]))
        ||
        CheckCollisionPointCircle({mousePosition[0], mousePosition[1]}, Vector2(circleCoord[0], circleCoord[1]), circleRadius)
        ) {

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            startDragging = true;
        }
    }

    if (startDragging) {
        size = Vector2D(mousePosition[0] - position[0], size[1]);
        if (size[0] > maxSize[0]) size[0] = maxSize[0];
        if (size[0] < 0) size[0] = 0;
        percentage = size[0] / maxSize[0];
        value = minValue + percentage * (maxValue - minValue);
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        startDragging = false;
}

void Slider::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

void Slider::DrawSecluded() {
    GUIObject::DrawSecluded();
    // desenarea borderului, a slider-ului
    // si la final a cercului
    float scale = Game::Main->GetScaleFactor();
    Vector2D circleCoord(position[0] + size[0], position[1] + size[1] / 2);
    circleCoord = circleCoord * scale;
    float circleRadius = size[1] * 1.5;
    float lineThickness = size[1] / 4;
    circleRadius = circleRadius * scale;
    lineThickness = lineThickness * scale;

    DrawRectangleRoundedLinesEx(Rectangle(position[0] * scale, position[1] * scale, maxSize[0] * scale, maxSize[1] * scale),
        roundness, 5.0f, lineThickness, BLACK);
    DrawCircle(circleCoord[0], circleCoord[1], circleRadius, ColorTint(backgroundColor, LIGHTGRAY));
}

void Slider::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

/*----------------------------------------------------------------------------------------------------------*/
/*
 * CheckBox Definitions
 */
/*----------------------------------------------------------------------------------------------------------*/

CheckBox::CheckBox(std::string name, Vector2D position, float size) :
    ImageLabel(name, "rsc/Cross.png", position, Vector2D(size, size), BLANK), checked(false) { }

CheckBox::CheckBox(const CheckBox &object)
    : ImageLabel(object) {
    this->setChecked(object.checked);
}
CheckBox::~CheckBox() = default;

bool CheckBox::getChecked() const { return checked; }
void CheckBox::setChecked(bool checked) { this->checked = checked; this->set_checked = true;}
void CheckBox::setSize(float size) { this->size = Vector2D(size, size); }

CheckBox &CheckBox::operator=(CheckBox &checkbox) { return checkbox; }
void CheckBox::UpdateSecluded() {

    Rectangle checkBoxCollision(position[0], position[1], size[0], size[1]);
    float scale = Game::Main->GetScaleFactor();
    Vector2D mouse_position = {GetMouseX() / scale, GetMouseY() / scale};

    if (CheckCollisionPointRec({mouse_position[0], mouse_position[1]}, checkBoxCollision) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (checked) {
            this->setImageTexture("rsc/Cross.png");
            checked = false;
        }
        else {
            this->setImageTexture("rsc/Check.png");
            checked = true;
        }
        set_checked = false;
    }

    if (set_checked) {
        if (checked)
            this->setImageTexture("rsc/Check.png");
        else
            this->setImageTexture("rsc/Cross.png");
        set_checked = false;
    }

}
void CheckBox::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

void CheckBox::DrawSecluded() {
    float scale = Game::Main->GetScaleFactor();
    float lineThickness = size[1] / 10;
    lineThickness *= scale;
    DrawRectangleLinesEx(Rectangle(position[0] * scale, position[1] * scale, size[0] * scale, size[1] * scale), lineThickness, BLACK);
    ImageLabel::DrawSecluded();
}

void CheckBox::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

Vector2D TextHandler::GetTextBoxSize(const std::string &text, TextLabel *textLabel) {
    return GetTextBoxSize(text, textLabel->getFont(), textLabel->getFontSize(), textLabel->getFontSpacing());
}





















