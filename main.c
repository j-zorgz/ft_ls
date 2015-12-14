#include <ft_ls.h>

int	main(int ac, char **av)
{
	t_opt	options;
	int	i;
	int	j;
	int	cont;

	cont = 1;
	i = 1;
	while(i < ac && cont)
	{
		if (av[i][0] == '-')
		{
			j = 0;
			while (av[i][j] != '\0')
			{
				if (av[i][j] == 'l')
					options.l = 1;
				else if (av[i][j] == 'R')
					options.R = 1;
				else if (av[i][j] == 'a')
					options.a = 1;
				else if (av[i][j] == 'r')
					options.r = 1;
				else if (av[i][j] == 't')
					options.t = 1;
				j++;
			}
			i++;
		}
		else
			cont = 0;
	}
	return (0);
}

/*
	directory = opendir(av[1]);
	t_dirent *truc;
	while ((truc = readdir(directory)))
	{
		ft_putendl(truc->d_name);
	}
	closedir(directory);
*/
