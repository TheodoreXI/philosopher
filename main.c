/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:19:04 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/14 17:35:24 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

int	main(int argc, char **argv)
{
	t_ph	**num_philo;
	t_mt	**mutex;
	int		num;

	if (check_input(argv, argc))
		return (ft_putstr_fd("input error\n", 2), 1);
	num = ft_atoi(argv[1]);
	if (num <= 0)
		return (1);
	num_philo = malloc(sizeof(t_ph *) * (num + 1));
	if (!num_philo)
		return (1);
	mutex = malloc(sizeof(t_mt *) * (num + 1));
	if (!mutex)
		return (1);
	ft_scan_input(num_philo, argv, num, mutex);
}
