# C++ Game Engine
  This started as a game for a faculty project in an Object Oriented Course in c++, but ended up becoming much larger. Although the game was never finished I managed to implement my own mini game engine. I'll talk about some features and things I would've done differently or things I didn't have time to do.

## Specifications
  - This project was made on a Mac with a M1 processor
  - It was made in CLion
  - It uitlizez the raylib library for the graphic interface

## The Base of the Project
  Every texture, image or thing you will see on the screen will be part of the Object class.
  <details>
    <summary> class Object; </summary>
  I will only copy the attributes, you can see the methods in the Object.h file in the Utilities folder.\

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
The Object class also has the following virtual functions.
```
  virtual void UpdateSecluded();
  virtual void Update();
  virtual void DrawSecluded(); 
  virtual void Draw();
```
