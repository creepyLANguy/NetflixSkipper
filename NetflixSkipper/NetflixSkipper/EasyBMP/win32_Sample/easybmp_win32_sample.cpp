/*************************************************
*                                                *
*  EasyBMP Cross-Platform Windows Bitmap Library * 
*                                                *
*  Authors: Paul Macklin                         *
*           Michael Nistler                      *
*   email: pmacklin@math.uci.edu                 *
*          <no_email_given>                      *
*                                                *
*    file: easybmp_win32_sample.cpp              *
*    date: 8-22-2005                             *
* version: 1.05.00                               *
*                                                *
*   License: BSD (revised)                       *
* Copyright: 2005-2006 by the EasyBMP Project    * 
*                                                *
* description: test of the EasyBMP_win32         *
*              extension                         *
*                                                *
*************************************************/

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <windows.h>

#include "EasyBMP.h"
#include "EasyBMP_win32.h"
#include "EasyBMP_Font.h"

using namespace std;

BOOL SaveBitmap(HDC hDC,HBITMAP hBitmap,const char* szPath);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int iCmdShow)
{ 
 char* DATE = "August 25";
 char* YEAR = "2005";
 char* AUTHOR = "Paul Macklin";
 char* LICENSE = "GPL v. 2";
 char* GROUP = "the EasyBMP Project";
 char* CONTACT = "http://easybmp.sourceforge.net"; 
 
 // spit out copyright / license information
 
 char* szAboutMessageBox;
 szAboutMessageBox = new char [2049];
 
 strcpy( szAboutMessageBox, "Created on ");
 strcat( szAboutMessageBox, DATE );
 strcat( szAboutMessageBox, ", ");
 strcat( szAboutMessageBox, YEAR );
 strcat( szAboutMessageBox, "\n\nUses the EasyBMP library, Version ");
// strcat( szAboutMessageBox, _EasyBMP_Version_ );
 strcat( szAboutMessageBox, "\n\nLicensed under " );
 strcat( szAboutMessageBox, LICENSE );
 strcat( szAboutMessageBox, " by " );
 strcat( szAboutMessageBox, GROUP );
 strcat( szAboutMessageBox, "\nCopyright (c) " );
 strcat( szAboutMessageBox, YEAR );
 strcat( szAboutMessageBox, " by " );
 strcat( szAboutMessageBox, GROUP );
 strcat( szAboutMessageBox, "\n\nContact: " );
 strcat( szAboutMessageBox, CONTACT );

 MessageBox(NULL,szAboutMessageBox,"About",MB_ICONINFORMATION);	   

 /*
   First, prepare a test font file. This tests geometry and font 
   routines
 */
 
 char* OutputFilename = "FontTest.bmp";
 int i,j;
 
 BMP FontOutput;
 FontOutput.SetSize( 570 , 330 );
 
 char TestString1 [2049];
 strcpy( TestString1 ,  "08: the quick brown fox jumps over the lazy dog.");
 char TestString2 [2049];
 strcpy( TestString2 ,  "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.");
 char TestString3 [2049];
 strcpy( TestString3 ,  "`1234567890-=[]\\;',./~!@#$%^&* ( ) _+{}|:\"<>?c");
 TestString3[strlen(TestString3)-1] = COPYRIGHT_SYMBOL;
 
 RGBApixel FontColor;
 FontColor.Red = 0;
 FontColor.Green = 0;
 FontColor.Blue = 0;
 
 RGBApixel BackgroundColor;
 BackgroundColor.Red = 192;
 BackgroundColor.Green = 192;
 BackgroundColor.Blue = 192; 
 
 for( j=0 ; j < FontOutput.TellHeight() ; j++ )
 {
  for( i=0 ; i < FontOutput.TellWidth() ; i++ )
  {
   *FontOutput(i,j) = BackgroundColor;  
  }
 }

 int Position = 3;
 int PixelsPerRow = 13;
 int Size = 8;
 
 PrintString( FontOutput , TestString1 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString2 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString3 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 
 TestString1[0] = '1';
 TestString1[1] = '0';
 PixelsPerRow += 2;
 Size += 2; 
 FontColor.Red = 255;
  
 PrintString( FontOutput , TestString1 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString2 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString3 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 
 TestString1[1] = '2';
 PixelsPerRow += 2;
 Size += 2;
 FontColor.Red = 0;
 FontColor.Green = 128;
 
 PrintString( FontOutput , TestString1 , 3 , Position , Size, FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString2 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString3 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 
 TestString1[1] = '4';
 PixelsPerRow += 2;
 Size += 2;
 FontColor.Green = 0;
 FontColor.Blue = 255;
 
 PrintString( FontOutput , TestString1 , 3 , Position , Size, FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString2 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString3 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 
 TestString1[1] = '6';
 PixelsPerRow += 2;
 Size += 2;
 FontColor.Red = 128;
 
 PrintString( FontOutput , TestString1 , 3 , Position , Size, FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString2 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString3 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 
 TestString1[1] = '8';
 PixelsPerRow += 2;
 Size += 2;
 FontColor.Red = 0;
 FontColor.Green = 128;
 
 PrintString( FontOutput , TestString1 , 3 , Position , Size, FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString2 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 PrintString( FontOutput , TestString3 , 3 , Position , Size , FontColor );
 Position += PixelsPerRow;
 
 FontOutput.WriteToFile( OutputFilename );               

 // setup
 
 WNDCLASS wc;
 HWND hWnd;
 HDC hDC;
 HGLRC hRC;
 MSG msg;
 BOOL quit = FALSE;
	
 // register window class
	
 wc.style = CS_OWNDC;
 wc.lpfnWndProc = WndProc;
 wc.cbClsExtra = 0;
 wc.cbWndExtra = 0;
 wc.hInstance = hInstance;
 wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
 wc.hCursor = LoadCursor( NULL, IDC_ARROW );
 wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
 wc.lpszMenuName = NULL;
 wc.lpszClassName = "EasyBMP_win32";
 RegisterClass( &wc );
    
 int width = 257;
 int height = 256;
	
 // create main window

 hWnd = CreateWindow( 
        "EasyBMP_win32", "EasyBMP_win32 Sample Application", 
        WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
        10, 10, width, height,
        NULL, NULL, hInstance, NULL );
        
 // get window-only cap
    
 BMP WindowImage;
 CaptureWindow(hWnd , WindowImage );
 
 // overwrite it
 RGBApixel WHITE;
 RGBApixel RED;
 RGBApixel BLUE;
 
 WHITE.Red = 255;
 WHITE.Green = 255;
 WHITE.Blue = 255;
 RED.Red = 255;
 RED.Green = 0;
 RED.Blue = 0;
 BLUE.Red = 0;
 BLUE.Green = 0;
 BLUE.Blue = 255;
  
 int CurrentPosition; 
  
 CurrentPosition = PrintString( WindowImage, "Easy" , 10 , 35 , 16 , BLUE );
 CurrentPosition = PrintString( WindowImage, "BMP " , CurrentPosition , 35 , 16 , RED );
 CurrentPosition = PrintString( WindowImage, "win32 test." , CurrentPosition , 35 , 16 , WHITE );

 // write this to a file
 
 WindowImage.WriteToFile("WindowCapture.bmp");
    
 // load an image
	
 HBITMAP hLoadedBitmap = 
 (HBITMAP) ::LoadImage(hInstance, "FontTest.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
 HDC tempDC = ::GetDC(NULL);

 // convert the win32 HBITMAP into a BMP object, and write it to a file
	
 BMP TempImage;
 HBITMAPtoBMP( tempDC , hLoadedBitmap , TempImage );
 TempImage.WriteToFile("HBITMAPtoBMP1.bmp");
	
 // convert this image to another HBITMAP/HDC pair
	
 HBITMAP hConvertedBitmap = BMPtoHBITMAP(tempDC,TempImage);
		
 // copy part of this to the screen (I don't know how.)
		
 // do a manual partial screencap, convert it to a BMP object, and write it to a file
        
 HDC screenDC = ::CreateDC("DISPLAY", NULL, NULL, NULL);
 HDC memoryDC = ::CreateCompatibleDC(screenDC);

 HBITMAP screenBitmap = ::CreateCompatibleBitmap(screenDC, width-12,height-10 );
 HGDIOBJ result =  ::SelectObject(memoryDC, screenBitmap);
	
 if( result )
 {
  ::BitBlt(memoryDC, 0, 0, width-12,height-10, screenDC, 0, 0, SRCCOPY);

  // convert the partial manual screencap to a BMP, and write it to a file
	 
  BMP Image;
  HBITMAPtoBMP(screenDC,screenBitmap,Image);
  Image.WriteToFile("HBITMAPtoBMP2.bmp");
	 
  // convert the BMP to a HBITMAP object and save it to prove it works
	 
  HBITMAP blah = BMPtoHBITMAP(screenDC,Image);
  ::SaveBitmap(screenDC,blah,"BMPtoHBITMAP.bmp");	 
 }
 else
 {
  MessageBox(NULL,"Failed in partial manual screenshot!","Error",MB_ICONWARNING);
 }

 ::DeleteDC(screenDC);
 ::DeleteDC(memoryDC);
 ::DeleteObject(result);
 ::DeleteObject(screenBitmap);       

 // get a full screen capture, and write it to a file	
	
 BMP ScreenShot;
 CaptureScreen( ScreenShot );
 ScreenShot.WriteToFile("ScreenCapture1.bmp");
	
 // program main loop
 while ( !quit )
 {
  // check for messages
  if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
  {
   // handle or dispatch messages
   if ( msg.message == WM_QUIT ) 
   { quit = TRUE; } 
   else 
   {
	TranslateMessage( &msg );
	DispatchMessage( &msg );
   }
  } 
  else 
  {
   // put in an sleep function to reduce CPU usage 
   ::Sleep( 100 );		 
  }
 }
	
 // destroy the window explicitly
 DestroyWindow( hWnd );
	
 return msg.wParam;	
}

// Window Procedure

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 if( message == WM_CREATE )
 { return 0; }
 if( message == WM_CLOSE )
 {
  // get a capture of the foreground window
 
  BMP ForegroundWindow;
  CaptureForegroundWindow( ForegroundWindow );
  ForegroundWindow.WriteToFile("ForegroundWindow.bmp");

  // get a full screen capture
  
  BMP Screencap;
  CaptureScreen( Screencap );
  Screencap.WriteToFile("ScreenCapture2.bmp");

  PostQuitMessage( 0 );
  return 0;
 }

 if( message == WM_DESTROY )
 { return 0; }
 
 if( message == WM_KEYDOWN )
 {
  switch ( wParam )
  {
   case VK_ESCAPE:
   PostQuitMessage(0);
   return 0;
  }
  return 0;
 }
 
 return DefWindowProc( hWnd, message, wParam, lParam );
}

// Function to save bitmap
BOOL SaveBitmap(HDC hDC,HBITMAP hBitmap,const char* szPath)
{
 FILE * fp= NULL;
 fp = fopen(szPath,"wb");
 if(fp == NULL)
 { return false; }
    
 BITMAP Bm;
 BITMAPINFO BitInfo;
 ZeroMemory(&BitInfo, sizeof(BITMAPINFO));
 BitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 BitInfo.bmiHeader.biBitCount = 0;

 if(!::GetDIBits(hDC, hBitmap, 0, 0, NULL, &BitInfo, DIB_RGB_COLORS))
 { return (false); }

 Bm.bmHeight = BitInfo.bmiHeader.biHeight;
 Bm.bmWidth  = BitInfo.bmiHeader.biWidth;

 BITMAPFILEHEADER    BmHdr;
    
 BmHdr.bfType        = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
 BmHdr.bfSize        = (((3 * Bm.bmWidth + 3) & ~3) * Bm.bmHeight) 
                     + sizeof(BITMAPFILEHEADER) 
                     + sizeof(BITMAPINFOHEADER);
 BmHdr.bfReserved1    = BmHdr.bfReserved2 = 0;
 BmHdr.bfOffBits      = (DWORD) sizeof(BITMAPFILEHEADER) 
                      + sizeof(BITMAPINFOHEADER);
    
 BitInfo.bmiHeader.biCompression = 0;
 // Writing Bitmap File Header ////
 fwrite(&BmHdr,sizeof(BITMAPFILEHEADER),1,fp);

 fwrite(&BitInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);

 BYTE *pData = new BYTE[BitInfo.bmiHeader.biSizeImage + 5];
 if(!::GetDIBits(hDC, hBitmap, 0, Bm.bmHeight, 
                 pData, &BitInfo, DIB_RGB_COLORS))
 { return (false); }
 if( pData != NULL)
 { fwrite(pData,1,BitInfo.bmiHeader.biSizeImage,fp); }
 fclose(fp);
 delete (pData);

 return (true);
}
