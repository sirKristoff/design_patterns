/**
 * @file observer_test.cpp
 * @brief Test for observer pattern
 *
 * @author Krzysztof Lasota
 *
 *
 * Imagine a game where one or more rats can attack a player.
 * Each individual rat has an attack value of 1. However, rats attack as a swarm,
 * so each rat's attack value is equal to the total number of rats in play.
 * Given that a rat enters play through the constructor and leaves play (dies) via its destructor,
 * please implement the Game and Rat classes so that, at any point in the game,
 * the attack value of a rat is always consistent.
 */

#include <iostream>
#include <unordered_set>

#include "observer/iobserver.hpp"
#include "observer/subject.hpp"


using namespace pattern::observer;

typedef Subject<> GameSubject;  // use vector as observers container
//typedef Subject<std::unordered_set> GameSubject;  // use std::unordered_set as observers container


class Game : public GameSubject
{
public:

    struct EventRatAttackChanged : public IEvent
    {
        EventRatAttackChanged( const ISubject& subject, int ratAttack )
         : IEvent(subject), ratAttack(ratAttack)
        {}
        
        int ratAttack;
    }; // struct EventRatAttackChanged

    void attach( IObserver& observer ) override
    {
        GameSubject::attach( observer );
        incrementRatAttack();
    }
    
    void detach( IObserver& observer ) override
    {
        GameSubject::detach( observer );
        decrementRatAttack();
    }
    
    void incrementRatAttack()
    {
        ++ratAttack;
        auto eventCreator = 
            [ this ]( const ISubject& subject )
            {  return IEvent::pointer( new EventRatAttackChanged( subject, ratAttack ) );  };
        notify( eventCreator );
    }
    
    void decrementRatAttack()
    {
        --ratAttack;
        auto eventCreator = 
            [ this ]( const ISubject& subject )
            {  return IEvent::pointer( new EventRatAttackChanged( subject, ratAttack ) );  };
        notify( eventCreator );
    }
    
private:

    int ratAttack{0};
};


class Rat : public IObserver
{
public:

    Rat( Game& game ) : game(game)
    {
        game.attach( *this );
        std::cout << "Rat< " << std::hash<const Rat*>{}(this) << ", attack: " << attack << " >: Created\n";
    }

    Rat( const Rat& src ) : game(src.game)
    {
        game.attach( *this );
        std::cout << "Rat< " << std::hash<const Rat*>{}(this) << ", attack: " << attack << " >: Created\n";
    }

    ~Rat() 
    { 
        game.detach( *this );
        std::cout << "Rat< " << std::hash<const Rat*>{}(this) << ", attack: " << attack << " >: Destroyed\n";
    }

    int doAttack() const
    {
        std::cout << "Rat< " << std::hash<const Rat*>{}(this) << ", attack: " << attack << " >: Do attack\n";
		return attack;
    }

private:

    void update( const ISubject::IEvent::pointer& event ) override
    {
        auto eventRatAttackChanged = dynamic_cast<Game::EventRatAttackChanged*>( &*event );
        if ( eventRatAttackChanged )
            attack = eventRatAttackChanged->ratAttack;
    }


    Game& game;
    int attack{1};

}; // class Rat



#include <gtest/gtest.h>
#include <gmock/gmock.h>


/**
 * @brief Unit Test for ...
 */
TEST(Observer_Test, T01_Simple)
{
    Game game;
    
    Rat r1{ game };
    EXPECT_EQ( 1, r1.doAttack() );
    //Rat r();
    {
    Rat r2[2] = { game, game };
    
    EXPECT_EQ( 3, r1.doAttack() );
    EXPECT_EQ( 3, r2[0].doAttack() );
    EXPECT_EQ( 3, r2[1].doAttack() );
    }
    EXPECT_EQ( 1, r1.doAttack() );
    
    std::unique_ptr<IObserver> obs( new Rat( game ) );
    EXPECT_EQ( 2, r1.doAttack() );
    
    obs = nullptr;
    EXPECT_EQ( 1, r1.doAttack() );
}