/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:25:54 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/15 10:02:55 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

void	ft_m_philo(t_ph **philo, t_mt **mutex)
{
	int	i;

	i = 0;
	while (philo[i])
	{
		philo[i]->left_fork = &mutex[i]->fork;
		i++;
	}
	i = 0;
	while (philo[i])
	{
		if (philo[i + 1])
			philo[i]->right_fork = &mutex[i + 1]->fork;
		else
			philo[i]->right_fork = &mutex[0]->fork;
		i++;
	}
}

int	ft_scan_input_helper(t_ph **philo, int num,
						t_mt **mutex, pthread_t monitor_thread)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_join(philo[i]->thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	ft_mutex_destroyer(mutex, num, philo[0]->data);
	return (1);
}

int	ft_one_philo(t_ph **philo, int num, t_mt **mutex)
{
	if (pthread_create(&philo[0]->thread, NULL,
			one_thread_routine, (void *) philo[0]))
		return (ft_free_array(philo, num),
			ft_mutex_destroyer(mutex, num, philo[0]->data),
			ft_free_mutex(mutex), 1);
	return (0);
}

int	ft_multi_philo(t_ph **philo, int num, t_mt **mutex)
{
	int	i;

	i = 0;
	while (i < num)
	{
		if (pthread_create(&philo[i]->thread, NULL,
				start_routine, (void *) philo[i]))
			return (ft_free_array(philo, num),
				ft_mutex_destroyer(mutex, num, philo[i]->data), 1);
		i++;
	}
	return (0);
}

int	ft_scan_input(t_ph **philo, char **argv, int num, t_mt **mutex)
{
	pthread_t	monitor_thread;

	if (ft_alloc_philo(philo, num, argv))
		return (ft_free_array(philo, num), 1);
	if (ft_mutex_initializer(mutex, num, philo[0]->data))
		return (ft_free_array(philo, num),
			ft_mutex_destroyer(mutex, num, philo[0]->data), 1);
	ft_m_philo(philo, mutex);
	if (num <= 1)
	{
		if (ft_one_philo(philo, num, mutex))
			return (1);
	}
	else
	{
		if (ft_multi_philo(philo, num, mutex))
			return (1);
	}
	if (pthread_create(&monitor_thread, NULL, monitor, (void *) philo[0]->data))
		return (ft_scan_input_helper(philo, num, mutex, monitor_thread));
	ft_scan_input_helper(philo, num, mutex, monitor_thread);
	return (ft_free_mutex(mutex), ft_free_array(philo, num), 0);
}
