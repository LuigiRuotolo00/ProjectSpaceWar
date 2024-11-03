#include <vector>
#include <thread>

#include "Game.h"
#include "Observer.h"

#ifndef BULLET
#define BULLET

class Bullet
{
    public:
        virtual void shot(NewGame& game, int linearPos) = 0;
        virtual bool shot(NewGame& game, int linearPos, int& playerLives, int position) = 0;
        virtual ~Bullet() = default;
};

class NewBullet : public Bullet
{
    private:
        char icon;
        NewObserver obs;
    
    public:
        NewBullet()
        {
            NewObserver obs(100);
            this->icon = 193;
            this->obs = obs;
        }

        NewBullet(int time, char icon)
        {
            this->icon = icon;
            this->obs = NewObserver(time);
        }

        char getIcon()
        {
            return this->icon;
        }

        void callObserver(bool hit = false)
        {
            if(hit) system("color 04");
            std::thread waitThread(NewObserver::waitFor, this->obs);
            waitThread.join();
            if(hit) system("color 02");
        }

        void shot(NewGame& game, int linearPos) override
        {
            int verticalPos = 18;
            char enemyIcon = 184;

            bool foundEnemy = false;

            std::vector<std::vector<char>> world;
    
            while(verticalPos > 1 && !foundEnemy)
            {
                callObserver();
                game.setWorld(verticalPos, linearPos, ' ');
                verticalPos--;

                world = game.getWorld();

                if(world[verticalPos][linearPos] != enemyIcon)
                {
                    game.setWorld(verticalPos, linearPos, this->icon);
                }else
                {
                    game.setWorld(verticalPos, linearPos, ' ');
                    foundEnemy = true;
                }
                if(verticalPos == 1)
                {
                    game.setWorld(verticalPos, linearPos, ' ');
                }
                

            }
        }

        bool shot(NewGame& game, int linearPos, int& playerLives, int position) override
        {
            std::vector<std::vector<char>> world;

            bool hit = false;

            position++;

            game.setWorld(position, linearPos, this->icon);

            while(position < 18)
            {
                game.setWorld(position, linearPos, ' ');
                position++;
                game.setWorld(position, linearPos, this->icon);
                callObserver();
                
            }

            game.setWorld(position, linearPos, ' ');

            position++;
            world = game.getWorld();
            if(world[position][linearPos] != ' ')
            {
                hit = true;
                callObserver(hit);
            }

            return hit;
        }
};

#endif