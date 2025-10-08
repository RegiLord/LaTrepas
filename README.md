# C++ Game Engine
&nbsp;&nbsp;&nbsp;This started as a game for a faculty project in an Object Oriented Course in c++, but ended up becoming much larger. Although the game was never finished I managed to implement my own mini game engine. I'll talk about some features and things I would've done differently or things I didn't have time to do.

## Specifications
  - This project was made on a Mac with a M1 processor
  - It was made in CLion
  - It uitlizez the raylib library for the graphic interface
## The Structure of the Project
&nbsp;&nbsp;&nbsp;Every texture, image or thing you will see on the screen will be part of the Object class.
  <details>
    <summary> class Object; </summary>
  I will only copy the attributes, you can see the methods in the Object.h file in the Utilities folder.

```
class Object {
protected:
    std::string name;
    Vector2D position;
    Vector2D size;
    bool isActive = true;
    bool stop_update = false;

    bool to_destroy = false;
    Tween* active_tween = nullptr;

    int zIndex = 0;
    const int ID; // unique id for objects
    Object* parent;
    std::vector<Object*> children;
    std::unordered_map<std::string, float> attributes;
public:
    static Object workspace;
    static Object nullObject;
    static int ObjectCounter;
};
```
</details>
The Object class also has the following virtual functions.

```
  virtual void UpdateSecluded();
  virtual void Update();
  virtual void DrawSecluded(); 
  virtual void Draw();
```
&nbsp;&nbsp;&nbsp;The objects are represented in a hierarchy each object havin a parent and 0 or more children objects. The *Update method* is called every frame on the *static Object workshop* (see Object class atribbutes) and after that it propagates through the hierarchy, calling update for each of it's children and then they and so on. After update the *Draw method* is called the same way.\
&nbsp;&nbsp;&nbsp;The update method deals with **position changes, size changes, input etc.** while the draw method **draws the object on the graphic interface**, whether it be an object or simply a colored rectangle.\
&nbsp;&nbsp;&nbsp;The *DrawSecluded* and *UpdateSecluded* work the same way as the Update and Draw except they no longer propagate through the hierarchy, that means that no update is called on the children of the object.

## Objects
  To add an object to the workflow we must firs make it a child of the *workspace* object. The Object is just the base there are many derivations from it that show their usefulness:
  - **GUIObject** it's a simple object that has a rectangle shape, with the option to choose a background-color. You can also change the roundness to make it an oval or even a square
  - **GUIContainer** it is very similar to a GUIObject except they usually cover the entire screen and act as a container for the GUIObjects. They can also have an image background
  - **ImageLabel** it derives from the GUIObject, as the name suggests it has an image label. More advanced the ImageLabel also contains an *AnimationHandler* (a class created in the *Utilities* folder), to which you can add animations to the *Label*. When you press play when running the project you will see a character that utilizez this AnimationHandler
  - **TextLabel** used for inserting text, the text will be automatically reformated to fit inside the size, if words exceed the box they will appear on the next line (the text can go lower than the size). The font size, font, font color etc. can be changed.
  - **Button** the Button is derived from the *ImageLabel*. In it's updated method it has a detection for when it's pressed. It also has two extra labels, named *onHoverTexture*, *onPressTexture*, the button label chaging depending on whether you're pressing or hovering the button (there is an auto conversion integrated using *color tint* so there is no need to add the *onHoverTexture* and *onPressTexture* except if you want something differetn).
  - **CheckBox** it's derived from the *ImageLabel*, it's a simple GUIObject that shows a cross or check on whether you agree or not (changes on click), used primarily in the settings menu.
  - **Slider** as the CheckBox it is primarily used in the settings menu, it's used to select a value between a min and max.
  - **OptionBox** the definitions is in fact *OptionBox<T>* with template T. The *OptionBox* is a very specific object, upon clicking it opens a multiple choice menu, showing multiple options defined by strings. The *OptionBox* returns a parameter of type T, the parameter being attributed to the selected Option.

Game Objects:
  - **Player** the player is an object that can be controlled using the w,a,s and d keys that will move and play animations depending on the direction of movement.
  - **QuickTime** the *QuickTime* is the base of the combat I imagined (unfortunately it's not finished and only a very minimal beta can be played). The ideea of a *QuickTime* is that you must press the letter showed in time or else you will get damaged.

&nbsp;&nbsp;&nbsp;That's about it for the Objects section, later on I will talk about what I would've changed about this sytem. Although it did the job I think it could have been better optimised. Also I forgot to add but the **Button** object has a vector of  
## Services

&nbsp;&nbsp;&nbsp;I use the services for necessary things, such as texture optimization, animation playing and moving objects per frame, that I frequently use throughout the project.
&nbsp;&nbsp;&nbsp;I currently have 3 services:
  - Texture Handler
  - Text Handler
  - Animation Handler
  - Tween Service
I'll talk a bit about each one.

### Texture Handler
<details>
  <summary> Texture Handler Code Declarations </summary>

```
    struct TextureHandlerCell {
      Texture2D texture;
      std::string filepath;
      int count;
    };

    namespace TextureHandler {
      extern std::list<TextureHandlerCell> Handler;
      void CleanTextureCache();
  
      Texture2D* CloneTexture(Texture2D* texture);
      Texture2D* AddTexture(const std::string &filepath);
  
      void ReleaseTexture(const std::string &filepath);
      void ReleaseTexture(Texture2D*& texture);
  
      void DeleteTexture(Texture2D *texture);
      void DeleteTexture(const std::string &filepath);
  
      void CopyTexture(Texture2D* &texture, Texture2D* texturetocopy);
}
```
</details>
&nbsp;&nbsp;&nbsp; I implemented the Texture Handler as an optimization when using the same texture multiple times, before I simply loaded the textures I needed when creating ImageLabels, Button etc. and ended up with multiple VRAM occupation of the same texture, for this I created the Texture Handler.\
&nbsp;&nbsp;&nbsp; The ideea is that we load the texture one time, and send out pointers to that same texture. We hold these textures in the TextureHandlerCell and everytime we delete an ImageLabel we use the ReleaseTexture function, which finds the correct TextureHandlerCell and decreases the count. When the count reaches 0 we delete the texture from vram.\

> [!CAUTION]
> The TextureHandler is not intended to be used by the programmer. They are a system optimization.
### Text Handler
<details>
  <summary> Text Handler Code Declarations </summary>
```
enum Alignment {
  Centered, Left, Right, NoAlignment
};

inline namespace TextHandler {
  Vector2D GetTextBoxSize(const std::string &Text, Font* font, float fontsize = 20, float spacing = 2);
  void DefFontFormatString(std::string &Text, Vector2D Box, Font* font, int fontsize = 20, int spacing = 2);
  void AlignCenterText(Rectangle Box, Vector2D& TextBoxPosition, Vector2D TextBoxSize);
  void AlignRightText(Rectangle Box, Vector2D& TextBoxPosition, Vector2D TextBoxSize);
  void AlignLeftText(Rectangle Box, Vector2D& TextBoxPosition, Vector2D TextBoxSize);
}
```
</details>
  
