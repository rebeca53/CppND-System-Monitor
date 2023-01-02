#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  string textHour = std::to_string(hours);
  if (textHour.length() == 1) {
    textHour = "0"+textHour;
  }
  long hour_remainder = seconds - hours*3600;
  long minutes = hour_remainder / 60;
  string textMinutes = std::to_string(minutes);
  if (textMinutes.length() == 1) {
    textMinutes = "0"+textMinutes;
  }

  long seconds_remainder = hour_remainder - minutes*60;
  string textSeconds = std::to_string(seconds_remainder);
  if (textSeconds.length() == 1) {
    textSeconds = "0"+textSeconds;
  }

  return textHour + ":" + textMinutes + ":" + textSeconds;
}