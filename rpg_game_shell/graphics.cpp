#include "graphics.h"
#include "globals.h"

void draw_player(int u, int v, int key)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, 0x4E5252);
    uLCD.filled_circle(u, v, 7, 0xFFB2CC);
    uLCD.filled_circle(u+2,v-2,1,0x001AFF);
    uLCD.filled_circle(u+4,v-2,1,0x001AFF);
    uLCD.line(u-1,v+3,u+5,v+3,0x001AFF);
    uLCD.line(u-1,v+5,u,v,0x001AFF);
}

#define YELLOW      0xFFFF00
#define BROWN       0xD2691E
#define DIRT        BROWN
#define GOLD        0xF5B536

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++) {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GOLD;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'K') colors[i] = BLACK;
        else if (img[i] == 'W') colors[i] = WHITE;

        //hamburger
        else if (img[i] == 'S') colors[i] = BACKGROUND;
        else if (img[i] == '1') colors[i] = BUN;
        else if (img[i] == '2') colors[i] = LETTUCE;
        else if (img[i] == 'T') colors[i] = TOMATO;
        else if (img[i] == 'C') colors[i] = CHEESE;
        else if (img[i] == 'M') colors[i] = MEAT;

        //fry guy and chair babies
        else if (img[i] == 'B') colors[i] = DARKBLUE;
        else if (img[i] == '6') colors[i] = SKIN1;
        else if (img[i] == '7') colors[i] = SKIN2;
        else if (img[i] == '8') colors[i] = CHAIR;
        else if (img[i] == '9') colors[i] = CHAIR2;
        else if (img[i] == '5') colors[i] = MOUTH;
        else if (img[i] == 'U') colors[i] = LIGHTBLUE;

        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, 0x4E5252);
}


void draw_plant(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_upper_status(int x, int y, int burgerCount, int burgerToggle)
{


    //rectangle and border
    uLCD.filled_rectangle(0,8,127,0,0xB9B2D1);
    uLCD.line(0, 8, 128, 8, RED);



    uLCD.textbackground_color(0xB9B2D1);
    uLCD.color(0xFF0000);

//add player coordinates
    uLCD.locate(1,0);
    uLCD.printf("(%d,",x);
    uLCD.color(0x0D3572);
    uLCD.locate(6,0);
    uLCD.printf("%d)",y);

}



void draw_lower_status(int burgerCount, int quest,int babyCount)
{
    uLCD.filled_rectangle(0,128,127,118,0xD3E671);
    // Draw top border of status bar
    uLCD.line(0, 118, 128, 118, RED);

    // Add other status info drawing code here

    //BURGER COUNT - change to "GIVE COUNT" when you get certain amount
    if (quest == 0) {
        uLCD.textbackground_color(LIGHTBLUE);
        uLCD.locate(0,15);
        uLCD.color(BLACK);
        uLCD.printf("Burgers: %d/5",burgerCount);
    } else if (quest == 1) { //part where you give burgers
        uLCD.textbackground_color(LETTUCE);
        uLCD.locate(1,15);
        uLCD.color(BLACK);
        uLCD.printf("Charity: %d/5",babyCount);
    }
    else if (quest == 3) {//display that you can drink soda
        uLCD.textbackground_color(CHEESE);
        uLCD.locate(2,15);
        uLCD.color(BLACK);
        uLCD.printf("DRINK DAT SODA!");
        return;
    }
    else if (quest == 5){
     uLCD.textbackground_color(CHEESE);
        uLCD.locate(0,15);
        uLCD.color(BLACK);
        uLCD.printf("Talk 2 Fry Man");
    
    }
    uLCD.filled_circle(120,121,4,BLACK);
    uLCD.filled_circle(120,121,3,LETTUCE);
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, BLUE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, BLUE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, BLUE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, BLUE); // Right
}





void draw_smiley(int u, int v)

{
    char smiley[122] ="YYYYYYYYYYYYYYYYYYYYYYYYRYYYYYRYYYYYYYYYYYYYYYYYYYYYYYYYYRYYYYYRYYYYYRYYYRYYYYYYYRRRYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY";
    draw_img(u,v,smiley);//test drawing image
}

void draw_fryman(int u, int v)
{
    char fryguy[122] = "SCSCSSSCSCSTCCCSCSCCCTTTCCCCCCCTTSTCCCCCCCTSSTT5KT5KTTSSTTKKTKKTTSTTBTTTTTBTTSTTBBBBBTTSSTTTTTTTTTSSSTTSSSTTSSSTTSSSSSTTS";
    draw_img(u,v,fryguy);
}

void draw_wall(int u, int v)
{
    char wall [122] = "WWWWWWWWWWWWKKKKKKKKKWWKTTTTTTTKWWKCCTTTTCKWWKCCTTTCCKWWKCTCCCTCKWWKCTCCCTCKWWKCTTTTTCKWWKCTTTTTCKWWKKKKKKKKKWWWWWWWWWWWW";
    draw_img(u,v,wall);
}

void draw_soda(int u, int v)

{
    char soda[122] = "SSSSSBBBBBSSSSSSBSSSBSSSSWWWWWSSSSSKTTTTTKSSSSKTUUUTKSSSSKTUTTTKSSSSKTUUUTKSSSSKTTTUTKSSSSKTUUUTKSSSSKTTTTTKSSSSKKKKKKKSS";
    draw_img(u,v,soda);
}

void draw_wall2(int u, int v)
{
    char wall2[122] = "KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKGGGGGGGKKKKGGGGGGGKKKKGGGGGGGKKKKGGGGGGGKKKKGGGGGGGKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK";
    draw_img(u,v,wall2);
}

void draw_burger(int u, int v)

{
    char hamburger[122] = "SS1111111SSS111111111S11111111111S222222222SSTTTTTTTTTSSCCCCCCCCCSSMMMMMMMMMSSMMMMMMMMMS11111111111S111111111SSS1111111SS";
    draw_img(u,v,hamburger);
}

void draw_baby1(int u, int v)

{
    char baby1[122] = "SSKKKKKKKSSSK6666666KSKK6666666KKK6WKK6WKK6KK6KKK6KKK6KK6KKK6KKK6KK6UU666UU6KK6U6KKK6U6KK6U6WWW6U6KKK6666666KKSKKKKKKKKKS";
    draw_img(u,v,baby1);
}

void draw_baby2(int u, int v)

{
    char baby2[122] = "SSKKKKKKKSSSK7777777KSKK7777777KKK7KKW7KKW7KK7KKK7KKK7KK7KKK7KKK7KK7UU777UU7KK7U7KKK7U7KK7U7WWW7U7KKK7777777KKSKKKKKKKKKS";
    draw_img(u,v,baby2);
}

