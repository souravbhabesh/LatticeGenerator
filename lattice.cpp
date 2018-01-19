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
#include "lattice.h"
#include "stdint.h"


int bond_mat[NMAX][NMAX];
int num_bonds = 0;
int dihedrals[MAXDIHEDRALS][4];
int cnt_dihedrals=0;
int particle_id[NMAX];
/*--------------------------------------------------------------------------------------*/
/*      Function for setting up Initial lattice coordinates                             */
/*--------------------------------------------------------------------------------------*/
int initialLatticeStruct(latticeStruct *ip,  int size)
{
  int k = 0;/*keeping track of coords along x-axis*/
  int j;
  for (int i = 0; i < size; i++)
  {
        j = i/NX;
        ip->x[i] = (j%2 == 0) ? (double)(k * a) : (double)(k * a + 0.5);
        k++;
        k = (k%NX == 0) ? 0 : k; /*resetting k to 0 once the lattice has been traversed in the x direction*/
        ip->y[i] = sqrt(3) * a * 0.5 * j;
        ip->z[i]=0; /*  Starting with flat configuration        */
        //ip->z[NX/2]=grandom(0,ZRAND);//Moving one node randomly
   }
   return 0;
}

/*----------------------------------------------------------------------------------------*/
/*Function for generating the Bond Matrix,Matrix[i][j] = 1,Bond between particles i and j */
/*----------------------------------------------------------------------------------------*/

int lattice_connectivity()
{

  /* Initializing elements of the Bond Matrix to 0	*/
  for(int i=0;i<LEN;i++)
  {
	for(int j=0;j<LEN;j++)
	{
		bond_mat[i][j]=0;
	}
  }
		
  for(int i=0;i<LEN;i++)
  {
     /*	EVEN Row	*/
     if((i/NX)%2==0)
     {
        /*	Left boundary lattice sites	*/
        if(i%NX==0)
        {
	   if(i+NX < LEN)
	      bond_mat[i][i+NX]=1;
	   if(i-NX >= 0)
	      bond_mat[i][i-NX]=1;
	   bond_mat[i][i+1]=1;
	}

	/*	Right boundary lattice sites     */
	else if(i%NX==NX-1)
	{
	   if(i+NX<LEN)
	      bond_mat[i][i+NX]=1;
	   if(i+NX-1<LEN)
	      bond_mat[i][i+NX-1]=1;
	   if(i-NX-1>=0)
	      bond_mat[i][i-NX-1]=1;
	   if(i-NX>=0)
	      bond_mat[i][i-NX]=1;
	   bond_mat[i][i-1]=1;
	}

        /*	Intermediate lattice sites	*/
        else
        {
           if(i+NX<LEN)
              bond_mat[i][i+NX]=1;
           if(i+NX-1<LEN)
              bond_mat[i][i+NX-1]=1;
           bond_mat[i][i-1]=1;
           bond_mat[i][i+1]=1;
           if(i-NX-1>=0)
	       bond_mat[i][i-NX-1]=1;
	   if(i-NX>=0)
	       bond_mat[i][i-NX]=1;
	}
      }

      /*  ODD Row   */
      else
      {
         /*	Left boundary lattice sites     */
	 if(i%NX==0)
	 {
	    if(i+NX+1<LEN)
	       bond_mat[i][i+NX+1]=1;
	    if(i+NX<LEN)
	       bond_mat[i][i+NX]=1;
	    bond_mat[i][i+1]=1;
	    if(i-NX>=0)
	       bond_mat[i][i-NX]=1;
	    if(i-NX+1>=0)
	       bond_mat[i][i-NX+1]=1;
	  }

	  /* Right boundary lattice sites     */
	  else if(i%NX==NX-1)
	  {
	     if(i+NX < LEN)
	        bond_mat[i][i+NX]=1;
             if(i-NX >= 0)
		bond_mat[i][i-NX]=1;
	     bond_mat[i][i-1]=1;
	  }

	  /*      Intermediate lattice sites      */
	  else
	  {
              if(i+NX+1<LEN)
                 bond_mat[i][i+NX+1]=1;
              if(i+NX<LEN)
                 bond_mat[i][i+NX]=1;
              bond_mat[i][i-1]=1;
              bond_mat[i][i+1]=1;
              if(i-NX>=0)
                 bond_mat[i][i-NX]=1;
              if(i-NX+1>=0)
                 bond_mat[i][i-NX+1]=1;
           }
        }							           
     }
  return 0;
}

/*	Bond Matrix is Symmetric	*/
/*	Function to check the same	*/

int check_bond_mat()
{
   for(int i=0;i<LEN;i++)
   {
	for(int j=0;j<i;j++)
	{
		if(bond_mat[i][j]!=bond_mat[j][i])
		{
			//printf("At row = %d Col = %d\n",i,j);
			print_and_exit("ERROR: Bond Matrix is not Symmetric\n");
		}
		if(bond_mat[i][j] == 1)
		{
                        num_bonds++;
                }
	}
   }
   return 0;
}

/*	Printing the Bond Pairs, no repeats	*/
int bonds(FILE *fp)
{
   for(int i=0;i<LEN;i++)
   {
        for(int j=0;j<i;j++)
        {
	       if(bond_mat[i][j] == 1)
 	       {   
			fprintf(fp,"%d,%d\n",i,j);
	       }
        }
   }
   return 0;
}

/*----------------------------------------------*/
/*	Generate dihedrals HOOMD convention	*/
/*----------------------------------------------*/
int generate_dihedrals()
{
   int j=0;
  
   /*      Type I dihedral         */
   for(int i=0;i<LEN-NX;i++)
   {
	/*	EVEN Row	*/
	if((i/NX)%2==0 && i%NX!=NX-1)
	{
		dihedrals[cnt_dihedrals][j]=i;j++;
		dihedrals[cnt_dihedrals][j]=i+NX;j++;
		dihedrals[cnt_dihedrals][j]=i+1;j++;
		dihedrals[cnt_dihedrals][j]=i+NX+1;
		cnt_dihedrals++;
		j=0;
	}
	
	/*	ODD Row		*/
	if((i/NX)%2==1 && i%NX!=NX-1 && i%NX!=NX-2)
	{  
                dihedrals[cnt_dihedrals][j]=i;j++;
                dihedrals[cnt_dihedrals][j]=i+NX+1;j++;
                dihedrals[cnt_dihedrals][j]=i+1;j++;
                dihedrals[cnt_dihedrals][j]=i+NX+2;
                cnt_dihedrals++;
                j=0;
        }
   }

   /*      Type II dihedral         */
   for(int i=NX;i<LEN;i++)
   {
        /*      EVEN Row        */
        if((i/NX)%2==0 && i%NX!=NX-1)
        {
                dihedrals[cnt_dihedrals][j]=i;j++;
                dihedrals[cnt_dihedrals][j]=i-NX;j++;
                dihedrals[cnt_dihedrals][j]=i+1;j++;
                dihedrals[cnt_dihedrals][j]=i-NX+1;
                cnt_dihedrals++;
                j=0;
        }

        /*      ODD Row         */
        if((i/NX)%2==1 && i%NX!=NX-1 && i%NX!=NX-2)
        {
                dihedrals[cnt_dihedrals][j]=i;j++;
                dihedrals[cnt_dihedrals][j]=i-NX+1;j++;
                dihedrals[cnt_dihedrals][j]=i+1;j++;
                dihedrals[cnt_dihedrals][j]=i-NX+2;
                cnt_dihedrals++;
                j=0;
        }
   }

   /*      Type III dihedral         */
   for(int i=2*NX;i<LEN;i++)
   {
        /*      EVEN Row        */
        if((i/NX)%2==0 && i%NX!=0)
        {
                dihedrals[cnt_dihedrals][j]=i;j++;
                dihedrals[cnt_dihedrals][j]=i-NX-1;j++;
                dihedrals[cnt_dihedrals][j]=i-NX;j++;
                dihedrals[cnt_dihedrals][j]=i-2*NX;
                cnt_dihedrals++;
                j=0;
        }

        /*      ODD Row         */
        if((i/NX)%2==1 && i%NX!=NX-1)
        {
                dihedrals[cnt_dihedrals][j]=i;j++;
                dihedrals[cnt_dihedrals][j]=i-NX;j++;
                dihedrals[cnt_dihedrals][j]=i-NX+1;j++;
                dihedrals[cnt_dihedrals][j]=i-2*NX;
                cnt_dihedrals++;
                j=0;
        }
   }

   return 0;
}	

/*		Print dihedrals		*/		
int out_dihedrals(FILE *fp)
{
   for(int i=0;i<cnt_dihedrals;i++)
   {
	fprintf(fp,"%d,%d,%d,%d\n",dihedrals[i][0],dihedrals[i][1],dihedrals[i][2],dihedrals[i][3]);
   }
   return 0;
}

/*	Particle Typeid		*/
/*	Normal nodes - particle Id 0 */
/*	Clamped left - particle Id  1	*/
/*	Right end free to slide along X - - particle Id  3 */
/*	Backbone of ribbon - particle Id 4 */

int particle_typeid()
{
   for(int i=0;i<LEN;i++)
   {
	if(i%NX==0 || i%NX==1) //Clamping two columns of lattice sites on the left
	{
		particle_id[i]=1;
		//printf("particle_id[%d] = %d\n",i,particle_id[i]);
	}
	else if (i%NX==NX-1 || i%NX==NX-2) //Two cols lattice sites on the right constrained to move only X
	{
		particle_id[i]=3;
		//printf("particle_id[%d] = %d\n",i,particle_id[i]);
	}
	else
                particle_id[i]=0;//Normal lattice sites 
  }
  //Backbone of the ribbon excluding two lattice sites at each boundary
  for(int i=2;i<NX-2;i++)
  {
	particle_id[(NY/2)*NX + i]=4;
  }
   return 0;
}	

/*	Print particles TypeId		*/
int out_typeId(FILE *fp)
{
   for(int i=0;i<LEN;i++)
   {
	fprintf(fp,"%u\n",particle_id[i]);
   }
   return 0;
}

void print_and_exit(char *format, ...)
{
    va_list list;
    va_start(list,format);
    vfprintf(stderr,format,list);
    va_end(list);
    exit(1);
}

