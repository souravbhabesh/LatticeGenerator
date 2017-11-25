#define a	1
#define NMAX	30000
#define MAXDIHEDRALS	100000


extern int NX,NY,LEN;

/*	Structure for position coordinates	*/
struct latticeStruct
{
    float x[NMAX];
    float y[NMAX];
    float  z[NMAX];
};

/*----------------------------------------------------------------------*/
/*		Bond Matrix LEN x LEN, Symmetric Matrix			*/	
/*	 	Matrix[i][j] = 1 , Bond between particles i and j	*/
/*----------------------------------------------------------------------*/
extern int bond_mat[NMAX][NMAX];
extern int num_bonds;
extern int dihedrals[MAXDIHEDRALS][4];
extern int cnt_dihedrals;
extern int particle_id[NMAX];

/*	Arrays to hold Bond Pairs	*/
extern int p1[NMAX][6],p2[NMAX][6];
