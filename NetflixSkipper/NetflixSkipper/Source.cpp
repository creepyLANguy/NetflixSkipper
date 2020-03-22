#include <iostream>
#include <iostream>
#include <vector>
#include <Windows.h>

#include "EasyBMP/EasyBMP_win32.h"
#include "wtypes.h"

using namespace std;
const float thresh_minSAD = 0.1;
float thresh = 0.9;
BMP src;
BMP screen;

const char* skipIntroFilename = "skip.bmp";
const int skipMS = 1000;

const char* runMsg = "Running...";
const char* errMsg = "ERROR\nFile not found: ";
const char* hitMsg = "Triggered Skip Button at: ";

void InitInputObject(INPUT& input)
{
  input.type = INPUT_MOUSE;
  input.mi.dx = 0;
  input.mi.dy = 0;
  input.mi.dwFlags = (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
  input.mi.mouseData = 0;
  input.mi.dwExtraInfo = NULL;
  input.mi.time = 0;
}


//AL.
/*
void NMultipleTemplateMatching(Mat& mInput, Mat& mTemplate, const float thresh, vector<Point2f>& matches)
{
  Mat result;
  const Size templateSize = mTemplate.size();
  
  matchTemplate(mInput, mTemplate, result, TM_CCOEFF_NORMED);
  threshold(result, result, thresh, 1.0, THRESH_TOZERO);
  while (true)
  {
    double minval, maxval;
    Point minloc, maxloc;
    minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

    if (maxval < thresh)
    {
      break;
    }

    matches.push_back(maxloc);
  }
}
*/


void TemplateMatch(pair<int, int>& match)
{
  float minSAD = thresh_minSAD;

  // loop through the search image
  for ( int x = 0; x <= screen.TellWidth() - src.TellWidth(); x++ ) 
  {
      for ( int y = 0; y <= screen.TellHeight()- src.TellHeight(); y++)
      {
          float fSAD = 0.0;
          // loop through the template image
          for (int i = 0; i < src.TellWidth(); i++)
          {
            for (int j = 0; j < src.TellHeight(); j++)
            {
              const RGBApixel px_screen = screen.GetPixel(x + i, y + j);
              const RGBApixel px_template = src.GetPixel(i, j);

              fSAD += abs(px_screen.Red - px_template.Red); // okay cos RGB vals are all same in greyscale
            }
          }

          // save the best found position
          if (fSAD < minSAD) {
            minSAD = fSAD;
            match.first = x;
            match.second = y;
          }
      }
  }
}


void ConvertToGreyscale(BMP& input)
{
  for (int j = 0; j < input.TellHeight(); j++)
  {
    for (int i = 0; i < input.TellWidth(); i++)
    {
      const int tmp = static_cast<int>(floor(0.299 * input(i, j)->Red + 0.587 * input(i, j)->Green + 0.114 * input(i, j)->Blue));
      const auto tmp_byte = static_cast<ebmpBYTE>(tmp);
      input(i, j)->Red = tmp_byte;
      input(i, j)->Green = tmp_byte;
      input(i, j)->Blue = tmp_byte;
    }
  }
}


void ClickOnMatch(
  const pair<int, int>& match,
  INPUT& input, 
  const int src_width, 
  const int src_height)
{
  const int x = match.first + src_width / 2;
  const int y = match.second + src_height / 2;

  POINT currentPos;
  GetCursorPos(&currentPos);
  SetCursorPos(x, y);
  SendInput(1, &input, sizeof(INPUT));
  SetCursorPos(currentPos.x, currentPos.y);

  cout << hitMsg << "[" << x << "," << y << "]" << endl;
}


void main()
{
  INPUT input;
  InitInputObject(input);

  if (src.ReadFromFile(skipIntroFilename) == false)
  {
    cout << errMsg << skipIntroFilename << endl;
    int x;
    cin >> x;
    return;
  }
  ConvertToGreyscale(src);

#ifndef DEBUG
  src.WriteToFile("template.bmp");
#endif

  cout << runMsg << endl;

  while (true)
  {
    CaptureScreen(screen);
    ConvertToGreyscale(screen);
#ifdef DEBUG
    screen.WriteToFile("screen.bmp");
#endif

    pair<int,int> matchResult = make_pair(-1,-1);
    TemplateMatch(matchResult);

    if (matchResult.first < 0 || matchResult.second < 0)
    {
      Sleep(skipMS);
      continue;
    }

    ClickOnMatch(matchResult, input, src.TellWidth(), src.TellHeight());
    Sleep(skipMS);
  }
}
