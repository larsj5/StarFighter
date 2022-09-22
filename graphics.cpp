#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include <time.h>
#include "Button.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color black(0, 0, 0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color yellow(.8, .8, 0);
const color cyan (0, 1, 1);
const color gray(.5, .5 , .5);

vector<Rect> enemies1;
vector<Rect> ammoBoxes;
vector<Rect> bulletsShot;
vector<Rect> enemies2;
vector<Rect> stars;
Rect user = Rect(white);
int ammo = 10;
int enemiesKilled = 0;

Button startButton({1, 0, 0}, {250, 400}, 100, 50, "Start");
//Button easyButton({0, 1, 0}, {250, 200}, 100, 50, "Easy");

enum Screen{START, RUN, END, WIN, RESTART};
//enum Difficulty
Screen gameState;

void initEnemies() {
    //create regular 10 enemies
    for (int i = 0; i < 15; ++i){
        Rect enemy = Rect(brickRed, rand() % 50 + 400, rand() % 490 + 5);
        enemy.setSize(15, 15);
        enemies1.push_back(enemy);

    }
}
void initEnemies2(){
    //create 3 fast enemies
    for (int i = 0; i < 5; ++i){
        Rect enemyFast = Rect(orange, rand() % 50 + 400, rand() % 490 + 5);
        enemyFast.setSize(15, 15);
        enemies2.push_back(enemyFast);
    }
}

void initAmmoBoxes(){
    //create 3 ammo boxes
    for (int i = 0; i < 3; ++i){
        Rect ammoBlock = Rect(grassGreen, rand() % 200 + 200, rand() % 475 + 10);
        ammoBlock.setSize(10, 10);
        ammoBoxes.push_back(ammoBlock);
    }
}

void initUser() {
    // Initialize the user to be a 20x20 white block
    // in the middle of the left side of screen
    user.setSize(20, 20);
    user.setCenter(10, 300);
}

void initStars(){
    //create a background of stars for the game, randomly sized from 1x1 to 3x3
    for (int i = 0; i < 150; ++i){
        int dimens = rand() % 3 + 1;
        Rect star = Rect(white, rand() % 500, rand() % 500);
        star.setSize(dimens, dimens);
        stars.push_back(star);
    }
}

void init() {
    width = 500;
    height = 500;
    srand(time(0));
    initEnemies();
    initEnemies2();
    initAmmoBoxes();
    initStars();
    initUser();
    gameState = START;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0, 0, 0, 1.0f);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)
    
    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE
    
    /*
     * Draw here
     */
    //print the starting message while game state is in the Start state
    if (gameState == START){
        //from: https://stackoverflow.com/questions/2183270/what-is-the-easiest-way-to-print-text-to-screen-in-opengl
        string message = "Welcome to StarFighter!";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(150, 100);
        int len, i;
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }
        //write rules of game
        message = "You are the white block. Your goal is to kill all the enemy ships";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(80, 130);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "Red enemies move predictably. However, don't let them get past you,";
        glColor3f(brickRed.red, brickRed.green,brickRed.blue);
        glRasterPos2f(65, 160);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "they'll come back with reinforcements";
        glColor3f(brickRed.red, brickRed.green,brickRed.blue);
        glRasterPos2f(140, 175);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "Orange enemies are less predictable. They are speedy and evasive,";
        glColor3f(orange.red, orange.green,orange.blue);
        glRasterPos2f(65, 205);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "but the enemy has only a few of them.";
        glColor3f(orange.red, orange.green,orange.blue);
        glRasterPos2f(140, 220);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "Pick up the green boxes to reload your ship's ammo.";
        glColor3f(grassGreen.red, grassGreen.green,grassGreen.blue);
        glRasterPos2f(100, 250);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "Don't run into an enemies ship, if you do, you die.";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(110, 280);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "Use arrow keys to move and space bar to shoot.";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(115, 310);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        message = "Press the button below when you're ready to begin the fight.";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(90, 340);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
        }

        startButton.draw();
    }

    //if button is clicked and game state is set to run, start drawing
    if (gameState == RUN){

        //draw stars first
        for (Rect &s : stars){
            s.draw();
        }

        //draw ammo remaining in top left corner
        //from: https://stackoverflow.com/questions/2183270/what-is-the-easiest-way-to-print-text-to-screen-in-opengl
        string message = "Ammo Left: " + to_string(ammo);
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(10, 20);
        int len, i;
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }


        //draw enemies
        for (Rect &e : enemies1){
            e.draw();
        }

        for (Rect &e : enemies2){
            e.draw();
        }

        //draw ammo boxes
        for (int a = 0; a < ammoBoxes.size(); ++a){
            //add 5 ammo if user overlaps with an ammo box
            if (user.isOverlapping(ammoBoxes[a])){
                ammoBoxes.erase(ammoBoxes.begin() + a);
                ammo += 5;
                Rect ammoBlock = Rect(grassGreen, rand() % 50 + 600, rand() % 500);
                ammoBlock.setSize(10, 10);
                ammoBoxes.push_back(ammoBlock);

            }
            else{
                ammoBoxes[a].draw();
            }
        }

        //draw bullets
        for (int b = 0; b < bulletsShot.size(); ++b ){
            for (int e = 0; e < enemies1.size(); ++e) {
                //delete bullet and enemy if they overlap
                if (enemies1[e].isOverlapping(bulletsShot[b])){
                    bulletsShot.erase(bulletsShot.begin() + b);
                    enemies1.erase(enemies1.begin() + e);
                    enemiesKilled += 1;
                    break;
                }
                else{
                    bulletsShot[b].draw();
                }
            }
            for (int e = 0; e < enemies2.size(); ++e) {
                //delete bullet and enemy if they overlap
                if (enemies2[e].isOverlapping(bulletsShot[b])){
                    bulletsShot.erase(bulletsShot.begin() + b);
                    enemies2.erase(enemies2.begin() + e);
                    enemiesKilled += 1;
                    break;
                }
                else{
                    bulletsShot[b].draw();
                }
            }
        }


        //draw user
        user.draw();

        //check if user overlapping with enemies, if so, end game
         for (Rect &e : enemies1){
            if (user.isOverlapping(e)){
                gameState = END;
            }
        }
        for (Rect &e : enemies2){
            if (user.isOverlapping(e)){
                gameState = END;
            }
        }
        if (enemies1.empty() && enemies2.empty()){
            gameState = WIN;
        }
    }

    //if game state now == to end, print ending message
    if (gameState == END){
        //from: https://stackoverflow.com/questions/2183270/what-is-the-easiest-way-to-print-text-to-screen-in-opengl
        string message = "Game Over. Press R to Restart or Esc to exit.";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(70, 200);
        int len, i;
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }

        //print out how many enemies they killed
        message = "Enemies Killed: " + to_string(enemiesKilled);
        glColor3f(1.0, 0,0);
        glRasterPos2f(175, 280);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }
    }

    if (gameState == WIN){
        //from: https://stackoverflow.com/questions/2183270/what-is-the-easiest-way-to-print-text-to-screen-in-opengl
        string message = "Congratulations, you Win!";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(140, 200);
        int len, i;
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }

        message = "Enemies Killed: " + to_string(enemiesKilled);
        glColor3f(1.0, 0,0);
        glRasterPos2f(175, 240);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }

        message = "Press R to Restart or Esc to exit.";
        glColor3f(1.0, 1.0,1.0);
        glRasterPos2f(115, 400);
        len = message.length();
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }
    }
    if (gameState == RESTART){
        enemies1.clear();
        ammoBoxes.clear();
        bulletsShot.clear();
        enemies2.clear();
        stars.clear();
        ammo = 10;
        enemiesKilled = 0;
        initEnemies();
        initEnemies2();
        initAmmoBoxes();
        initStars();
        initUser();
        gameState = RUN;
    }

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    //if space bar pressed and game is running, add a bullet shot
    if (key == 32 && gameState == RUN){
        if (ammo > 0){
            Rect bullet = Rect(gray);
            bullet.setSize(10, 5);
            bullet.setCenter(user.getCenterX(), user.getCenterY());
            bulletsShot.push_back(bullet);
            ammo -= 1;
        }
    }
    if ((gameState == END || gameState == WIN) && (key == 'r' || key == 'R')){
        gameState = RESTART;
    }
    
    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    //handles movement of the ship
    if (gameState == RUN){
        switch(key) {
            case GLUT_KEY_DOWN:
                if (user.getCenterY() < 490 ){
                    user.moveY(7);
                }
                break;
            case GLUT_KEY_LEFT:
                if (user.getCenterX() > 10){
                    user.moveX(-7);
                }
                break;
            case GLUT_KEY_RIGHT:
                if (user.getCenterX() < 490){
                    user.moveX(7);
                }
                break;
            case GLUT_KEY_UP:
                if (user.getCenterY() > 10){
                    user.moveY(-7);
                }
                break;
        }
    }
    
    glutPostRedisplay();
}

void cursor(int x, int y) {
    //checks to see if the cursor is overlapping with the start button, if so,
    //call hover method.
    if (gameState == START){
        if (startButton.isOverlapping(x, y)){
            startButton.hover();
        }
        else{
            startButton.release();
        }
    }
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    if (gameState == START){
        // If the left button is down and the cursor is overlapping with the Button, call the pressDown method. Otherwise, call the release method.
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && startButton.isOverlapping(x, y)){
            startButton.pressDown();
        }
        else{
            startButton.release();
        }
        // If the left button is up and the cursor is overlapping with the Button, set game state to run.
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && startButton.isOverlapping(x, y)){
            gameState = RUN;
        }
    }
    glutPostRedisplay();
}

void enemyTimer(int dummy) {
    if (gameState == RUN){
        for (Rect &r : enemies1) {
            // Move all the enemies to the left
            r.moveX(-3);
            // If an enemy has moved off the screen
            if (r.getCenterX() < -10) {
                // Set it to the right of the screen so that it passes through again
                r.setCenterX(520);
                //also add another enemy
                Rect enemy = Rect(brickRed, rand() % 50 + 500, rand() % 490 + 5);
                enemy.setSize(15, 15);
                enemies1.push_back(enemy);
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(50, enemyTimer, dummy);
}

void enemyFastTimer(int dummy) {
    if (gameState == RUN){
        for (Rect &c : enemies2) {
            // Move all the enemies to the left at a faster rate
            c.moveX(-6);
            //randomly generate 0 or 1 to dictate if the enemy will move up or down
            int upDown = rand() % 2;
            //if value is 0, move enemy down a random amount from 0 to 4
            if (upDown == 0){
                c.moveY(0 - rand() % 6);
            }
            //if value is 1, move enemy up a random amount from 0 to 4
            else{
                c.moveY(rand() % 6);
            }
            // If a shape has moved off the screen, potentially up or down too for these ones
            if (c.getCenterX() < -20 || c.getCenterY() > 500 || c.getCenterY() < 0) {
                // Set it to the right of the screen so that it passes through again
                c.setCenterX(520);
                c.setCenterY(rand() % 600);
            }
        }

    }

    glutPostRedisplay();
    glutTimerFunc(50, enemyFastTimer, dummy);
}

void ammoTimer(int dummy) {
    if (gameState == RUN){
        for (Rect &r : ammoBoxes) {
            // Move all the boxes to the left
            r.moveX(-5);
            // If a shape has moved off the screen
            if (r.getCenterX() < -20) {
                // Set it to the right of the screen so that it passes through again
                r.setCenterX(520);
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, ammoTimer, dummy);
}

void bulletTimer(int dummy) {
    if (gameState == RUN){
        for (int i = 0; i < bulletsShot.size(); ++i) {
            // Move bullets quickly to the right
            bulletsShot[i].moveX(10);
            // If bullet has moved off the side of the screen
            if (bulletsShot[i].getCenterX() > 500) {
                // Delete it
                bulletsShot.erase(bulletsShot.begin() + i);
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, bulletTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("StarFighter" /* title */ );
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    
    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);
    
    // handles mouse movement
    glutPassiveMotionFunc(cursor);
    
    // handles mouse click
    glutMouseFunc(mouse);
    
    // handles timer
    glutTimerFunc(0, enemyTimer, 0);
    glutTimerFunc(0, ammoTimer, 0);
    glutTimerFunc(0, bulletTimer, 0);
    glutTimerFunc(0, enemyFastTimer, 0);
    
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
