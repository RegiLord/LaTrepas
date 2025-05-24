#include <iostream>
#include <string>
#include "raylib.h"
#include "Vector2D.h"
#include "TextHandler.h"

using namespace std;

Vector2D TextHandler::GetTextBoxSize(const std::string &Text, Font* font, float fontsize, float spacing) {
  Vector2 textSize = MeasureTextEx(*font, Text.c_str(), fontsize, spacing);
  return {textSize.x, textSize.y};
}


void TextHandler::DefFontFormatString(string &Text, Vector2D Box, Font* font, int fontsize, int spacing) {
  // Trebuie sa scoatem \n
  for (int i = 0; i < Text.length(); i++) {
    if (Text[i] == '\n') {
      Text.erase(i, 1);
      i--;
    }
  }
  // In default font cu measure text la fiecare 10 fontsize creste spacing-ul cu 1
  // String Formatting-ul va avea word wrap on
  // wa afisa chiar si daca intrece Box-ul, dar va afisa vertical, nu orizontal
  // Daca exista un cuvant mai mare decat intreg-ul box atunci il va afisa doar pe ala
  string NewText = "";

  while (Text.length() > 0) {
    int pos = -2;
    for (int i = 0; i < Text.length(); i++) {
      Vector2 Measure = MeasureTextEx(*font, Text.substr(0,i + 1).c_str(), fontsize, spacing);
      if (Measure.x > Box[0]) {
        pos = i - 1;
        break;
      }
    }

    switch (pos) {
      case -2: {
        NewText = Text;
        Text.clear();
        break;
      }
      default: {
        int found = Text.find(' ');

        if (found == string::npos) {
          NewText = Text;
          Text.clear();
        }
        else {
          NewText = Text.substr(0, found);
          Text.erase(0, found + 1);
          if (Text.length() > 0)
            NewText += '\n';
        }
        break;
      }

    }

  }

  Text = NewText;
}

void TextHandler::AlignCenterText(Rectangle Box, Vector2D &TextBoxPosition, Vector2D TextBoxSize) {
  Vector2D center(Box.x + Box.width / 2, Box.y + Box.height / 2);

  TextBoxPosition = Vector2D(center[0] - TextBoxSize[0] / 2, center[1] - TextBoxSize[1] / 2);
}

void TextHandler::AlignRightText(Rectangle Box, Vector2D &TextBoxPosition, Vector2D TextBoxSize) {
  Vector2D center(Box.x + Box.width / 2, Box.y + Box.height / 2);

  TextBoxPosition[1] = center[1] - TextBoxSize[1] / 2;
  TextBoxPosition[0] = Box.x + Box.width - TextBoxSize[0] - 4;
}

void TextHandler::AlignLeftText(Rectangle Box, Vector2D &TextBoxPosition, Vector2D TextBoxSize) {
  Vector2D center(Box.x + Box.width / 2, Box.y + Box.height / 2);

  TextBoxPosition[1] = center[1] - TextBoxSize[1] / 2;
  TextBoxPosition[0] = Box.x + 4;
}
