#include <iostream>
#include <conio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <windows.h>

#include "Menu.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

using namespace std;

void hideCursor() {
    // get console handle
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // structure to manage cursor information
    CONSOLE_CURSOR_INFO cursorInfo;

    // get current cursor information
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);

    // set to false cursor visibility (hide)
    cursorInfo.bVisible = FALSE;

    // apply new setting on cursor
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

int main()
{
    bool tSession = true; //session loop
    bool tMenu = true; //main menu loop
    bool tGame; //game loop
    bool tGameover = false; //gameover menu loop
    bool tcommand; //command menu loop

    bool displayMenu = false; //true if user press any key in main menu
    bool displayRetMenu = false; //true if user press any key in gameover menu
    bool playerHasMoved = false; //true if user press any key during game
    bool playerBulletChanged = false; //true while player bullet is active
    bool enemyHasMoved = false; //true while enemies move
    bool enemyBulletChanged = false; //true while enemy bullet is active

    char getChar;

    int position;
    int hToShot;

    int keyPressed;

    int playerHeight = 19; //row position of player icon

    MainMenu mainmenu;
    ReturnMenu retmenu;
    CommandMenu commenu;
    NewGame game;
    NewPlayer player;
    NewEnemy enemy;
    NewBullet bullet;

    mutex worldMtx;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution distrib(1,2); //distribution used for casual movement of enemies
    uniform_int_distribution distEne(-2,2); //distribution used for casual shot by enemies

    int leftOrRight;
    bool leftRight;

    hideCursor();

    system("color 02");

    while(tSession) //loop session
    {
        system("cls");
        mainmenu.display();

        while(tMenu) //loop main menu
        {
            displayMenu = false;
            keyPressed = _kbhit();

            if(keyPressed) //check if any key is pressed and change the icon in menu
            {
                displayMenu = true;

                getChar = _getch();

                switch(getChar)
                {
                    case 'w': //go up in menu
                        if(mainmenu.getPosition() > 0) mainmenu.setPosition(true);
                        break;
                    case 's': //go down in menu
                        if(mainmenu.getPosition() < 2) mainmenu.setPosition(false);
                        break;
                    case 13: //press enter
                        if(mainmenu.getPosition() == 1)
                        {
                            system("cls");
                            commenu.display();
                            tcommand = true;
                            while(tcommand)
                            {
                                keyPressed = _kbhit();

                                if(keyPressed) tcommand = false;
                            }
                        }else tMenu = false;
                        break;
                    default:
                        break;
                }
            }

            if(displayMenu) //if any key has been pressed update and display menu
            {
                system("cls");
                mainmenu.changeLine();
                mainmenu.display();
            }
        }

        if(mainmenu.getPosition() == 0) //init and start new game
        {
            game.init(player.getIcon(), enemy.getIcon());
            enemy.counter(game);
            tGame = true;
            player.reset();

            system("cls");
            game.display(player.getLives());
        }else tSession = false; //exit the session and close the game


        while(tGame) //game loop
        {
            enemy.counter(game);
            if(player.getLives() == 0 || enemy.getCount() == 0)
            {
                tGame = false;
            }

            playerHasMoved = false;
            keyPressed = _kbhit();

            if(keyPressed)
            {
                playerHasMoved = true;

                getChar = _getch();

                switch(getChar) //check which key is pressed
                {
                    case 'a': //move on left
                        game.setWorld(playerHeight, player.getColumn(), ' ');
                        player.move(true);
                        game.setWorld(playerHeight, player.getColumn(), player.getIcon());
                        break;
                    case 'd': //move on right
                        game.setWorld(playerHeight, player.getColumn(), ' ');
                        player.move(false);
                        game.setWorld(playerHeight, player.getColumn(), player.getIcon());
                        break;
                    case 32: //shot player
                        if(player.getLockShot())
                        {
                            game.setWorld(playerHeight - 1, player.getColumn(), player.getBullet().getIcon());

                            thread shotBullet(&NewPlayer::shot, &player, ref(game), ref(playerBulletChanged));
                            shotBullet.detach();
                        }
                        
                        break;
                    default:
                        break;
                }
            }

            if(enemy.getLockMov()) //enemy movement
            {
                leftOrRight = distrib(gen);
                if(leftOrRight == 1)
                {
                    leftRight = true;
                }else leftRight = false;
                thread moveEnemy(&NewEnemy::move, &enemy, ref(game), ref(worldMtx), leftRight, ref(enemyHasMoved));
                moveEnemy.detach();
            }

            if(enemy.getLockShot()) //check if enemy shot
            {
                position = distEne(gen);
                position += player.getColumn();
                hToShot = enemy.checkPosition(game, position);
                if(hToShot != -1)
                {
                    thread bulletEnemyThread(&NewEnemy::shot, &enemy, ref(game), position, ref(enemyBulletChanged), player.getLives(), hToShot);
                    bulletEnemyThread.detach();
                }
                if(enemy.getHit())
                {
                    player.subLives();
                    enemy.unsetHit();
                }
            }

            if(playerHasMoved || playerBulletChanged || enemyHasMoved || enemyBulletChanged) //if any of this condition is verified, display game
            {
                system("cls");
                game.display(player.getLives());
            }

        }

        if(player.getLives() == 0 || enemy.getCount() == 0) tGameover = true;
        retmenu.display();

        while(tGameover && tSession)
        {
            displayRetMenu = false;
            keyPressed = _kbhit();

            if(keyPressed)
            {
                displayRetMenu = true;

                getChar = _getch();

                switch(getChar)
                {
                    case 'w':
                        if(retmenu.getPosition() > 0) retmenu.setPosition(false);
                        break;
                    case 's':
                        if(retmenu.getPosition() < 1) retmenu.setPosition(true);
                        break;
                    case 13:
                        tGameover = false;
                        break;
                    default:
                        break;
                }
            }
            if(displayRetMenu)
            {
                system("cls");
                game.display(player.getLives());
                if(enemy.getCount() == 0) retmenu.set();
                retmenu.changeLine();
                retmenu.display();
            }
        }

        if(retmenu.getPosition() == 0)
        {
            tMenu = false;
            tGame = true;
        }else
        {
            tMenu = true;
            tGame = true;
        }

    }
}