/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrichir <jrichir@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:24:01 by jrichir           #+#    #+#             */
/*   Updated: 2024/08/28 14:35:04 by jrichir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void	free_draw_zone(char **zone, int height)
{
	int	i;

	i = 0;
	while (i <= height)
	{
		free(zone[i]);
		i++;
	}
	free(zone);
}

char	**set_drawzone(int w, int h, char bg)
{
	char	**zone;
	int		i;
	int		j;

	zone = (char **)malloc((h + 1) * sizeof(char *));
	if (!zone)
		return (NULL);
	zone[h] = NULL;
	i = 0;
	while (i < h)
	{
		zone[i] = (char *)malloc((w + 2) * sizeof(char));
		if (!zone[i])
		{
			free_draw_zone(zone, i);
			return (NULL);
		}
		zone[i][w] = '\n';
		zone[i][w + 1] = '\0';
		j = 0;
		while (zone[i][j] != '\n')
		{
			zone[i][j] = bg;
			j++;
		}
		i++;
	}	
	return (zone);
}

void	display_drawing(char **draw_zone, int width)
{
	int	i;

	i = 0;
	while (draw_zone[i])
	{
		write(1, draw_zone[i], width + 1);
		i++;
	}
}

float	get_distance(float zx, float zy, float bx, float by)
{
	float distance;

	distance = 0;
	distance = sqrtf((zx - bx) * (zx - bx) + (zy - by) * (zy - by));
	return (distance);
}

char	**draw_full_circle(char **zone, float x, float y, float radius, char print)
{
	int		zx;
	int		zy;

	zy = 0;
	while (zone[zy])
	{
		zx = 0;
		while (zone[zy][zx] != '\n')
		{
			if (get_distance((float)zx, (float)zy, x, y) <= radius)
				zone[zy][zx] = print;
			zx++;
		}
		zy++;
	}
	return (zone);
}

char	**draw_empty_circle(char **zone, float x, float y, float radius, char print)
{
	int		zx;
	int		zy;
	float	diff;

	zy = 0;
	while (zone[zy])
	{
		zx = 0;
		while (zone[zy][zx] != '\n')
		{
			diff = get_distance(zx, zy, x, y) - radius;
			if (diff < 0)
				diff = diff * (-1);
			if (diff < 1)
				zone[zy][zx] = print;
			zx++;
		}
		zy++;
	}
	return (zone);
}

// foire qd j'entre des valeurs a virgule pour les params des fonctions draw
int	main(int argc, char **argv)
{
	int 	scan_count;
	int		zwidth;
	int		zheight;
	char	bgchar;
	FILE	*file;
	char	ctype;
	float	cx;
	float	cy;
	float	radius;
	char	newline;
	char	cchar;
	char	**draw_zone;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	scan_count = fscanf(file,"%d %d %c", &zwidth, &zheight, &bgchar);
	if (scan_count != 3)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		fclose(file);
		return (1);
	}
	draw_zone = set_drawzone(zwidth, zheight, bgchar);
	if (!draw_zone)
	{
		write(1, "Error: memory allocation failed\n", 32);
		fclose(file);
		return (1);
	}
	while (fscanf(file, "%c %c %f %f %f %c", &newline, &ctype, &cx, &cy, &radius, &cchar) == 6)
	{
		if (ctype == 'c')
			draw_zone = draw_empty_circle(draw_zone, cx, cy, radius, cchar);
		else if (ctype == 'C')
			draw_zone = draw_full_circle(draw_zone, cx, cy, radius, cchar);
	}
	// should test if EOF was reached using fscanf or fread output
	display_drawing(draw_zone, zwidth);
	fclose(file);
	free_draw_zone(draw_zone, zheight);
	return (0);
}
