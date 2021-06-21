// łańcuch 5 znaków
// po przes. o 3 schodzi w dół, a potem po 3 znowu na górę
// jak się wyświetlacz skończy, to schodzi i zawraca
// w łańcuchu własny symbol
// albo za każdym razem czyścić ekran, albo korzystać z pamięci (2 wektory), i potem przepisywać wektory do pamięci wyświetlacza


#include <msp430x14x.h>
#include "lcd.h"
#include "portyLcd.h"

int x[16], y[16];
void filltabs(int dir)  // 0 - prawo, 1 - dół, 2 - lewo, 3 - góra
{
  for (int i = 0; i < 16; i++)
  {
    if (x[i] >= 1) x[i]--;
    if (y[i] >= 1) y[i]--;

  }
  for (int i = 0; i < 16; i++)
  {
    if (dir == 0)
    {
      if (x[i] == 4 && i != 15) x[i+1] = 5;
      else if (y[i] == 4 && i != 15) y[i+1] = 5;
    }
    if (dir == 1)
    {
      if (x[i] == 4) y[i] = 5;
    }
    if (dir == 2)
    {
      if (x[i] == 4 && i != 0) x[i-1] = 5;
      else if (y[i] == 4 && i != 0) y[i-1] = 5;
    }
    if (dir == 3)
    {
      if (y[i] == 4) x[i] = 5;
    }
  }
}

void headchar()
{
  SEND_CMD(CG_RAM_ADDR);
  SEND_CHAR(18);
  SEND_CHAR(0);
  SEND_CHAR(78);
  SEND_CHAR(32);
  SEND_CHAR(6);
  SEND_CHAR(50);
  SEND_CHAR(20);
  SEND_CHAR(77);    
}


void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;

  for (int i = 0; i < 16; i++) 
  {
    x[i] = 0;
    y[i] = 0;
  }
  
  InitPortsLcd();
  InitLCD();
  headchar();
  
  int countLR = 0, LRdir; // 0 - prawo, 1 - lewo
  int UDpos = 0; // 0 - góra, 1 - dół
  while (1)
  {
    for (LRdir = 0; LRdir <= 1; LRdir++)
    {
      if (LRdir == 0) x[0] = 5;
      if (LRdir == 1) y[15] = 5;
      
      countLR = 0;
      for (int i = 0; i < 27; i++)
      {
        countLR++;
        if (countLR == 3)
        {
          countLR = 0;
          if (UDpos == 0) {
            UDpos = 1;
            filltabs(1);
          } else {
            UDpos = 0;
            filltabs(3);
          }
        }
        else if (LRdir == 0) filltabs(0);
        else if (LRdir == 1) filltabs(2);
        
        clearDisplay();
        SEND_CMD(DD_RAM_ADDR);
        for (int i = 0; i < 16; i++) 
        {
          if (x[i] == 5) SEND_CHAR(0);
          else if (x[i] >= 1) SEND_CHAR('o');
          else SEND_CMD(CUR_SHIFT_RIGHT);        
        }
        SEND_CMD(DD_RAM_ADDR2);    
        for (int i = 0; i < 16; i++) 
        {
          if (y[i] == 5) SEND_CHAR(0);
          else if (y[i] >= 1) SEND_CHAR('o');
          else SEND_CMD(CUR_SHIFT_RIGHT);        
        }
          
        Delayx100us(200);
      }
    }
  }
}
