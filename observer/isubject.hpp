#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include <functional>
#include <memory>


namespace pattern
{
namespace observer
{

class IObserver;

class ISubject
{
public:

    class IEvent
    {
    public:
    
        typedef  std::unique_ptr<ISubject::IEvent>  pointer;
        typedef  std::function< IEvent::pointer ( const ISubject& ) >  CreatorFun;
        
        IEvent( const ISubject& subject ) : subject(subject) {}
        virtual ~IEvent() = default;
        
        static IEvent::pointer create( const ISubject& subject )
        {
            return std::make_unique<IEvent>( subject );
        }
        
        const ISubject& subject;
    }; // class IEvent

    virtual ~ISubject() = default;

    virtual void attach( IObserver& observer ) = 0;
    virtual void detach( IObserver& observer ) = 0;

protected:

    virtual void notify( IEvent::CreatorFun eventCreator = &ISubject::IEvent::create ) const = 0;

}; // class ISubject

} // namespace observer
} // namespace pattern

#endif //ISUBJECT_HPP
