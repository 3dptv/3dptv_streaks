#include "stdafx.h"
#include "parameters.h"
#include "matrix_op.h"
#include "path.h"
#include "spline.h"
#include "splinelist.h"


//this routine gives the pointer to the element 
//with the number i of SplineList
SplineListElem *SplineList::operator [](int i)
{
	if(i>=0 && i<number)
	{
		SplineListElem *p = first_spline;
		while(i-- >0)
			p=p->next;

		return p;
	}
	else 
		return first_spline;
}

//this routine adds the spline at the end of the list
void SplineList::add(Spline *spline)
{
	//SplineListElem * new_spline = new SplineListElem(*spline);
	SplineListElem *new_spline=new SplineListElem;

	new_spline->element=spline;
	new_spline->next=0;
	if(number==0)
	{
		first_spline=new_spline;
		last_spline=new_spline;
	}
	else
	{
		last_spline->next=new_spline;
		last_spline=new_spline;
	}
	++number;
}


Spline* SplineList::create()
{
	SplineListElem *new_spline=new SplineListElem;
	Spline *spline=new Spline;

	new_spline->element=spline;
	new_spline->next=0;
	if(number==0)
	{
		first_spline=new_spline;
		last_spline=new_spline;
	}
	else
	{
		last_spline->next=new_spline;
		last_spline=new_spline;
	}
	++number;

	return spline;
}


//this routine deletes the spline from the position i
bool  SplineList::del(int i)
{
	if(i<0 || i>=number)
		return false;

	SplineListElem *ptr2=first_spline;
	SplineListElem *ptr1;

	while(i-- > 0)
	{
		ptr1 = ptr2;
		ptr2 = ptr2->next;
	}

	//if the first element is to delete
	if(ptr2 == first_spline)
		first_spline=ptr2->next;
	else
		ptr1->next=ptr2->next;

	delete ptr2->element;
	delete ptr2;
	--number;

	//update last_spline
	if(number)
	{
		ptr1=first_spline;
		for(i=0;i<number-1;i++)
			ptr1 = ptr1->next;

		last_spline=ptr1;
		last_spline->next=0;
	}
	return true;
}




//this routine removes the spline from the position i
bool  SplineList::rem(int i)
{
	if(i<0 || i>=number)
		return false;

	SplineListElem *ptr2=first_spline;
	SplineListElem *ptr1;

	while(i-- > 0)
	{
		ptr1 = ptr2;
		ptr2 = ptr2->next;
	}

	//if the first element is to delete
	if(ptr2 == first_spline)
		first_spline=ptr2->next;
	else
		ptr1->next=ptr2->next;

	delete ptr2;
	--number;

	//update last_spline
	if(number)
	{
		ptr1=first_spline;
		for(i=0;i<number-1;i++)
			ptr1 = ptr1->next;

		last_spline=ptr1;
		last_spline->next=0;
	}
	return true;
}


//this routine deletes the list entirely
void SplineList::destroy()
{
	while(first_spline !=0)
	{
		SplineListElem *tmp = first_spline;
		first_spline=first_spline->next;
		delete tmp;
	}
}