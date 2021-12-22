#ifndef GAMESTATE_H
#define GAMESTATE_H

//All the different states of the game

// waiting = game on break
// started = game start
// gameover = game end
// menuStart = main menu
// menuDifficulties = menu with the difficulties
// menuSettings = settings menu
// onBreak = break menu
// victory = win menu
enum GameState {
    waiting,
    started,
    gameover,
    menuStart,
    menuSettings,
    menuDifficulties,
    onBreak,
    victory
};

#endif // GAMESTATE_H
