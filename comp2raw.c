/* gcc -o comp2raw comp2raw.c */

#include <stdio.h>

int main (int argc, char **argv)
{
	FILE *infp, *outfp;
	unsigned char i;
	int a;
	
	/* Exponential table */
	char ex[16] = { 0, 1, 3, 7, 15, 31, 63, 127, -128, -64, -32, -16, -8, -4, -2, -1};
	
	
	if (argc != 3)
	{
		fprintf (stderr, "Usage: %s infile.comp outfile.raw\n", argv[0]);
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
	
	a = 0;
	while (!feof(infp))
	{	
		/* Read two samples */
		fread (&i, 1, 1, infp);
		
		/* Accumulate first sample */
		a += ex[i >> 4];
		if (a > 127) a = 127;
		if (a < -128) a = -128;
		fwrite (&a, 1, 1, outfp);
		
		/* Accumulate second sample */
		a += ex[i & 15];
		if (a > 127) a = 127;
		if (a < -128) a = -128;
		fwrite (&a, 1, 1, outfp);
	}
	fclose (infp);
	fclose (outfp);
}