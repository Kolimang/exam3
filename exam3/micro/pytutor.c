#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct s_rc
{
	char	type;
	float	x;
	float	y;
	float	w;
	float	h;
	char	sy;
}	t_rc;

typedef struct s_cv
{
	int		w;
	int		h;
	char	bg;
	char	**ig;
}	t_cv;

void	free_i(char **ig, int h)
{
	int	i;

	i = 0;
	while (i < h)
	{
		free(ig[i]);
		i++;
	}
	free(ig);
	return ;
}

char	**set_ig(t_cv cv)
{
	char	**ig;
	int		i;

	ig = malloc((cv.h + 1) * sizeof(char *));
	if (!ig)
		return (NULL);
	ig[cv.h] = NULL;
	i = 0;
	while (i < cv.h)
	{
		ig[i] = malloc((cv.w + 2) * sizeof(char));
		if (!ig[i])
		{
			free_i(ig, i);
			return (NULL);
		}
		memset(ig[i], cv.bg, cv.w);
		ig[i][cv.w] = '\n';
		ig[i][cv.w + 1] = '\0';
		i++;
	}
	return (ig);
}

float	dst(float a, float b)
{
	if (a - b < 0.00)
		return ((a - b) * (-1));
	return (a - b);
}

void	disp(t_cv cv)
{
	int	i;

	i = 0;
	while (i < cv.h)
	{
		write(1, cv.ig[i], cv.w + 1);
		i++;
	}
}

char	**addrc(t_cv cv, t_rc rc)
{
	int		x;
	int		y;

	y = 0;
	while (y < cv.h)
	{
		x = 0;
		while (x < cv.w)
		{
			if ((float)x >= rc.x && (float)x <= rc.x + rc.w && (float)y >= rc.y && (float)y <= rc.y + rc.h)
			{
				if (rc.type == 'R')
					cv.ig[y][x] = rc.sy;
				else if (dst(x, rc.x) < 1 || dst(x, rc.x + rc.w) < 1 || dst(y, rc.y) < 1 || dst(y, rc.y + rc.h) < 1 )
					cv.ig[y][x] = rc.sy;
			}
			x++;
		}
		y++;
	}
	return (cv.ig);
}

int	exc(FILE *file)
{
	t_cv	cv;
	t_rc		rc;
	int			rt;

	rt = fscanf(file, "%d %d %c\n", &cv.w, &cv.h, &cv.bg);
	if (rt != 3 || cv.w < 1 || cv.h < 1 || cv.w > 300 || cv.h > 300)
		return (1);
	cv.ig = set_ig(cv);
	if (!cv.ig)
		return (1);
	rt = fscanf(file, "%c %f %f %f %f %c\n", &rc.type, &rc.x, &rc.y, &rc.w, &rc.h, &rc.sy);
	if (rt != -1 && rt != 5)
		return (1);
	while (rt == 5)
	{
		if ((rc.type != 'r' && rc.type != 'R') || rc.w <= 0.00 || rc.h <= 0.00)
			return (1);
		cv.ig = addrc(cv, rc);
		rt = fscanf(file, "%c %f %f %f %f %c\n", &rc.type, &rc.x, &rc.y, &rc.w, &rc.h, &rc.sy);
	}
	if (rt != -1)
		return (1);
	disp(cv);
	return (0);
}

int	main(int argc, char **argv)
{
	FILE		*file;

	if (argc != 2)
	{
		write(1, "E1\n", 3);
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file || exc(file) == 1)
	{
		write(1, "E2\n", 3);
		fclose(file);
		return (1);
	}
	fclose(file);
	return (0);
}
