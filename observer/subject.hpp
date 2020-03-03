#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <algorithm>
#include <vector>

#include "observer/isubject.hpp"
#include "observer/iobserver.hpp"


namespace pattern
{
namespace observer
{
namespace subject
{
template < class T >
class vector_adapter : public std::vector<T>
{
public:

    typename std::vector<T>::size_type erase( T key )
    {
        auto it = std::find( this->begin(), this->end(), key );
        if ( it != this->end() )
        {
            std::vector<T>::erase( it );
            return 1;
        }
        return 0;
    }

}; // class vector_adapter
} // namespace subject


template <
  template < class... > class ObserverContainerPolicy = subject::vector_adapter
>
class Subject : public ISubject
{
public:

    Subject() : observers() {}
    virtual ~Subject() {}

    virtual void attach( IObserver& observer ) override
    {
        observers.insert( observers.end(), &observer );
    }
    
    virtual void detach( IObserver& observer ) override
    {
        observers.erase( &observer );
    }

protected:

    virtual void notify( ISubject::IEvent::CreatorFun eventCreator = &ISubject::IEvent::create ) const override
    {
        auto event = eventCreator( *this );
        for ( auto& observer : observers )
            observer->update( event );
    }
    
private:

    ObserverContainerPolicy< IObserver* > observers;

}; // class Subject

} // namespace observer
} // namespace pattern

#endif //SUBJECT_HPP
