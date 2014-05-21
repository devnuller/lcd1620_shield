#include <MsTimer2.h>
#include <LiquidCrystal.h>

#define BUTTONS  0

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

enum buttons { NONE, RIGHT, UP, DOWN, LEFT, SELECT };

// Debounced pressed keystate
volatile byte pressed_key = NONE;


/*
 * Translate analog value into button pressed
 */
byte SelectedButton(int analogval)
{
  if ( analogval > 872 ) return NONE;
  if ( analogval < 65 ) return RIGHT;
  if ( analogval < 219 ) return UP;
  if ( analogval < 394 ) return DOWN;
  if ( analogval < 600 ) return LEFT;
  return SELECT;
}


/*
 * Read keys and set pressed_key once keystate is debounced.
 * Function is called interrupt time (from timer interrupt)
 */
void scan_keys()
{
  static byte last_reading = NONE;
  static byte debounce_counter = 0;
  
  byte current_reading;
  
  current_reading = SelectedButton(analogRead(BUTTONS));
  
  if ( current_reading == last_reading ) {
    if ( debounce_counter > 0 )
      --debounce_counter;
    else
      pressed_key = current_reading;
  } else {
    debounce_counter = 3;
    last_reading = current_reading;
  }
  
}


char *buttonToString(byte button)
{
  switch(button) {
    case NONE:
      return "";
      break;
    case RIGHT:
      return "Right";
      break;
    case UP:
      return "Up";
      break;
    case DOWN:
      return "Down";
      break;
    case LEFT:
      return "Left";
      break;
    case SELECT:
      return "Select";
      break;
    default:
      return "Unknown";
      break;
  }
}



void setup()
{
  lcd.begin(16,2);
  lcd.clear();
  MsTimer2::set(5, scan_keys);
  MsTimer2::start();
}

void loop()
{
  byte button=NONE; 
 
  while(1) {
    if ( button != pressed_key ) {
      button = pressed_key;
      lcd.clear();
      lcd.print(buttonToString(button));
    }
  }
  
  
}
