#include <thread>

#ifndef OBSERVER
#define OBSERVER

/*the observer is called to stop and wait a thread for a given time (in milliseconds)*/

class Observer
{
    public:
        virtual void waitFor() = 0;
        virtual ~Observer() = default;
};

class NewObserver : public Observer
{
    private:
        int millisecs;

    public:
        NewObserver()
        {
            this->millisecs = 0;
        }

        NewObserver(int millisecs)
        {
            this->millisecs = millisecs;
        }

        void waitFor() override
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(this->millisecs));
        }
};

class EnemyObserver : public Observer
{
    private:
        int millisecs;
    
    public:
        EnemyObserver()
        {
            this->millisecs = 0;
        }

        EnemyObserver(int millisecs)
        {
            this->millisecs = millisecs;
        }

        void waitFor() override
        {
            system("color 04");
            std::this_thread::sleep_for(std::chrono::milliseconds(this->millisecs));
            system("color 02");
        }
};

#endif