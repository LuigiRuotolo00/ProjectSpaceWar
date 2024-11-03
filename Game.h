#include <iostream>
#include <vector>

#ifndef GAME
#define GAME

class Game
{
    public:
        virtual void display(int lives) = 0;
        virtual void init(char playerIcon, char enemyIcon) = 0;
        virtual ~Game() = default;
};

class NewGame : public Game
{
    private:
        std::vector<std::vector<char>> gameworld;
    
    public:

        NewGame()
        {
            for(auto rows = 0; rows < 20; rows++)
            {
                gameworld.push_back(std::vector<char>(60));
            }
        }

        void display(int lives) override
        {
            char life = '|';

            std::cout<<"LIVES ";

            for(auto i = 0; i < lives; i++)
            {
                std::cout<<life<<" ";
            }

            std::cout << std::endl;

            for(auto rows = 0; rows < 20; rows++)
	        {
		        std::cout<<"\t";
		        for(auto columns = 0; columns < 60; columns++)
		        {
                    std::cout << this->gameworld[rows][columns];
		        }
		        std::cout << std::endl;
	        }
        }

        std::vector<std::vector<char>> getWorld()
        {
            return this->gameworld;
        }

        void init(char playerIcon, char enemyIcon) override
        {
            for(auto rows = 0; rows < 20; rows++)
	        {
		
	            for(auto columns = 0; columns < 60; columns++)
	            {
			        this->gameworld[rows][columns] = ' ';
		            if((columns > 10 && columns < 50) && (rows > 0 && rows < 9))
		            {
		    	        if(columns % 4 == 0) this->gameworld[rows][columns] = enemyIcon;
                    }
                }
	            this->gameworld[19][3] = playerIcon;
            }
        }

        void setWorld(int x, int y, char icon)
        {
            this->gameworld[x][y] = icon;
        }
};

#endif