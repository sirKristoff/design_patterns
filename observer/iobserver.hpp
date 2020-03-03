#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

#include <memory>

#include "observer/isubject.hpp"


namespace pattern
{
namespace observer
{

class IObserver
{
public:

    virtual ~IObserver() = default;
    virtual void update( const ISubject::IEvent::pointer& ) = 0;

};

} // namespace observer
} // namespace pattern

#endif //IOBSERVER_HPP