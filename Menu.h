#include <iostream>
#include <vector>

#ifndef MENU
#define MENU

class Menu
{
    public:
        virtual void display() = 0;
        virtual void changeLine() = 0;
        virtual void setPosition(bool pos) = 0;
        virtual int getPosition() = 0;
        virtual ~Menu() = default;
};

class MainMenu : public Menu
{
    private:
        char icon;
        int position;
        std::vector<char> menuIcon;

    public:
        MainMenu() : icon(254)
        {
            //this->icon = icon;
            this->position = 0;
            this->menuIcon.push_back(this->icon);
            this->menuIcon.push_back(' ');
            this->menuIcon.push_back(' ');
        }

        MainMenu(char icon)
        {
            this->icon = icon;
            this->position = 0;
            this->menuIcon.push_back(icon);
            this->menuIcon.push_back(' ');
            this->menuIcon.push_back(' ');
        }

        void setPosition(bool pos) override
        {
            if(pos) this->position--;
            else this->position++;
        }

        int getPosition() override
        {
            return this->position;
        }

        void display() override
        {
            std::cout<<"\t\t\t\tMAIN MENU'"<<std::endl;
            std::cout<<"\t\t\t"<< this->menuIcon[0] <<" NEW GAME"<<std::endl;
            std::cout<<"\t\t\t"<< this->menuIcon[1] <<" COMMANDS"<<std::endl;
            std::cout<<"\t\t\t"<< this->menuIcon[2] <<" EXIT"<<std::endl;
            std::cout<<"NAVIGATE THE MENU':"<<std::endl;
            std::cout<<"W --> GO UP"<<std::endl;
            std::cout<<"S --> GO DOWN"<<std::endl;
            std::cout<<"ENTER --> SELECT VOICE MENU"<<std::endl;
        }

        void changeLine() override
        {
            for(auto i = 0; i < this->menuIcon.size(); i++)
            {
                this->menuIcon[i] = ' ';
            }

            this->menuIcon[this->position] = this->icon;
        }
};

class ReturnMenu : public Menu
{
    private:
        char icon;
        bool win;
        int position;
        std::vector<char> menuIcon;

    public:
        ReturnMenu() : icon(254)
        {
            this->win = false;
            this->position = 0;
            this->menuIcon.push_back(this->icon);
            this->menuIcon.push_back(' ');
        }

        void set()
        {
            this->win = true;
        }

        void setPosition(bool pos) override
        {
            if(pos) this->position++;
            else this->position--;
        }

        int getPosition() override
        {
            return this->position;
        }

        void display() override
        {
            if(this->win) std::cout<<"\t\t\t\tWINNER"<<std::endl;
            else std::cout<<"\t\t\t\tLOSER"<<std::endl;
            std::cout<<"\t\t\t"<< this->menuIcon[0] <<" TRY AGAIN"<<std::endl;
            std::cout<<"\t\t\t"<< this->menuIcon[1] <<" BACK TO MENU'"<<std::endl;
        }

        void changeLine() override
        {
            for(auto i = 0; i < this->menuIcon.size(); i++)
            {
                this->menuIcon[i] = ' ';
            }

            this->menuIcon[this->position] = this->icon;
        }
};

class CommandMenu : public Menu
{
    public:
        void display() override
        {
            std::cout<<"\t\t\tMOVE LEFT: A"<<std::endl;
            std::cout<<"\t\t\tMOVE RIGHT: D"<<std::endl;
            std::cout<<"\t\t\tSHOT: SPACE"<<std::endl;
            std::cout<<"\t\t\tPRESS ANY KEY TO CONTINUE"<<std::endl;
        }

        void changeLine()
        {
            return;
        }
        void setPosition(bool pos)
        {
            return;
        }

        int getPosition()
        {
            return 0;
        }
};

#endif
