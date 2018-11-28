/* This program is a translation with a few adaptations of the 
   Fortran program.f, made by FO @ CDS  (francois@simbad.u-strasbg.fr)  
   in November 1996.
   On a Unix station, don't forget the -lm flag, i.e. create the program by:
   cc program.c -o program -lm
*/

#include <stdio.h>	/* Standard i/o functions */
#include <string.h>	/* for strchr routine     */
#include <ctype.h>      /* for isgraph, isspace   */
#include <math.h>       /* All mathematics        */
#define DCOS	cos
#define DSIN	sin
#define DATAN2	atan2
#define DASIN	asin

#define X1(i)	x1[i-1]
#define X2(i)	x2[i-1]
#define R(i,j)	r[i-1][j-1]

void HGTPRC(RA1,DEC1,EPOCH1,EPOCH2,RA2,DEC2)
/*------------------------------------------
C      HERGET PRECESSION, SEE P. 9 OF PUBL. CINCINNATI OBS. NO. 24
C INPUT=  RA1 AND DEC1 MEAN PLACE, IN RADIANS, FOR EPOCH1, IN YEARS A.D.
C OUTPUT= RA2 AND DEC2 MEAN PLACE, IN RADIANS, FOR EPOCH2, IN YEARS A.D.
--------------------------------------------*/
  double *RA1, *DEC1, *EPOCH1, *EPOCH2, *RA2, *DEC2 ;
{
  static double CDR = 0.17453292519943e-01 ;
  static double EP1=0, EP2=0 ;
  static double x1[3],x2[3],r[3][3], 
	T,ST,A,B,C,CSR,SINA,SINB,SINC,COSA,COSB,COSC ;
  int i, j;
    /* Compute input direction cosines */
    A=DCOS(*DEC1) ;
    X1(1)=A*DCOS(*RA1) ;
    X1(2)=A*DSIN(*RA1) ;
    X1(3)=DSIN(*DEC1) ;
    /* Set up rotation matrix (R) */
    if(EP1 == *EPOCH1 &&  EP2 == *EPOCH2) ;
    else {
      EP1 = *EPOCH1 ; EP2 = *EPOCH2 ;
      CSR=CDR/3600. ;
      T=0.001*(EP2-EP1) ;
      ST=0.001*(EP1-1900.) ;
      A=CSR*T*(23042.53+ST*(139.75+0.06*ST)+T*(30.23-0.27*ST+18.0*T)) ;
      B=CSR*T*T*(79.27+0.66*ST+0.32*T)+A ;
      C=CSR*T*(20046.85-ST*(85.33+0.37*ST)+T*(-42.67-0.37*ST-41.8*T)) ;
      SINA=DSIN(A) ;
      SINB=DSIN(B) ;
      SINC=DSIN(C) ;
      COSA=DCOS(A) ;
      COSB=DCOS(B) ;
      COSC=DCOS(C) ;
      R(1,1)=COSA*COSB*COSC-SINA*SINB ;
      R(1,2)=-COSA*SINB-SINA*COSB*COSC ;
      R(1,3)=-COSB*SINC ;
      R(2,1)=SINA*COSB+COSA*SINB*COSC ;
      R(2,2)=COSA*COSB-SINA*SINB*COSC ;
      R(2,3)=-SINB*SINC ;
      R(3,1)=COSA*SINC ;
      R(3,2)=-SINA*SINC ;
      R(3,3)=COSC ;
    }
    /* Perform the rotation to get the direction cosines at epoch2 */
    for (i=1; i<=3; i++) {
      X2(i)=0. ;
      for (j=1; j<=3; j++) X2(i)+=R(i,j)*X1(j) ;
    }
    *RA2=DATAN2(X2(2),X2(1)) ;
    if(*RA2 <  0) *RA2 = 6.28318530717948 + *RA2 ;
    *DEC2=DASIN(X2(3)) ;
    return ;
}

main()
/*-------------------------------------------------------
C PROGRAM TO FIND CONSTELLATION NAME  FROM A POSITION
C ---> Modified FO, CDS Strasbourg, DARSIN is NOT a standard F77 function...
C	... but DASIN !!!
C
C THE FIRST RECORD IN THE DATA SET MUST BE THE EQUINOX FOR THE POSITIONS
C  IN THE FORMAT XXXX.X
C THE REMAINING RECORDS MUST BE THE POSITION IN HOURS AND DECIMALS OF
C  AN HOUR FOLLOWED BY THE DECLINATION IN DEGREES AND FRACTIONS OF A
C  DEGREE IN THE FORMAT: F7.4,F8.4
C THE OUTPUT WILL REPEAT THE POSITION ENTERED AND GIVE THE THREE LETTER
C  ABBREVIATION OF THE CONSTELLATION IN WHICH IT IS LOCATED
C
-------------------------------------------------------*/
{
  char *CON ;
  static double CONVH = 0.2617993878;
  static double CONVD = 0.1745329251994e-01;
  static double PI4 = 6.28318530717948;
  static double E75 = 1875.;
  double ARAD,DRAD,A,D,E,RAH,RA,DEC,RAL,RAU,DECL,DECD;
  char buf[BUFSIZ], *p ;
  FILE *f2 ;
    f2 = fopen("data.dat", "r"); if (!f2) perror("data.dat") ;
    E = 2000.;
    while(1) {
      if (isatty(0)) printf("Enter the position (dd.ddd DD.dddd): "); 
      if (scanf("%lf %lf", &RAH, &DECD) < 2) break;

      /* PRECESS POSITION TO 1875.0 EQUINOX */
      ARAD = CONVD * RAH ;
      DRAD = CONVD * DECD ;
      HGTPRC(&ARAD,&DRAD,&E,&E75,&A,&D) ;
      if(A <  0.)A=A+PI4 ;
      if(A >= PI4)A=A-PI4 ;
      RA= A/CONVH ;
      DEC=D/CONVD ;

    /* FIND CONSTELLATION SUCH THAT THE DECLINATION ENTERED IS HIGHER THAN
       THE LOWER BOUNDARY OF THE CONSTELLATION WHEN THE UPPER AND LOWER
       RIGHT ASCENSIONS FOR THE CONSTELLATION BOUND THE ENTERED RIGHT
       ASCENSION
    */
      fseek(f2, 0, 0) ;
      while(fscanf(f2, "%lf %lf %lf %s", &RAL, &RAU, &DECL, CON) > 0) {

      	if(DECL >  DEC) continue ;
   	if (RAU <= RA) continue ;
   	if(RAL >  RA) continue ;
	/* if CONSTELLATION HAS BEEN FOUND, WRITE RESULT AND RERUN PROGRAM FOR
	   NEXT ENTRY.  OTHERWISE, CONTINUE THE SEARCH BY RETURNING TO RAU
	*/
      	if (RA >= RAL &&  RA <  RAU &&  DECL <= DEC) 
          printf(" RA =%8.4f Dec = %8.4f  is in Constellation: %s\n",
		RAH, DECD,CON);
        else if(RAU <  RA) continue ;
        else printf(" Constellation NOT FOUND for: RA =%8.4f Dec = %8.4f\n",
		RAH,DECD);
	break ;
      }
    }
    /* printf(" End of input positions after: RA = %7.4f   DEC = %8.4f\n", 
	RAH,DECD) ;
    */
    if (isatty(0)) puts("") ;
    printf("===============The Equinox for these positions is: %6.1f\n", E) ;
}


