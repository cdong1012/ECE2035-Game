#include "speech.h"

#include "globals.h"
#include "hardware.h"
#include "graphics.h"


GameInputs inputs;
Timer waitTime;


/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();


/**
 * Erase the speech bubble.
 */
//static void erase_speech_bubble();


/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();



/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
int toggle = 0;
static void draw_speech_line( char* line);



void draw_speech_bubble()
{
    uLCD.filled_rectangle(0, 113 , 128, 93, 0xAAA3AB); //big purple rectangle

    //border of bubble
    uLCD.filled_rectangle(0,    93, 128,  94, 0xA007AB); // Top
    uLCD.filled_rectangle(0,   113, 128, 112, 0xA007AB); // Bottom

    uLCD.filled_rectangle(0,    113,   1, 93, 0xA007AB); // Left
    uLCD.filled_rectangle(128,  113, 126, 93, 0xA007AB); // Right
}


static void speech_bubble_wait()
{
    waitTime.start();
    int timePass;

    do {
        // wait until ~1.5 seconds pass before being able to remove
        inputs = read_inputs();
        timePass = waitTime.read_ms();

        wait_ms(200);
        toggle = !toggle;
        if (toggle) uLCD.filled_circle(120,121,4,RED);
        else uLCD.filled_circle(120,121,4,BLACK);
        inputs = read_inputs();
    } while(inputs.b1 || !(timePass > 300 && !inputs.b1));

    uLCD.filled_circle(120,121,4,BLACK);
    return;
}


void draw_speech_line( char* line)
{

    uLCD.text_bold(TEXTBOLD);
    uLCD.textbackground_color(0xAAA3AB);
    uLCD.color(BLACK);
    const char* space = " ";
    int len = strlen(line);

    int i;
    while (*line)

    {
        uLCD.locate(1,12);
        i = 0;
        draw_speech_bubble();

        while (i<17 && *line) {

            if (i == 0) {
                uLCD.text_bold(TEXTBOLD);

                if (*line == *space) {
                    i++;
                    line++;
                    continue;
                }
            }

            uLCD.printf("%c", *line);
            wait_ms(55);
            i++;
            line++;
        }
        if (!*line) break;
        uLCD.locate(1,13);

        while(i<34 && *line) {
            if (i == 17) {
                uLCD.text_bold(TEXTBOLD);

                if (*line == *space) {
                    i++;
                    line++;
                    continue;
                }
            }

            uLCD.printf("%c", *line);
            wait_ms(50);
            i++;
            line++;
        }
         speech_bubble_wait();

    }
    //if(len > 36) speech_bubble_wait();
    //else 
    //wait_ms(250);
}



void speech( char* line)
{
    draw_speech_bubble();
    draw_speech_line(line);
    draw_border();
    uLCD.filled_circle(120,121,3,LETTUCE);
}
