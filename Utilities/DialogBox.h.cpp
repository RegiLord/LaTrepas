#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "raylib.h"
#include "DialogBox.h"
using namespace std;

DialogBox* DialogBox::ActiveDialog = nullptr;

DialogBox::DialogBox() = default;
DialogBox::~DialogBox() {
    UnloadTexture(profilePic);
}

DialogBox::DialogBox(string name, Texture2D profilePic, vector<string> dialogLines) {
    this->name = name;
    this->profilePic = profilePic;
    this->dialogLines = dialogLines;
    line = column = 0;
    finished = false;
    // ptr usurinta o sa presupunem ca profilePic are 144x144 pixels
    // daca are witdh mai mare inseamna ca e animat
    animFrames = profilePic.width / 144;
    animFrame = 0;

    dest.x = 0;
    dest.y = 3 * GetScreenHeight() / 4;
    dest.width = GetScreenWidth();
    dest.height = GetScreenHeight() / 4;
}

DialogBox::DialogBox(string name, const char* profilePic, vector<string> dialogLines) {
    this->name = name;
    this->profilePic = LoadTexture(profilePic);
    this->dialogLines = dialogLines;
    line = column = 0;
    finished = false;
    // ptr usurinta o sa presupunem ca profilePic are 144x144 pixels
    // daca are witdh mai mare inseamna ca e animat
    animFrames = this->profilePic.width / 144;
    animFrame = 0;

    dest.x = 0;
    dest.y = 3 * GetScreenHeight() / 4;
    dest.width = GetScreenWidth();
    dest.height = GetScreenHeight() / 4;
}

DialogBox::DialogBox(const DialogBox& dialogBox) {
  this->name = dialogBox.name;
  this->profilePic = dialogBox.profilePic;
  this->dialogLines = dialogBox.dialogLines;
  this->animFrames = dialogBox.animFrames;
  this->dest = dialogBox.dest;

  this->finished = false;
  this->line = this->column = 0;
  this->animFrame = 0;
}

int DialogBox::getLine() const { return line; }
int DialogBox::getColumn() const { return column; }
void DialogBox::setLine(int line) { this->line = line; }
void DialogBox::setColumn(int column) { this->column = column; }

string DialogBox::getName() const { return name; }
vector<string> DialogBox::getDialogLines() const { return dialogLines; }

void DialogBox::setName(string name) { this->name = name; }
void DialogBox::setProfilePic(const char* filename) {
    this->profilePic = LoadTexture(filename);
    animFrames = profilePic.width / 144;
    animFrame = 0;
}
void DialogBox::setProfilePic(Texture2D profilePic) {
  this->profilePic = profilePic;
  animFrames = this->profilePic.width / 144;
}

DialogBox& DialogBox::operator=(const DialogBox& dialogBox) {
  this->name = dialogBox.name;
  this->profilePic = dialogBox.profilePic;
  this->dialogLines = dialogBox.dialogLines;
  line = column = 0;
  finished = false;

  // ptr usurinta o sa presupunem ca profilePic are 144x144 pixels
  // daca are witdh mai mare inseamna ca e animat
  animFrames = this->profilePic.width / 144;
  animFrame = 0;

  dest.x = 0;
  dest.y = 3 * GetScreenHeight() / 4;
  dest.width = GetScreenWidth();
  dest.height = GetScreenHeight() / 4;

  return *this;
}

DialogBox& DialogBox::operator+(string text) {
    dialogLines.push_back(text);
    return *this;
}

DialogBox& DialogBox::operator+(const char* text) {
  dialogLines.push_back(string(text));
  return *this;
}

string DialogBox::operator*(int scalar) const {
    if (scalar > dialogLines.size())
      scalar = dialogLines.size();

    string Text;
    for (int i = 0; i < scalar; i++) {
      Text += dialogLines[i];
      Text += "\n";
    }
    return Text;
}

bool DialogBox::operator==(const DialogBox& dialog) { return name == dialog.name; }
bool DialogBox::operator!=(const DialogBox& dialog) { return name != dialog.name; }

bool DialogBox::operator<(const DialogBox& dialog) {
  return (*this * dialogLines.size()).size() < (dialog * dialog.dialogLines.size()).size();
}
bool DialogBox::operator>(const DialogBox& dialog) {
  return (*this * dialogLines.size()).size() > (dialog * dialog.dialogLines.size()).size();
}

DialogBox& DialogBox::operator++(int dummy) {
  if (finished) {
    ActiveDialog = nullptr;
    return *this;
  }
  line++;
  column = 0;
  if (line >= dialogLines.size())
    finished = true;
  return *this;
}

string& DialogBox::operator[](int index) { return dialogLines[index]; }

ostream& operator<<(ostream& os, const DialogBox& dialogBox) {
  for (int i = 0; i < dialogBox.dialogLines.size(); i++)
    os << dialogBox.name << ": " << dialogBox.dialogLines[i] << endl;
  return os;
}

istream& operator>>(istream& is, DialogBox& dialogBox) {
  string name;
  int lines;
  Texture2D profilePic;
  vector<string> dialogLines;

  cout << "Creating DialogBox" << endl;
  cout << "Enter name: "; is >> dialogBox.name;
  cout << "Enter number of dialog lines: "; is >> lines;
  dialogBox.dialogLines = vector<string> (lines);

  cout << "Enter " << lines << " dialog lines: " << endl;
  for (int i = 0; i < lines; i++)
    is >> dialogBox.dialogLines[i];
  return is;
}


void DialogBox::Draw() {
  if (this->finished)
    return;

  // destination rectangle va acoperi toata suprafata textului
  // profile pic-ul va fi o 1/4 din width sau height in functie
  // de care e mai mic;

  // Dam prima oara black out la partea de jos a ecranului
  DrawRectangleRec(dest, Color(0,0,0,200));
  // Desenam acum black bar-ul ptr nume
  Rectangle NameBar(144, dest.y, dest.width - 144, 20);
  DrawRectangleRec(NameBar, BLACK);

  // Desenam un outline negru
  Rectangle TextBox(144, dest.y + 20, dest.width - 144, dest.height - 20);
  DrawRectangleLinesEx(TextBox, 4, BLACK);
  // Scoatem TextBox-ul din outline
  TextBox = Rectangle(TextBox.x + 4, TextBox.y + 4, TextBox.width - 8, TextBox.height - 8);


  // presupunem ca profile pic are 144pixels;
  //void DrawTexturePro(Texture2D texture,
  // Rectangle source, Rectangle dest,
  // Vector2 origin, float rotation, Color tint);
  DrawTexturePro(
    this->profilePic,
    Rectangle(0 * animFrame, 0, profilePic.width, profilePic.height),
    Rectangle(dest.x, dest.y, 144, 144),
    Vector2(0, 0), 0, WHITE);

  int widthRemaining = dest.width - 144;
  //void DrawText(const char *text, int posX, int posY,
  //               int fontSize, Color color);
  DrawText(this->name.c_str(), NameBar.x + 4, NameBar.y + 2, NameBar.height, WHITE);

  column += GetRandomValue(1, 2);
  if (column > dialogLines[line].length())
    column = dialogLines[line].length();
  string Text = dialogLines[line].substr(0, column);
  float fontsize = TextBox.height / 3;
  DrawText(Text.c_str(), TextBox.x + 4, TextBox.y + 4, fontsize, WHITE);

}