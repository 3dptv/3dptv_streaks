#include "stdafx.h"
#include "parameters.h"
#include "matrix_op.h"
#include "path.h"
#include "spline.h"
#include "pathlist.h"


//this routine gives the pointer to the element 
//with the number i of PathList
PathListElem *PathList::operator [](int i)
{
	if(i>=0 && i<number)
	{
		PathListElem *p = first_path;
		while(i-- >0)
			p=p->next;

		return p;
	}
	else 
		return first_path;
}

//this routine adds the path at the end of the list
void PathList::add(Path *path)
{
	//PathListElem * new_path = new PathListElem(*path);
	PathListElem *new_path=new PathListElem;

	new_path->element=path;
	new_path->next=0;
	if(number==0)
	{
		first_path=new_path;
		last_path=new_path;
	}
	else
	{
		last_path->next=new_path;
		last_path=new_path;
	}
	++number;
}


Path* PathList::create()
{
	PathListElem *new_path=new PathListElem;
	Path *path=new Path;

	new_path->element=path;
	new_path->next=0;
	if(number==0)
	{
		first_path=new_path;
		last_path=new_path;
	}
	else
	{
		last_path->next=new_path;
		last_path=new_path;
	}
	++number;

	return path;
}

//this routine deletes the path from the position i
bool  PathList::del(int i)
{
	if(i<0 || i>=number)
		return false;

	PathListElem *ptr2=first_path;
	PathListElem *ptr1;

	while(i-- > 0)
	{
		ptr1 = ptr2;
		ptr2 = ptr2->next;
	}

	//if the first element is to delete
	if(ptr2 == first_path)
		first_path=ptr2->next;
	else
		ptr1->next=ptr2->next;

	delete ptr2->element;
	delete ptr2;
	--number;

	//update last_spline
	if(number)
	{
		ptr1=first_path;
		for(i=0;i<number-1;i++)
			ptr1 = ptr1->next;

		last_path=ptr1;
		last_path->next=0;
	}
	return true;
}

//this routine removes the path from the position i
bool  PathList::rem(int i)
{
	if(i<0 || i>=number)
		return false;

	PathListElem *ptr2=first_path;
	PathListElem *ptr1;

	while(i-- > 0)
	{
		ptr1 = ptr2;
		ptr2 = ptr2->next;
	}

	//if the first element is to delete
	if(ptr2 == first_path)
		first_path=ptr2->next;
	else
		ptr1->next=ptr2->next;

	delete ptr2;
	--number;

	//update last_path
	if(number)
	{
		ptr1=first_path;
		for(i=0;i<number-1;i++)
			ptr1 = ptr1->next;

		last_path=ptr1;
		last_path->next=0;
	}
	return true;
}


//this routine deletes the list entirely
void PathList::destroy()
{
	while(first_path !=0)
	{
		PathListElem *tmp = first_path;
		first_path=first_path->next;
		delete tmp;
	}
}