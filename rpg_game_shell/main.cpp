// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

int omni_pressed = 0;
int speechTog =  0;
int startScreen = 1;


int burgerToggle = 0; // when you're done with burgers
int questToggle = 0; // to be able to be told what to do with the burgers
int babyToggle = 0; // to be able to feed babies
int narratorToggle = 0;
int sodaToggle = 0;

GameInputs in;
int action;
int update;

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();

//my added functions
int go_right(int x, int y);
int go_left(int x, int y);
int go_up(int x, int y);
int go_down(int x, int y);

int check_obj(int x, int y);
void draw_start();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int burgerCount;
    int prevBurgerCount;
    int babyCount;
    int prevBabyCount;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define OMNI_MODE 7
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define END_GAME 7


char *lineStartQuest = "Mr. Fry Guy: Kirby, welcome to McDonald's. Land of the Free.  I am actually the most powerful being in the Universe, Mr. Fry Guy. so don't try to eat me you fat lard." \
                       " All I ask of you is to please not eat "\
                       "the hamburgers scattered throughout the land. There a bunch of "\
                       "hungry babies and if you eat they will starve. Got it? Now get out of my sight you disgusing pink blob.";

char *narrator1   = "Mr. Narrator: Kirby, being the fat ass he is decides to eat all the burgers he can. After all he's in Smash Bros and that fry dude isn't so why should Kirby listen to him!? Time to eat some burgers.";

char *kirbyFull = "Mr. Narrator: Kirby ate all he could fit in his stomach. What a fat ass. You think of how the fry man told you not to eat the burgers and you are fille with guilt." \
                  "You think of escaping McDonald's but then you remember the fry man is all powerful and your only option is to ask for forgiveness. Shame on you Kirby. :(";

char *kirbyFull2 = "You can't eat anymore. Give it up. You gotta go ask the fry man for forgiveness.";

char *questPart2 = "Mr. Fry Guy: Hey Kirby. Sorry for being so rude earlier, my wife and I got in a fight about whether she could dip me in Ketchup for her pleasure. Anyway, too much information." \
                   "...... Wait. Kirby why is your belly so full!? I should smite you right now with my godly powers but I'll give you one more chance. Find the babies scattered throughout the map."\
                   "Regurgiate your burgers and feed 5 babies. Maybe then you will find happiness in giving to others. Out of my sight you insufferable fool.";


char *questPart3 = "Mr. Fry Guy: I gotta say, you surprised me Kirby. Maybe you're not so bad after all. Wait... Did you actually throw up on the babies.....Ok, that's gross, I thought you knew I was joking," \
                   " you should have given them fresh burgers but whatever they're dumb babies anyway. I guess you probably want to leave McDonald's now don't ya. To get out I recommend finding the delicious soda hidden" \
                   " in part of the map. I'm giving you the divine ability of drinking the soda. Once you do, talk to me again, and I'll teleport you out of McDonald's. See you then :)";

char *babysFed =   "Mr. Narrator : You fed enough babies! Go talk to the Mr Fry Man to see what he has to say now .";

char *cantDrink = "Kirby does not yet possess the power to drink that delicious soda.";
char *drankSoda = "Kirby drank the delicious soda. YUMMMMMMM. Slurp. Gulp. BURP.";

char *wonGame   = "Mr. Fry Guy: Hey Kirby. Thanks so much for playing my game. Touch my delicious salty head and I'll teleport you outta here.";

char *wonGame2 = "Mr. Narrator: And with that, Kirby's amazing visit to McDonald's was over. He learned the true meaning of life, giving to others. But would he stop being a fat ass? Of course not, because he's Kirby, the fattest pink man to ever live.";

char *smileyLine = "Please don't eat me Kirby- san. :3. hehehe";

char *burgerLine = "Oh shit you ate a burger";

char *babyLine1 = "Feed me, bitch. Or I'll tell my mommy.";
char *babyLine2 = "WAAAAHH! GIVE ME BURGER";



int get_action(GameInputs inputs)
{

    //to signal next part of quest
    if (Player.burgerCount == BURGER_COUNT) {
        wait_ms(800);
        speech(kirbyFull);
        Player.burgerCount++;
         draw_lower_status(0,5,1);
        questToggle = 1;
        return GO_DOWN;
    }

    if(Player.babyCount == BABY) {
        wait_ms(800);
        speech(babysFed);
        Player.babyCount++;
        draw_lower_status(0,5,1);
        questToggle = 2;
        return GO_DOWN;
    }

    //for narrator at beginning
    if (narratorToggle == 1) {
        wait_ms(800);
        speech(narrator1);
        narratorToggle = 0;
        draw_lower_status(0, 0, 0);
        return GO_DOWN;
    }
    if(babyToggle) {
        draw_lower_status(5,1,0);
        babyToggle = 0;
    }

    if (sodaToggle == 2) {
        wait_ms(800);
        speech(wonGame2);
        sodaToggle = 3;
        wait_ms(800);
        return END_GAME;
    }

    // for omni mode
    if (!inputs.b2) omni_pressed = !omni_pressed;

    //for action button
    if (!inputs.b1) return ACTION_BUTTON;

    if (inputs.ay >= 0.43) return GO_UP;
    if (inputs.ay < -0.55) return GO_DOWN;
    if (inputs.ax < -0.50)  return GO_LEFT;
    if (inputs.ax >= 0.50) return GO_RIGHT;

    else return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 4
#define FULL_DRAW 2
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    Player.prevBurgerCount = Player.burgerCount;
    Player.prevBabyCount = Player.babyCount;
    if (Player.burgerCount  >= BURGER_COUNT) burgerToggle = 1;
    if(Player.babyCount == BABY) babyToggle = 0;

    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action) {
        case GO_UP:
            if (go_up(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case GO_LEFT:
            if (go_left(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case GO_DOWN:
            if (go_down(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case GO_RIGHT:
            if (go_right(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case ACTION_BUTTON:
            if(check_obj(Player.x,Player.y))return FULL_DRAW;
            else break;
        case MENU_BUTTON:
            break;
        case END_GAME:
            return GAME_OVER;
        default:
            break;
    }
    return NO_RESULT;
}

int check_obj(int x, int y)

{
    MapItem *item1 = get_east(x+0.5, y); //get item to right
    MapItem *item2 = get_east(x+0.5,y-1);
    MapItem *item3 = get_west(x-1, y); //get item to left
    MapItem *item4 = get_west(x-1,y-1);
    MapItem *item5 = get_north(x-1, y-1); //get item to north
    MapItem *item6 = get_north(x, y-1);
    MapItem *item7 = get_north(x-1, y);
    MapItem *item8 = get_south(x, y+0.5); //get item to south
    MapItem *item9 = get_south(x-1, y+0.5);


    //check what type
    //SMILEY
    if (item1->type == SMILEY || item2->type == SMILEY || item3->type == SMILEY || item4->type == SMILEY \
            ||item5->type == SMILEY || item6->type == SMILEY || item7->type == SMILEY || item8->type == SMILEY \
            || item9->type == SMILEY) {
        speech(smileyLine);
        return 1;
    }
///////////////////
    //FRYGUY
    if (item1->type == FRY || item2->type == FRY || item3->type == FRY || item4->type == FRY \
            ||item5->type == FRY || item6->type == FRY || item7->type == FRY || item8->type == FRY \
            || item9->type == FRY) {
        if (questToggle == 0) {
            speech(lineStartQuest);
            narratorToggle = 1;
        }

        else if(questToggle == 1) {
            speech(questPart2);
            babyToggle = 1;
        } else if (questToggle == 2) {
            speech(questPart3);
            draw_lower_status(0,3,0);
            sodaToggle = 1;
        } else if (questToggle == 3) {
            speech(wonGame);
            sodaToggle = 2;
        }
        return 1;
    }
//////////////////////




    if (item1->type == BURGER || item2->type == BURGER || item3->type == BURGER || item4->type == BURGER \
            ||item5->type == BURGER || item6->type == BURGER || item7->type == BURGER || item8->type == BURGER \
            || item9->type == BURGER) {

        if(Player.burgerCount < BURGER_COUNT) {
            map_erase(x+1.5,y);
            map_erase(x+1.5,y-1);
            map_erase(x-2,y-1);
            map_erase(x-2,y);
            map_erase(x-1,y-1);
            map_erase(x-1,y-2);
            map_erase(x,y-2);
            map_erase(x+1.5,y);
            map_erase(x,y+1.5);
            map_erase(x,y+1.5);
            map_erase(x-1,y+1.5);

            speech(burgerLine);
            Player.burgerCount++;

            return 1;
        } else {
            speech(kirbyFull2);
            return 1;
        }

    }

//for babies
    if (item1->type == BABY || item2->type == BABY || item3->type == BABY || item4->type == BABY \
            ||item5->type == BABY || item6->type == BABY || item7->type == BABY || item8->type == BABY \
            || item9->type == BABY) {

        if(questToggle == 1) {

            if (!strcmp((char*)item1->data, "baby1") || !strcmp((char*)item2->data, "baby1")|| !strcmp((char*)item3->data, "baby1") || !strcmp((char*)item4->data, "baby1") \
                    ||!strcmp((char*)item5->data, "baby1") || !strcmp((char*)item6->data, "baby1")||!strcmp((char*)item7->data, "baby1")|| !strcmp((char*)item8->data, "baby1") \
                    || !strcmp((char*)item9->data, "baby1")) {
                speech(babyLine2);
                Player.babyCount++;

                map_erase(x+1.5,y);
                map_erase(x+1.5,y-1);
                map_erase(x-2,y-1);
                map_erase(x-2,y);
                map_erase(x-1,y-1);
                map_erase(x-1,y-2);
                map_erase(x,y-2);
                map_erase(x+1.5,y);
                map_erase(x,y+1.5);
                map_erase(x,y+1.5);
                map_erase(x-1,y+1.5);

                return 1;
            } else {
                speech(babyLine1);
                Player.babyCount++;

                map_erase(x+1.5,y);
                map_erase(x+1.5,y-1);
                map_erase(x-2,y-1);
                map_erase(x-2,y);
                map_erase(x-1,y-1);
                map_erase(x-1,y-2);
                map_erase(x,y-2);
                map_erase(x+1.5,y);
                map_erase(x,y+1.5);
                map_erase(x,y+1.5);
                map_erase(x-1,y+1.5);

                return 1;
            }

        } else speech(babysFed);
    }



    ////for soda
    if (item1->type == SODA || item2->type == SODA || item3->type == SODA || item4->type == SODA \
            ||item5->type == SODA || item6->type == SODA || item7->type == SODA || item8->type == SODA \
            || item9->type == SODA) {
        if(sodaToggle  == 1)

        {
            speech(drankSoda);
            questToggle = 3;
            map_erase(x+1.5,y);
            map_erase(x+1.5,y-1);
            map_erase(x-2,y-1);
            map_erase(x-2,y);
            map_erase(x-1,y-1);
            map_erase(x-1,y-2);
            map_erase(x,y-2);
            map_erase(x+1.5,y);
            map_erase(x,y+1.5);
            map_erase(x,y+1.5);
            map_erase(x-1,y+1.5);
            return 1;

        } else {
            speech(cantDrink);
            return 1;
        }
    }
    return 0;
}


int go_right(int x, int y)
{
    MapItem *item = get_east(x+0.5, y); //get item to right
    MapItem *item2 = get_east(x+0.5,y-1);
    if ( (item->walkable && item2->walkable) || omni_pressed) { // check if walkable
        Player.x++;
        return 1;
    } else return 0;
}

int go_left(int x, int y)
{
    MapItem *item = get_west(x-1, y); //get item to left
    MapItem *item2 = get_west(x-1,y-1);
    if ( (item->walkable && item2->walkable) || omni_pressed)  { // check if walkable
        Player.x--;
        return 1;
    } else return 0;
}

int go_up(int x, int y)
{
    MapItem *item = get_north(x-1, y-1); //get item to north
    MapItem *item2 = get_north(x, y-1);
    MapItem *item3 = get_north(x-1, y);
    if ((item->walkable && item2->walkable && item3->walkable) || omni_pressed) { // check if walkable
        Player.y--;
        return 1;
    } else return 0;
}

int go_down(int x, int y)
{
    MapItem *item = get_south(x, y+0.5); //get item to south
    MapItem *item2 = get_south(x-1, y+0.5);
    if ((item->walkable && item2->walkable) || omni_pressed) { // check if walkable
        Player.y++;
        return 1;
    } else return 0;
}



/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */

void draw_game(int init)
{
    //game over
    if (init == 4) {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(2,4);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(LIGHTBLUE);
        uLCD.printf("THE END");
        uLCD.locate(1,5);
        uLCD.text_height(2);
        uLCD.text_width(1);
        uLCD.color(WHITE);
        uLCD.printf("ty 4 playin :)");
        wait(10000000000000);
    }

    // Draw game border first
    if(init)draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) { // Only draw the player on init
                draw_player(u, v, Player.has_key);
                continue;
            } else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                }
            } else if (init) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars, only update upper if position changes
    if ( (Player.x!=Player.px) || (Player.py != Player.y) || (Player.burgerCount != Player.prevBurgerCount)) {
        draw_upper_status(Player.x,Player.y,Player.burgerCount,burgerToggle);
    }
    //quest status on bottom
    if ((Player.prevBurgerCount!= Player.burgerCount) || (Player.prevBabyCount != Player.babyCount)) \
        draw_lower_status(Player.burgerCount, questToggle, Player.babyCount);
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */

void init_main_map()
{

    draw_start();


    uLCD.text_width(1);
    uLCD.text_height(1);
    Map* map = set_active_map(0);

// "Random" burgers
    for( int i = map_width() + 4; i < map_area(); i += 60) {
        add_burger(i % map_width(), i / map_width());
    }

    pc.printf("burgers\r\n");

//Add smiley NPC :)

    add_smiley(3,10);
    add_smiley(10,25);
    add_smiley(6,19);
    add_smiley(26,21);
    add_smiley(30,21);
    add_smiley(40,28);

    add_fryman(6,5);
    add_fryman(36,36);

    add_soda(35,30);
    add_soda(36,30);
    add_soda(34,30);

    add_baby1(12,2);
    add_baby2(12,30);
    add_baby1(6,17);
    add_baby2(5,27);
    add_baby1(10,37);
    add_baby1(15,30);
    add_baby2(30,40);
    add_baby1(30,3);
    add_baby2(25,50);
    add_baby1(19,12);


    pc.printf("added NPC!");

    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");


    //draw little room
    add_wall2(30,            40,             HORIZONTAL, 11);
    add_wall2(30,            30,             VERTICAL,   10);
    add_wall2(40,            30,             VERTICAL, 10);
    add_wall2(30,            30,             HORIZONTAL,  4);
    add_wall2(37,            30,             HORIZONTAL,  4);


    print_map();

}



/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // Initialize the maps
    draw_border();
    maps_init();
    init_main_map();
    draw_upper_status(5,5,0,0);
    draw_lower_status(0,5,1);
    uLCD.filled_circle(120,121,4,BLACK);
    uLCD.filled_circle(120,121,3,LETTUCE);



    // Initialize game state
    set_active_map(0);
    Player.x =2;
    Player.y = 3;
    Player.burgerCount = 0;
    Player.babyCount = 0;

    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // Actually do the game update:
        // 1. Read inputs
        in = read_inputs();

        //draw_img(29,20,test);


        // 2. Determine action (get_action)
        action = get_action(in);


        // 3. Update game (update_game)
        update = update_game(action);
        // 3b. Check for game over

        // 4. Draw frame (draw_game)
        draw_game(update);

        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();//change back to 100
        if (dt < 100) wait_ms(100 - dt);
    }
}


//draws start menu
void draw_start()
{
    if (startScreen == 1) {
        startScreen = 0;
        in = read_inputs();
        int i;
        int count = 0;
        int colorTog = 0;
        uLCD.textbackground_color(BLACK);
        char *startText = "KIRBY'S AMAZING TRIP To MCDONALD'S";
        char *top = "~~~~~~~~~~~~~~~~~~";
        char *bottomText = "By Kevin Wilson";



        while (in.b1) {
            in = read_inputs();
            //print out first line of start screen
            if (count == 0) {
                uLCD.locate(0,0);
                uLCD.color(WHITE);
                uLCD.printf(top);
                uLCD.locate(1,2);
                for (i = 0; i<8; ++i) {
                    if (i == 0) uLCD.text_bold(TEXTBOLD);
                    uLCD.text_italic(TEXTITALIC);

                    if (colorTog)uLCD.color(PINK2);
                    else uLCD.color(LIGHTBLUE);

                    uLCD.text_width(2);
                    uLCD.text_height(2);

                    uLCD.printf("%c", *startText);
                    wait_ms(175);
                    startText++;
                    pc.printf("Help i'm stuck");
                    colorTog = !colorTog;
                }
            }

            //print out second line of start screen
            if (count == 1) {
                uLCD.locate(1,3);
                for (i = 9; i<21; ++i) {
                    if (i == 9) uLCD.text_bold(TEXTBOLD);



                    uLCD.text_italic(TEXTITALIC);
                    if(i<17)  {
                        if(colorTog) uLCD.color(WHITE);
                        else uLCD.color(LIGHTGREEN);
                    } else {
                        if(colorTog)uLCD.color(DARKBLUE);
                        else uLCD.color(GRAY2);
                    }

                    uLCD.text_width(1);
                    uLCD.text_height(2);

                    uLCD.printf("%c", *startText);
                    wait_ms(150);
                    startText++;
                    colorTog = !colorTog;
                }
            }
            //print out "TO"
            if (count == 2) {
                uLCD.locate(7,4);
                for (i = 22; i<26; ++i) {

                    uLCD.text_bold(TEXTBOLD);
                    if(i<22)  uLCD.color(LETTUCE);
                    else uLCD.color(TOMATO);

                    uLCD.text_width(1);
                    uLCD.text_height(2);

                    uLCD.printf("%c", *startText);
                    wait_ms(200);
                    startText++;
                }
            }

            //print out "MCDONALDS"
            if (count == 3) {
                uLCD.locate(0,6);
                for (i = 26; i<37; ++i) {
                    if(i == 26) {
                        //uLCD.text_bold(TEXTBOLD);
                        uLCD.text_height(5);
                        uLCD.text_width(2);
                        uLCD.color(BUN);
                    } else if (i< 34) {
                        uLCD.set_font(FONT_7X8);
                        if (i!=33)uLCD.text_italic(TEXTITALIC);
                        uLCD.text_width(2);
                        if(colorTog) uLCD.text_height(2);
                        else uLCD.text_height(2);
                        uLCD.color(CHEESE);
                    } else {
                        uLCD.text_width(1);
                        if(colorTog) uLCD.text_height(2);
                        else uLCD.text_height(1);
                        uLCD.color(CHEESE);

                    }
                    uLCD.printf("%c", *startText);
                    wait_ms(100);
                    startText++;
                    colorTog = !colorTog;
                }

                //print name

                uLCD.text_height(1);
                uLCD.text_width(1);
                uLCD.locate(3,15);
                uLCD.color(WHITE);
                uLCD.printf(bottomText);

            }
            count++;
            in = read_inputs();
        }
    }
}
