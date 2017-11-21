#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <limits>
#include <string.h>
#include <iostream>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include "lattice_variables.h"
#include "stdint.h"
#include "streamCompaction.h"

int p1[NMAX][6],p2[NMAX][6];


int bond_compaction()
{
int l=0;
for(int j=0;j<LEN;j++)
{
	for(int k=0;k<LEN;k++)
	{
		if(bond_mat[j][k]==1)
		{
			//printf("%d\t%d\n",j,k);
			p1[j][l] = j;
			p2[j][l] = k;
			l++;
		}
	}
	if(l<5)
	{
		for(int e=l;e<6;e++)
		{
			p1[j][e] = 0;
			p2[j][e] = 0;
		}
	}
	l=0;
}
/*
   for(int e=0;e<6;e++)
   {
	printf("%d\t%d\n",p1[1][e],p2[1][e]);
   }
*/
   return 0;
}

/*	Sorting dihedrals using the 2nd particle	*/
int insertionSortDihedrals(int num_dihedrals)
{
	int tempSorter[4];
	int k;
        for (int i=1; i<num_dihedrals; i++)
        {
		for(int j=0;j<4;j++)
		{	
			tempSorter[j] = dihedrals[i][j];
		}
			k = i-1;
			while (k>=0 && dihedrals[k][1] > tempSorter[1])
				{
					for(int j=0;j<4;j++)
					{
						dihedrals[k+1][j] = dihedrals[k][j];
					}
					//neighborList[i+1] = neighborList[i];
                                        k = k-1;
				}
			for(int j=0;j<4;j++)
                	{
                        	dihedrals[k+1][j] = tempSorter[j];
                	}
        }
        return 0;
}

