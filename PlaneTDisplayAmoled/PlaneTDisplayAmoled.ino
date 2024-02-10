
#include "gauges.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

#define LILYGO_TDISPLAY_AMOLED_SERIES
#include "LilyGo_AMOLED.h" 

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite alti = TFT_eSprite(&tft);
TFT_eSprite bck = TFT_eSprite(&tft);
TFT_eSprite gng = TFT_eSprite(&tft);
LilyGo_Class amoled;

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#define blue1 0x0559
#define brown1 0x7800
#define blue2 0x0370
#define brown2 0x0187

unsigned short grays[13];
double rad=0.01745;
int vol,deb,deb2=0;

unsigned long st,en;
float fps=0;

int ax=120;
int ay=120;
int ar=120;
int axpos[360];
int aypos[360];
int valueX=0;
int valueY=0;
int valueX2=0;

int linesAngles[6]={0,180,210,240,300,330};
int dotsAngles[6]={225,250,260,280,290,315};
int linesX[6],linesY[6],linesX2[6],linesY2[6],dotX[6],dotY[6],dotX2[6],dotY2[6];


void drawGNG()
{
gng.pushImage(0,0,120,120,gauges[0]);
amoled.pushColors(10, 0, 120, 120, (uint16_t *)gng.getPointer());
gng.pushImage(0,0,120,120,gauges[1]);
amoled.pushColors(10, 120, 120, 120, (uint16_t *)gng.getPointer());
gng.pushImage(0,0,120,120,gauges[2]);
amoled.pushColors(398, 0, 120, 120, (uint16_t *)gng.getPointer());
gng.pushImage(0,0,120,120,gauges[3]);
amoled.pushColors(398, 120, 120, 120, (uint16_t *)gng.getPointer());
}

void setup() {
 
  pinMode(0,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  amoled.begin();
  amoled.setBrightness(110);
  
   bck.createSprite(280,280);
   alti.createSprite(240,240);
   gng.createSprite(120,120);

   gng.setSwapBytes(1);   
     Wire.begin(43,44);   // will pause Zero, Leonardo, etc until serial console opens
     lis.begin(0x19);
  
    for(int i=0;i<360;i++)
    {
       axpos[i]=(ar*cos(rad*(i)))+ax;
       aypos[i]=(ar*sin(rad*(i)))+ay;
    }

    for(int i=0;i<6;i++)
    {
      linesX[i]=(110*cos(rad*(linesAngles[i])))+ax;
      linesY[i]=(110*sin(rad*(linesAngles[i])))+ay;
      linesX2[i]=(86*cos(rad*(linesAngles[i])))+ax;
      linesY2[i]=(86*sin(rad*(linesAngles[i])))+ay;

       dotX[i]=(98*cos(rad*(dotsAngles[i])))+ax;
      dotY[i]=(98*sin(rad*(dotsAngles[i])))+ay;
      dotX2[i]=(86*cos(rad*(dotsAngles[i])))+ax;
      dotY2[i]=(86*sin(rad*(dotsAngles[i])))+ay;
    }

     int co=220;
     for(int i=0;i<13;i++)
     {grays[i]=tft.color565(co, co, co);
     co=co-20;}

     drawGNG();
}


  void drawAlti()
  { 
    alti.setPivot(ax, ay-valueY);
    bck.fillRect(0,0,281,138,blue1);
    bck.fillRect(0,138,281,4,TFT_WHITE);
    bck.fillRect(0,142,281,138,brown1);

    bck.drawWedgeLine(0,172,281,172,1,1,grays[2]);
    bck.drawWedgeLine(0,214,281,214,1,1,grays[2]);

    bck.drawWedgeLine(140,140,0,235,1,1,grays[3]);
    bck.drawWedgeLine(140,140,281,235,1,1,grays[3]);
    bck.drawWedgeLine(140,140,100,280,1,1,grays[3]);
    bck.drawWedgeLine(140,140,180,280,1,1,grays[3]);

    bck.fillTriangle(140,64+valueY,134,80+valueY,146,80+valueY,TFT_WHITE);

    alti.fillSprite(TFT_DARKGREY);
    bck.pushRotated(&alti,valueX);
    
    //alti.drawWedgeLine(axpos[valueX],aypos[valueX],axpos[valueX2],aypos[valueX2],3,3,TFT_WHITE);
    alti.drawArc(ax,ay,210,120,0,359,TFT_BLACK,blue2,false);
    alti.drawArc(ax,ay,120,84,0,90,brown2,TFT_BLACK,false);
    alti.drawArc(ax,ay,120,84,270,360,brown2,TFT_BLACK,false);
    alti.drawArc(ax,ay,120,84,90,270,blue2,TFT_BLACK,false);

    alti.drawArc(ax,ay,150,112,0,360,grays[8],TFT_BLACK,false);
    alti.drawArc(ax,ay,114,110,30,330,grays[5],TFT_BLACK,false);

    for(int i=0;i<6;i++)
    {
     alti.drawWedgeLine(linesX[i],linesY[i],linesX2[i],linesY2[i],2,2,TFT_WHITE);
     if(i==0 || i==5)
     alti.fillCircle(dotX[i],dotY[i],4,TFT_WHITE);
     else
     alti.drawWedgeLine(dotX[i],dotY[i],dotX2[i],dotY2[i],1,1,TFT_WHITE);
    }
    alti.fillTriangle(110, 3, 130, 3, 120, 24, TFT_WHITE);
    
   alti.fillRect(32,190,176,24,grays[8]);
   alti.fillRect(32,190,176,4,grays[5]);
   alti.fillRect(50,214,140,24,grays[8]);

  alti.drawWedgeLine(ax-40, ay,  ax-18, ay, 2, 2, TFT_YELLOW);
  alti.drawWedgeLine(ax+40, ay,  ax+18, ay, 2, 2, TFT_YELLOW);
  alti.drawArc(ax,ay,18,15,0,90,TFT_YELLOW,TFT_BLACK,false);
  alti.drawArc(ax,ay,18,15,270,360,TFT_YELLOW,TFT_BLACK,false);
  alti.fillCircle(ax,ay,2,TFT_YELLOW);

  alti.fillRect(105,ay-41,30,3,TFT_YELLOW);
  alti.fillRect(110,ay-21,20,3,TFT_YELLOW);
 
  alti.fillRect(115,ay-31,10,3,TFT_YELLOW);

  alti.fillRect(105,ay+41,30,3,TFT_YELLOW);
  alti.fillRect(110,ay+21,20,3,TFT_YELLOW);
 
  alti.fillRect(115,ay+31,10,3,TFT_WHITE);

    alti.setTextColor(grays[3],grays[8]);
    alti.drawString("ALTITUDE",40,196,2);
    alti.setTextColor(grays[6],grays[8]);
    alti.drawString("VP24",140,196,4);
    //alti.drawString(String(valueX),60,210,4);
    //alti.drawString(String(valueY),160,210,4);

    alti.fillCircle(ax, 215, 17, grays[10]);
    alti.fillCircle(25, 25, 9, grays[9]);
    alti.fillCircle(215, 25, 9, grays[9]);
    alti.fillCircle(25, 215, 9, grays[9]);
    alti.fillCircle(215, 215, 9, grays[9]);

    amoled.pushColors(148, 0, 240, 240, (uint16_t *)alti.getPointer());
  }



int temp,temp2=0;
int t=0;

void loop() {
  
st=millis();
vol = (analogRead(4) * 2 * 3.3 * 1000) / 4096; 

  sensors_event_t event;
  lis.getEvent(&event);

  temp2=temp2+event.acceleration.y*100;
  temp=temp+event.acceleration.x*100;
  t++;
  
  if(t==3){
    temp=temp/t;
    temp2=temp2/t;

     if(temp2>=0)
  {valueY=map(temp2,0,1000,0,90);
  if(valueY>90) valueY=90;
  }

   if(temp2<0)
  {valueY=map(temp2,-1,-1000,-1,-90);
  if(valueY<-90) valueY=-90;
  }

  if(temp>=0)
  {valueX=map(temp,0,1000,0,89);
  if(valueX>89) valueX=89;
  valueX2=valueX+180;}

   if(temp<0)
  {valueX=map(temp,-1,-1000,359,269);
  if(valueX<269) valueX=269;
  valueX2=valueX-180;}

  temp2=0;
  temp=0;
  t=0;
  }

if(digitalRead(14)==0)
{
if(deb==0)
{deb=1; }
}else deb=0;

if(digitalRead(0)==0)
{
if(deb2==0)
{deb2=1; 

}  
}else deb2=0;

drawAlti();
en=millis();
fps=(1000.00/(en-st));
}



