/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:06:06 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/17 10:31:38 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

void	ft_mutex_destroyer(t_mt **mutex, int num, t_dt *data)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&mutex[i]->fork);
		i++;
	}
	pthread_mutex_destroy(&data->p);
	pthread_mutex_destroy(&data->c);
	pthread_mutex_destroy(&data->t);
}

void	ft_lock(t_ph *philo)
{
	pthread_mutex_lock(philo->left_fork);
	ft_print_mutex(1, philo);
	pthread_mutex_lock(philo->right_fork);
}
