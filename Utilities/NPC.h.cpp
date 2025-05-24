#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "Vector2D.h"
#include "Defines.h"
#include "NPC.h"
using namespace std;

vector<NPC*> NPC::npcs = vector<NPC*>();
NPC* NPC::getNPC(string name){
    for (int i = 0; i < npcs.size(); i++)
      if (npcs[i]->getName() == name)
        return npcs[i];
    return nullptr;
}

NPC::NPC() : name((string)"NPC" + to_string(npcs.size())) {
    position = Vector2D(-BLOCK_SIZE, -BLOCK_SIZE);
    permList = nullptr;
    dialogLines = vector<string>();

    animFrames = 1;
    animFrame = 0;

    npcs.push_back(this);
}

NPC::NPC(string name, int* permlist) : name(name) {
    position = Vector2D(-BLOCK_SIZE, -BLOCK_SIZE);
    this->permList = permlist;
    dialogLines = vector<string>();

    animFrames = 1;
    animFrame = 0;

    npcs.push_back(this);
}

NPC::NPC(const NPC& npc) : name(npc.name + to_string(npcs.size())) {
  position = npc.position;
  this->permList = new int[3];
  for (int i = 0; i < 3; i++)
    this->permList[i] = npc.permList[i];
  dialogLines = npc.dialogLines;

  profilePic = npc.profilePic;
  sprite = npc.sprite;
  animFrames = npc.animFrames;
  animFrame = npc.animFrame;
  voiceSound = npc.voiceSound;

  npcs.push_back(this);
}

NPC::~NPC() {
  if (permList != nullptr)
    delete [] this->permList;
}

string NPC::getName() { return name; }
Vector2D& NPC::Position() { return position; }
vector<string>& NPC::getDialogLines() { return dialogLines; }
void NPC::LoadSpriteTexture(const char* filepath) {
    UnloadTexture(sprite);
    sprite = LoadTexture(filepath);
    animFrames = sprite.width / BLOCK_SIZE;
}
void NPC::LoadSpriteTexture(Texture2D texture) {
    UnloadTexture(sprite);
    sprite = texture;
    animFrames = sprite.width / BLOCK_SIZE;
}
void NPC::LoadProfilePic(const char* filepath) {
  UnloadTexture(profilePic);
  profilePic = LoadTexture(filepath);
}
void NPC::LoadProfilePic(Texture2D texture) {
  UnloadTexture(profilePic);
  profilePic = texture;
}
void NPC::LoadVoiceSound(const char* filepath) {
  UnloadSound(voiceSound);
  voiceSound = LoadSound(filepath);
}
void NPC::LoadVoiceSound(Sound voice) {
  UnloadSound(voiceSound);
  voiceSound = voice;
}
void NPC::AddDialogLine(const char* line) {
    dialogLines.push_back(string(line));
}
void NPC::AddDialogLine(string line) {
  dialogLines.push_back(line);
}

void NPC::Draw() {
  //void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
  Rectangle source( animFrame*64, 0, BLOCK_SIZE, BLOCK_SIZE);
  Rectangle dest(position[0], position[1], BLOCK_SIZE, BLOCK_SIZE);
  DrawTexturePro(sprite, source, dest, Vector2(source.width / 2, source.height / 2), 0, WHITE);
}

NPC& NPC::operator=(NPC& npc) { return npc; }

bool NPC::operator==(const NPC& npc) {return this->name == npc.name;}
bool NPC::operator!=(const NPC& npc) { return this->name != npc.name;}

bool NPC::operator<(const NPC& npc) {
  for (int i = 0; i < 3; i++)
    if (this->permList[i] < npc.permList[i])
      return true;
  return false;
}
bool NPC::operator>(const NPC& npc) {
  for (int i = 0; i < 3; i++)
    if (this->permList[i] > npc.permList[i])
      return true;
  return false;
}

NPC& NPC::operator+(const Vector2D& pos) {
  this->position = position + pos;
  return *this;
}
NPC& NPC::operator-(const Vector2D& pos) {
  this->position = position - pos;
  return *this;
}

NPC& NPC::operator++(int dummy) {
  animFrame = (animFrame + 1) % animFrames;
  return *this;
}
string& NPC::operator[](int index) {
  if (index < 0 || index >= dialogLines.size()) {
    cout<< "ERROR: tried to access index out of range" << endl;
    cout<< "At dialogLines index " << index << " NPC " << name << endl;
    exit(1);
  }
  return dialogLines[index];
}

ostream& operator<<(ostream& os, const NPC& npc) {
  os << npc.name << " at position " << npc.position;
  return os;
}

istream& operator>>(istream& is, NPC& npc) {
  string name;
  int *p = new int[3];
  cout<< "Enter NPC name: "; is >> name;
  cout<< "Enter permissions (3 -> 0 / 1): ";
  is >> p[0] >> p[1] >> p[2];
  npc.name = name;
  npc.permList[0] = p[0];
  npc.permList[1] = p[1];
  npc.permList[2] = p[2];
  return is;
}

