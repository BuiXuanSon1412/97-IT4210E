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

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::onPlayButtonClicked() {
	
}

void Screen1View::handleTickEvent() {
	
}
