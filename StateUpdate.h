//
// Created by Leustean Stefan on 14.05.2025.
//

#ifndef STATEUPDATE_H
#define STATEUPDATE_H

using namespace std;

void MainMenuUpdateState(GUIContainer*);
void SettingsMenuUpdateState(GUIContainer*);
void GameScreenUpdateState(GUIContainer*);
void SettingsMenu_settings_changed(GUIContainer*);
void GameMenuScreenUpdateState(GUIContainer*);
void FightScreenUpdateState(GUIContainer*);
void SpawnQuickTimes(Object*);
vector<Vector2D> GeneratePointZigZag_Seq(float);

/*
 * MAIN MENU CONFIGURATION
 */
void MainMenuUpdateState(GUIContainer* MainMenu) {
    MainMenu->setStopUpdate(false);
    MainMenu->setActive(true);

    Button* exit_button = dynamic_cast<Button*>
        (MainMenu->FindFirstChild("ButtonsContainer")->FindFirstChild("ExitButton"));
    if (exit_button == nullptr) {
        cout << "WARNING: SettingsMenu button not found" << endl;
        throw bad_cast();
    }

    if (exit_button->getButtonState() == Pressed) {
        Object::workspace.setActive(false);
        return;
    }

    Button* settings_button = dynamic_cast<Button*>
        (MainMenu->FindFirstChild("ButtonsContainer")->FindFirstChild("SettingsButton"));
    if (settings_button == nullptr) {
        cout << "WARNING: SettingsMenu button not found" << endl;
        throw bad_cast();
    }

    if (settings_button->getButtonState() == Pressed) {
        settings_button->setButtonState(Normal);
        MainMenu->setStopUpdate(true);

        Game::LastState = Game::CurrentState;
        Game::CurrentState = SettingsMenu_State;
        return;
    }

    Button* play_button = dynamic_cast<Button*>
        (MainMenu->FindFirstChild("ButtonsContainer")->FindFirstChild("PlayButton"));
    if (play_button == nullptr) {
        cout << "WARNING: SettingsMenu button not found" << endl;
        throw bad_cast();
    }

    if (play_button->getButtonState() == Pressed) {
        play_button->setButtonState(Normal);
        MainMenu->setActive(false);

        Game::LastState = Game::CurrentState;
        Game::CurrentState = GameScreen_State;
    }
}
/*
 *
 */


/*
 *  SETTINGS MENU CONFIGURATION
 */
void SettingsMenuUpdateState(GUIContainer* SettingsMenu) {
    SettingsMenu->setActive(true);
    SettingsMenu->setStopUpdate(false);

    Button* back_button = dynamic_cast<Button*>(SettingsMenu->FindFirstChild("BackButton"));
    if (back_button == nullptr) throw bad_cast();


    SettingsMenu_settings_changed(SettingsMenu);

    if (back_button->getButtonState() == Pressed) {

        back_button->setButtonState(Normal);
        swap(Game::LastState, Game::CurrentState);
        SettingsMenu->setActive(false);
    }
}

void SettingsMenu_settings_changed(GUIContainer* SettingsMenu) {
    Object* settingsContainer = SettingsMenu->FindFirstDescendant("SettingsContainer");

    OptionBox<pair<float,float>> *resolution = dynamic_cast< OptionBox<pair<float,float>> * >
            (settingsContainer->FindFirstChild("resolution")->FindFirstChild("OptionBox"));
    if (resolution == nullptr) throw bad_cast();
    Game::Main->SetResolution(resolution->getValue());

    Slider* volumeSlider = dynamic_cast<Slider*>
        (settingsContainer->FindFirstChild("volume")->FindFirstChild("volumeSlider"));
    if (volumeSlider == nullptr) throw bad_cast();
    Game::Main->SetVolume(volumeSlider->getValue());

    CheckBox* musicCheckBox = dynamic_cast<CheckBox*>
        (settingsContainer->FindFirstChild("music")->FindFirstChild("musicCheckBox"));
    if (musicCheckBox == nullptr) throw bad_cast();
    Game::Main->SetBackgroundMusic(musicCheckBox->getChecked());

    CheckBox* soundCheckBox = dynamic_cast<CheckBox*>
        (settingsContainer->FindFirstChild("sound_effects")->FindFirstChild("sound_effectsCheckBox"));
    if (soundCheckBox == nullptr) throw bad_cast();
    Game::Main->SetSoundEffects(soundCheckBox->getChecked());

    CheckBox* voiceCheckBox = dynamic_cast<CheckBox*>
        (settingsContainer->FindFirstChild("voice")->FindFirstChild("voiceCheckBox"));
    if (voiceCheckBox == nullptr) throw bad_cast();
    Game::Main->SetVoice(voiceCheckBox->getChecked());

    if (resolution->getShowOptions()) {
        volumeSlider->setStopUpdate(true);
        soundCheckBox->setStopUpdate(true);
    }
    else {
        volumeSlider->setStopUpdate(false);
        soundCheckBox->setStopUpdate(false);
    }
}
/*
 *
 */

/*
 *  GAME SCREEN CONFIGURATION
 */

void GameScreenUpdateState(GUIContainer* GameScreen) {
    GameScreen->setActive(true);
    GameScreen->setStopUpdate(false);

    Button* GameSettingsButton = dynamic_cast<Button*>
        (GameScreen->FindFirstChild("GameSettingsButton"));
    if (GameSettingsButton == nullptr) throw bad_cast();

    if (GameSettingsButton->getButtonState() == Pressed) {
        GameSettingsButton->setButtonState(Normal);
        GameScreen->setStopUpdate(false);

        Game::LastState = Game::CurrentState;
        Game::CurrentState = GameMenuScreen_State;
    }

    Button* tempButton = dynamic_cast<Button*>
        (GameScreen->FindFirstChild("TempButton"));
    if (tempButton == nullptr) throw bad_cast();

    if (tempButton->getButtonState() == Pressed) {
        tempButton->setButtonState(Normal);
        GameScreen->setActive(false);

        Game::LastState = Game::CurrentState;
        Game::CurrentState = FightScreen_State;
    }
}

/*
 *
 */

/*
 * GAME MENU SCREEN CONFIGURATION
 */

void GameMenuScreenUpdateState(GUIContainer *GameMenuScreen) {
    static GameState from_where = Game::LastState;
    if (GameMenuScreen->getActive() == false)
        from_where = Game::LastState;

    GameMenuScreen->setActive(true);
    GameMenuScreen->setStopUpdate(false);

    Button* SettingsButton = dynamic_cast<Button*>
        (GameMenuScreen->FindFirstChild("ButtonsContainer")->FindFirstChild("SettingsButton"));
    if (SettingsButton == nullptr) throw bad_cast();

    if (SettingsButton->getButtonState() == Pressed) {
        SettingsButton->setButtonState(Normal);
        GameMenuScreen->setStopUpdate(true);

        Game::LastState = Game::CurrentState;
        Game::CurrentState = SettingsMenu_State;
    }

    Button* MainMenuButton = dynamic_cast<Button*>
        (GameMenuScreen->FindFirstChild("ButtonsContainer")->FindFirstChild("MainMenuButton"));
    if (MainMenuButton == nullptr) throw bad_cast();

    if (MainMenuButton->getButtonState() == Pressed) {
        MainMenuButton->setButtonState(Normal);
        GameMenuScreen->setActive(false);

        switch (from_where) {
            case FightScreen_State: {
                Object::workspace.FindFirstChild("FightScreen")->setActive(false);
                break;
            }
            case GameScreen_State: {
                Object::workspace.FindFirstChild("GameScreen")->setActive(false);
                break;
            }
            case DialogScreen_State: {
                // momentan nu am nimic
            }
            default: {
                cout << "ERROR: Entered Game Menu from invalid state, " << from_where << endl;
                exit(0);
            }
        }



        Game::LastState = Game::CurrentState;
        Game::CurrentState = MainMenu_State;
    }

    Button* ContinueButton = dynamic_cast<Button*>
        (GameMenuScreen->FindFirstChild("ButtonsContainer")->FindFirstChild("ContinueButton"));
    if (ContinueButton == nullptr) throw bad_cast();

    if (ContinueButton->getButtonState() == Pressed) {
        ContinueButton->setButtonState(Normal);
        GameMenuScreen->setActive(false);

        Game::LastState = Game::CurrentState;
        Game::CurrentState = from_where;
    }
}

/*
 *
 */

/*
 * FIGHT SCREEN CONFIGURATION
 */

void FightScreenUpdateState(GUIContainer *FightScreen) {
    static bool entered = true;
    if (entered) {
        FightScreen->FindFirstChild("quicktime_folder")->ClearChildren();
        Player::player->setAttribute("Health", 5);
        entered = false;
    }
    FightScreen->setActive(true);
    FightScreen->setStopUpdate(false);

    Button* GameSettingsButton = dynamic_cast<Button*>
       (FightScreen->FindFirstChild("GameSettingsButton"));
    if (GameSettingsButton == nullptr) throw bad_cast();

    TextLabel* healthbar = dynamic_cast<TextLabel*>
        (FightScreen->FindFirstChild("healthbar"));
    if (healthbar == nullptr) throw bad_cast();

    healthbar->setText("Health:" + to_string(int(Player::player->getAttribute("Health"))));

    SpawnQuickTimes(FightScreen->FindFirstChild("quicktime_folder"));

    if (GameSettingsButton->getButtonState() == Pressed) {
        GameSettingsButton->setButtonState(Normal);
        FightScreen->setStopUpdate(true);

        Game::LastState = Game::CurrentState;
        Game::CurrentState = GameMenuScreen_State;
        entered = true;
    }

    if (Player::player->IsDead()) {
        FightScreen->setActive(false);
        Game::LastState = Game::CurrentState;
        Game::CurrentState = GameScreen_State;
        entered = true;
    }

}

void SpawnQuickTimes(Object* quicktime_folder) {
    static double time_till_spawn = 1;

    time_till_spawn -= GetFrameTime();
    if (time_till_spawn <= 0) {
        time_till_spawn = 1;

        QuickTime* quick_time = new QuickTime("QuickTime", "rsc/Sword.png");
        quick_time->setParent(quicktime_folder);
        quick_time->setZIndex(2);
        AnimInfo info;
        info.loop = true;
        info.duration = 0.7;
        info.LoadAnimation("rsc/Sword_Idle_Anim.png");
        quick_time->getAnimationHandler().AddAnimation("Idle", info);
        quick_time->getAnimationHandler().PlayAnimation("Idle");

        info.duration = 0.5;
        info.LoadAnimation("rsc/Pinky_Idle_Anim.png");
        info.loop = false;
        quick_time->getAnimationHandler().AddAnimation("Pop", info);
        info.LoadAnimation("rsc/Sword_Attack_Anim.png");
        quick_time->getAnimationHandler().AddAnimation("Attack", info);
        quick_time->setSize(80, 80);

        quick_time->setLetter(GetRandomValue('A', 'Z'));


        float y = GetRandomValue(0, DEFAULT_RESOLUTION.second - 80);
        quick_time->setPosition(0, y);
        auto tween = TweenService::GetTweenService("FightTweenService")->
            CreateTween(quick_time, GeneratePointZigZag_Seq(y), 5);
        quick_time->setTime(5);
        tween->Play();
    }
}

vector<Vector2D> GeneratePointZigZag_Seq(float y) {

    float d[] = {-100, +100, -100, +100,- 100};
    float spacing = DEFAULT_RESOLUTION.first / 5;
    vector<Vector2D> points;
    for (int i = 0; i < 5; i++) {
        Vector2D point;
        point.setX(min(spacing * (i + 1), 1.0f*DEFAULT_RESOLUTION.first - 80));

        point.setY(y + d[i]);
        if (y + d[i] < 0)
            point.setY(0);
        if (y + d[i] >= DEFAULT_RESOLUTION.second - 80)
            point.setY(DEFAULT_RESOLUTION.second - 80);
        points.push_back(point);
    }
    return points;
}

/*
 *
 */

#endif //STATEUPDATE_H
