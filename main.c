/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:19:04 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/10 21:00:37 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

long	ft_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
int	check_input(char **argv, int argc)
{
	int	i;
	int	j;

	i = 1;
	if (argc > 6)
		return (1);
	while (argv[i])
	{
		j = 0;
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
	pthread_mutex_lock(&philo->data->p);
	if (check == 1 && !philo->data->dead)
		printf("%ld %d has taken a fork\n", (ft_time() - philo->data->start), philo->id);
	else if (check == 2 && !philo->data->dead)
		printf("%ld %d is eating\n", (ft_time() - philo->data->start), philo->id);
	else if (check == 3 && !philo->data->dead)
		printf("%ld %d is sleeping\n", (ft_time() - philo->data->start), philo->id);
	else if (check == 4 && !philo->data->dead)
		printf("%ld %d is thinking\n", (ft_time() - philo->data->start), philo->id);
	else if (check == 5)
		printf("%ld %d died\n", (ft_time() - philo->data->start), philo->id);
	pthread_mutex_unlock(&philo->data->p);
}


void	ft_get_last_meal(t_ph *philo)
{
	pthread_mutex_lock(&philo->data->c);
	philo->l_meal = ft_time();
	pthread_mutex_unlock(&philo->data->c);
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
		while (data->philo[i])
		{
			pthread_mutex_lock(&data->c);
			if (data->t_die < ft_time() - data->philo[i]->l_meal)
			{
				ft_print_mutex(5, data->philo[i]);
				pthread_mutex_lock(&data->p);
 				data->dead = 1;
				pthread_mutex_unlock(&data->p);
				return (pthread_mutex_unlock(&data->c), NULL);
			}
			pthread_mutex_unlock(&data->c);
			i++;
		}
	}
	return (NULL);
}

void	*start_routine(void *arg)
{
	t_ph	*philo;
	
	philo = (t_ph *)arg;
	if (philo->id % 2 == 0)
		usleep (philo->data->t_eat * 1000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->c);
		if (philo->data->dead)
			return (pthread_mutex_unlock(&philo->data->c), NULL);
		pthread_mutex_unlock(&philo->data->c);
		// pthread_mutex_lock(&philo->data->t);
		pthread_mutex_lock(philo->left_fork);
		ft_print_mutex(1, philo);
		pthread_mutex_lock(philo->right_fork);
		ft_print_mutex(1, philo);
		usleep(philo->data->t_eat * 1000);
		ft_print_mutex(2, philo);
		ft_get_last_meal(philo);
		pthread_mutex_lock(&philo->data->c);
		if (philo->data->dead)
			return (pthread_mutex_unlock(&philo->data->c), pthread_mutex_unlock(&philo->data->t), NULL);
		pthread_mutex_unlock(&philo->data->c);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		ft_print_mutex(3, philo);
		usleep(philo->data->t_sleep * 1000);
		pthread_mutex_lock(&philo->data->c);
		if (philo->data->dead)
			return (pthread_mutex_unlock(&philo->data->c), pthread_mutex_unlock(&philo->data->t), NULL);
		pthread_mutex_unlock(&philo->data->c);
		ft_print_mutex(4, philo);
		// pthread_mutex_unlock(&philo->data->t);
	}
}

void	ft_initializer(t_ph *philo)
{
	philo->id = 0;
}

int	ft_mutex_initializer(t_mt **mutex, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		mutex[i] = malloc(sizeof(t_mt));
		if (!mutex[i])
			return (1);
		if (pthread_mutex_init(&mutex[i]->fork, NULL))
			return (1);
		i++;
	}
	mutex[i] = NULL;
	return (0);
}

void	ft_free_array(t_ph **philo, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		free (philo[i]);
		philo[i] = NULL;
		i++;
	}
	free (philo);
	philo = NULL;
}

int	ft_alloc_philo(t_ph **philo, int num, char **argv)
{
	int	i;
	t_dt	*data;

	i = 0;
	data = malloc(sizeof(t_dt));
	if (!data)
		return (1);
	data->start = ft_time();
	data->t_die = ft_atoi(argv[2]);
	if (data->t_die < 0)
		return (1);
	data->t_eat = ft_atoi(argv[3]);
	if (data->t_eat < 0)
		return (1);
	data->t_sleep = ft_atoi(argv[4]);
	if (data->t_sleep < 0)
		return (1);
	data->t_num_eat = ft_atoi(argv[4]);
	if (data->t_num_eat < 0)
		return (1);
	data->dead = 0;
	pthread_mutex_init(&data->p, NULL);
	pthread_mutex_init(&data->c, NULL);
	pthread_mutex_init(&data->t, NULL);
	while (i < num)
	{
		philo[i] = malloc(sizeof(t_ph));
		if (!philo[i])
			return (1);
		philo[i]->left_fork = NULL;
		philo[i]->right_fork = NULL;
		philo[i]->id = i + 1;
		philo[i]->l_meal = ft_time();
		philo[i]->data = data;
		i++;
	}
	philo[i] = NULL;
	data->philo = philo;
	return (0);
}

void	ft_mlx_destroyer(t_mt **mutex, int num, t_dt *data)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&mutex[i]->fork);
		i++;
	}
	pthread_mutex_destroy(&data->p);
	pthread_mutex_destroy(&data->l_mutex);
	pthread_mutex_destroy(&data->r_mutex);

}

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

int	ft_scan_input(t_ph **philo, char **argv, int num, t_mt **mutex)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	if (ft_alloc_philo(philo, num, argv))
		return (ft_free_array(philo, num), 1);
	if (ft_mutex_initializer(mutex, num))
		return (ft_mlx_destroyer(mutex, num, philo[i]->data), 1);
	ft_m_philo(philo, mutex);
	while (i < num)
	{
		if (pthread_create(&philo[i]->thread, NULL, start_routine, (void *) philo[i]))
			return (1);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor, (void *) philo[0]->data);
	i = 0;
	while (i < num)
	{
		pthread_join(philo[i]->thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	ft_mlx_destroyer(mutex, num, philo[0]->data);
	return (0);
	// join the thread and then destroy the mutx
}


int	main(int argc, char **argv)
{
	t_ph	**num_philo;
	t_mt	**mutex;
	int		num;

	if (check_input(argv, argc))
		return (ft_putstr_fd("input error\n", 2), 1);
	num = ft_atoi(argv[1]);
	if (num < 0)
		return (1);
	num_philo = malloc(sizeof(t_ph *) * (num + 1));
	if (!num_philo)
		return (1);
	mutex = malloc(sizeof(t_mt *) * (num + 1));
	if (!mutex)
		return (1);
	ft_scan_input(num_philo, argv, num, mutex);
}
