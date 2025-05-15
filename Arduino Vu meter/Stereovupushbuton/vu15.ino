/*
 * Old-skool green and red from bottom or middle
 */
// ------------------
// -- VU functions --
// ------------------

uint16_t auxReading(uint8_t channel) {

  int n = 0;
  uint16_t height = 0;

  if(channel == 0) {
    int n = analogRead( MIC_PIN); // Raw reading from left line in
    n = abs(n - 512 - DC_OFFSET); // Center on zero
    n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
    lvlLeft = ((lvlLeft * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)
    volLeft[volCountLeft] = n; // Save sample for dynamic leveling
    volCountLeft = ++volCountLeft % SAMPLES;
    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = TOP * (lvlLeft - minLvlAvgLeft) / (long)(maxLvlAvgLeft - minLvlAvgLeft);
  }
  
  else {
    int n = analogRead( MIC_PIN_2); // Raw reading from mic
    n = abs(n - 512 - DC_OFFSET); // Center on zero
    n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
    lvlRight = ((lvlRight * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)
    volRight[volCountRight] = n; // Save sample for dynamic leveling
    volCountRight = ++volCountRight % SAMPLES;
    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = TOP * (lvlRight - minLvlAvgRight) / (long)(maxLvlAvgRight - minLvlAvgRight);
  }

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = constrain(height, 0, TOP);

  return height;
}

/*
 * Function for dropping the peak
 */
//uint8_t peakLeft, peakRight;
void dropPeak(uint8_t channel) {
  
  static uint8_t dotCountLeft, dotCountRight;
 
  if(channel == 0) {
    if(++dotCountLeft >= PEAK_FALL) { //fall rate 
      if(peakLeft > 0) peakLeft--;
      dotCountLeft = 0;
    }
  } else {
    if(++dotCountRight >= PEAK_FALL) { //fall rate 
      if(peakRight > 0) peakRight--;
      dotCountRight = 0;
    }
  }
}

/*
 * Function for averaging the sample readings
 */
void averageReadings(uint8_t channel) {

  uint16_t minLvl, maxLvl;

  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if(channel == 0) {
    minLvl = maxLvl = volLeft[0];
    for (int i = 1; i < SAMPLES; i++) {
      if (volLeft[i] < minLvl) minLvl = volLeft[i];
      else if (volLeft[i] > maxLvl) maxLvl = volLeft[i];
    }
    if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
    
    minLvlAvgLeft = (minLvlAvgLeft * 63 + minLvl) >> 6; // Dampen min/max levels
    maxLvlAvgLeft = (maxLvlAvgLeft * 63 + maxLvl) >> 6; // (fake rolling average)
  }

  else {
    minLvl = maxLvl = volRight[0];
    for (int i = 1; i < SAMPLES; i++) {
      if (volRight[i] < minLvl) minLvl = volRight[i];
      else if (volRight[i] > maxLvl) maxLvl = volRight[i];
    }
    if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
    minLvlAvgRight = (minLvlAvgLeft * 63 + minLvl) >> 6; // Dampen min/max levels
    maxLvlAvgRight = (maxLvlAvgLeft * 63 + maxLvl) >> 6; // (fake rolling average)
  }
}



  void vu15 (bool is_centered, uint8_t channel){

   
  CRGB* leds;
  
  uint8_t i = 0;
  uint8_t *peak;      // Pointer variable declaration
  uint16_t height = auxReading(channel);

  if(channel == 0) {
    leds = ledsLeft;    // Store address of peakLeft in peak, then use *peak to
    peak = &peakLeft;   // access the value of that address
  }
  else {
    leds = ledsRight;
    peak = &peakRight;
  }

  if (height > *peak)
    *peak = height; // Keep 'peak' dot at top

  if(is_centered) {
    // Color pixels based on old school green / red
    for (uint8_t i = 0; i < N_PIXELS_HALF; i++) {
      if (i >= height) {
        // Pixels greater than peak, no light
        leds[N_PIXELS_HALF - i - 1] = CRGB::Black;
        leds[N_PIXELS_HALF + i] = CRGB::Black;
      } else {
        if (i > N_PIXELS_HALF - (N_PIXELS_HALF / 3)){
          leds[N_PIXELS_HALF - i - 1] = CRGB::Red;
          leds[N_PIXELS_HALF + i] = CRGB::Red;
        }
        else {
          leds[N_PIXELS_HALF - i - 1] = CRGB::Green;
          leds[N_PIXELS_HALF + i] = CRGB::Green;
        }
      }
    }
  
    // Draw peak dot
    if (*peak > 0 && *peak <= N_PIXELS_HALF - 1) {
      if (*peak > N_PIXELS_HALF - (N_PIXELS_HALF / 3)){
        leds[N_PIXELS_HALF - *peak - 1] = CRGB::Red;
        leds[N_PIXELS_HALF + *peak] = CRGB::Red;
      } else {
        leds[N_PIXELS_HALF - *peak - 1] = CRGB::Green;
        leds[N_PIXELS_HALF + *peak] = CRGB::Green;
      }
    }
    
  } else {
    // Color pixels based on old school green/red vu
    for (uint8_t i = 0; i < N_PIXELS; i++) {
      if (i >= height) leds[i] = CRGB::Black;
      else if (i > N_PIXELS - (N_PIXELS / 3)) leds[i] = CRGB::Red;
      else leds[i] = CRGB::Green;
    }
  
    // Draw peak dot
    if (*peak > 0 && *peak <= N_PIXELS - 1)
      if (*peak > N_PIXELS - (N_PIXELS / 3)) leds[*peak] = CRGB::Red;
      else leds[*peak] = CRGB::Green;
  
  } // switch step
  
  dropPeak(channel);

  averageReadings(channel);

  FastLED.show();
}
