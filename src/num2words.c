#include "num2words.h"
#include "string.h"

static const char* const ONES[] = {
  "zero",
  "uma",
  "duas",
  "tres",
  "quatro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove"
};

static const char* const TEENS[] ={
  "dez",
  "onze",
  "doze",
  "treze",
  "quatorze",
  "quinze",
  "dezesseis",
  "dezessete",
  "dezoito",
  "dezenove"
};

static const char* const TENS[] = {
  "",
  "dez",
  "vinte",
  "trinta",
  "quarenta",
  "cinquenta",
  "sessenta",
  "setenta",
  "oitenta",
  "noventa"
};

static const char* const TENSPLUS[] = {
  "",
  "deze",
  "vinte e ",
  "trinta e ",
  "quarenta e ",
  "cinquenta e ",
  "sessenta e ",
  "setenta e ",
  "oitenta e ",
  "noventa e "
};

static const char* STR_OH_CLOCK = "em ponto";
static const char* STR_NOON = "meio dia";
static const char* STR_MIDNIGHT = "meia noite";
static const char* STR_QUARTER = "quinze";
static const char* STR_TO = "pras";
static const char* STR_PAST = "e";
static const char* STR_HALF = "meia";
static const char* STR_AFTER = "e";

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    
    if (ones_val == 0) {
      strcat(words, TENS[tens_val]);
      len += strlen(TENS[tens_val]);
    } else {
      strcat(words, TENSPLUS[tens_val]);
      len += strlen(TENSPLUS[tens_val]);
    }
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }

  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

void fuzzy_time_to_words(int hours, int minutes, char* words, size_t length) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = ((minutes + 2) / 5) * 5;

  // Handle hour & minute roll-over.
  if (fuzzy_minutes > 55) {
    fuzzy_minutes = 0;
    fuzzy_hours += 1;
    if (fuzzy_hours > 23) {
      fuzzy_hours = 0;
    }
  }

  if (fuzzy_minutes > 30) {
    fuzzy_hours = (fuzzy_hours + 1) % 24;
  }

  size_t remaining = length;
  memset(words, 0, length);

 
  if (fuzzy_hours == 0 && fuzzy_minutes == 0) {
    remaining -= append_string(words, remaining, STR_MIDNIGHT);
  } else if (fuzzy_hours == 12 && fuzzy_minutes == 0) {
    remaining -= append_string(words, remaining, STR_NOON);
  } else if (fuzzy_hours % 12 == 0) {
    remaining -= append_number(words, 12);
  } else {
    remaining -= append_number(words, fuzzy_hours % 12);
  }

  if (fuzzy_minutes != 0 && (fuzzy_minutes >= 10 || fuzzy_minutes == 5 || fuzzy_hours == 0 || fuzzy_hours == 12)) {
    if (fuzzy_minutes == 15) {
       remaining -= append_string(words, remaining, " ");      
       remaining -= append_string(words, remaining, STR_AFTER);
       remaining -= append_string(words, remaining, " ");
       remaining -= append_string(words, remaining, STR_QUARTER);      
    } else if (fuzzy_minutes == 45) {
       remaining -= append_string(words, remaining, " ");      
       remaining -= append_string(words, remaining, STR_TO);
       remaining -= append_string(words, remaining, " ");
       remaining -= append_string(words, remaining, STR_QUARTER);
    } else if (fuzzy_minutes == 30) {
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_PAST);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_HALF);
    } else if (fuzzy_minutes < 30) {
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_AFTER);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_number(words, fuzzy_minutes);
    } else {
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_TO);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_number(words, 60 - fuzzy_minutes);
    }
  }


  if (fuzzy_minutes == 0 && !(fuzzy_hours == 0 || fuzzy_hours == 12)) {
    remaining -= append_string(words, remaining, " ");
    remaining -= append_string(words, remaining, STR_OH_CLOCK);
  }
}
