#include <vector>
#include <mutex>

#include "Game.h"
#include "Bullet.h"
#include "Observer.h"

#ifndef ENEMY
#define ENEMY

class Enemy
{
    public:
        virtual void move(NewGame& game, std::mutex& mtx, bool direction, bool& gameEnemyChange) = 0;
        virtual int checkPosition(NewGame game, int position) = 0;
        virtual void shot(NewGame& game, int linearPos, bool& gameChangeShotEn, int playerLives, int position) = 0;
        virtual ~Enemy() = default;
};

class NewEnemy : public Enemy
{
    private:
        char icon;
        int count;
        int columnGap;
        bool lockMov;
        bool lockShot;
        bool playerHit;
        NewBullet bullet;

    public:
        

        NewEnemy()
        {
            this->icon = 184;
            this->lockMov = true;
            this->columnGap = 37;
            this->lockShot = true;
            this->bullet = NewBullet(80, 197);
        }

        char getIcon()
        {
            return this->icon;
        }

        bool getLockMov()
        {
            return this->lockMov;
        }

        bool getLockShot()
        {
            return this->lockShot;
        }

        template <typename T>
        void callObserver(T obs)
        {
            if constexpr (std::is_same_v<T, NewObserver>)
            {
                std::thread waitThread(NewObserver::waitFor, obs);
                waitThread.join();
            }else if constexpr (std::is_same_v<T, EnemyObserver>)
            {
                std::thread waitThread(EnemyObserver::waitFor, obs);
                waitThread.join();   
            }
        }

        void counter(NewGame game)
        {
            std::vector<std::vector<char>> world;

            int firstColumn = -1;
            int lastColumn = 0;

            int row;
            int column;

            int countCol;

            int count = 0;
            world = game.getWorld();

            for(column = 3; column < 57; column++)
            {
                countCol = count;
                for(row = 1; row < 9; row++)
                {
                    if(world[row][column] == this->icon)
                    {
                        count++;
                    }
                }
                if(countCol != count)
                {
                    if(firstColumn == -1) firstColumn = column;
                    lastColumn = column;
                }
            }

            this->columnGap = lastColumn - firstColumn + 1;
            this->count = count;
        }

        int getCount()
        {
            return this->count;
        }

        bool getHit()
        {
            return this->playerHit;
        }

        void unsetHit()
        {
            this->playerHit = false;
        }

        void move(NewGame& game, std::mutex& mtx, bool direction, bool& gameEnemyChange) override
        {
            int column = 0;
            int row = 0;
            int countCol = 0;

            std::vector<std::vector<char>> world;

            int subColumns = 0;

            bool foundEnemy = false;

            char enemyIcon = 184;

            gameEnemyChange = true;

            this->lockMov = false;

            world = game.getWorld();
    
            column = 38;

            std::unique_lock<std::mutex> lock(mtx);

            if(direction)
            {
                column = 0;
                while(column < 60)
                {
                    countCol++;
                    if((countCol + this->columnGap) > 55)
                    {
                        column = 60;
                        row = 17;
                    }
                    row = 0;
                    while(row < 17)
                    {
                        world = game.getWorld();
                        if(world[row][column] == enemyIcon)
                        {
                            game.setWorld(row, column, ' ');
                            game.setWorld(row, column + 1, this->icon);
                            
                            countCol = 0;
                            foundEnemy = true;
                        }
                        row++;
            
                    }
                    if(foundEnemy) column++;
                    foundEnemy = false;
                    column++;
                }
            }else
            {
                column = 59;
                while(column >= 0)
                {
                    countCol++;
                    row = 0;
                    if((countCol + this->columnGap) > 55)
                    {
                        column = -1;
                        row = 17;
                    }
                    while(row < 17)
                    {
                        world = game.getWorld();
                        if(world[row][column] == enemyIcon)
                        {
                            game.setWorld(row, column, ' ');
                            game.setWorld(row, column - 1, this->icon);

                            countCol = 0;
                            foundEnemy = true;
                        }
                        row++;
                    }
                    if(foundEnemy) column--;
                    foundEnemy = false;
                    column--;
                }
            }
            gameEnemyChange = false;
            
            callObserver(NewObserver(200));
    
            this->lockMov = true;
        }

        int checkPosition(NewGame game, int position)
        {
            int i = 7;

            std::vector<std::vector<char>> world;

            world = game.getWorld();

            if(world[8][position] == this->icon)
            {
                return 8;
            }else
            {
                while(i > 0)
                {
                    if(world[i][position] == this->icon)
                    {
                        return i;
                    }
                    i--;
                }
            }
            return -1;
        }

        void shot(NewGame& game, int linearPos, bool& gameChangeShotEn, int playerLives, int position) override
        {
            this->lockShot = false;
            
            gameChangeShotEn = true;

            this->playerHit = this->bullet.shot(game, linearPos, playerLives, position);
            gameChangeShotEn = false;
            this->lockShot = true;
        }
};

#endif