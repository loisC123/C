#ifndef	_POLYNOMIAL_H
#define	_POLYNOMIAL_H
#ifndef	_POLYNOMIAL_H_
#define	_POLYNOMIAL_H_

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */


#define	YES	(0 == 0)
#define	NO	(1 == 0)

#define	Polynomial_MaxDegree	100

/* Definition: Polynomial */
typedef	struct	{
	float	coef[Polynomial_MaxDegree+1];
} Polynomial;

/* API (Application Programming Interface */

int	Polynomial_Print(Polynomial *poly);
/* print the polynomial pointed by poly as 2x^8-6x^4+3x-5 */
/* return: number of terms */

void	Polynomial_Zero(Polynomial *poly);
/* initialize the polynomial pointed by poly to a zero ploynomial */

int	Polynomial_isZero(Polynomial *poly);
/* test whether the polynomial pointed by poly is a zero polynomial */

int	Polynomial_Degree(Polynomial *poly);
/* return the degree of the polynomial pointed by poly */

float	Polynomial_Coef(Polynomial *poly, int degree);
/* return the coefficient of the specified term
   in the polynomial pointed by poly */

void	Polynomial_Attach(Polynomial *poly, float coef, int degree);
/* attach a term onto the polynomial pointed by poly */

void	Polynomial_Remove(Polynomial *poly, int degree);
/* remove the specified term from the polynomial pointed by poly */

void	Polynomial_Add(Polynomial *pa, Polynomial *pb, Polynomial *pc);
/* add two polynomials pointed by pa and pb, the resultant is stored in
   the polynomial pointed by pc */

void	Polynomial_Mul(Polynomial *pa, Polynomial *pb, Polynomial *pc);
/* multiply two polynomials pointed by pa and pb, the resultant is stored in
   the polynomial pointed by pc */

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */

#endif	/* _POLYNOMIAL_H_ */
#endif	/* _POLYNOMIAL_H */
