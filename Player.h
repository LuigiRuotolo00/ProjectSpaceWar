#include <thread>
#include <vector>

#include "Bullet.h"
#include "Game.h"

#ifndef PLAYER
#define PLAYER

class Player
{
    public:
        virtual void shot(NewGame& game, bool& gameChangeShot) = 0;
        virtual void move(bool direction) = 0;
        virtual ~Player() = default;
};

class NewPlayer : public Player
{
    private:
        int lives;
        int column;

        char icon;
        char bulletIcon;

        bool lockShot;
        NewBullet bullet;

    public:
        NewPlayer()
        {
            this->icon = 143;
            this->lives = 3;
            this->column = 3;
            this->lockShot = true;
        }

        int getLives()
        {
            return this->lives;
        }

        char getIcon()
        {
            return this->icon;
        }

        int getColumn()
        {
            return this->column;
        }

        bool getLockShot()
        {
            return this->lockShot;
        }

        void setLockShot(bool lock)
        {
            this->lockShot = lock;
        }

        NewBullet getBullet()
        {
            return this->bullet;
        }

        void shot(NewGame& game, bool& gameChangeShot) override
        {
            this->lockShot = false;
            gameChangeShot = true;
            this->bullet.shot(game, this->column);
            
            gameChangeShot = false;
            this->lockShot = true;

        }

        void reset()
        {
            this->lives = 3;
            this->column = 3;
        }

        void subLives()
        {
            this->lives--;
        }

        void move(bool direction) override
        {
            if(direction)
            {
                if(this->column > 3) this->column--;
            }else
            {
                if(this->column < 56) this->column++;
            }
        }
};

#endif