/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:24:12 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/17 10:58:34 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

int	ft_routine_check(t_dt *data)
{
	pthread_mutex_lock(&data->t);
	if (data->end_sim)
		return (pthread_mutex_unlock(&data->t), 1);
	pthread_mutex_unlock(&data->t);
	return (0);
}

int	ft_dead_check(t_ph *philo, int check)
{
	if (check == 1)
	{
		pthread_mutex_lock(&philo->data->c);
		if (philo->data->dead)
			return (pthread_mutex_unlock(&philo->data->c), 1);
		pthread_mutex_unlock(&philo->data->c);
	}
	else if (check == 2)
	{
		pthread_mutex_lock(&philo->data->c);
		if (philo->data->dead)
			return (ft_unlock(philo->right_fork, philo->left_fork),
				pthread_mutex_unlock(&philo->data->c), 1);
		pthread_mutex_unlock(&philo->data->c);
	}
	return (0);
}

void	*start_routine(void *arg)
{
	t_ph	*philo;

	philo = (t_ph *)arg;
	if (philo->id % 2 == 0)
		usleep((philo->data->t_eat * 1000) / 2);
	while (!ft_dead_check(philo, 1) && !ft_routine_check(philo->data))
	{
		ft_lock(philo);
		if (ft_dead_check(philo, 2) || ft_routine_check(philo->data))
			return (NULL);
		ft_print_mutex(1, philo);
		if (ft_dead_check(philo, 2) || ft_routine_check(philo->data))
			return (NULL);
		ft_print_mutex(2, philo);
		ft_get_last_meal(philo);
		if (ft_sleep(philo->data->t_eat, philo->data))
			return (ft_unlock(philo->left_fork, philo->right_fork), NULL);
		ft_unlock(philo->left_fork, philo->right_fork);
		ft_print_mutex(3, philo);
		if (ft_sleep(philo->data->t_sleep, philo->data))
			return (NULL);
		ft_print_mutex(4, philo);
	}
	return (NULL);
}

void	*one_thread_routine(void *arg)
{
	t_ph	*philo;
	int		check;

	philo = (t_ph *)arg;
	check = 0;
	while (!ft_dead_check(philo, 1))
	{
		if (ft_routine_check(philo->data))
			return (NULL);
		if (!check)
		{
			pthread_mutex_lock(philo->left_fork);
			ft_print_mutex(1, philo);
		}
		ft_sleep(philo->data->t_die, philo->data);
		if (ft_dead_check(philo, 1))
			return (NULL);
		if (!check)
			pthread_mutex_unlock(philo->left_fork);
		check = 1;
	}
	return (NULL);
}

void	ft_free_mutex(t_mt **mutex)
{
	int	i;

	i = 0;
	while (mutex[i])
	{
		free (mutex[i]);
		mutex[i] = NULL;
		i++;
	}
	free (mutex);
	mutex = NULL;
}
