/*
 * FluxGarage RoboEyes for OLED Displays V 1.1.1
 * Draws smoothly animated robot eyes on OLED displays, based on the Adafruit GFX 
 * library's graphics primitives, such as rounded rectangles and triangles.
 *   
 * Copyright (C) 2024-2025 Dennis Hoelscher
 * www.fluxgarage.com
 * www.youtube.com/@FluxGarage
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef _FLUXGARAGE_ROBOEYES_H
#define _FLUXGARAGE_ROBOEYES_H


// Display colors
uint8_t BGCOLOR = 0; // background and overlays
uint8_t MAINCOLOR = 1; // drawings

// For mood type switch
#define DEFAULT 0
#define TIRED 1
#define ANGRY 2
#define HAPPY 3
#define ALERT 4
#define BORED 5
#define DESPAIR 6
#define DISORIENTED 7
#define EXCITED 8
#define FOCUSED 9
#define FURIOUS 10
#define SAD 11
#define SCARED 12
#define SLEEPY 13
#define SURPRISED 14
#define WORRIED 15
#define ANNOYED 16
#define CONFUSED 17
#define CURIOUS 18
#define PROUD 19
#define RELAXED 20

// For turning things on or off
#define ON 1
#define OFF 0

// For switch "predefined positions"
#define N 1 // north, top center
#define NE 2 // north-east, top right
#define E 3 // east, middle right
#define SE 4 // south-east, bottom right
#define S 5 // south, bottom center
#define SW 6 // south-west, bottom left
#define W 7 // west, middle left
#define NW 8 // north-west, top left 
// for middle center set "DEFAULT"


// Constructor: takes a reference to the active Adafruit display object (e.g., Adafruit_SSD1327)
// Eg: roboEyes<Adafruit_SSD1327> = eyes(display);
template<typename AdafruitDisplay>
class RoboEyes
{
private:

// Yes, everything is currently still accessible. Be responsible and don't mess things up :)

public:

// Reference to Adafruit display object
AdafruitDisplay *display;

// For general setup - screen size and max. frame rate
int screenWidth = 128; // OLED display width, in pixels
int screenHeight = 64; // OLED display height, in pixels
int frameInterval = 20; // default value for 50 frames per second (1000/50 = 20 milliseconds)
unsigned long fpsTimer = 0; // for timing the frames per second

// For controlling mood types and expressions
bool tired = 0;
bool angry = 0;
bool happy = 0;
bool alert = 0;
bool bored = 0;
bool despair = 0;
bool disoriented = 0;
bool excited = 0;
bool focused = 0;
bool furious = 0;
bool sad = 0;
bool scared = 0;
bool sleepy = 0;
bool surprised = 0;
bool worried = 0;
bool annoyed = 0;
bool confusedMood = 0; // confused expression mood (not to be mixed up with the "confused" shake animation flag)
bool curiousMood = 0; // curious expression mood (not to be mixed up with the "curious" gaze feature flag)
bool proudMood = 0;
bool relaxedMood = 0;
bool curious = 0; // if true, draw the outer eye larger when looking left or right
bool cyclops = 0; // if true, draw only one eye
bool eyeL_open = 0; // left eye opened or closed?
bool eyeR_open = 0; // right eye opened or closed?


//*********************************************************************************************
//  Eyes Geometry
//*********************************************************************************************

// EYE LEFT - size and border radius
int eyeLwidthDefault = 36;
int eyeLheightDefault = 36;
int eyeLwidthCurrent = eyeLwidthDefault;
int eyeLheightCurrent = 1; // start with closed eye, otherwise set to eyeLheightDefault
int eyeLwidthNext = eyeLwidthDefault;
int eyeLheightNext = eyeLheightDefault;
int eyeLheightOffset = 0;
// Border Radius
byte eyeLborderRadiusDefault = 8;
byte eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
byte eyeLborderRadiusNext = eyeLborderRadiusDefault;

// EYE RIGHT - size and border radius
int eyeRwidthDefault = eyeLwidthDefault;
int eyeRheightDefault = eyeLheightDefault;
int eyeRwidthCurrent = eyeRwidthDefault;
int eyeRheightCurrent = 1; // start with closed eye, otherwise set to eyeRheightDefault
int eyeRwidthNext = eyeRwidthDefault;
int eyeRheightNext = eyeRheightDefault;
int eyeRheightOffset = 0;
// Border Radius
byte eyeRborderRadiusDefault = 8;
byte eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
byte eyeRborderRadiusNext = eyeRborderRadiusDefault;

// EYE LEFT - Coordinates
int eyeLxDefault = ((screenWidth)-(eyeLwidthDefault+spaceBetweenDefault+eyeRwidthDefault))/2;
int eyeLyDefault = ((screenHeight-eyeLheightDefault)/2);
int eyeLx = eyeLxDefault;
int eyeLy = eyeLyDefault;
int eyeLxNext = eyeLx;
int eyeLyNext = eyeLy;

// EYE RIGHT - Coordinates
int eyeRxDefault = eyeLx+eyeLwidthCurrent+spaceBetweenDefault;
int eyeRyDefault = eyeLy;
int eyeRx = eyeRxDefault;
int eyeRy = eyeRyDefault;
int eyeRxNext = eyeRx;
int eyeRyNext = eyeRy;

// BOTH EYES 
// Eyelid top size
byte eyelidsHeightMax = eyeLheightDefault/2; // top eyelids max height
byte eyelidsTiredHeight = 0;
byte eyelidsTiredHeightNext = eyelidsTiredHeight;
byte eyelidsAngryHeight = 0;
byte eyelidsAngryHeightNext = eyelidsAngryHeight;
// Bottom happy eyelids offset
byte eyelidsHappyBottomOffsetMax = (eyeLheightDefault/2)+3;
byte eyelidsHappyBottomOffset = 0;
byte eyelidsHappyBottomOffsetNext = 0;
// Alert eyelids (raised)
byte eyelidsAlertHeight = 0;
byte eyelidsAlertHeightNext = 0;
// Bored eyelids (droopy outer)
byte eyelidsBoredHeight = 0;
byte eyelidsBoredHeightNext = 0;
// Despair eyelids (extreme sad)
byte eyelidsDespairHeight = 0;
byte eyelidsDespairHeightNext = 0;
// Disoriented (asymmetric)
byte eyelidsDisorientedHeightL = 0;
byte eyelidsDisorientedHeightLNext = 0;
byte eyelidsDisorientedHeightR = 0;
byte eyelidsDisorientedHeightRNext = 0;
// Excited (wide)
byte eyelidsExcitedHeight = 0;
byte eyelidsExcitedHeightNext = 0;
// Focused (narrowed)
byte eyelidsFocusedHeight = 0;
byte eyelidsFocusedHeightNext = 0;
// Furious (extreme angry)
byte eyelidsFuriousHeight = 0;
byte eyelidsFuriousHeightNext = 0;
// Sad (droopy outer)
byte eyelidsSadHeight = 0;
byte eyelidsSadHeightNext = 0;
// Scared (raised brows)
byte eyelidsScaredHeight = 0;
byte eyelidsScaredHeightNext = 0;
// Sleepy (heavy lids)
byte eyelidsSleepyHeight = 0;
byte eyelidsSleepyHeightNext = 0;
// Surprised (max height, round)
byte eyelidsSurprisedHeight = 0;
byte eyelidsSurprisedHeightNext = 0;
// Worried (furrowed inner)
byte eyelidsWorriedHeight = 0;
byte eyelidsWorriedHeightNext = 0;
// Annoyed (half-closed asymmetric)
byte eyelidsAnnoyedHeight = 0;
byte eyelidsAnnoyedHeightNext = 0;
// Confused (asymmetric eyelid positions)
byte eyelidsConfusedHeightL = 0;
byte eyelidsConfusedHeightLNext = 0;
byte eyelidsConfusedHeightR = 0;
byte eyelidsConfusedHeightRNext = 0;
// Curious (raised inner brows)
byte eyelidsCuriousHeight = 0;
byte eyelidsCuriousHeightNext = 0;
// Proud bottom eyelids (narrowed lower lids)
byte eyelidsProudBottomOffset = 0;
byte eyelidsProudBottomOffsetNext = 0;
// Relaxed top eyelids
byte eyelidsRelaxedHeight = 0;
byte eyelidsRelaxedHeightNext = 0;
// Space between eyes
int spaceBetweenDefault = 10;
int spaceBetweenCurrent = spaceBetweenDefault;
int spaceBetweenNext = 10;

// Mood-specific geometry targets (computed once per mood change)
int mood_eyeLheightTarget = eyeLheightDefault;
int mood_eyeRheightTarget = eyeRheightDefault;
int mood_eyeLwidthTarget = eyeLwidthDefault;
int mood_eyeRwidthTarget = eyeRwidthDefault;
byte mood_eyeLborderRadiusTarget = eyeLborderRadiusDefault;
byte mood_eyeRborderRadiusTarget = eyeRborderRadiusDefault;
int mood_eyeLyOffset = 0;
int mood_eyeRyOffset = 0;
int mood_spaceBetweenTarget = spaceBetweenDefault;
bool mood_usesHeightOffset = false;
bool mood_autoblinker = false;
int mood_blinkInterval = 1;
int mood_blinkVariation = 4;
bool mood_hFlicker = false;
byte mood_hFlickerAmplitude = 0;
bool mood_vFlicker = false;
byte mood_vFlickerAmplitude = 0;
bool mood_sweat = false;
unsigned long moodChangeTime = 0;
unsigned char currentMood = DEFAULT;
unsigned char previousMood = DEFAULT;
bool moodInitialized = false;

// Smoothing factors for fluid transitions
float geometrySmoothing = 0.15f;  // slower = smoother
float eyelidSmoothing = 0.2f;
float positionSmoothing = 0.15f;

//*********************************************************************************************
//  Macro Animations
//*********************************************************************************************

// Animation - horizontal flicker/shiver
bool hFlicker = 0;
bool hFlickerAlternate = 0;
byte hFlickerAmplitude = 2;

// Animation - vertical flicker/shiver
bool vFlicker = 0;
bool vFlickerAlternate = 0;
byte vFlickerAmplitude = 10;

// Animation - breathing: eyes slowly moving up and down
bool breathing = 0;
byte breathingAmplitude = 2; // in pixels, up and down from the eyes' position
unsigned int breathingPeriod = 4000; // one breath (up and down) in milliseconds
float breathingPhase = 0; // 0 ... 1 within the current breath
unsigned long breathingTimer = 0; // when the phase was last advanced
int breathingOffset = 0; // vertical offset for the current frame, only while drawing

// Eyelashes: three lashes at the upper outer corner of each eye
bool eyelashes = 0;
byte eyelashLength = 12; // in pixels

// Closed eyes: how thick the line of a closed eye is, in pixels (also the
// moment of a blink)
byte closedHeight = 1;

// Animation - auto blinking
bool autoblinker = 0; // activate auto blink animation
int blinkInterval = 1; // basic interval between each blink in full seconds
int blinkIntervalVariation = 4; // interval variaton range in full seconds, random number inside of given range will be add to the basic blinkInterval, set to 0 for no variation
unsigned long blinktimer = 0; // for organising eyeblink timing

// Animation - idle mode: eyes looking in random directions
bool idle = 0;
bool idleHorizontalOnly = 0; // only move left and right, keep the height (e.g. looking down while moving)
int idleInterval = 1; // basic interval between each eye repositioning in full seconds
int idleIntervalVariation = 3; // interval variaton range in full seconds, random number inside of given range will be add to the basic idleInterval, set to 0 for no variation
unsigned long idleAnimationTimer = 0; // for organising eyeblink timing

// Lean: pins the eyes to an edge, e.g. when the device is tilted; -1 = left
// (top), +1 = right (bottom), 0 = free. Overrides positions and idle moves on
// that axis; animations play on top.
int8_t leanX = 0;
int8_t leanY = 0;

// Animation - eyes confused: eyes shaking left and right
bool confused = 0;
unsigned long confusedAnimationTimer = 0;
int confusedAnimationDuration = 500;
bool confusedToggle = 1;

// Animation - eyes laughing: eyes shaking up and down
bool laugh = 0;
unsigned long laughAnimationTimer = 0;
int laughAnimationDuration = 500;
bool laughToggle = 1;

// Animation - eyes startled: eyes jump up, open wide and shiver up and down
bool startled = 0;
unsigned long startledAnimationTimer = 0;
int startledAnimationDuration = 1000;
byte startledHeightOffset = 15; // how much taller the eyes get, in pixels
byte startledJump = 8; // how far the eyes jump up at the start, in pixels; they sink back during the animation
byte startledShiverAmplitude = 4; // up and down, in pixels
int startledJumpOffset = 0; // the jump in the current frame, only while drawing

// Animation - eyes fluttering: the eyelids twitch, e.g. closed eyes while
// dreaming or snoring
bool flutter = 0;
unsigned long flutterAnimationTimer = 0;
int flutterAnimationDuration = 900;
int flutterInterval = 80; // milliseconds between the twitches
byte flutterAmplitude = 0; // how much taller the eyes get with each twitch, in pixels (set by anim_flutter)

// Animation - dozing off, fighting against sleep: the eyelids sink, jerk back
// open, sink further, jerk back again, fall shut, and the eyes pop open a bit
// wider (see dozeShutAt() for the timing); the eyes droop down with the lids
bool doze = 0;
unsigned long dozeAnimationTimer = 0;
byte dozeDroop = 6; // how far the eyes droop down when shut, in pixels
int dozeDroopOffset = 0; // the droop in the current frame, only while drawing

// Animation - perky wink: one eye shuts for a moment, the other one squints,
// and both hop up a little; then a short stare
bool winking = 0; // also during the stare
bool winkLeft = 0; // which eye winks
unsigned long winkAnimationTimer = 0;
int winkDuration = 400; // how long the winking eye stays shut, in milliseconds
int winkStare = 1500; // after the wink the eyes hold still this long (no blinks, no glances), in milliseconds
byte winkSquint = 8; // how much shorter the other eye gets, in pixels
byte winkHop = 4; // how far both eyes hop up, in pixels
int winkHopOffset = 0; // the hop in the current frame, only while drawing

int drawOffsetY = 0; // breathing, jump, hop and droop in the current frame, only while drawing

// Animation - sweat on the forehead
bool sweat = 0;
byte sweatBorderradius = 3;

// Sweat drop 1
int sweat1XPosInitial = 2;
int sweat1XPos;
float sweat1YPos = 2;
int sweat1YPosMax;
float sweat1Height = 2;
float sweat1Width = 1;

// Sweat drop 2
int sweat2XPosInitial = 2;
int sweat2XPos;
float sweat2YPos = 2;
int sweat2YPosMax;
float sweat2Height = 2;
float sweat2Width = 1;

// Sweat drop 3
int sweat3XPosInitial = 2;
int sweat3XPos;
float sweat3YPos = 2;
int sweat3YPosMax;
float sweat3Height = 2;
float sweat3Width = 1;


//*********************************************************************************************
//  GENERAL METHODS
//*********************************************************************************************

RoboEyes(AdafruitDisplay &disp) : display(&disp) {};

// Startup RoboEyes with defined screen-width, screen-height and max. frames per second
void begin(int width, int height, byte frameRate) {
	screenWidth = width; // OLED display width, in pixels
	screenHeight = height; // OLED display height, in pixels
  display->clearDisplay(); // clear the display buffer
  display->display(); // show empty screen
  eyeLheightCurrent = 1; // start with closed eyes
  eyeRheightCurrent = 1; // start with closed eyes
  setFramerate(frameRate); // calculate frame interval based on defined frameRate
}

void update(){
  // Limit drawing updates to defined max framerate
  if(millis()-fpsTimer >= frameInterval){
    drawEyes();
    fpsTimer = millis();
  }
}


//*********************************************************************************************
//  SETTERS METHODS
//*********************************************************************************************

// Calculate frame interval based on defined frameRate
void setFramerate(byte fps){
  frameInterval = 1000/fps;
}

// Set color values
void setDisplayColors(uint8_t background, uint8_t main) {
  BGCOLOR = background; // background and overlays, choose 0 for monochrome displays and 0x00 for grayscale displays such as SSD1322
  MAINCOLOR = main; // drawings, choose 1 for monochrome displays and 0x0F for grayscale displays such as SSD1322 (0x0F = maximum brightness)
}

void setWidth(byte leftEye, byte rightEye) {
	eyeLwidthNext = leftEye;
	eyeRwidthNext = rightEye;
  eyeLwidthDefault = leftEye;
  eyeRwidthDefault = rightEye;
}

void setHeight(byte leftEye, byte rightEye) {
	eyeLheightNext = leftEye;
	eyeRheightNext = rightEye;
  eyeLheightDefault = leftEye;
  eyeRheightDefault = rightEye;
}

// Set border radius for left and right eye
void setBorderradius(byte leftEye, byte rightEye) {
	eyeLborderRadiusNext = leftEye;
	eyeRborderRadiusNext = rightEye;
  eyeLborderRadiusDefault = leftEye;
  eyeRborderRadiusDefault = rightEye;
}

// Set space between the eyes, can also be negative
void setSpacebetween(int space) {
  spaceBetweenNext = space;
  spaceBetweenDefault = space;
}

// Set mood expression
void setMood(unsigned char mood)
  {
    switch (mood)
    {
    case TIRED:
      tired=1; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case ANGRY:
      tired=0; angry=1; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case HAPPY:
      tired=0; angry=0; happy=1; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case ALERT:
      tired=0; angry=0; happy=0; alert=1; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case BORED:
      tired=0; angry=0; happy=0; alert=0; bored=1; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case DESPAIR:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=1; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case DISORIENTED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=1; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case EXCITED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=1; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case FOCUSED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=1; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case FURIOUS:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=1; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case SAD:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=1; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case SCARED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=1; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case SLEEPY:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=1; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case SURPRISED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=1; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case WORRIED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=1; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case ANNOYED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=1; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case CONFUSED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=1; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    case CURIOUS:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=1; proudMood=0; relaxedMood=0;
      break;
    case PROUD:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=1; relaxedMood=0;
      break;
    case RELAXED:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=1;
      break;
    default:
      tired=0; angry=0; happy=0; alert=0; bored=0; despair=0; disoriented=0; excited=0; focused=0; furious=0; sad=0; scared=0; sleepy=0; surprised=0; worried=0; annoyed=0; confusedMood=0; curiousMood=0; proudMood=0; relaxedMood=0;
      break;
    }
    setMoodGeometry(mood);
  }

// Set predefined position
void setPosition(unsigned char position)
  {
    switch (position)
    {
    case N:
      // North, top center
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = getScreenTop_Y();
      break;
    case NE:
      // North-east, top right
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = getScreenTop_Y();
      break;
    case E:
      // East, middle right
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = (getScreenTop_Y()+getScreenConstraint_Y())/2;
      break;
    case SE:
      // South-east, bottom right
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = getScreenConstraint_Y();
      break;
    case S:
      // South, bottom center
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = getScreenConstraint_Y();
      break;
    case SW:
      // South-west, bottom left
      eyeLxNext = 0;
      eyeLyNext = getScreenConstraint_Y();
      break;
    case W:
      // West, middle left
      eyeLxNext = 0;
      eyeLyNext = (getScreenTop_Y()+getScreenConstraint_Y())/2;
      break;
    case NW:
      // North-west, top left
      eyeLxNext = 0;
      eyeLyNext = getScreenTop_Y();
      break;
    default:
      // Middle center
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = (getScreenTop_Y()+getScreenConstraint_Y())/2;
      break;
    }
  }

// Set automated eye blinking, minimal blink interval in full seconds and blink interval variation range in full seconds
void setAutoblinker(bool active, int interval, int variation){
  autoblinker = active;
  blinkInterval = interval;
  blinkIntervalVariation = variation;
}
void setAutoblinker(bool active){
  autoblinker = active;
}

// Set idle mode - automated eye repositioning, minimal time interval in full seconds and time interval variation range in full seconds
void setIdleMode(bool active, int interval, int variation){
  idle = active;
  idleInterval = interval;
  idleIntervalVariation = variation;
}
void setIdleMode(bool active) {
  idle = active;
}

// Set idle mode to move the eyes only left and right, keeping their height
// (e.g. set by setPosition(S) to look down while looking around)
void setIdleHorizontalOnly(bool horizontalOnly) {
  idleHorizontalOnly = horizontalOnly;
}

// Set lean - pins the eyes to an edge: sideways -1 = left, +1 = right;
// forward -1 = top, +1 = bottom; 0 = free on that axis
void setLean(int8_t sideways, int8_t forward) {
  leanX = sideways;
  leanY = forward;
}

// Set curious mode - the respectively outer eye gets larger when looking left or right
void setCuriosity(bool curiousBit) {
  curious = curiousBit;
}

// Set cyclops mode - show only one eye 
void setCyclops(bool cyclopsBit) {
  cyclops = cyclopsBit;
}

// Set horizontal flickering (displacing eyes left/right)
void setHFlicker (bool flickerBit, byte Amplitude) {
  hFlicker = flickerBit; // turn flicker on or off
  hFlickerAmplitude = Amplitude; // define amplitude of flickering in pixels
}
void setHFlicker (bool flickerBit) {
  hFlicker = flickerBit; // turn flicker on or off
}

// Set vertical flickering (displacing eyes up/down)
void setVFlicker (bool flickerBit, byte Amplitude) {
  vFlicker = flickerBit; // turn flicker on or off
  vFlickerAmplitude = Amplitude; // define amplitude of flickering in pixels
}
void setVFlicker (bool flickerBit) {
  vFlicker = flickerBit; // turn flicker on or off
}

// Set breathing (eyes slowly moving up and down, like breathing): amplitude
// in pixels, one breath in milliseconds
void setBreathing (bool breathingBit, byte amplitude, unsigned int period) {
  breathing = breathingBit; // turn breathing on or off
  breathingAmplitude = amplitude; // how far up and down in pixels
  breathingPeriod = period > 0 ? period : 1; // duration of one breath in milliseconds
}
void setBreathing (bool breathingBit) {
  breathing = breathingBit; // turn breathing on or off
}

// Set eyelashes: three lashes at the upper outer corner of each eye, length
// in pixels; they follow the eyes when blinking, moving and with the eyelids
void setEyelashes (bool eyelashesBit, byte length) {
  eyelashes = eyelashesBit; // turn eyelashes on or off
  eyelashLength = length; // length of the lashes in pixels
}
void setEyelashes (bool eyelashesBit) {
  eyelashes = eyelashesBit; // turn eyelashes on or off
}

// Set how thick closed eyes are, in pixels (default 1: a thin line); blinks
// close to this height too
void setClosedHeight (byte height) {
  closedHeight = height > 0 ? height : 1;
}

void setSweat (bool sweatBit) {
  sweat = sweatBit; // turn sweat on or off
}


//*********************************************************************************************
//  GETTERS METHODS
//*********************************************************************************************

// Returns the max x position for left eye
int getScreenConstraint_X(){
  return screenWidth-eyeLwidthCurrent-spaceBetweenCurrent-eyeRwidthCurrent;
} 

// Distance the eyes keep to the top and bottom edge: breathing moves them up
// and down by its amplitude, so they don't leave the screen
int getScreenMargin_Y(){
 return breathing ? breathingAmplitude : 0;
}

// Returns the min y position for left eye
int getScreenTop_Y(){
 return getScreenMargin_Y();
}

// Returns the max y position for left eye
int getScreenConstraint_Y(){
 return screenHeight-eyeLheightDefault-getScreenMargin_Y(); // using default height here, because height will vary when blinking and in curious mode
}

// Helper: fluid step toward a target, always moving at least 1 px/frame so interpolation never stalls
int moodStep(int current, int target, float smoothing) {
  int diff = target - current;
  if (diff == 0) return current;
  int step = (int)(diff * smoothing);
  if (diff > 0 && step < 1) step = 1;
  if (diff < 0 && step > -1) step = -1;
  return current + step;
}

// Helper: apply mood-specific geometry targets smoothly
void applyMoodGeometry() {
  // Smooth interpolation toward mood targets (never stalls, keeps frames fluid)
  eyeLheightNext = moodStep(eyeLheightNext, mood_eyeLheightTarget, geometrySmoothing);
  eyeRheightNext = moodStep(eyeRheightNext, mood_eyeRheightTarget, geometrySmoothing);
  eyeLwidthNext = moodStep(eyeLwidthNext, mood_eyeLwidthTarget, geometrySmoothing);
  eyeRwidthNext = moodStep(eyeRwidthNext, mood_eyeRwidthTarget, geometrySmoothing);
  eyeLborderRadiusNext = moodStep(eyeLborderRadiusNext, mood_eyeLborderRadiusTarget, geometrySmoothing);
  eyeRborderRadiusNext = moodStep(eyeRborderRadiusNext, mood_eyeRborderRadiusTarget, geometrySmoothing);
  spaceBetweenNext = moodStep(spaceBetweenNext, mood_spaceBetweenTarget, geometrySmoothing);

  // Vertical offset: only applied while the active mood requests it,
  // otherwise the user/idle position stays in control of eyeLyNext/eyeRyNext.
  if (mood_usesHeightOffset){
    eyeLyNext = eyeLyDefault + mood_eyeLyOffset;
    eyeRyNext = eyeRyDefault + mood_eyeRyOffset;
  }

  // Apply macro animations only when the active mood explicitly requests them.
  // One-shot animations (laugh, confused) and manual setters keep control otherwise.
  if (mood_autoblinker){
    autoblinker = true;
    blinkInterval = mood_blinkInterval;
    blinkIntervalVariation = mood_blinkVariation;
  }
  if (mood_hFlicker){
    hFlicker = true;
    hFlickerAmplitude = mood_hFlickerAmplitude;
  }
  if (mood_vFlicker){
    vFlicker = true;
    vFlickerAmplitude = mood_vFlickerAmplitude;
  }
  if (mood_sweat){
    sweat = true;
  }
}

// Helper: set mood geometry targets
void setMoodGeometry(unsigned char mood) {
  previousMood = currentMood;
  bool previousUsedOffset = mood_usesHeightOffset;
  currentMood = mood;
  moodChangeTime = millis();
  moodInitialized = true;

  // When a mood that forced an animation ends, release that animation
  if (previousMood == EXCITED || previousMood == SLEEPY || previousMood == BORED || previousMood == SAD || previousMood == DESPAIR || previousMood == RELAXED){ autoblinker = false; }
  if (previousMood == DISORIENTED || previousMood == FURIOUS){ hFlicker = false; }
  if (previousMood == SCARED){ vFlicker = false; }
  if (previousMood == DESPAIR){ sweat = false; }

  // Reset all mood targets to defaults first
  mood_eyeLheightTarget = eyeLheightDefault;
  mood_eyeRheightTarget = eyeRheightDefault;
  mood_eyeLwidthTarget = eyeLwidthDefault;
  mood_eyeRwidthTarget = eyeRwidthDefault;
  mood_eyeLborderRadiusTarget = eyeLborderRadiusDefault;
  mood_eyeRborderRadiusTarget = eyeRborderRadiusDefault;
  mood_eyeLyOffset = 0;
  mood_eyeRyOffset = 0;
  mood_usesHeightOffset = false;
  mood_spaceBetweenTarget = spaceBetweenDefault;
  mood_autoblinker = false;
  mood_blinkInterval = 1;
  mood_blinkVariation = 4;
  mood_hFlicker = false;
  mood_hFlickerAmplitude = 0;
  mood_vFlicker = false;
  mood_vFlickerAmplitude = 0;
  mood_sweat = false;

  // Restore centered position when leaving a mood that owned the vertical offset
  // and the incoming mood does not take over the vertical position itself
  if (previousUsedOffset && !mood_usesHeightOffset){
    eyeLyNext = eyeLyDefault;
    eyeRyNext = eyeRyDefault;
  }

  switch (mood) {
    case ALERT:
      mood_eyeLheightTarget = eyeLheightDefault + 6;
      mood_eyeRheightTarget = eyeRheightDefault + 6;
      mood_eyeLborderRadiusTarget = 4;
      mood_eyeRborderRadiusTarget = 4;
      mood_eyeLyOffset = -3;
      mood_eyeRyOffset = -3;
      mood_usesHeightOffset = true;
      break;
    case BORED:
      mood_eyeLheightTarget = eyeLheightDefault / 2;
      mood_eyeRheightTarget = eyeRheightDefault / 2;
      mood_eyeLyOffset = 2;
      mood_eyeRyOffset = 2;
      mood_usesHeightOffset = true;
      mood_autoblinker = true;
      mood_blinkInterval = 4;
      mood_blinkVariation = 2;
      break;
    case DESPAIR:
      mood_eyeLheightTarget = eyeLheightDefault - 6;
      mood_eyeRheightTarget = eyeRheightDefault - 6;
      mood_eyeLyOffset = 6;
      mood_eyeRyOffset = 6;
      mood_usesHeightOffset = true;
      mood_sweat = true;
      mood_autoblinker = true;
      mood_blinkInterval = 5;
      mood_blinkVariation = 3;
      break;
    case DISORIENTED:
      mood_eyeLwidthTarget = eyeLwidthDefault - 4;
      mood_eyeRwidthTarget = eyeRwidthDefault + 4;
      mood_hFlicker = true;
      mood_hFlickerAmplitude = 6;
      break;
    case EXCITED:
      mood_eyeLheightTarget = eyeLheightDefault + 10;
      mood_eyeRheightTarget = eyeRheightDefault + 10;
      mood_eyeLborderRadiusTarget = eyeLheightDefault/2;
      mood_eyeRborderRadiusTarget = eyeRheightDefault/2;
      mood_eyeLyOffset = -4;
      mood_eyeRyOffset = -4;
      mood_usesHeightOffset = true;
      mood_autoblinker = true;
      mood_blinkInterval = 1;
      mood_blinkVariation = 2;
      break;
    case FOCUSED:
      mood_eyeLheightTarget = eyeLheightDefault / 3;
      mood_eyeRheightTarget = eyeRheightDefault / 3;
      mood_eyeLwidthTarget = eyeLwidthDefault + 6;
      mood_eyeRwidthTarget = eyeRwidthDefault + 6;
      mood_eyeLborderRadiusTarget = 2;
      mood_eyeRborderRadiusTarget = 2;
      mood_eyeLyOffset = 1;
      mood_eyeRyOffset = 1;
      mood_usesHeightOffset = true;
      break;
    case FURIOUS:
      mood_eyeLborderRadiusTarget = 2;
      mood_eyeRborderRadiusTarget = 2;
      mood_hFlicker = true;
      mood_hFlickerAmplitude = 4;
      break;
    case SAD:
      mood_eyeLheightTarget = eyeLheightDefault - 4;
      mood_eyeRheightTarget = eyeRheightDefault - 4;
      mood_eyeLyOffset = 5;
      mood_eyeRyOffset = 5;
      mood_usesHeightOffset = true;
      mood_autoblinker = true;
      mood_blinkInterval = 3;
      mood_blinkVariation = 2;
      break;
    case SCARED:
      mood_eyeLheightTarget = eyeLheightDefault + 12;
      mood_eyeRheightTarget = eyeRheightDefault + 12;
      mood_eyeLborderRadiusTarget = eyeLheightDefault/2;
      mood_eyeRborderRadiusTarget = eyeRheightDefault/2;
      mood_eyeLyOffset = -8;
      mood_eyeRyOffset = -8;
      mood_usesHeightOffset = true;
      mood_vFlicker = true;
      mood_vFlickerAmplitude = 3;
      break;
    case SLEEPY:
      mood_eyeLheightTarget = eyeLheightDefault / 4;
      mood_eyeRheightTarget = eyeRheightDefault / 4;
      mood_eyeLyOffset = 4;
      mood_eyeRyOffset = 4;
      mood_usesHeightOffset = true;
      mood_autoblinker = true;
      mood_blinkInterval = 5;
      mood_blinkVariation = 3;
      break;
    case SURPRISED:
      mood_eyeLheightTarget = eyeLheightDefault + 14;
      mood_eyeRheightTarget = eyeRheightDefault + 14;
      mood_eyeLborderRadiusTarget = eyeLheightDefault/2;
      mood_eyeRborderRadiusTarget = eyeRheightDefault/2;
      mood_eyeLyOffset = -10;
      mood_eyeRyOffset = -10;
      mood_usesHeightOffset = true;
      break;
    case WORRIED:
      mood_eyeLheightTarget = eyeLheightDefault - 2;
      mood_eyeRheightTarget = eyeRheightDefault - 2;
      mood_eyeLborderRadiusTarget = 4;
      mood_eyeRborderRadiusTarget = 4;
      mood_eyeLyOffset = 2;
      mood_eyeRyOffset = 2;
      mood_usesHeightOffset = true;
      break;
    case ANNOYED:
      mood_eyeLheightTarget = eyeLheightDefault / 2;
      mood_eyeRheightTarget = eyeRheightDefault / 2;
      mood_eyeLyOffset = 3;
      mood_eyeRyOffset = -1;
      mood_usesHeightOffset = true;
      mood_spaceBetweenTarget = spaceBetweenDefault + 2;
      break;
    case CONFUSED:
      // Uneven, questioning look: asymmetric eye sizes and tilted vertical offsets
      mood_eyeRheightTarget = eyeRheightDefault - 4;
      mood_eyeRwidthTarget = eyeRwidthDefault + 2;
      mood_eyeLyOffset = -1;
      mood_eyeRyOffset = 2;
      mood_usesHeightOffset = true;
      break;
    case CURIOUS:
      // Slight eye tilt (left eye raised) + attentive gaze, inner brows are drawn as raised eyelids
      mood_eyeLheightTarget = eyeLheightDefault + 2;
      mood_eyeLyOffset = -3;
      mood_eyeRyOffset = 1;
      mood_usesHeightOffset = true;
      break;
    case PROUD:
      // Raised chin posture (eyes positioned higher) + slightly narrowed eyes
      mood_eyeLheightTarget = eyeLheightDefault - 4;
      mood_eyeRheightTarget = eyeRheightDefault - 4;
      mood_eyeLyOffset = -5;
      mood_eyeRyOffset = -5;
      mood_usesHeightOffset = true;
      break;
    case RELAXED:
      // Soft, gentle: full eye size, relaxed top lids drawn in drawEyes(), slow relaxed blinking
      mood_eyeLyOffset = 1;
      mood_eyeRyOffset = 1;
      mood_usesHeightOffset = true;
      mood_autoblinker = true;
      mood_blinkInterval = 6;
      mood_blinkVariation = 3;
      break;
    default:
      break;
  }
}

//*********************************************************************************************
//  BASIC ANIMATION METHODS
//*********************************************************************************************

// BLINKING FOR BOTH EYES AT ONCE
// Close both eyes
void close() {
	eyeLheightNext = closedHeight; // closing left eye
  eyeRheightNext = closedHeight; // closing right eye
  eyeL_open = 0; // left eye not opened (=closed)
	eyeR_open = 0; // right eye not opened (=closed)
}

// Open both eyes
void open() {
  eyeL_open = 1; // left eye opened - if true, drawEyes() will take care of opening eyes again
	eyeR_open = 1; // right eye opened
}

// Trigger eyeblink animation
void blink() {
  close();
  open();
}

// BLINKING FOR SINGLE EYES, CONTROL EACH EYE SEPARATELY
// Close eye(s)
void close(bool left, bool right) {
  if(left){
    eyeLheightNext = closedHeight; // blinking left eye
    eyeL_open = 0; // left eye not opened (=closed)
  }
  if(right){
      eyeRheightNext = closedHeight; // blinking right eye
      eyeR_open = 0; // right eye not opened (=closed)
  }
}

// Open eye(s)
void open(bool left, bool right) {
  if(left){
    eyeL_open = 1; // left eye opened - if true, drawEyes() will take care of opening eyes again
  }
  if(right){
    eyeR_open = 1; // right eye opened
  }
}

// Trigger eyeblink(s) animation
void blink(bool left, bool right) {
  close(left, right);
  open(left, right);
}

// WINK - BLINK WITH ONLY ONE EYE
// Trigger wink animation with one eye, keeping it fluid:
// the height transition is tweened frame by frame inside drawEyes(),
// so the eye closes and opens again smoothly without blocking
void wink(bool left) {
  if(left){
    close(true, false); // close left eye only (eyeLheightNext = 1, smoothly tweened)
    open(true, false); // flag left eye to reopen smoothly once fully closed
  } else {
    close(false, true); // close right eye only (eyeRheightNext = 1, smoothly tweened)
    open(false, true); // flag right eye to reopen smoothly once fully closed
  }
}


//*********************************************************************************************
//  MACRO ANIMATION METHODS
//*********************************************************************************************

// Play confused animation - one shot animation of eyes shaking left and right
void anim_confused() {
	confused = 1;
}

// Play laugh animation - one shot animation of eyes shaking up and down
void anim_laugh() {
  laugh = 1;
}

// Play startled animation - one shot animation of eyes jumping up, opening wide and
// shivering up and down, e.g. after a loud noise
void anim_startled() {
  startled = 1;
  startledAnimationTimer = millis();
  setVFlicker(1, startledShiverAmplitude);
}

// Play flutter animation - one shot animation of the eyelids twitching, e.g.
// closed eyes while snoring; amplitude: how much taller the eyes get with each
// twitch, in pixels
void anim_flutter(byte amplitude) {
  flutterAmplitude = amplitude;
  flutter = 1;
  flutterAnimationTimer = millis();
}

// Play doze animation - one shot animation of fighting against sleep, e.g. when
// tired: the eyelids sink, jerk back open twice, fall shut, then the eyes pop
// open a bit wider
void anim_doze() {
  doze = 1;
  dozeAnimationTimer = millis();
}

// The doze's course: how shut the eyes are (0 = open, 1 = shut, below 0 = a
// bit wider than open) at a time in milliseconds, straight lines between them
struct DozeKey {
  unsigned int ms;
  float shut;
};
static const DozeKey *dozeKeys(size_t &count) {
  static const DozeKey KEYS[] = {
    {0, 0},       // awake
    {700, 0.5},   // the eyelids sink halfway
    {850, 0.1},   // jerk back open
    {1700, 0.85}, // sink further
    {1850, 0.2},  // jerk back again, weaker
    {2900, 1},    // fall shut
    {3400, 1},    // stay shut
    {3550, -0.15},// pop open, a bit wider
    {3800, 0},    // back to normal
  };
  count = sizeof(KEYS) / sizeof(KEYS[0]);
  return KEYS;
}

unsigned long dozeDuration() const {
  size_t count;
  const DozeKey *keys = dozeKeys(count);
  return keys[count - 1].ms;
}

float dozeShutAt(unsigned long elapsed) const {
  size_t count;
  const DozeKey *keys = dozeKeys(count);
  for (size_t i = 1; i < count; i++) {
    if (elapsed < keys[i].ms) {
      const float along = float(elapsed - keys[i - 1].ms) / (keys[i].ms - keys[i - 1].ms);
      return keys[i - 1].shut + (keys[i].shut - keys[i - 1].shut) * along;
    }
  }
  return keys[count - 1].shut;
}

// Play wink animation - one shot animation of a perky wink: one eye (left =
// true: the left one) shuts for a moment, the other one squints, both hop up;
// then the eyes stare for a moment
void anim_wink(bool left) {
  winking = 1;
  winkLeft = left;
  winkAnimationTimer = millis();
  close(left, !left); // stays shut until the wink is over
}

//*********************************************************************************************
//  PRE-CALCULATIONS AND ACTUAL DRAWINGS
//*********************************************************************************************

void drawEyes(){

  //// PRE-CALCULATIONS - EYE SIZES AND VALUES FOR ANIMATION TWEENINGS ////
  // Smoothly drive eye geometry toward the current mood targets
  applyMoodGeometry();
  
  eyeLheightOffsetAnim = 0;
  eyeRheightOffsetAnim = 0;

  // ② Curious → escribe en Mood (persistente)
  if(curious && !doze && !winking){
    if(eyeLxNext<=10){eyeLheightOffsetAnim=8;}
    else if (eyeLxNext>=(getScreenConstraint_X()-10) && cyclops){eyeLheightOffsetAnim=8;}
    else{eyeLheightOffsetAnim=0;}
    if(eyeRxNext>=screenWidth-eyeRwidthCurrent-10){eyeRheightOffsetAnim=8; /* o Mood, ver nota */}
    else{eyeRheightOffsetAnim=0;}
  } else {
    eyeLheightOffsetAnim=0;
    eyeRheightOffsetAnim=0;
  }

  // Vertical size offset for larger eyes when looking left or right (curious gaze)
 /* if(curious){
    if(eyeLxNext<=10){eyeLheightOffsetAnim=8;}
    else if (eyeLxNext>=(getScreenConstraint_X()-10) && cyclops){eyeLheightOffsetAnim=8;}
    else{eyeLheightOffsetAnim=0;} // left eye
    if(eyeRxNext>=screenWidth-eyeRwidthCurrent-10){eyeRheightOffset=8;}
    else{eyeRheightOffset=0;} // right eye
  } else {
    eyeLheightOffsetAnim=0; // reset height offset for left eye
    eyeRheightOffset=0; // reset height offset for right eye
  } */

  // Startled - for the duration defined by startledAnimationDuration (default =
  // 1000ms) the eyes are taller, growing and shrinking smoothly through the
  // height offset, and they jump up at the start and sink back
  startledJumpOffset = 0;
  if(startled){
    const unsigned long elapsed = millis() - startledAnimationTimer;
    if(elapsed >= (unsigned long)startledAnimationDuration){
      setVFlicker(0, 0);
      startled = 0;
    } else {
      eyeLheightOffsetAnim += startledHeightOffset;
      eyeRheightOffsetAnim += startledHeightOffset;
      startledJumpOffset = lroundf(startledJump * (1 - float(elapsed) / startledAnimationDuration));
    }
  }

  // Flutter - for the duration defined by flutterAnimationDuration (default =
  // 900ms) the eyes get taller by flutterAmplitude every other flutterInterval,
  // so the eyelids twitch
  if(flutter){
    const unsigned long elapsed = millis() - flutterAnimationTimer;
    if(elapsed >= (unsigned long)flutterAnimationDuration){
      flutter = 0;
    } else if((elapsed / flutterInterval) % 2 == 0){
      eyeLheightOffsetAnim += flutterAmplitude;
      eyeRheightOffsetAnim += flutterAmplitude;
    }
  }

  // Doze - the eyes get shorter (down to closedHeight when shut) through a
  // negative height offset, which moves their top edge down like a sinking
  // eyelid, and they droop down a little; how shut they are follows
  // dozeShutAt(), then the animation ends
  dozeDroopOffset = 0;
  if(doze){
    const unsigned long elapsed = millis() - dozeAnimationTimer;
    if(elapsed >= dozeDuration()){
      doze = 0;
    } else {
      const float shut = dozeShutAt(elapsed);
      eyeLheightOffsetAnim -= lroundf((eyeLheightDefault - closedHeight) * shut);
      eyeRheightOffsetAnim -= lroundf((eyeRheightDefault - closedHeight) * shut);
      dozeDroopOffset = lroundf(dozeDroop * (shut > 0 ? shut : 0));
    }
  }

  // Wink - for winkDuration one eye is shut (closed by anim_wink), the other
  // one squints, and both hop up and down once; then the eye opens again and
  // the eyes stare for winkStare (no blinks, no glances: see below)
  winkHopOffset = 0;
  if(winking){
    const unsigned long elapsed = millis() - winkAnimationTimer;
    if(elapsed >= (unsigned long)(winkDuration + winkStare)){
      winking = 0;
    } else if(elapsed >= (unsigned long)winkDuration){
      open(winkLeft, !winkLeft);
    } else {
      if(winkLeft){
        eyeRheightOffsetAnim -= winkSquint;
      } else {
        eyeLheightOffsetAnim -= winkSquint;
      }
      winkHopOffset = lroundf(winkHop * sinf(PI * float(elapsed) / winkDuration));
    }
  }

  // Left eye height. The target is never below closedHeight: a negative
  // offset (e.g. dozing) together with a blink would otherwise give a negative
  // height, which is drawn inside out.
  int eyeLheightTarget = eyeLheightNext + eyeLheightOffsetAnim;
  if(eyeLheightTarget < closedHeight){eyeLheightTarget = closedHeight;}
  eyeLheightCurrent = (eyeLheightCurrent + eyeLheightTarget)/2;
  eyeLy+= ((eyeLheightDefault-eyeLheightCurrent)/2); // vertical centering of eye when closing
  eyeLy-= eyeLheightOffsetAnim/2;
  // Right eye height
  int eyeRheightTarget = eyeRheightNext + eyeRheightOffsetAnim;
  if(eyeRheightTarget < closedHeight){eyeRheightTarget = closedHeight;}
  eyeRheightCurrent = (eyeRheightCurrent + eyeRheightTarget)/2;
  eyeRy+= (eyeRheightDefault-eyeRheightCurrent)/2; // vertical centering of eye when closing
  eyeRy-= eyeRheightOffsetAnim/2;


  // Open eyes again after closing them: once they are as closed as they get
  // (a positive offset, e.g. curious, keeps them a bit taller)
	if(eyeL_open){
  	if(eyeLheightCurrent <= closedHeight + (eyeLheightOffsetAnim > 0 ? eyeLheightOffsetAnim : 0)){eyeLheightNext = eyeLheightDefault;}
  }
  if(eyeR_open){
  	if(eyeRheightCurrent <= closedHeight + (eyeRheightOffsetAnim > 0 ? eyeRheightOffsetAnim : 0)){eyeRheightNext = eyeRheightDefault;}
  }

  // Left eye width
  eyeLwidthCurrent = (eyeLwidthCurrent + eyeLwidthNext)/2;
  // Right eye width
  eyeRwidthCurrent = (eyeRwidthCurrent + eyeRwidthNext)/2;


  // Space between eyes
  spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext)/2;

  // Lean: overrides the target every frame, so positions and idle moves can't
  // leave the pinned edge
  if(leanX < 0){eyeLxNext = 0;}
  else if(leanX > 0){eyeLxNext = getScreenConstraint_X();}
  if(leanY < 0){eyeLyNext = getScreenTop_Y();}
  else if(leanY > 0){eyeLyNext = getScreenConstraint_Y();}

  // Left eye coordinates
  eyeLx = (eyeLx + eyeLxNext)/2;
  eyeLy = (eyeLy + eyeLyNext)/2;
  // Right eye coordinates
  eyeRxNext = eyeLxNext+eyeLwidthCurrent+spaceBetweenCurrent; // right eye's x position depends on left eyes position + the space between
  eyeRyNext = eyeLyNext; // right eye's y position should be the same as for the left eye
  eyeRx = (eyeRx + eyeRxNext)/2;
  eyeRy = (eyeRy + eyeRyNext)/2;

  // Left eye border radius
  eyeLborderRadiusCurrent = (eyeLborderRadiusCurrent + eyeLborderRadiusNext)/2;
  // Right eye border radius
  eyeRborderRadiusCurrent = (eyeRborderRadiusCurrent + eyeRborderRadiusNext)/2;
  

  //// APPLYING MACRO ANIMATIONS ////

	if(autoblinker && !doze && !winking){ // no blinks while fighting against sleep, winking (a blink would open the winking eye) or staring after it
		if(millis() >= blinktimer){
		blink();
		blinktimer = millis()+(blinkInterval*1000)+random(blinkIntervalVariation*1000); // calculate next time for blinking; random to the millisecond, so blinking and idle moves stay independent
		}
	}

  // Laughing - eyes shaking up and down for the duration defined by laughAnimationDuration (default = 500ms)
  if(laugh){
    if(laughToggle){
      setVFlicker(1, 5);
      laughAnimationTimer = millis();
      laughToggle = 0;
    } else if(millis() >= laughAnimationTimer+laughAnimationDuration){
      setVFlicker(0, 0);
      laughToggle = 1;
      laugh=0; 
    }
  }

  // Confused - eyes shaking left and right for the duration defined by confusedAnimationDuration (default = 500ms)
  if(confused){
    if(confusedToggle){
      setHFlicker(1, 20);
      confusedAnimationTimer = millis();
      confusedToggle = 0;
    } else if(millis() >= confusedAnimationTimer+confusedAnimationDuration){
      setHFlicker(0, 0);
      confusedToggle = 1;
      confused=0; 
    }
  }

  // Idle - eyes moving to random positions on screen
  if(idle && !winking){ // no glances while winking and staring after it
    if(millis() >= idleAnimationTimer){
      eyeLxNext = random(getScreenConstraint_X());
      if(!idleHorizontalOnly){
        eyeLyNext = getScreenTop_Y()+random(getScreenConstraint_Y()-getScreenTop_Y()+1);
      }
      idleAnimationTimer = millis()+(idleInterval*1000)+random(idleIntervalVariation*1000); // calculate next time for eyes repositioning; random to the millisecond
    }
  }

  // Adding offsets for horizontal flickering/shivering
  if(hFlicker){
    if(hFlickerAlternate) {
      eyeLx += hFlickerAmplitude;
      eyeRx += hFlickerAmplitude;
    } else {
      eyeLx -= hFlickerAmplitude;
      eyeRx -= hFlickerAmplitude;
    }
    hFlickerAlternate = !hFlickerAlternate;
  }

  // Adding offsets for horizontal flickering/shivering
  if(vFlicker){
    if(vFlickerAlternate) {
      eyeLy += vFlickerAmplitude;
      eyeRy += vFlickerAmplitude;
    } else {
      eyeLy -= vFlickerAmplitude;
      eyeRy -= vFlickerAmplitude;
    }
    vFlickerAlternate = !vFlickerAlternate;
  }

  // Cyclops mode, set second eye's size and space between to 0
  if(cyclops){
    eyeRwidthCurrent = 0;
    eyeRheightCurrent = 0;
    spaceBetweenCurrent = 0;
  }

  // Breathing: a smooth up and down offset, applied only while drawing, so
  // it doesn't add up in the eyes' position from frame to frame. The phase
  // advances with the time since the last frame, so changing the period
  // changes the speed without a jump.
  if(breathing){
    breathingPhase += float(millis() - breathingTimer) / breathingPeriod;
    breathingPhase -= floorf(breathingPhase); // keep it within 0 ... 1
    breathingOffset = lroundf(breathingAmplitude * sinf(2 * PI * breathingPhase));
  } else {
    breathingOffset = 0;
  }
  breathingTimer = millis();
  // Breathing, the startled jump and the wink hop (upwards) and the doze droop (downwards) move the eyes only while drawing
  drawOffsetY = breathingOffset - startledJumpOffset - winkHopOffset + dozeDroopOffset;
  eyeLy += drawOffsetY;
  eyeRy += drawOffsetY;

  //// ACTUAL DRAWINGS ////

  display->clearDisplay(); // start with a blank screen

  // Draw basic eye rectangles
  display->fillRoundRect(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, eyeLborderRadiusCurrent, MAINCOLOR); // left eye
  if (!cyclops){
    display->fillRoundRect(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, eyeRborderRadiusCurrent, MAINCOLOR); // right eye
  }

  // Prepare mood type transitions
  if (tired){eyelidsTiredHeightNext = eyeLheightCurrent/2; eyelidsAngryHeightNext = 0;} else{eyelidsTiredHeightNext = 0;}
  if (angry){eyelidsAngryHeightNext = eyeLheightCurrent/2; eyelidsTiredHeightNext = 0;} else{eyelidsAngryHeightNext = 0;}
  if (happy){eyelidsHappyBottomOffsetNext = eyeLheightCurrent/2;} else{eyelidsHappyBottomOffsetNext = 0;}

  // Draw tired top eyelids 
    eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext)/2;
    if (!cyclops){
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // left eye 
      display->fillTriangle(eyeRx, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy+eyelidsTiredHeight-1, BGCOLOR); // right eye
    } else {
      // Cyclops tired eyelids
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // left eyelid half
      display->fillTriangle(eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // right eyelid half
    }

  // Draw angry top eyelids 
    eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext)/2;
    if (!cyclops){ 
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy+eyelidsAngryHeight-1, BGCOLOR); // left eye
      display->fillTriangle(eyeRx, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy-1, eyeRx, eyeRy+eyelidsAngryHeight-1, BGCOLOR); // right eye
    } else {
      // Cyclops angry eyelids
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy+eyelidsAngryHeight-1, BGCOLOR); // left eyelid half
      display->fillTriangle(eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy+eyelidsAngryHeight-1, BGCOLOR); // right eyelid half
    }

  // Draw happy bottom eyelids
    eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext)/2;
    display->fillRoundRect(eyeLx-1, (eyeLy+eyeLheightCurrent)-eyelidsHappyBottomOffset+1, eyeLwidthCurrent+2, eyeLheightDefault, eyeLborderRadiusCurrent, BGCOLOR); // left eye
    if (!cyclops){ 
      display->fillRoundRect(eyeRx-1, (eyeRy+eyeRheightCurrent)-eyelidsHappyBottomOffset+1, eyeRwidthCurrent+2, eyeRheightDefault, eyeRborderRadiusCurrent, BGCOLOR); // right eye
    }

  // Draw eyelashes at the outer corners
    if (eyelashes){
      drawEyelashes(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, eyeLborderRadiusCurrent, true); // left eye
      if (!cyclops){
        drawEyelashes(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, eyeRborderRadiusCurrent, false); // right eye
      }
    }

  // Add sweat drops
    if (sweat){
      // Sweat drop 1 -> left corner
      if(sweat1YPos <= sweat1YPosMax){sweat1YPos+=0.5;} // vertical movement from initial to max
      else {sweat1XPosInitial = random(30); sweat1YPos = 2; sweat1YPosMax = (random(10)+10); sweat1Width = 1; sweat1Height = 2;} // if max vertical position is reached: reset all values for next drop
      if(sweat1YPos <= sweat1YPosMax/2){sweat1Width+=0.5; sweat1Height+=0.5;} // shape grows in first half of animation ...
      else {sweat1Width-=0.1; sweat1Height-=0.5;} // ... and shrinks in second half of animation
      sweat1XPos = sweat1XPosInitial-(sweat1Width/2); // keep the growing shape centered to initial x position
      display->fillRoundRect(sweat1XPos, sweat1YPos, sweat1Width, sweat1Height, sweatBorderradius, MAINCOLOR); // draw sweat drop


      // Sweat drop 2 -> center area
      if(sweat2YPos <= sweat2YPosMax){sweat2YPos+=0.5;} // vertical movement from initial to max
      else {sweat2XPosInitial = random((screenWidth-60))+30; sweat2YPos = 2; sweat2YPosMax = (random(10)+10); sweat2Width = 1; sweat2Height = 2;} // if max vertical position is reached: reset all values for next drop
      if(sweat2YPos <= sweat2YPosMax/2){sweat2Width+=0.5; sweat2Height+=0.5;} // shape grows in first half of animation ...
      else {sweat2Width-=0.1; sweat2Height-=0.5;} // ... and shrinks in second half of animation
      sweat2XPos = sweat2XPosInitial-(sweat2Width/2); // keep the growing shape centered to initial x position
      display->fillRoundRect(sweat2XPos, sweat2YPos, sweat2Width, sweat2Height, sweatBorderradius, MAINCOLOR); // draw sweat drop


      // Sweat drop 3 -> right corner
      if(sweat3YPos <= sweat3YPosMax){sweat3YPos+=0.5;} // vertical movement from initial to max
      else {sweat3XPosInitial = (screenWidth-30)+(random(30)); sweat3YPos = 2; sweat3YPosMax = (random(10)+10); sweat3Width = 1; sweat3Height = 2;} // if max vertical position is reached: reset all values for next drop
      if(sweat3YPos <= sweat3YPosMax/2){sweat3Width+=0.5; sweat3Height+=0.5;} // shape grows in first half of animation ...
      else {sweat3Width-=0.1; sweat3Height-=0.5;} // ... and shrinks in second half of animation
      sweat3XPos = sweat3XPosInitial-(sweat3Width/2); // keep the growing shape centered to initial x position
      display->fillRoundRect(sweat3XPos, sweat3YPos, sweat3Width, sweat3Height, sweatBorderradius, MAINCOLOR); // draw sweat drop
    }

  display->display(); // show drawings on display

  // Remove the breathing and jump offset again, see above
  eyeLy -= drawOffsetY;
  eyeRy -= drawOffsetY;

} // end of drawEyes method

// Draws three eyelashes at the upper outer corner of one eye, pointing outwards
// from its rounded corner; outerLeft: the outer corner is on the left (left eye).
// Where the tired or angry eyelids cut the top of the eye, the lashes start on
// the eyelid's edge, so they stay attached to the visible eye.
void drawEyelashes(int x, int y, int width, int height, int radius, bool outerLeft){
  const int r = constrain(radius, 0, height/2); // a closing eye gets flatter corners
  const int side = outerLeft ? -1 : 1; // outwards
  const int cornerX = outerLeft ? x + r : x + width - r; // center of the rounded corner
  const int cornerY = y + r;
  static const float ANGLES[] = {0.3, 0.8, 1.3}; // from sideways (0) to upwards (PI/2)
  for (float angle : ANGLES){
    const float dx = cosf(angle);
    const float dy = sinf(angle);
    const int baseX = cornerX + side * lroundf(r * dx);
    int baseY = cornerY - lroundf(r * dy);
    // The eyelids are triangles over the top of the eye: tired ones cut the
    // outer corner, angry ones the inner corner (0 = outer side ... 1 = inner side)
    const float fromOuter = outerLeft ? float(baseX - x) / width : float(x + width - baseX) / width;
    const int lid = max(lroundf(eyelidsTiredHeight * (1 - fromOuter)), lroundf(eyelidsAngryHeight * fromOuter));
    baseY = max(baseY, y + lid);
    const int endX = baseX + side * lroundf(eyelashLength * dx);
    const int endY = baseY - lroundf(eyelashLength * dy);
    // 3 pixels thick: the line, plus one pixel up and one outwards
    display->drawLine(baseX, baseY, endX, endY, MAINCOLOR);
    display->drawLine(baseX, baseY - 1, endX, endY - 1, MAINCOLOR);
    display->drawLine(baseX + side, baseY, endX + side, endY, MAINCOLOR);
  }
}


}; // end of class roboEyes


#endif
