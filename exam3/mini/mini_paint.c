/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrichir <jrichir@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:20:54 by jrichir           #+#    #+#             */
/*   Updated: 2024/09/10 14:36:12 by jrichir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> //memset
#include <math.h>   //sqrtf

typedef struct s_circle	t_circle;
typedef struct s_canvas	t_canvas;

struct s_circle
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	symbol;
};

struct s_canvas
{
	int		w;
	int		h;
	char	bg;
	char	**img;
};

void	free_img(char **img, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(img[i]);
		i++;
	}
	free(img);
	return ;
}

char	**set_img(t_canvas canvas)
{
	char	**img;
	int		i;

	img = malloc((canvas.h + 1) * sizeof(char *));
	if (!img)
		return (NULL);
	img[canvas.h] = NULL;
	i = 0;
	while (i < canvas.h)
	{
		img[i] = malloc((canvas.w + 2) * sizeof(char));
		if (!img[i])
		{
			free_img(img, i);
			return (NULL);
		}
		memset(img[i], canvas.bg, canvas.w);
		img[i][canvas.w] = '\n';
		img[i][canvas.w + 1] = '\0';
		i++;
	}
	return (img);
}

float	dist(float ax, float ay, float bx, float by)
{
	return (sqrtf((ax - bx) * (ax - bx) + (ay - by) * (ay - by)));
}

void	display(t_canvas canvas)
{
	int	i;

	i = 0;
	while (i < canvas.h)
	{
		write(1, canvas.img[i], canvas.w + 1);
		i++;
	}
}

// if (diff <= -1.00 && circle.type == 'C') --> inside
// if (diff > -1.00 && diff <= 0.00) --> border
char	**addcircle(t_canvas canvas, t_circle circle)
{
	int		i;
	int		j;
	float	diff;

	i = 0;
	while (i < canvas.h)
	{
		j = 0;
		while (j < canvas.w)
		{
			diff = dist((float)j, (float)i, circle.x, circle.y) - circle.radius;
			if (diff <= -1.00 && circle.type == 'C')
				canvas.img[i][j] = circle.symbol;
			if (diff > -1.00 && diff <= 0.00)
				canvas.img[i][j] = circle.symbol;
			j++;
		}
		i++;
	}
	return (canvas.img);
}

int	execute(FILE *file)
{
	t_canvas	canvas;
	t_circle	circle;
	int			scan_ret;

	scan_ret = fscanf(file, "%d %d %c\n", &canvas.w, &canvas.h, &canvas.bg);
	if (scan_ret != 3 || canvas.w < 1 || canvas.h < 1 || canvas.w > 300 || canvas.h > 300)
		return (1);
	canvas.img = set_img(canvas);
	if (!canvas.img)
		return (1);
	scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.symbol);
	if (scan_ret != -1 && scan_ret != 5)
		return (1);
	while (scan_ret == 5)
	{
		if ((circle.type != 'c' && circle.type != 'C') || circle.radius <= 0.0)
			return (1);
		canvas.img = addcircle(canvas, circle);
		scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.symbol);
	}
	if (scan_ret != -1)
		return (1);
	display(canvas);
	return (0);
}

int	main(int argc, char **argv)
{
	FILE		*file;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file || execute(file) == 1)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		fclose(file);
		return (1);
	}
	fclose(file);
	return (0);
}

