#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h> // minipaint

typedef struct s_canvas
{
	int		w;
	int 	h;
	char	bg;
	char	*img;
}	t_canvas;

typedef struct s_rect
{
	char	type;
	float	x;
	float	y;
	float	w;
	float	h;
	char	col;
}	t_rect;

int	set_canvas(FILE *file, t_canvas canvas)
{
	if (fscanf(file, "%d %d %c\n", &canvas.w, &canvas.h, &canvas.bg) != 3)
		return(1);
	if (canvas.w < 1 || canvas.w > 300 || canvas.h < 1 || canvas.h > 300)
		return (1);
	if (!(canvas.img = malloc(sizeof(char) * canvas.w * canvas.h)))
		return (1);
	memset(canvas.img, canvas.bg, canvas.w * canvas.h);
	return (0);
}

int	is_in_shape(float x, float y, t_rect rect)
{
	if (x < rect.x || x > rect.x + rect.w || y < rect.y || y > rect.y + rect.h)
		return (0);
	if (rect.type == 'R')
		return (1);
	if (x - rect.x < 1.000 || x - (rect.x + rect.w) < 1.000 || y - rect.y < 1.000 || y - (rect.y + rect.h) < 1.000)
		return (1);
	return (0);
}

int	exec(FILE *file, t_canvas canvas)
{
	t_rect	rect;

	while(fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.w, &rect.h, &rect.col) == 6)
	{
		if ((rect.type != 'r' && rect.type != 'R') || rect.w < 1.000 || rect.h < 1.000)
			return (1);
		for (int x = 0; x < canvas.w; x++)
			for (int y = 0; y < canvas.h; y++)
				if (is_in_shape((float)x, (float)y, rect))
					canvas.img[y * canvas.w + x] = rect.col;
	}
	return (0);
}

int	main(int ac, char **av)
{
	FILE		*file;
	t_canvas	canvas;

	if (ac != 2)
		return (write(1, "Error: argument\n", 16), 1);
	if (!(file = fopen(av[1], "r")) || set_canvas(file, canvas))
		return (write(1, "Error: Operation file corrupted\n", 32), 1);
	if (exec(file, canvas))
		return (free(canvas.img), write(1, "Error: Operation file corrupted\n", 32), 1);
	for (int y = 0; y < canvas.h; y++)
	{
		write(1, canvas.img + y * canvas.w, canvas.w);
		write(1, "\n", 1);
	}
	fclose(file);
	free(canvas.img);
	return (0);
}
