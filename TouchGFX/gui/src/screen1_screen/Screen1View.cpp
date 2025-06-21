#include <gui/screen1_screen/Screen1View.hpp>

#include <cmsis_os.h>
#include <cmath>
#include <utility>
#include <cstring>
#include <queue>

extern uint16_t controllerX, controllerY;
extern uint16_t prevControllerX, prevControllerY;
extern uint16_t currentScore;
extern uint16_t highScore;

Screen1View::Screen1View()
{
	// prepare for game flow
	gameState = false;

	// from the start, there will be 3 types of eggs
	eggBitmapIDRange = 3;

	// generate seed for random generator
	seed = 1;
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    remove(scoreContainer);

    add(shootingEgg);
    shootingEgg.setVisible(false);
    shootingEgg.invalidate();

    add(nextShootingEgg);
    nextShootingEgg.setVisible(false);
    nextShootingEgg.invalidate();
    for (int i = 0; i < NUM_ROWS; i++) {
    	for (int j = 0; j < NUM_COLS; j++) {
       		add(eggBatch[i][j]);
       	}
    }

    shootingLine.setVisible(false);
    shootingLine.invalidate();

    realtimeScoreTextArea.setVisible(false);
    realtimeScoreTextArea.invalidate();
    add(scoreContainer);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::onPlayButtonClicked() {
	
}

void Screen1View::handleTickEvent() {
	
}
