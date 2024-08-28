/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrichir <jrichir@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:24:01 by jrichir           #+#    #+#             */
/*   Updated: 2024/08/28 16:49:40 by jrichir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

char	**draw_full_rect(char **zone, float x, float y, float w, float h, char print)
{
	int	zx;
	int	zy;

	zy = 0;
	while (zone[zy])
	{
		zx = 0;
		while (zone[zy][zx] != '\n')
		{
			if (((float)zx >= x && (float)zx < (x + w)) && ((float)zy >= y && (float)zy < (y + h)))
				zone[zy][zx] = print;
			zx++;
		}
		zy++;
	}
	return (zone);
}

float	dist(float a, float b)
{
	float	dist;

	dist = a - b;
	if (dist < 0)
		return (dist * (-1));
	return (dist);
}

char	**draw_empty_rect(char **zone, float x, float y, float w, float h, char print)
{
	int		zx;
	int		zy;

	zy = 0;
	while (zone[zy])
	{
		zx = 0;
		while (zone[zy][zx] != '\n')
		{
			if (((float)zx >= x && (float)zx < (x + w)) && ((float)zy >= y && (float)zy < (y + h)))
			{
				if (dist((float)zx, x) < 1 || dist((float)zx, x + w) < 1 || dist((float)zy, y) < 1 || dist((float)zy, y + h) < 1)
					zone[zy][zx] = print;
			}
			zx++;
		}
		zy++;
	}
	return (zone);
}

int	main(int argc, char **argv)
{
	int 	scan_count;
	int		zwidth;
	int		zheight;
	char	bgchar;
	FILE	*file;
	char	rtype;
	float	rx;
	float	ry;
	float	rwidth;
	float	rheight;
	char	newline;
	char	rchar;
	char	**draw_zone;
	size_t	check_eof;
	char	buffer[11];

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
	while (fscanf(file, "%c %c %f %f %f %f %c", &newline, &rtype, &rx, &ry, &rwidth, &rheight, &rchar) == 7)
	{
		if (rtype == 'r')
			draw_zone = draw_empty_rect(draw_zone, rx, ry, rwidth, rheight, rchar);
		else if (rtype == 'R')
			draw_zone = draw_full_rect(draw_zone, rx, ry, rwidth, rheight, rchar);
	}
	memset(buffer, '\0', 11);
	check_eof = fread(buffer, 1, 10, file);
	if (check_eof > 0 && buffer[0] != '\n')
		write(1, "Error: Operation file corrupted\n", 32);
	display_drawing(draw_zone, zwidth);
	fclose(file);
	free_draw_zone(draw_zone, zheight);
	return (0);
}
