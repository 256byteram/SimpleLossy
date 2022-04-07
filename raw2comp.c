/* gcc -o raw2comp raw2comp.c */

#include <stdio.h>

int main (int argc, char **argv)
{
	FILE *infp, *outfp;
	char i;
	unsigned char w;
	int l, shift, sw, d, e, a;
	
	/* Exponential table */
	char ex[16] = { 0, 1, 3, 7, 15, 31, 63, 127, -128, -64, -32, -16, -8, -4, -2, -1};
	
	
	if (argc != 3)
	{
		fprintf (stderr, "Usage: %s infile.raw outfile.comp\n", argv[0]);
		return -1;
	}
	
	if ((infp = fopen(argv[1], "rb")) == NULL)
	{
		fprintf (stderr, "Unable to open %s for reading\n", argv[1]);
		return -1;
	}
	
	if ((outfp = fopen(argv[2], "wb")) == NULL)
	{
		fclose (infp);
		fprintf (stderr, "Unable to open %s for reading\n", argv[1]);
		return -1;
	}
	
	i = 0;
	e = 0;
	a = 0;
	sw = 0;
	while (!feof(infp))
	{
		/* Difference between previous an new sample */
		d = i;
		/* New sample */
		fread (&i, 1, 1, infp);
		
		/* Calculate change from previous sample, include error */
		d -= e - i;
		
		/* Clamp to 8-bits signed */
		if (d > 127) d = 127;
		if (d < -128) d = -128;
		
		/* Calculate log of d. Negative logs and 0 are valid */
		l = 0;
		shift = d;
		if (shift < 0) shift = -shift;
		if (shift) while (shift >>= 1) l++;
		if (d < 0) l = -l & 15;
		
		/* Extract the log (lossy) and add it to the accumulator.
		 * Multiplication of 3 improves fidelity by increasing 
		 * feedback gain.
		 */
		a += ex[l] * 3;
		
		/* Calculate the error. Difference between result and input */
		e = a - i;
		
		/* Write file on alternate samples */
		if (sw)
		{
			w |= l;
			fwrite (&w, 1, 1, outfp);
		}
		else
		{
			w = l << 4;
		}	
		sw = !sw;
	}
	
	fclose (infp);
	fclose (outfp);
	return 0;
}
