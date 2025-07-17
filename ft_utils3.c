/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:26:24 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/17 11:03:59 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

int	ft_mutex_initializer(t_mt **mutex, int num, t_dt *data)
{
	int	i;

	i = 0;
	while (i < num)
	{
		mutex[i] = malloc(sizeof(t_mt));
		if (!mutex[i])
			return (ft_free_mutex(mutex), 1);
		if (pthread_mutex_init(&mutex[i]->fork, NULL))
			return (ft_mutex_destroyer(mutex, i, data),
				ft_free_mutex(mutex), 1);
		i++;
	}
	mutex[i] = NULL;
	return (0);
}

void	ft_free_array(t_ph **philo, int num)
{
	int	i;

	i = 0;
	if (!philo[0] || !philo[0]->data)
	{
		free (philo);
		return ;
	}
	free(philo[i]->data);
	while (i < num)
	{
		free (philo[i]);
		philo[i] = NULL;
		i++;
	}
	free (philo);
	philo = NULL;
}

int	ft_data_initializer(t_dt *data)
{
	if (pthread_mutex_init(&data->p, NULL))
		return (1);
	if (pthread_mutex_init(&data->c, NULL))
		return (pthread_mutex_destroy(&data->p), 1);
	if (pthread_mutex_init(&data->t, NULL))
		return (pthread_mutex_destroy(&data->c),
			pthread_mutex_destroy(&data->p), 1);
	return (0);
}

t_dt	*ft_alloc_helper(char **argv, int num)
{
	t_dt	*data;

	data = malloc(sizeof(t_dt));
	if (!data)
		return (NULL);
	data->start = ft_time();
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->t_num_eat = ft_atoi(argv[5]);
	else
		data->t_num_eat = -1;
	if (data->t_die < 0 || data->t_eat < 0
		|| data->t_sleep < 0 || data->t_num_eat < -1)
		return (free(data), NULL);
	data->dead = 0;
	data->num = num;
	data->end_sim = 0;
	if (ft_data_initializer(data))
		return (free(data), NULL);
	return (data);
}

int	ft_alloc_philo(t_ph **philo, int num, char **argv)
{
	int		i;
	t_dt	*data;

	i = 0;
	data = ft_alloc_helper(argv, num);
	if (!data)
		return (1);
	while (i < num)
	{
		philo[i] = malloc(sizeof(t_ph));
		if (!philo[i])
			return (1);
		philo[i]->left_fork = NULL;
		philo[i]->right_fork = NULL;
		philo[i]->id = i + 1;
		philo[i]->l_meal = data->start;
		philo[i]->count_meal = 0;
		philo[i]->data = data;
		i++;
	}
	philo[i] = NULL;
	data->philo = philo;
	return (0);
}
