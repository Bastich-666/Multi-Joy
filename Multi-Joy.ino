// -------------------------------------------------------------------------------------------------------------------------------------
// Dual 9 pin Amiga (Atari Specification) joysticks to Arduino USB HID (v1.1)
//
// I made this simple Arduino sketch so others can produce their own joystick
// adapters and avoid some of the extortionate prices I have seen for similar devices.
// 
// If you make any fixes or improvements please send them to me :)
// 
// Arduino Sketch by J.S.Roberts (james@nebula-design.com)
// Required library by MHeironimus (https://github.com/MHeironimus/ArduinoJoystickLibrary)
//
//
// -------------------------------------------------------------------------------------------------------------------------------------
//  MIT License
//  
//  Copyright (c) [year] [fullname]
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//  
// -------------------------------------------------------------------------------------------------------------------------------------

#include <Joystick.h>

// Joystick definitions
#define JOYSTICK_COUNT 2

Joystick_ Joystick[JOYSTICK_COUNT] = 
{
  Joystick_(0x03, JOYSTICK_TYPE_JOYSTICK, 2, 1, true, true, false, false, false, false, false, false, false, false, false),
  Joystick_(0x04, JOYSTICK_TYPE_JOYSTICK, 2, 1, true, true, false, false, false, false, false, false, false, false, false),
};

// Arduino pin assignments
#define Joy1_Up     9
#define Joy1_Down   8
#define Joy1_Left   7
#define Joy1_Right  6
#define Joy1_Fire1  4
#define Joy1_Fire2  5

#define Joy2_Up     15
#define Joy2_Down   14
#define Joy2_Left   16
#define Joy2_Right  10
#define Joy2_Fire1  18
#define Joy2_Fire2  19

// Variables used to store the joysticks last states
// This is done so that duplicate (pointless) data does not get sent to the host system
int last_j1, last_j2;

// Setup function
void setup() 
{
  // Set default input pin modes
  pinMode(Joy1_Up,    INPUT_PULLUP);
  pinMode(Joy1_Down,  INPUT_PULLUP);
  pinMode(Joy1_Left,  INPUT_PULLUP);
  pinMode(Joy1_Right, INPUT_PULLUP);
  pinMode(Joy1_Fire1, INPUT_PULLUP);
  pinMode(Joy1_Fire2, INPUT_PULLUP);

  pinMode(Joy2_Up,    INPUT_PULLUP);
  pinMode(Joy2_Down,  INPUT_PULLUP);
  pinMode(Joy2_Left,  INPUT_PULLUP);
  pinMode(Joy2_Right, INPUT_PULLUP);
  pinMode(Joy2_Fire1, INPUT_PULLUP);
  pinMode(Joy2_Fire2, INPUT_PULLUP);

  // Clear joystick stored sztates
  last_j1 = 0;
  last_j2 = 0;

  // Clear joysticks structures  and initialise
  for (int index = 0; index < JOYSTICK_COUNT; index++)
  {
    Joystick[index].setXAxisRange(-127, 127);
    Joystick[index].setYAxisRange(-127, 127);
  
    Joystick[index].begin(false);
  }

  pinMode(A0, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

// Main loop
void loop() 
{
  bool up, down, left, right;
  int  joy_val;

  // Joystick 1 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Joystick[0].setXAxis(0);
  Joystick[0].setYAxis(0);

  // Read joystick direction
  if( !digitalRead(Joy1_Up) )
    up = true;
  else
    up = false;

  if( !digitalRead(Joy1_Down) )
    down = true;
  else
    down = false;
    
  if( !digitalRead(Joy1_Left) )
    left = true;
  else
    left = false;

  if( !digitalRead(Joy1_Right) )
    right = true;
  else
    right = false;
    
  // Set reported analogue axis to match the digital input
  // Some emulators do not read the hat settings
  if( up )
    Joystick[0].setXAxis(127);

  if( down )
    Joystick[0].setXAxis(-127);

  if( left )
    Joystick[0].setYAxis(127);

  if( right )
    Joystick[0].setYAxis(-127);

  // Setup hat angle according to digital  input
  if( up && !(left || right) )
    joy_val = 90;
  else if( up &&  right )
    joy_val = 45;
  else if( right && !(up || down) )
    joy_val = 0;
  else if( right && down )
    joy_val = 315;
  else if( down && !(left || right) )
    joy_val = 270;
  else if( left && down )
    joy_val = 225;
  else if( left && !(up || down) )
    joy_val = 180;
  else if( left && up )
    joy_val = 135;
  else 
    joy_val = -1;

  Joystick[0].setHatSwitch( 0, joy_val );
    
  // Read fire button 1
  if( digitalRead(Joy1_Fire1) )
  {
    Joystick[0].releaseButton(0);
    joy_val += 1;
  }
  else
  {
    Joystick[0].pressButton(0);
    joy_val += 2;
  }

  // Read fire button 2
  if( digitalRead(Joy1_Fire2) )
  {
    Joystick[0].releaseButton(1);
    joy_val += 3;    
  }
  else
  {
    Joystick[0].pressButton(1);
    joy_val += 4;
  }

  // Send data if different to last time
  if( last_j1 != joy_val )
  {
    Joystick[0].sendState();
    last_j1 = joy_val;
  }

  // Joystick 2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Joystick[1].setXAxis(0);
  Joystick[1].setYAxis(0);
  
  // Read joystick direction
  if( !digitalRead(Joy2_Up) )
    up = true;
  else
    up = false;

  if( !digitalRead(Joy2_Down) )
    down = true;
  else
    down = false;
    
  if( !digitalRead(Joy2_Left) )
    left = true;
  else
    left = false;

  if( !digitalRead(Joy2_Right) )
    right = true;
  else
    right = false;

  // Set reported analogue axis to match the digital input
  // Some emulators do not read the hat settings
  if( up )
    Joystick[1].setXAxis(127);

  if( down )
    Joystick[1].setXAxis(-127);

  if( left )
    Joystick[1].setYAxis(127);

  if( right )
    Joystick[1].setYAxis(-127);

  // Setup hat angle according to digital  input
  if( up && !(left || right) )
    joy_val = 90;
  else if( up &&  right )
    joy_val = 45;
  else if( right && !(up || down) )
    joy_val = 0;
  else if( right && down )
    joy_val = 315;
  else if( down && !(left || right) )
    joy_val = 270;
  else if( left && down )
    joy_val = 225;
  else if( left && !(up || down) )
    joy_val = 180;
  else if( left && up )
    joy_val = 135;
  else 
    joy_val = -1;

  Joystick[1].setHatSwitch( 0, joy_val );
    
  // Read fire button 1
  if( digitalRead(Joy2_Fire1) )
  {
    Joystick[1].releaseButton(0);
    joy_val += 1;
  }
  else
  {
    Joystick[1].pressButton(0);
    joy_val += 2;
  }

  // Read fire button 2
  if( digitalRead(Joy2_Fire2) )
  {
    Joystick[1].releaseButton(1);
    joy_val += 3;    
  }
  else
  {
    Joystick[1].pressButton(1);
    joy_val += 4;
  }

  // Send data if different to last time
  if( last_j2 != joy_val )
  {
    Joystick[1].sendState();
    last_j2 = joy_val;
  }
}
