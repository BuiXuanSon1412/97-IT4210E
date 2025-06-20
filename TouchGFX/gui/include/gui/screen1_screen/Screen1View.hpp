#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <images/BitmapDatabase.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void handleTickEvent();
    void onPlayButtonClicked();

protected:
    // varibles to control game flow
    const int framePerSecond = 30;
    float dEggBatchY;
    bool gameState;
    uint32_t lastUpdateTickCount;

    // seed for random generator
    uint32_t seed;

    // variables to manage the batch of eggs
    touchgfx::Image eggBatch[NUM_ROWS][NUM_COLS];
    bool eggBatchState[NUM_ROWS][NUM_COLS];
    uint16_t eggBatchBitmapID[NUM_ROWS][NUM_COLS];
    int eggBatchDegree[NUM_ROWS][NUM_COLS];
    float eggBatchY[NUM_ROWS];
    int startRowIndex;

    // variable to manage the shooting egg and the next shooting egg
    touchgfx::Image shootingEgg, nextShootingEgg;
    EggState shootingEggState;
    uint16_t shootingEggBitmapID, nextShootingEggBitmapID;
    float shootingEggX, shootingEggY;
    float dShootingEggX, dShootingEggY, dShootingEgg;
    int leftToRight;
    int shootingLineEndX, shootingLineEndY;
    
private:
    
};

#endif // SCREEN1VIEW_HPP
