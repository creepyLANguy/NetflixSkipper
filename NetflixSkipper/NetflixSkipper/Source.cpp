#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "wtypes.h"
#include <iostream>
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;


float thresh = 0.9;
float closeness = 0.9;
const char* skipIntroFilename = "skip.jpg";
const int skipMS = 1000;

const char* runMsg = "Running...";
const char* errMsg = "ERROR\nFile not found: ";
const char* hitMsg = "Triggered Skip Button at: ";

Mat hwnd2mat(const HWND hwnd)
{
  const HDC hwindowDC = GetDC(hwnd);
  const HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
  SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

  RECT windowsize;    // get the height and width of the screen
  GetClientRect(hwnd, &windowsize);

  const int srcheight = windowsize.bottom;
  const int srcwidth = windowsize.right;
  const int height = windowsize.bottom;  //change this to whatever size you want to resize to
  const int width = windowsize.right;

  Mat src;
  src.create(height, width, CV_8UC4);

  // create a bitmap
  const HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
  BITMAPINFOHEADER  bi;
  bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
  bi.biWidth = width;
  bi.biHeight = -height;  //this is the line that makes it draw upside down or not
  bi.biPlanes = 1;
  bi.biBitCount = 32;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed = 0;
  bi.biClrImportant = 0;

  // use the previously created device context with the bitmap
  SelectObject(hwindowCompatibleDC, hbwindow);
  // copy from the window device context to the bitmap device context
  StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
  GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

  DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

  return src;
}


void NMultipleTemplateMatching(Mat& mInput, Mat& mTemplate, const float thresh, const float closeness, vector<Point2f>& matches)
{
  Mat result;
  const Size templateSize = mTemplate.size();
  const Size templateCloseRadius((templateSize.width / 2) * closeness, (templateSize.height / 2) * closeness);

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
    rectangle(result, Point2f(maxloc.x - templateCloseRadius.width, maxloc.y - templateCloseRadius.height), Point2f(maxloc.x + templateCloseRadius.width, maxloc.y + templateCloseRadius.height), Scalar(0), -1);
  }
}


void main()
{
  HWND hDesktopWnd = GetDesktopWindow();

  Mat mTemplate_Bgr;      
  mTemplate_Bgr = imread(skipIntroFilename, 1);
  if (mTemplate_Bgr.rows == 0)
  {
    cout << errMsg << skipIntroFilename << endl;
    int x;
    cin >> x;
    return;
  }
  Mat mTemplate_Gray;
  cvtColor(mTemplate_Bgr, mTemplate_Gray, COLOR_BGR2GRAY);

  INPUT input;
  input.type = INPUT_MOUSE;
  input.mi.dx = 0;
  input.mi.dy = 0;
  input.mi.dwFlags = (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
  input.mi.mouseData = 0;
  input.mi.dwExtraInfo = NULL;
  input.mi.time = 0;

  cout << runMsg << endl;

  while (true)
  {
    Mat mScreenShot = hwnd2mat(hDesktopWnd);
    
    Mat mResult_Bgr = mScreenShot.clone();
    Mat mSource_Gray;
    cvtColor(mScreenShot, mSource_Gray, COLOR_BGR2GRAY);

    vector<Point2f> matches;
    NMultipleTemplateMatching(mSource_Gray, mTemplate_Gray, thresh, closeness, matches);

    if (matches.empty())
    {
      Sleep(skipMS);
      continue;
    }

    /*
    for (auto& match : matches)
    {
      rectangle(mResult_Bgr, match, Point(match.x + mTemplate_Bgr.cols, match.y + mTemplate_Bgr.rows), Scalar(0, 255, 0), 2);
    }   
    namedWindow("Final Results", WINDOW_NORMAL);
    imshow("Final Results", mResult_Bgr);
    waitKey();
    */

    int x = 0;
    int y = 0;
    for (const auto& match : matches)
    {
      x += match.x;
      y += match.y;
    }
    x /= matches.size();
    y /= matches.size();
    x += mTemplate_Bgr.cols / 2;
    y += mTemplate_Bgr.rows / 2;

    POINT p;
    GetCursorPos(&p);
    SetCursorPos(x, y);
    SendInput(1, &input, sizeof(INPUT));
    SetCursorPos(p.x, p.y);

    cout << hitMsg << "[" << x << "," << y << "]" << endl;
    Sleep(skipMS);
  }
}