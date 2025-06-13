#ifndef epd

// Return the minimum of two values a and b
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

#define USE_SPI_BUFFER // Comment out to use slower 16 bit pushColor()

//====================================================================================
//   Opens the image file and prime the Jpeg decoder
//====================================================================================
bool drawJpeg(String filename, int xpos, int ypos) {


  // Open the named file (the Jpeg decoder library will close it after rendering image)
#ifdef pin_sd
  /*
    #ifdef debug
    Serial.print("SD ");
    Serial.println(filename);
    #endif
  */
  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
  /*
  #ifdef debug
    Serial.print("Spiffs ");
    Serial.println(filename);
  #endif
  */
  fs::File jpegFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS

#endif

  if ( !jpegFile ) {
    Serial.print(F("ERROR not found: "));
    Serial.println(filename);
    jpegFile.close();
    yield();
    return false;
  }

  // Use one of the three following methods to initialise the decoder:
  //boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
#ifdef pin_sd
  boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
#else
  boolean decoded = JpegDec.decodeFsFile(filename);  // or pass the filename (leading / distinguishes SPIFFS files)
#endif                              // Note: the filename can be a String or character array type

  if (decoded) {
    // print information about the image to the serial port
    // jpegInfo();

    //if (tft2_act)     jpegRender2(xpos, ypos);
    // else
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println(F("Jpeg file format not supported!"));
    jpegFile.close();

    return false;
  }
  jpegFile.close();


  return true;
}

//====================================================================================
//       2 TFT jpegRender
//====================================================================================
#if defined (two_tft)
void jpegRender(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  // uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while ( JpegDec.read()) {

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      for (int h = 1; h < win_h - 1; h++)
      {
        memcpy(pImg + h * win_w, pImg + (h + 1) * mcu_w, win_w << 1);
      }
    }

    // draw image MCU block only if it will fit on the screen
    //-------------------

    if (tft2_act) //active tft №2
    {
      if ( ( mcu_x + win_w) <= tft2.width() && ( mcu_y + win_h) <= tft2.height())
      {
        tft2.drawRGBBitmap(mcu_x, mcu_y, pImg, win_w, win_h);
      }

      else if ( ( mcu_y + win_h) >= tft2.height()) JpegDec.abort();
    }
    else
    {
      if ( ( mcu_x + win_w) <= tft.width() && ( mcu_y + win_h) <= tft.height())
      {
        tft.drawRGBBitmap(mcu_x, mcu_y, pImg, win_w, win_h);
      }

      else if ( ( mcu_y + win_h) >= tft.height()) JpegDec.abort();


    }

    //--------------------

  }


}

//---------------------------------------------------------------------------
//        1 TFT jpegRender
//---------------------------------------------------------------------------
#else //1  tft  
void jpegRender(int xpos, int ypos) {


  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while (JpegDec.read()) {

    // save a pointer to the image block
    pImg = JpegDec.pImage ;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    tft.startWrite();

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
    {

      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft.setAddrWindow(mcu_x, mcu_y, win_w, win_h);

      // Write all MCU pixels to the TFT window
      while (mcu_pixels--) {
        // Push each pixel to the TFT MCU area
        tft.pushColor(*pImg++);
      }

    }
    else if ( (mcu_y + win_h) >= tft.height()) JpegDec.abort(); // Image has run off bottom of screen so abort decoding

    tft.endWrite();
  }


}

#endif // 1 TFT


#endif //#ifndef epd