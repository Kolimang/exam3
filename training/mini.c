#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

typedef struct s_canvas
{
	int		argc;
	char	**argv;
	int		w;
	int		h;
	int		symbol;
	char	**img;
}	t_canvas;

typedef struct s_circle
{
	char	ctype;
	float	cx;
	float	cy;
	float	cradius;
	char	csymbol;
}	t_circle;

char	**set_canvas(t_canvas *canvas, int argc, char **argv)
{
	canvas->argc = argc;
	canvas->argv = argv;
	return (canvas->img);
}

int	main(int argc, char **argv)
{
	t_canvas	canvas;
	FILE		*file;

	// parsing de argc et argv
	file = fopen(argv[1], "r");
	if (!file)
		return (1);
	while (fread(file, 1, ... , argv[1]))
	{
		/* code */
	}
	
	canvas.img = set_canvas(&canvas, argc, argv);
	fclose(file);
	return (0);
}
