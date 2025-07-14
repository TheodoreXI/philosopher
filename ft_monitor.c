/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:22:31 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/14 14:46:01 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

void	ft_get_last_meal(t_ph *philo)
{
	pthread_mutex_lock(&philo->data->t);
	philo->l_meal = ft_time();
	pthread_mutex_unlock(&philo->data->t);
	pthread_mutex_lock(&philo->data->p);
	philo->count_meal += 1;
	pthread_mutex_unlock(&philo->data->p);
}

int	ft_meal_check(t_ph **philo, t_dt *data)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	if (data->t_num_eat == -1)
		return (0);
	while (philo[i])
	{
		pthread_mutex_lock(&data->p);
		if (data->t_num_eat <= philo[i]->count_meal)
			check++;
		pthread_mutex_unlock(&data->p);
		i++;
	}
	if (check == data->num)
		return (1);
	return (0);
}

void	*ft_monitor_helper(t_dt *data, int i)
{
	pthread_mutex_unlock(&data->t);
	pthread_mutex_lock(&data->c);
	printf("%ld %d died\n", (ft_time() - data->start), data->philo[i]->id);
	data->dead = 1;
	pthread_mutex_unlock(&data->c);
	return (pthread_mutex_unlock(&data->t), NULL);
}

void	*monitor(void *arg)
{
	t_dt	*data;
	int		i;

	data = (t_dt *)arg;
	i = 0;
	while (1)
	{
		i = 0;
		if (ft_meal_check(data->philo, data))
		{
			pthread_mutex_lock(&data->t);
			data->end_sim = 1;
			return (pthread_mutex_unlock(&data->t), NULL);
		}
		while (data->philo[i])
		{
			pthread_mutex_lock(&data->t);
			if (data->t_die < ft_time() - data->philo[i]->l_meal)
				return (ft_monitor_helper(data, i));
			pthread_mutex_unlock(&data->t);
			i++;
		}
		usleep (1000);
	}
	return (NULL);
}

int	ft_sleep(long time_to_sleep, t_dt *data)
{
	long	begin;

	begin = ft_time();
	while (time_to_sleep > (ft_time() - begin))
	{
		pthread_mutex_lock(&data->c);
		if (data->dead)
			return (pthread_mutex_unlock(&data->c), 1);
		pthread_mutex_unlock(&data->c);
		usleep(100);
	}
	return (0);
}
