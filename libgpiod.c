/*************************************************************************************
* Implementering av libgpiod, Linux officiella inteface f�r GPIO-implementering.
* Aktuellt GPIO-chip gpiochip0 innehar enhetens samtliga GPIO-linjer (PINs).
* GPIO-chippet m�ste �ppnas vid start f�r att kunna aktivera GPIO-linjer f�r
* sensorer och aktuatorer.
*
* Kompilera programmet och skapa en k�rbar fil d�pt libgpiod1 enligt nedan:
* gcc main.c -o libgpiod1 -Wall -l gpiod
*
* K�r sedan programmet via f�ljande kommando:
* ./libgpiod1
**************************************************************************************/

/* Inkluderingsdirektiv: */
#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

/*************************************************************************************
* leds_blink: Blinkar tv� lysdioder med valbar f�rdr�jningstid.
**************************************************************************************/
static void leds_blink(struct gpiod_line* led1, struct gpiod_line* led2, const size_t delay_time)
{
   gpiod_line_set_value(led1, 1);
   gpiod_line_set_value(led2, 0);
   usleep(delay_time * 1000);

   gpiod_line_set_value(led1, 0);
   gpiod_line_set_value(led2, 1);
   usleep(delay_time * 1000);

   gpiod_line_set_value(led2, 0);
   return;
}

/*************************************************************************************
* main: Ansluter tv� lysdioder till PIN 17 och 22 samt en tryckknapp till PIN 27,
*       vilket implementeras via GPIO-linjepekare led1, led2 samt button1. Vid
*       nedtryckning av tryckknappen blinkar lysdioderna var 500:e millisekund,
*       �vrig tid h�lls de sl�ckta.
**************************************************************************************/
int main(void)
{
   struct gpiod_chip* chip0 = gpiod_chip_open("/dev/gpiochip0");
   struct gpiod_line* led1 = gpiod_chip_get_line(chip0, 17);
   struct gpiod_line* led2 = gpiod_chip_get_line(chip0, 22);
   struct gpiod_line* button1 = gpiod_chip_get_line(chip0, 27);

   gpiod_line_request_output(led1, "led1", 0);
   gpiod_line_request_output(led2, "led2", 0);
   gpiod_line_request_input(button1, "button1");

   printf("%s at PIN %u connected as output!\n", gpiod_line_consumer(led1), gpiod_line_offset(led1));
   printf("%s at PIN %u connected as output!\n", gpiod_line_consumer(led2), gpiod_line_offset(led2));
   printf("%s at PIN %u connected as input!\n", gpiod_line_consumer(button1), gpiod_line_offset(button1));

   while (1)
   {
      if (gpiod_line_get_value(button1))
      {
         leds_blink(led1, led2, 500);
      }
      else
      {
         gpiod_line_set_value(led1, 0);
         gpiod_line_set_value(led2, 0);
      }
   }

   return 0;
}