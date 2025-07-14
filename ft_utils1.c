/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:18:55 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/14 16:39:25 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

long	ft_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_unlock(pthread_mutex_t *thread1, pthread_mutex_t *thread2)
{
	pthread_mutex_unlock(thread1);
	pthread_mutex_unlock(thread2);
}

int	check_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != 32 && str[i] != 9)
			return (0);
		i++;
	}
	return (1);
}

int	check_input(char **argv, int argc)
{
	int	i;
	int	j;

	i = 1;
	if (argc > 6 || argc < 5)
		return (1);
	while (argv[i])
	{
		j = 0;
		if (check_empty(argv[i]))
			return (1);
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void	ft_print_mutex(int check, t_ph *philo)
{
	pthread_mutex_lock(&philo->data->c);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->c);
		return ;
	}
	if (check == 1 && !philo->data->dead)
		printf("%ld %d has taken a fork\n",
			(ft_time() - philo->data->start), philo->id);
	else if (check == 2 && !philo->data->dead)
		printf("%ld %d is eating\n",
			(ft_time() - philo->data->start), philo->id);
	else if (check == 3 && !philo->data->dead)
		printf("%ld %d is sleeping\n",
			(ft_time() - philo->data->start), philo->id);
	else if (check == 4 && !philo->data->dead)
		printf("%ld %d is thinking\n",
			(ft_time() - philo->data->start), philo->id);
	pthread_mutex_unlock(&philo->data->c);
}
