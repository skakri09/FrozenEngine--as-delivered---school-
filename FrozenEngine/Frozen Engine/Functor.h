#ifndef Functor_h__
#define Functor_h__


class CFunctor
{
public:

	virtual void operator() = 0;

	virtual void Call() = 0;
};

template <class T> class CSpecificFunctor : public CFunctor
{
public:
	CSpecificFunctor(T* _pt2Object, void(T::*_ftp))
	{
		pt2Object = _pt2Object;
		ftp = _ftp;
	};

	virtual void operator()
	{
		(*pt2Object.*ftp);
	};

	virtual void Call()
	{
		(*pt2Object.*ftp)
	};

private:
	void(T::*_ftp);
	T* pt2Object;
};


#endif // Functor_h__
