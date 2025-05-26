#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <raylib.h>
#include <raymath.h>
#include <Utilities.h>
#include <GUI.h>
#include <STDGame.h>
#include "StateUpdate.h"

using namespace std;

/*
 * Clase ptr proiect -> Vector2D, Object
 * Next-> Dialogue Box
 */

/*
 * Thing To Add :
 * -> Harta, TileMap probabil
 * -> movement, sa fie mai bun, poate fac si click movement
 * -> Sa pun in foldere diferite executabilele (CMake)
 * -> Sa fac un meniu (Preferabil sa poti schimba si aspect ratio / resolutie, dar vad cu asta)
 * -> O clasa NPC, ar trebui s-o folosec si la dialogue pe asta
 * -> Sa decid cum va avea loc battle
 * -> Background Music
 * -> Sa mai continui cu scrisul povestii
 * -> De redefiniti player-ul + poate game
 * -> multe altele
 *
 * -> De gandit idee de salvat progres
 *  READ ABOUT VARIADIC TEMPLATES AND VARIADIC FUNTIONS
 *
 *  DE ADAUGAT DUPA CE INVAT EXCEPTIONS
 *  isTextureValid in toate load-urile care sa dea throw la exception cand da gresit
 */
GUIContainer* SettingsMenu;
GUIContainer* MainMenu;
GUIContainer* GameScreen;
GUIContainer* GameMenuScreen;
GUIContainer* FightScreen;

void CreateSettingsMenu(GUIContainer *SettingsMenu);
void CreateMainMenu(GUIContainer* MainMenu);
void CreateGameScreen(GUIContainer* GameScreen);
void CreateGameMenuScreen(GUIContainer* GameMenuScreen);
void CreateFightScreen(GUIContainer* FightScreen);

Game* Game::Main = Game::StartGame();

int main() {
    // Vreau sa mai lucrez un pic in legatura cu game class
    // sa fie chestiile acestea in functia de start si sa fie totul initializat asa
    // sa pot schimba rezolutia
    // o sa vreau sa incep si sa adaug audio
    // trebuie sa ma gandesc cum o sa fac cu el
    // poate pentru sunete temporare adaug ca copil obiectului care il face
    // si il adaug ca atribut cine stie
    /*
     * GOAL MOMENTAN
     * -> UN WORKING MENU
     * -> RESOLUTION CHANGER, VOLUME CHANGER, OTHERSS
     * -> TWEEN SERVICE CARE SA FUNCTIONEZE BINE
     * -> SA MA MAI JOC CU VARIADIC TEMPLATE FUNCTIONS
     * -> NOROC MIE
     */
    MainMenu = new GUIContainer("MainMenu");
    CreateMainMenu(MainMenu);
    SettingsMenu = new GUIContainer("SettingsMenu");
    CreateSettingsMenu(SettingsMenu);
    GameScreen = new GUIContainer("GameScreen");
    CreateGameScreen(GameScreen);
    GameMenuScreen = new GUIContainer("GameMenuScreen");
    CreateGameMenuScreen(GameMenuScreen);
    FightScreen = new GUIContainer("FightScreen");
    CreateFightScreen(FightScreen);

    TweenService::CreateTweenService("FightTweenService", FightScreen);

    Player::player = Player::CreatePlayer();
    Player::player->setParent(GameScreen);


    while (!WindowShouldClose()) {
        Object::workspace.Update();
        switch (Game::CurrentState) {
            case MainMenu_State: {
                MainMenuUpdateState(MainMenu);
                break;
            }
            case SettingsMenu_State: {
                SettingsMenuUpdateState(SettingsMenu);
                break;
            }
            case GameScreen_State: {
                GameScreenUpdateState(GameScreen);
                break;
            }
            case FightScreen_State: {
                FightScreenUpdateState(FightScreen);
                break;
            }
            case GameMenuScreen_State: {
                GameMenuScreenUpdateState(GameMenuScreen);
                break;
            }
            case DialogScreen_State: {break;}
        }


        if (!Object::workspace.getActive())
            break;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            Object::workspace.Draw();
        EndDrawing();
    }

    // for (auto &i : TextureHandler::Handler) {
    //     cout << i.count << " " << i.filepath << endl;
    // }

    CloseWindow();
    delete Game::Main;
    return 0;
}

void CreateSettingsMenu(GUIContainer *SettingsMenu) {
    SettingsMenu->setParent(Object::workspace);
    SettingsMenu->setActive(false);
    SettingsMenu->setBackgroundColor(223,132,10,220);
    SettingsMenu->setZIndex(3);

    GUIObject* settingsLines = new GUIObject("settingsLines");
    settingsLines->setParent(SettingsMenu);
    settingsLines->setBackgroundColor(86,50,50);
    settingsLines->setRoundness(0.2f);
    settingsLines->setSize(500, 300);
    settingsLines->setPosition(DEFAULT_RESOLUTION.first / 2 - settingsLines->Width() / 2, DEFAULT_RESOLUTION.second / 2 - settingsLines->Height() / 2);
    settingsLines->setPosition(settingsLines->X(), settingsLines->Y() + 50);

    GUIObject* settingsContainer = new GUIObject(*settingsLines);
    settingsContainer->setName("SettingsContainer");
    settingsContainer->setParent(settingsLines);
    settingsContainer->setBackgroundColor(255,193,140);
    settingsContainer->setSize(460,260);
    settingsContainer->setPosition(settingsLines->getPosition() + Vector2D(20,20));

    GUIContainer* resolution = new GUIContainer("resolution");
    resolution->setParent(settingsContainer);
    resolution->setSize(420, 40);
    resolution->setPosition(settingsContainer->getPosition() + Vector2D(20,20));

    TextLabel* resolutionText = new TextLabel("resolutionTextLabel", "Resolution: ");
    resolutionText->setParent(resolution);
    resolutionText->setSize(GetTextBoxSize("Resolution: ", resolutionText->getFont(), resolutionText->getFontSize(), resolutionText->getFontSpacing()));

    Vector2D resolutionTextPosition = resolution->getPosition();
    Rectangle resolutionBox = {resolution->X(), resolution->Y(), resolution->Width(), resolution->Height()};
    AlignLeftText(resolutionBox, resolutionTextPosition, resolution->getSize());
    resolutionText->setFontSize(40);
    resolutionText->setPosition(resolutionTextPosition);

    OptionBox<pair<float,float>>* option_box = new OptionBox<pair<float,float>>("OptionBox",
        {{"512x288", {512, 288}},
              {"800x450", {800, 450}},
            {"928x522", {928, 522}},
            {"1024x576", {1024, 576}},
            {"1600x900",{1600, 900}}}
        );
    option_box->setParent(resolution);
    option_box->TextLabel()->setFontSize(40);
    option_box->setSize(184, 40);
    option_box->setCurrentValue(Game::Main->GetResolution());

    Vector2D optionBoxPosition = option_box->getPosition();
    AlignRightText(Rectangle(resolution->X(), resolution->Y(), resolution->Width(), resolution->Height()),
        optionBoxPosition, option_box->getSize());
    option_box->setPosition(optionBoxPosition);
    option_box->setCurrentValue(Game::Main->GetResolution());

    option_box->TextLabel()->setFontColor(BLACK);
    option_box->setNormalColor(218,109,66);


    GUIContainer* volume = new GUIContainer(*resolution);
    volume->setName("volume");
    TextLabel* volumeText = new TextLabel(*resolutionText);
    volumeText->setParent(volume);
    volumeText->setName("volumeText");
    volumeText->setText("Volume: ");
    volume->setPosition(volume->getPosition() + Vector2D(0, volume->Height()));

    Slider* volumeSlider = new Slider("volumeSlider");
    volumeSlider->setParent(volume);
    volumeSlider->setMaxSize({200, 10});
    volumeSlider->setPosition(volume->X() + volume->Width() - 200, volume->Y() + volume->Height() / 2 - 5);
    volumeSlider->setValue(Game::Main->GetVolume());

    GUIContainer* music = new GUIContainer("music");
    music->setSize(200, 40);
    music->setPosition(volume->getPosition() + Vector2D(0, volume->Height()));
    music->setParent(settingsContainer);

    TextLabel* musicText = new TextLabel("musicText", "Music: ");
    musicText->setParent(music);
    musicText->setFontSize(40);
    musicText->setSize(GetTextBoxSize("Music: ", musicText->getFont(), musicText->getFontSize(), musicText->getFontSpacing()));
    musicText->setPosition(music->getPosition());

    CheckBox* musicCheck = new CheckBox("musicCheckBox");
    musicCheck->setSize(40);
    musicCheck->setParent(music);
    musicCheck->setPosition(musicText->getPosition() + Vector2D(musicText->Width() + 10, 0));
    musicCheck->setChecked(Game::Main->GetBackgroundMusic());

    GUIContainer* sound_effects = new GUIContainer(*music);
    sound_effects->setName("sound_effects");
    sound_effects->setParent(settingsContainer);
    sound_effects->setPosition(music->X(), music->Y() + music->Height());

    TextLabel* sound_effectsText = new TextLabel(*musicText);
    sound_effectsText->setParent(sound_effects);
    sound_effectsText->setSize(GetTextBoxSize("Sound Effects: ", sound_effectsText->getFont(), sound_effectsText->getFontSize(), sound_effectsText->getFontSpacing()));
    sound_effectsText->setText("Sound Effects: ");
    sound_effectsText->setName("sound_effectsText");
    sound_effectsText->setPosition(sound_effects->getPosition());

    CheckBox* sound_effectsCheck = new CheckBox("sound_effectsCheckBox");
    sound_effectsCheck->setSize(40);
    sound_effectsCheck->setParent(sound_effects);
    sound_effectsCheck->setChecked(Game::Main->GetSoundEffects());
    sound_effectsCheck->setPosition(sound_effectsText->X() + sound_effectsText->Width() + 10, sound_effectsText->Y());

    GUIContainer* voice = new GUIContainer(*sound_effects);
    voice->setName("voice");
    voice->setParent(settingsContainer);
    voice->setPosition(sound_effects->getPosition() + Vector2D(0, sound_effectsText->Height()));

    TextLabel* voiceText = new TextLabel("voiceText", "Voice: ");
    voiceText->setFontSize(40);
    voiceText->setParent(voice);
    voiceText->setSize(GetTextBoxSize("Voice: ", voiceText));
    voiceText->setPosition(voice->getPosition());

    CheckBox* voiceCheck = new CheckBox("voiceCheckBox");
    voiceCheck->setSize(40);
    voiceCheck->setParent(voice);
    voiceCheck->setChecked(Game::Main->GetVoice());
    voiceCheck->setPosition(voiceText->X() + voiceText->Width() + 10, voiceText->Y());

    resolution->setZIndex(1);

    Button* backButton = new Button("BackButton");
    backButton->setParent(SettingsMenu);
    backButton->TextLabel()->setActive(false);
    backButton->setNormalTexture("rsc/back_arrow.png");
    backButton->setSize(90, 64);
    backButton->setPosition(settingsLines->getPosition() - Vector2D(0, backButton->Height()));

    TextLabel* SettingsMenuText = new TextLabel("SettingsMenuText", "Settings");
    SettingsMenuText->setParent(SettingsMenu);
    SettingsMenuText->setFontSize(70);
    SettingsMenuText->UpdateSecluded();

    Vector2D SettingsMenuTextPosition(0, 0);
    Rectangle SettingsMenuTextBox(settingsLines->X(), settingsLines->Y() - 80, settingsLines->Width(), 70);
    AlignCenterText(SettingsMenuTextBox, SettingsMenuTextPosition, SettingsMenuText->getSize());
    SettingsMenuText->setPosition(SettingsMenuTextPosition);
}

void CreateMainMenu(GUIContainer* MainMenu) {
    MainMenu->setParent(Object::workspace);
    MainMenu->setBackgroundTexture("rsc/MenuImage.png");

    GUIContainer* ButtonContainer = new GUIContainer("ButtonsContainer", {0, 0}, {260, 260});
    ButtonContainer->setParent(MainMenu);

    Button* SettingsButton = new Button();
    SettingsButton->setParent(ButtonContainer);
    SettingsButton->setSize(260, 80);
    SettingsButton->setPosition(0, 90);
    SettingsButton->setName("SettingsButton");
    SettingsButton->TextLabel()->setText("Settings");
    SettingsButton->TextLabel()->setFontSize(60);
    SettingsButton->TextLabel()->setFontSpacing(5);
    SettingsButton->setNormalTexture("rsc/MenuButton.png");

    Button* PlayButton = new Button(*SettingsButton);
    PlayButton->setName("PlayButton");
    PlayButton->TextLabel()->setText("Play");
    PlayButton->setPosition(0, 0);

    Button* ExitButton = new Button(*PlayButton);
    ExitButton->setName("ExitButton");
    ExitButton->TextLabel()->setText("Exit");
    ExitButton->setPosition(0, 180);

    ButtonContainer->setPosition(
        DEFAULT_RESOLUTION.first / 2 - 130,
        DEFAULT_RESOLUTION.second / 2 - 60
    );
}

void CreateGameScreen(GUIContainer *GameScreen) {
    GameScreen->setParent(Object::workspace);
    GameScreen->setActive(false);

    Button* GameSettingsButton = new Button("GameSettingsButton");
    GameSettingsButton->setParent(GameScreen);
    GameSettingsButton->TextLabel()->setActive(false);
    GameSettingsButton->setZIndex(10);
    GameSettingsButton->setNormalTexture("rsc/Cogwheel.png");
    GameSettingsButton->setSize(64, 64);

    Button* tempButton = new Button("TempButton");
    tempButton->TextLabel()->setText("FightScreen");
    tempButton->setParent(GameScreen);
    tempButton->setPosition(800, 0);

    GameScreen->setBackgroundColor(DARKGREEN);
}

void CreateGameMenuScreen(GUIContainer *GameMenuScreen) {
    GameMenuScreen->setParent(Object::workspace);
    GameMenuScreen->setActive(false);
    GameMenuScreen->setZIndex(2);

    GameMenuScreen->setBackgroundColor(0, 0, 0, 200);

    GUIContainer* ButtonContainer = new GUIContainer("ButtonsContainer", {0, 0}, {260, 260});
    ButtonContainer->setParent(GameMenuScreen);

    Button* SettingsButton = new Button();
    SettingsButton->setParent(ButtonContainer);
    SettingsButton->setSize(260, 80);
    SettingsButton->setPosition(0, 90);
    SettingsButton->setName("SettingsButton");
    SettingsButton->TextLabel()->setText("Settings");
    SettingsButton->TextLabel()->setFontSize(60);
    SettingsButton->TextLabel()->setFontSpacing(5);
    SettingsButton->setNormalTexture("rsc/MenuButton.png");

    Button* MainMenuButton = new Button(*SettingsButton);
    MainMenuButton->setName("MainMenuButton");
    MainMenuButton->TextLabel()->setText("MainMenu");
    MainMenuButton->setPosition(0,0);
    MainMenuButton->TextLabel()->setFontSize(47);

    Button* ContinueButton = new Button(*SettingsButton);
    ContinueButton->setName("ContinueButton");
    ContinueButton->TextLabel()->setText("Continue");
    ContinueButton->setPosition(0, 180);
    ContinueButton->TextLabel()->setFontSize(52);


    ButtonContainer->setPosition(DEFAULT_RESOLUTION.first / 2 - 130, DEFAULT_RESOLUTION.second / 2 - 85);
}

void CreateFightScreen(GUIContainer *FightScreen) {
    FightScreen->setParent(Object::workspace);
    FightScreen->setActive(false);
    FightScreen->setBackgroundColor(DARKGREEN);

    GUIObject* sky = new GUIObject("sky");
    sky->setParent(FightScreen);
    sky->setBackgroundColor(BLUE);
    sky->setSize(DEFAULT_RESOLUTION.first, DEFAULT_RESOLUTION.second * 0.4);


    Button* GameSettingsButton = new Button("GameSettingsButton");
    GameSettingsButton->setParent(FightScreen);
    GameSettingsButton->TextLabel()->setActive(false);
    GameSettingsButton->setNormalTexture("rsc/Cogwheel.png");
    GameSettingsButton->setSize(64, 64);

    GUIContainer* quicktime_folder = new GUIContainer("quicktime_folder");
    quicktime_folder->setParent(FightScreen);
    quicktime_folder->setZIndex(2);

    TextLabel* healthbar = new TextLabel("healthbar");
    healthbar->setParent(FightScreen);
    healthbar->setText("HEALTH:5");
    healthbar->setFontColor(RED);
    healthbar->UpdateSecluded();
    healthbar->setPosition(0, DEFAULT_RESOLUTION.second - healthbar->Height());

    ImageLabel *testKnight = new ImageLabel("knight");
    testKnight->setParent(FightScreen);
    testKnight->setSize(256, 320);
    testKnight->setZIndex(0);
    testKnight->setPosition(600, 50);
    testKnight->setImageTexture("rsc/Knight.png");

    ImageLabel* pinkyback = new ImageLabel("pinky");
    pinkyback->setSize(250, 250);
    pinkyback->setPosition(60, DEFAULT_RESOLUTION.second - pinkyback->Height() - 50);
    pinkyback->setImageTexture("rsc/PinkyBack.png");
    pinkyback->setParent(FightScreen);
}




