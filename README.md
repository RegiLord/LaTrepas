# C++ Game Engine
:This started as a game for a faculty project in an Object Oriented Course in c++, but ended up becoming much larger. Although the game was never finished I managed to implement my own mini game engine. I'll talk about some features and things I would've done differently or things I didn't have time to do.

## Specifications
  - This project was made on a Mac with a M1 processor
  - It was made in CLion
  - It uitlizez the raylib library for the graphic interface

## The Structure of the Project
  Every texture, image or thing you will see on the screen will be part of the Object class.
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
  The objects are represented in a hierarchy each object havin a parent and 0 or more children objects. The update method is called every frame on the *static Object workshop* (see Object class atribbutes) and after that it propagates through the hierarchy, calling update for each of it's children and then they and so on. After update the Draw method is called the same way.\
  The update method deals with position changes, size changes, input etc. while the draw method draws the object on the graphic interface, wheter it be an object or simply a colored rectangle.\
  The DrawSecluded and UpdateSecluded work the same way as the Update and Draw except they no longer propagate through the hierarchy, that means that no update is called on the children of the object.
