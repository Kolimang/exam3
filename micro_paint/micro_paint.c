/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrichir <jrichir@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:20:54 by jrichir           #+#    #+#             */
/*   Updated: 2024/09/11 10:49:40 by jrichir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> //memset

typedef struct s_rect	t_rect;
typedef struct s_canvas	t_canvas;

struct s_rect
{
	char	type;
	float	x;
	float	y;
	float	w;
	float	h;
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

float	dist(float a, float b)
{
	if (a - b < 0.0000)
		return ((a - b) * (-1));
	return (a - b);
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

char	**addrect(t_canvas canvas, t_rect rect)
{
	int		x;
	int		y;

	y = 0;
	while (y < canvas.h)
	{
		x = 0;
		while (x < canvas.w)
		{
			if ((float)x >= rect.x && (float)x <= rect.x + rect.w && (float)y >= rect.y && (float)y <= rect.y + rect.h)
			{
				if (rect.type == 'R')
					canvas.img[y][x] = rect.symbol;
				else if (dist(x, rect.x) < 1 || dist(x, rect.x + rect.w) < 1 || dist(y, rect.y) < 1 || dist(y, rect.y + rect.h) < 1 )
					canvas.img[y][x] = rect.symbol;
			}
			x++;
		}
		y++;
	}
	return (canvas.img);
}

int	execute(FILE *file)
{
	t_canvas	canvas;
	t_rect		rect;
	int			scan_ret;

	scan_ret = fscanf(file, "%d %d %c\n", &canvas.w, &canvas.h, &canvas.bg);
	if (scan_ret != 3 || canvas.w < 1 || canvas.h < 1 || canvas.w > 300 || canvas.h > 300)
		return (1);
	canvas.img = set_img(canvas);
	if (!canvas.img)
		return (1);
	scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.w, &rect.h, &rect.symbol);
	if (scan_ret != -1 && scan_ret != 6)
		return (1);
	while (scan_ret == 6)
	{
		if ((rect.type != 'r' && rect.type != 'R') || rect.w <= 0.0000 || rect.h <= 0.0000)
			return (1);
		canvas.img = addrect(canvas, rect);
		scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.w, &rect.h, &rect.symbol);
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
