/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrichir <jrichir@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:24:01 by jrichir           #+#    #+#             */
/*   Updated: 2024/08/27 17:11:10 by jrichir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// WIDTH HEIGHT BACKGROUND_CHAR
// This line is always the first line of the file and it defines the zone where to draw. Your program should not display anything outside the draw zone. It must be there only once.
// - WIDTH: must be a int with 0 < WIDTH <= 300, the horizontal number of characters to use for the draw zone
// - HEIGHT: must be a int with 0 < HEIGHT <= 300, the vertical number of characters to use for the draw zone 
// - BACKGROUND_CHAR: any empty space will be filled with BACKGROUND_CHAR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ok sauf protection incomplete des mallocs (tout ne se free pas si erreur ds boucle)
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
			free(zone);
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

// ok
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
			if ((zx >= x && zx < x + w) && (zy >= y && zy < y + h))
				zone[zy][zx] = print;
			zx++;
		}
		zy++;
	}
	return (zone);
}

char	**draw_empty_rect(char **zone, float x, float y, float w, float h, char print)
{
	int	zx;
	int	zy;

	zy = 0;
	while (zone[zy])
	{
		zx = 0;
		while (zone[zy][zx] != '\n')
		{
			if ((zx >= x && zx < x + w) && (zy == y || zy == y + h - 1) || (zx == x || zx == x + w - 1) && (zy >= y && zy < y + h))
				zone[zy][zx] = print;
			zx++;
		}
		zy++;
	}
	return (zone);
}

// ok mais foire qd j'entre des valeurs a virgule pour les params des fonctions draw
//int	main(int argc, char **argv)
int	main(void)
{
	int 	error;
	int		width;
	char	**draw_zone;

	error = 0;
	width = 50;
	// if (argc != 2)
	// {
	// 	error = 1;
	// 	printf("Error: argument");
	// }
	draw_zone = set_drawzone(width, 25, '#');
	draw_zone = draw_full_rect(draw_zone, 4, 2, 17, 7, '-');
	draw_zone = draw_empty_rect(draw_zone, 9, 7, 19, 11, '*');
	draw_zone = draw_empty_rect(draw_zone, 15, 15, 9, 6, '.');
	draw_zone = draw_empty_rect(draw_zone, 40, 18, 12, 12, '+');
	display_drawing(draw_zone, width);
	if (error)
		return (1);
	return (0);
}
