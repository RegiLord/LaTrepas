//
// Created by Leustean Stefan on 26.03.2025.
//

#ifndef GUIObject_H
#define GUIObject_H

#define DEFAULT_FONT LoadFont("rsc/fonts/alagard.png")
#define DEFAULT_FONT_SIZE 20
#define DEFAULT_FONT_SPACING 2
#define DEFAULT_FONT_COLOR BLACK
#define DEFAULT_TEXT_BOX_PADDING 2
#define DEFAULT_ANIMATION_SPEED 24
#define DEFAULT_TINT_COLOR Color(255, 255, 255)
#define DEFAULT_BACKGROUND_COLOR BLANK
#define DEFAULT_BUTTON_PRESS_COOLDOWN 0
#define DEFAULT_PRESS_COLOR Color(200, 200, 200, 255)
#define DEFAULT_HOVER_COLOR Color(230, 230, 230, 255)
#define FRAME_WAIT 10

// Momentan pastrez constructorii de copiere pe default, nu cred ca e nevoie sa ma complic momentan
// daca observ ca nu merge ceva am sa schimb

class GUIObject : public Object {
    protected:
        float roundness;
        Color backgroundColor;

    public:
        GUIObject();
        GUIObject(const std::string& name, Vector2D position = Vector2D(0, 0),
            Vector2D size = Vector2D(64, 64), Color backgroundColor = DEFAULT_BACKGROUND_COLOR);
        GUIObject(const GUIObject& object);
        ~GUIObject() override;

        float getRoundness() const;
        Color getBackgroundColor() const;

        void setRoundness(float roundness);
        void setBackgroundColor(Color backgroundColor);
        void setBackgroundColor(int r, int g, int b, int a = 255);

        GUIObject& operator=(GUIObject& object);

        //void Destroy() override;
        void UpdateSecluded() override;
        void Update() override;
        void DrawSecluded() override;
        void Draw() override;
};


class ImageLabel; // : virtual public GUIObject
class TextLabel; // : virtual public GUIObject {};
class Button; // : virtual public ImageLabel {};
class CheckBox; // : public GUIObject {};
class Slider; // : public GUIObject {};
template <class T> class OptionBox; // : public GUIObject {};

class ImageLabel : public GUIObject {
    protected:
        Texture2D* imageTexture = nullptr;
        Color colorTint = DEFAULT_TINT_COLOR;
        float transparency = 0; // valoare intre 0 si 1

        AnimationHandler anim_handler;
    public:
        ImageLabel(std::string name = "ImageLabel", Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(64, 64), Color backgroundColor = DEFAULT_BACKGROUND_COLOR);
        ImageLabel(std::string name, Texture2D* texture,
            Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(64, 64), Color backgroundColor = DEFAULT_BACKGROUND_COLOR);
        ImageLabel(std::string name, std::string filePath,
            Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(64, 64), Color backgroundColor = DEFAULT_BACKGROUND_COLOR);
        ImageLabel(const ImageLabel& label);
        ~ImageLabel() override;

        Color getColorTint() const;
        float getTransparency() const;
        Texture2D* getImageTexture() const;
        AnimationHandler& getAnimationHandler();

        void setImageTexture(Texture2D* imageTexture);
        void setImageTexture(const std::string &filepath);
        void setColorTint(Color color);
        void setColorTint(int r, int g, int b, int a = 255);
        void setTransparency(float transparency);

       // void Destroy() override
        ImageLabel& operator=(ImageLabel& label);
        void UpdateSecluded() override;
        void Update() override;
        void DrawSecluded() override;
        void Draw() override;
};

class TextLabel : public GUIObject {
    private:
        bool shrink_to_fit = true;
    protected:
        bool textChanged = true;
        Font* font = Game::Main->GetDefaultFont();
        std::string text; ; // pixels
        Color fontColor = DEFAULT_FONT_COLOR;
        float fontSize = DEFAULT_FONT_SIZE, fontSpacing = DEFAULT_FONT_SPACING;

    public:
        friend Button;
        TextLabel(std::string name = "TextLabel", std::string text = "Text",
            Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(64, 64));
        TextLabel(const TextLabel& label);
        ~TextLabel();

        std::string getText() const;
        Font* getFont() const;
        Color getFontColor() const;
        float getFontSpacing() const;
        float getFontSize() const;

        void setText(std::string text);
        void setFont(Font* font);
        void setFont(std::string filepath);
        void setFontSize(float fontSize); // !! Atentie setarea fontsize-ului schimba fontspacing la /10 din fontsize
        void setFontColor(Color color);
        void setFontColor(int r, int g, int b, int alpha = 255);
        void setFontSpacing(float fontSpacing);

        void setSize(Vector2D size) override;
        void setSize(float x, float y) override;
       // void Destroy() override;
        TextLabel& operator=(TextLabel& label);
        void UpdateSecluded() override;
        void Update() override;
        void DrawSecluded() override;
        void Draw() override;
};

enum ButtonState {
    ButtonVoidState, Normal, Selected, Clicked, Pressed, Wait
};

class Button : public ImageLabel {
    private:
        ButtonState lastState = ButtonVoidState;
    protected:

        TextLabel* text = nullptr;
        Alignment alignment = Centered;

        Texture2D* normalTexture = nullptr;
        Color normalColor;

        Texture2D* onHoverTexture = nullptr;
        Color onHoverColor = DEFAULT_HOVER_COLOR;

        Texture2D* onPressTexture = nullptr;
        Color onPressColor = DEFAULT_PRESS_COLOR;

        // momentan functioneaza doar cu maus
        // va trebui sa adaug si o optiune ptr tastatura
        // sa poti selecta cu tastatura
        // si sa poti da enter
        float pressCooldown = DEFAULT_BUTTON_PRESS_COOLDOWN;
        float lastTimePressed = -1;
        bool canBePressed = true;

        ButtonState buttonState = Normal;

        // momentan pentru ca nu sunt priceput las asa
        std::vector<void(*)(Button*)> onClickCallbacks;
    public:
        Button();
        Button(std::string name, Vector2D position = Vector2D(0, 0),
            Vector2D size = Vector2D(128, 64), Color backgroundColor = WHITE);
        Button(const Button& object);
        ~Button();

        Alignment getTextAlignment() const;
        void setTextAlignment(Alignment new_alignment);
        ButtonState getButtonState() const;
        void setButtonState(ButtonState new_state);
        TextLabel* TextLabel();

        float getPressCooldown() const;
        bool getCanBePressed() const;
        std::vector<void(*)(Button*)>& getOnClickCallbacks();

        void setPressCooldown(float cooldown);
        void setCanBePressed(bool canBePressed);

        void setPressColor(Color color);
        void setPressColor(int r, int g, int b, int a = 255);
        void setHoverColor(Color color);
        void setHoverColor(int r, int g, int b, int a = 255);
        void setNormalColor(Color color);
        void setNormalColor(int r, int g, int b, int a = 255);
        void setNormalTexture(Texture2D* texture);
        void setNormalTexture(const std::string& filepath);
        void setHoverTexture(Texture2D* texture);
        void setHoverTexture(const std::string& filepath);
        void setPressTexture(Texture2D* texture);
        void setPressTexture(const std::string& filepath);

        void addClickEvent(void(*callback)(Button*));
        void removeClickEvent(void(*callback)(Button*));
        void clearEvents();

        void setPosition(Vector2D position) override;
        void setPosition(float x, float y) override;

        Button& operator=(Button& button);
        void UpdateSecluded() override;
        void Update() override;
        void DrawSecluded() override;
        void Draw() override;
        // update si draw o sa fie tricky de facut, vreau la un momentan sa fac un input handler function
        // si cu aceea sa pot determina totul
        // momentan update-ul va verifica daca butonul este hoveruit / apasat
        // si va apela niste mouse controls
};

class Slider : public GUIObject {
 private:
    bool startDragging = false;
 protected:
    double minValue, maxValue, value;
    Vector2D maxSize;

 public:
    Slider(std::string name = "Slider", double minValue = 0, double maxValue = 100,
        Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(200, 10), Color backgroundColor = RED);
    Slider(const Slider& object);
    ~Slider();

    double getMinValue() const;
    double getMaxValue() const;
    double getValue() const;
    Vector2D getMaxSize() const;

    void setValue(double value);
    void setMinValue(double min);
    void setMaxValue(double max);
    void setMaxSize(Vector2D maxSize);

    Slider& operator=(Slider& slider);
    //Update va verifica daca mouse-ul e down in rectangle-ul generat de maxSize si acolo unde e va pozition slider-ul
    void UpdateSecluded() override;
    void Update() override;

    // DrawRectangleRoundedLinesEx -> ptr a face borderul pentru slider
    // roundness-ul va fi 1, iar marimea cercului de slider va fi double the height
    // Slider circle-ul va fi o culoare mai deschisa un pic decat restul
    // borderul for simplicity va avea direct culoarea negru
    void DrawSecluded() override;
    void Draw() override;
};

class CheckBox : public ImageLabel {
 private:
    bool set_checked = false;
 protected:
    bool checked;
 public:

    CheckBox(std::string name = "CheckBox", Vector2D position = Vector2D(0, 0), float size = 30);
    CheckBox(const CheckBox& object);
    ~CheckBox();

    bool getChecked() const;
    void setChecked(bool checked);
    void setSize(float size);

    CheckBox& operator=(CheckBox& checkbox);
    // schimbam image texture in functie de bool checkd, sunt standardizate deja texturil
    void UpdateSecluded() override;
    void Update() override;

    // in draw secluded facem doar un border de thicknexx o zecime din size
    void DrawSecluded() override;
    void Draw() override;
};

template<class T>
class OptionBox : public Button {
 private:
    // va trece prin toate optinuile si le va schimba size-ul font size padding si tot
    // in functie de size-ul OptionBox-ului
    void reconfigure();
    bool toReconfigure = true;
    int framewait = -1;
 protected:
    std::pair<std::string, T> currentValue;
    std::vector<T> values = std::vector<T>();
    std::vector<Button* > options = std::vector<Button*>();

    bool showOptions = false;
public:

    // De tinut minte ca size-ul afecteaza si restul butoanelor
    OptionBox(std::string name, std::vector<std::pair<std::string, T>> initvals,
        Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(100, 30), Color backgroundColor = BLACK);

    OptionBox(const OptionBox& object);
    ~OptionBox() override;


    //ShowButonList controls
    bool getShowOptions() const;
    void setShowOptions(bool showOptions);

    // Setting the main values
    void setCurrentValue(const std::string& name);
    void setCurrentValue(T value);
    T getValue() const;

    // Adding new options
    std::vector<Button*>& getOptions();
    std::vector<T>& getValues();
    void addOption(std::string name, T value);

    OptionBox& operator=(OptionBox& optionbox);
    // in update voi verifica daca difera size-ul obiectului de primu buton
    // sau fontsize-ul obiectului de primu buton
    // si daca da atunci va da reconfigure la lista de butoane

    void UpdateSecluded() override;
    void Update() override;
    void DrawSecluded() override;
    void Draw() override;
};

inline namespace TextHandler {
    Vector2D GetTextBoxSize(const std::string& text, TextLabel* textLabel);
}

#include "OptionBox.tpp"

#endif //GUIObject_H
