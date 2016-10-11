/*
 *  Generates 'a' so that it has a given condition number
 *  Sets eigenvalues spaced linearly from 1.0/cond to 1.0
 *  We assume the constant NMAX is already #defined elsewehere
 */

void matcondgen(float cond, int n, float a[NMAX][NMAX], float b[NMAX])
{
  float t[NMAX][NMAX];
  float tmp;

  float det;

  int i, j, info;
  int ipiv[NMAX];

  for (i=0; i < n; i++)
    {
      for (j=0; j < n; j++)
        {
          a[i][j] = 0.0;
        }
    }
	
  det = 1.0;

  for (i=0; i < n; i++)
    {
      a[i][i] = (1.0 + (cond-1.0)*((float) i)/((float) n-1))/cond;
      det = det * a[i][i];
    }

  matgen(n, t, b);

  /*           T T
   *  set a = t.a
   *
   *  ie a[i][j] <- t[k][i]*a[j][k] = a[j][j] t[j][i]
   */


  for (j=0; j < n; j++)
    {
      tmp = a[j][j];

      for (i=0; i < n; i++)
        {
          a[i][j] = tmp*t[j][i];
        }
    }

  /*             T                      T
   *  Now solve a = t^-1.a, ie solve t.a = a, equivalent to
   *
   *    T T T   T
   *  (t ) a = a 
   *
   *  Routine sees tranposed matrices, so pass a and t "as-is", and solve
   *  transpose eqns for t
   */

  sgetrf(n, n, &(t[0][0]), NMAX, ipiv, &info);
  sgetrs('t', n, n, &(t[0][0]), NMAX, ipiv, &(a[0][0]), NMAX, &info);

/*                 T
 *  We solved for a  so now tranpose a in-place
 */

  for (i=0; i < n; i++)
  {
    for (j=0; j < i; j++)
    {
      tmp = a[i][j];
      a[i][j] = a[j][i];
      a[j][i] = tmp;
    }
  }

  /*
   *  set b appropriately so solution is x(i) = 1.0 for all i
   */

  for (i=0; i < n; i++)
    {
      b[i] = 0.0;

      for (j=0; j < n; j++)
        {
          b[i] = b[i] + a[i][j];
        }
    }
}
