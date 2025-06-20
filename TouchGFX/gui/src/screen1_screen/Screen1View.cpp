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

void Screen1View::initializeEggBatch() {
	dEggBatchY = 0.03;

	int i = startRowIndex;
	float lastY = BASE_Y;
	do {
		// Y coordinate
		if (lastY == BASE_Y) eggBatchY[i] = startRowIndex * EGG_HEIGHT + 0.5 * EGG_HEIGHT;
		else eggBatchY[i] = lastY - EGG_HEIGHT;
		lastY = eggBatchY[i];

		int numOfCols, padding;
		if (i % 2 == 0) {
			padding = PADDING + EGG_WIDTH * 0.5;
			numOfCols = NUM_COLS - 1;
		}
		else {
			padding = PADDING;
			numOfCols = NUM_COLS;
		}

		for (int j = 0; j < numOfCols; j++) {
			eggBatchBitmapID[i][j] = generateRandomEggBitmapID();
			eggBatch[i][j].setBitmap(touchgfx::Bitmap(eggBatchBitmapID[i][j]));
			eggBatch[i][j].moveTo(std::round(padding + EGG_WIDTH * j), std::round(eggBatchY[i] - 0.5 * EGG_HEIGHT));
			eggBatchState[i][j] = true;
			eggBatch[i][j].setVisible(eggBatchState[i][j]);
		}
		i = i-1;
		if (i < 0) i += NUM_ROWS;
	} while (i != startRowIndex);
}

void Screen1View::renderEggBatch() {
	for (int i = 0; i < NUM_ROWS; i++) {
		int numOfCols = (i%2 == 0 ? NUM_COLS-1 : NUM_COLS);
		for (int j = 0; j < numOfCols; j++) {
			eggBatch[i][j].setBitmap(touchgfx::Bitmap(eggBatchBitmapID[i][j]));
			eggBatch[i][j].moveTo(eggBatch[i][j].getX(), std::round(eggBatchY[i] - 0.5 * EGG_HEIGHT));
			eggBatch[i][j].setVisible(eggBatchState[i][j]);
			//eggBatch[i][j].invalidate();
		}
	}
}

void Screen1View::updateEggBatch() {
	// update Y coordinate of all rows in 'eggBatch'
	for (int i = 0; i < NUM_ROWS; i++) {
		eggBatchY[i] += dEggBatchY;
	}
}
void Screen1View::initializeShootingEgg() {
	shootingEggX = SCREEN_WIDTH * 0.5;
	shootingEggY = BASE_Y;
	dShootingEgg = 2;
	shootingEggState = IDLE;

	shootingEggBitmapID = generateRandomEggBitmapID();
	shootingEgg.setBitmap(touchgfx::Bitmap(shootingEggBitmapID));
	shootingEgg.setXY(std::round(shootingEggX - 0.5 * EGG_WIDTH), (int)std::round(BASE_Y - 0.5 * EGG_HEIGHT));
	shootingEgg.setVisible(true);
}
void Screen1View::updateShootingEgg() {
	//float dx, dy;
	float dx = controllerX - 127.5f;
	float dy = controllerY - 127.5f;
	float d = sqrt(dx * dx + dy * dy);

	switch (shootingEggState) {
	case IDLE:
		if (dy > 5) {
			shootingEggState = READY;
		}
		else {
			dShootingEggX = 0;
			dShootingEggY = 0;
		}
		break;
	case READY:
		if (abs(dx) <= 5 && abs(dy) <= 5) {
			dx = prevControllerX - 127.5f;
			dy = prevControllerY - 127.5f;

			if (abs(dy / dx) < (BASE_Y - LIMIT_Y) / (0.5 * SCREEN_WIDTH)) {
				dx = (dx < 0 ? -0.5 * SCREEN_WIDTH : 0.5 * SCREEN_WIDTH);
				dy = BASE_Y - LIMIT_Y;
			}
			if (dy <= 0) break;
			d = sqrt(dx * dx + dy * dy);

			dShootingEggX = -dx / d;
			dShootingEggY = -dy / d;

			shootingLineEndX = (int) (0.5 * SCREEN_WIDTH + dShootingEggX * 150);
			shootingLineEndY = (int) (BASE_Y + dShootingEggY * 150);

			shootingEggState = AIRBORNE;
		}
		else if (dy > 0) {
			if (abs(dy / dx) < (BASE_Y - LIMIT_Y) / (0.5 * SCREEN_WIDTH)) {
				dx = (dx < 0 ? -0.5 * SCREEN_WIDTH : 0.5 * SCREEN_WIDTH);
				dy = BASE_Y - LIMIT_Y;
				d = sqrt(dx * dx + dy * dy);
			}
			dShootingEggX = -dx / d;
			dShootingEggY = -dy / d;

			shootingLineEndX = (int) (0.5 * SCREEN_WIDTH + dShootingEggX * 150);
			shootingLineEndY = (int) (BASE_Y + dShootingEggY * 150);
		}
		break;
	case AIRBORNE:
		if (shootingEggX + 0.5 * EGG_WIDTH >= SCREEN_WIDTH
			|| shootingEggX - 0.5 * EGG_WIDTH <= 0) {
			dShootingEggX = -dShootingEggX;
		}
		shootingEggX += dShootingEggX * dShootingEgg;
		shootingEggY += dShootingEggY * dShootingEgg;
		break;
	default:
		break;
	}

}

void Screen1View::renderShootingEgg() {
	shootingEgg.setBitmap(touchgfx::Bitmap(shootingEggBitmapID));
	shootingEgg.moveTo((int)std::round(shootingEggX - 0.5 * EGG_WIDTH), std::round(shootingEggY - 0.5 * EGG_HEIGHT));
	//shootingEgg.invalidate();
}

void Screen1View::initializeNextShootingEgg() {
	nextShootingEggBitmapID = generateRandomEggBitmapID();
	nextShootingEgg.setXY(10, BASE_Y - 0.5 * EGG_HEIGHT);
	nextShootingEgg.setVisible(true);
}
void Screen1View::updateNextShootingEgg() {
	//sampleEgg.moveTo(sampleEgg.getX(), sampleEgg.getY() + 2);
}

void Screen1View::renderNextShootingEgg() {
	nextShootingEgg.setBitmap(touchgfx::Bitmap(nextShootingEggBitmapID));
	//nextShootingEgg.invalidate();
}

void Screen1View::initializeShootingLine() {
	shootingLine.setStart((int)(0.5 * SCREEN_WIDTH), BASE_Y);
	shootingLine.setVisible(false);
	shootingLine.invalidate();
}

void Screen1View::updateShootingLine() {
	if (shootingEggState == READY
		|| shootingEggState == AIRBORNE) {
		shootingLine.setEnd(shootingLineEndX, shootingLineEndY);
	}
}

void Screen1View::renderShootingLine() {
	if (shootingEggState == READY || shootingEggState == AIRBORNE) {
		shootingLine.setVisible(true);
		shootingLine.invalidate();
	}
	else {
		shootingLine.setVisible(false);
		shootingLine.invalidate();
	}
}
