#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"
#include "matrix_op.h"
#include "path.h"
#include "write_out.h"



void Write_out::Pixels(FILE* file, Pixel point[], int nr)
{
	for(int i=0; i < nr;i++)
		fprintf(file,"%10.6f%10.6f\n",point[i].x,point[i].y);
}


void Write_out::Pixels(FILE* file, CamPoint point[], int nr)
{
	for(int i=0; i < nr;i++)
		fprintf(file,"%10.6f%10.6f\n",point[i].x,point[i].y);
}



void Write_out::W_path(FILE* file, Path *path)
{
	for(Path_seg *s = path->path_seg; s < path->path_seg+path->nr_seg; s++)
	{
		for(CamPoint *hp = s->hpixel; hp < s->hpixel +s->p_nr; hp++)
			fprintf(file,"%10.6f%10.6f\n",hp->x,hp->y);
	}
}