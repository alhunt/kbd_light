#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NODEPATH		"/sys/class/leds/smc::kbd_backlight/"
#define BRFILE			NODEPATH "brightness"
#define MAXFILE			NODEPATH "max_brightness"
#define DELTA			10

void usage(){
	printf("Usage: kbd_light -set <value> - Set brightness value\n");
	printf("       kbd_light -get         - Get brightness value\n");
	printf("       kbd_light [-up|-down]  - Increment brightness\n");
	printf("       kbd_light [-off|-max]  - Off or max brightness\n");
	
	exit(0);
}

int main(int argc, char* argv[])
{
	FILE *file;
	int cur, new, maxval;
	char *endptr;

	if (argc < 2)
	{
		usage();
		return 0;
	}
	
	// get current brightness value
	if ((file = fopen(BRFILE, "r")))
	{
		fscanf(file, "%d", &cur);
		fclose(file);
	}
	else
	{
		perror("Failed to open: " BRFILE);
		return 1;
	}

	// get max brightness value
	if ((file = fopen(MAXFILE, "r")))
	{
		fscanf(file, "%d", &maxval);
		fclose(file);
	}
	else
		maxval = 255;
		
	// check arguments
	if (strcmp(argv[1], "-set") == 0 && argc == 3)
	{
		// check argv[2] is numeric
		if(!(new = strtol(argv[2], &endptr, 10)))
		{
			fprintf(stderr, "Set value not numeric: %s\n", argv[2]);

			return 1;
		}
	}
	else if (strcmp(argv[1], "-up") == 0 && argc == 2)
		new = cur + DELTA;
	else if (strcmp(argv[1], "-down") == 0 && argc == 2)
		new = cur - DELTA;
	else if (strcmp(argv[1], "-off") == 0 && argc == 2)
		new = 0;
	else if (strcmp(argv[1], "-max") == 0 && argc == 2)
		new = maxval;
	else if (strcmp(argv[1], "-get") == 0 && argc == 2)
	{
		printf("%d\n", cur);

		return 0;
	}
	else // invalid argument
		usage();
	
	// New value must be between 0 and MAX
	if (new < 0)
		new = 0;
	else if (new > maxval)
		new = maxval;

	// write new value to device node
	if ((file = fopen(BRFILE, "w")))
	{
		fprintf(file, "%d", new);
		fclose(file);	
	}
	else
		perror("Failed to open: " BRFILE);

	return 0;
}


