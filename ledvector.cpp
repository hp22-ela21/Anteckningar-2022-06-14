/*************************************************************************************
* Implementering av libgpiod i C++. I detta exempel anv�nds bibliotekets C-interface,
* d� detta �r mycket enkelt att anv�nda. Det finns ocks� C++-wrappers, s�som klasser
* f�r GPIO-chip samt GPIO-linjer, definierade i headerfilen gpiod.hpp. Dessa klasser
* bygger p� de C-funktioner som anv�nds i detta exempel, men fungerar i �vrigt samma.
* 
* Kompilera programmet och skapa en k�rbar fil d�pt ledvector enligt nedan:
* g++ main.cpp -o ledvector -Wall -l gpiod
*
* K�r sedan programmet via f�ljande kommando:
* ./ledvector
**************************************************************************************/

// Inkluderingsdirektiv:
#include <iostream>
#include <vector>
#include <gpiod.h>
#include <unistd.h>

/*************************************************************************************
* leds_blink: Blinkar lysdioder lagrade i en vektor med valbar f�rdr�jningstid.
**************************************************************************************/
static void leds_blink(std::vector<gpiod_line*>& leds, const std::size_t delay_time)
{
   for (auto& i : leds)
   {
      gpiod_line_set_value(i, 1);
      usleep(delay_time * 1000);
      gpiod_line_set_value(i, 0);
   }
   return;
}

/*************************************************************************************
* leds_off: Sl�cker lysdioder lagrade i en vektor.
**************************************************************************************/
static void leds_off(std::vector<gpiod_line*>& leds)
{
   for (auto& i : leds)
   {
      gpiod_line_set_value(i, 0);
   }
   return;
}

/*************************************************************************************
* main: Ansluter tre lysdioder till PIN 17, 22 och 23 samt en tryckknapp till PIN 27,
*       vilket implementeras via GPIO-linjepekare led1, led2, led3 samt button1. 
*       GPIO-linjepekarna lagras i en vektor f�r enkel blinkning och avst�ngning.
*       Vid behov kan ytterligare lysdioder l�ggas till. Vid nedtryckning av 
*       tryckknappen s� blinkar lysdioderna var 100:e millisekund, �vrig tid 
*       h�lls de sl�ckta.
**************************************************************************************/
int main(void)
{
   auto chip0 = gpiod_chip_open("/dev/gpiochip0");
   auto led1 = gpiod_chip_get_line(chip0, 17);
   auto led2 = gpiod_chip_get_line(chip0, 22);
   auto led3 = gpiod_chip_get_line(chip0, 23);
   auto button1 = gpiod_chip_get_line(chip0, 27);

   gpiod_line_request_output(led1, "led1", 0);
   gpiod_line_request_output(led2, "led2", 0);
   gpiod_line_request_output(led3, "led3", 0);
   gpiod_line_request_input(button1, "button1");

   std::cout << gpiod_line_consumer(led1) << " at PIN " << gpiod_line_offset(led1) << " connected as output!\n";
   std::cout << gpiod_line_consumer(led2) << " at PIN " << gpiod_line_offset(led2) << " connected as output!\n";
   std::cout << gpiod_line_consumer(led3) << " at PIN " << gpiod_line_offset(led3) << " connected as output!\n";
   std::cout << gpiod_line_consumer(button1) << " at PIN " << gpiod_line_offset(button1) << " connected as input!\n";

   std::vector<gpiod_line*> leds = { led1, led2, led3 };

   while (1)
   {
      if (gpiod_line_get_value(button1))
      {
         leds_blink(leds, 100);
      }
      else
      {
         leds_off(leds);
      }
   }

   return 0;
}