#ifndef OBSERVER_H_
#define OBSERVER_H_


class IObserver
{
public:
    virtual ~IObserver() {}
    virtual void Notify() = 0;
};


class ISubject
{
public:
    virtual ~ISubject() {}

    virtual double GetFrameDuration() const = 0;
	virtual void AddObserver(IObserver * pObserver) = 0;
	virtual void RemoveObserver(IObserver * pObserver) = 0;

};


#endif
