/******************/
/*** List Class ***/
/******************/

#pragma warning( disable : 4786 )

#if !defined	__LIST_CLASS_HEADER__
#define			__LIST_CLASS_HEADER__

#include	<Vector>

/*****************************************************************************/

template <class T> class CList
{
public:

	int		Find(T &Item)
	{
		int		ListSize=List.size();

		for (int i=0; i<ListSize; i++)
		{
			if (List[i]==Item) return(i);
		}	
		return(-1);
	}

	int		Add(T &Item)
	{
		int		ListSize=List.size();
		int		Idx=Find(Item);	// Check if exists

		if (Idx!=-1) return(Idx);
		List.push_back(Item);
		return(ListSize);
	}

	int		push_back(T &Item)
	{
		int		ListSize=List.size();
		List.push_back(Item);
		return(ListSize);
	}

	void	clear()					{List.clear();}
	void	resize(int i)			{List.resize(i);}
	void	erase(int i)			{List.erase(List.begin()+i);}
	void	erase(int First,int Last)	{List.erase(List.begin()+First,List.begin()+Last);}
	void	insert(int i,T &Item)	{List.insert(List.begin()+i,Item);}
//	int		begin()					{return(List.begin);}


	int		size()					{return(List.size());}

	std::vector<T>	&GetList()		{return(List);}
	T& operator []( int nIndex )	{return(List[nIndex]);}

private:
	std::vector<T>		List;

};

/*****************************************************************************/
#endif
