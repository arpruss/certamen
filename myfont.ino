#ifdef USE_TFT

// Font by Ron Buelow
// "use the fonts any way you wish, as always credit the source as applicable"

template<typename T> void draw_nicefont(T tft, const char* text, int x, int y, unsigned int color) {
  while(*text) {
    switch(*text) {
    case 32:
      x+=2;
      break;
    case 39:
      tft.drawPixel(x+0, y+1, color);
      tft.drawPixel(x+0, y+2, color);
      tft.drawPixel(x+0, y+3, color);
      x+=2;
      break;
    case 51:
      tft.drawPixel(x+1, y+2, color);
      tft.drawPixel(x+2, y+2, color);
      tft.drawPixel(x+0, y+3, color);
      tft.drawPixel(x+3, y+3, color);
      tft.drawPixel(x+3, y+4, color);
      tft.drawPixel(x+2, y+5, color);
      tft.drawPixel(x+3, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      x+=5;
      break;
    case 67:
      tft.drawPixel(x+2, y+2, color);
      tft.drawPixel(x+3, y+2, color);
      tft.drawPixel(x+1, y+3, color);
      tft.drawPixel(x+4, y+3, color);
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+4, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=6;
      break;
    case 71:
      tft.drawPixel(x+3, y+2, color);
      tft.drawPixel(x+4, y+2, color);
      tft.drawPixel(x+2, y+3, color);
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+4, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+4, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+4, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      tft.drawPixel(x+4, y+8, color);
      x+=6;
      break;
    case 76:
      tft.drawPixel(x+0, y+2, color);
      tft.drawPixel(x+0, y+3, color);
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 77:
      tft.drawPixel(x+0, y+2, color);
      tft.drawPixel(x+5, y+2, color);
      tft.drawPixel(x+0, y+3, color);
      tft.drawPixel(x+4, y+3, color);
      tft.drawPixel(x+5, y+3, color);
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+3, y+4, color);
      tft.drawPixel(x+5, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+2, y+5, color);
      tft.drawPixel(x+5, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+5, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+5, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      tft.drawPixel(x+4, y+8, color);
      x+=7;
      break;
    case 80:
      tft.drawPixel(x+0, y+2, color);
      tft.drawPixel(x+1, y+2, color);
      tft.drawPixel(x+2, y+2, color);
      tft.drawPixel(x+3, y+2, color);
      tft.drawPixel(x+0, y+3, color);
      tft.drawPixel(x+4, y+3, color);
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+4, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+1, y+5, color);
      tft.drawPixel(x+2, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      x+=6;
      break;
    case 97:
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+1, y+6, color);
      tft.drawPixel(x+2, y+6, color);
      tft.drawPixel(x+3, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 99:
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 100:
      tft.drawPixel(x+3, y+2, color);
      tft.drawPixel(x+3, y+3, color);
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+3, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+3, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 101:
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+2, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+1, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 102:
      tft.drawPixel(x+2, y+2, color);
      tft.drawPixel(x+3, y+2, color);
      tft.drawPixel(x+1, y+3, color);
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+1, y+5, color);
      tft.drawPixel(x+1, y+6, color);
      tft.drawPixel(x+1, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      x+=4;
      break;
    case 104:
      tft.drawPixel(x+0, y+2, color);
      tft.drawPixel(x+0, y+3, color);
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+1, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+3, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 105:
      tft.drawPixel(x+0, y+2, color);
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      x+=2;
      break;
    case 108:
      tft.drawPixel(x+0, y+2, color);
      tft.drawPixel(x+0, y+3, color);
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      x+=2;
      break;
    case 109:
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+4, y+4, color);
      tft.drawPixel(x+5, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+6, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+3, y+6, color);
      tft.drawPixel(x+6, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+6, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      tft.drawPixel(x+6, y+8, color);
      x+=8;
      break;
    case 110:
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+1, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+3, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 114:
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+1, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      x+=4;
      break;
    case 115:
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+1, y+6, color);
      tft.drawPixel(x+2, y+7, color);
      tft.drawPixel(x+0, y+8, color);
      tft.drawPixel(x+1, y+8, color);
      x+=4;
      break;
    case 116:
      tft.drawPixel(x+1, y+3, color);
      tft.drawPixel(x+1, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+3, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+1, y+5, color);
      tft.drawPixel(x+1, y+6, color);
      tft.drawPixel(x+1, y+7, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 117:
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+3, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+3, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+3, y+6, color);
      tft.drawPixel(x+0, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+2, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=5;
      break;
    case 119:
      tft.drawPixel(x+0, y+4, color);
      tft.drawPixel(x+2, y+4, color);
      tft.drawPixel(x+4, y+4, color);
      tft.drawPixel(x+0, y+5, color);
      tft.drawPixel(x+2, y+5, color);
      tft.drawPixel(x+4, y+5, color);
      tft.drawPixel(x+0, y+6, color);
      tft.drawPixel(x+2, y+6, color);
      tft.drawPixel(x+4, y+6, color);
      tft.drawPixel(x+1, y+7, color);
      tft.drawPixel(x+3, y+7, color);
      tft.drawPixel(x+1, y+8, color);
      tft.drawPixel(x+3, y+8, color);
      x+=6;
      break;
    }
    text++;
  }
}
#endif
