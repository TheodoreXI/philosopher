/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:19:04 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/14 12:11:54 by aakroud          ###   ########.fr       */
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
		printf("%ld %d has taken a fork\n", (ft_time() - philo->data->start), philo->id);
	else if (check == 2 && !philo->data->dead)
		printf("%ld %d is eating\n", (ft_time() - philo->data->start), philo->id);
	else if (check == 3 && !philo->data->dead)
		printf("%ld %d is sleeping\n", (ft_time() - philo->data->start), philo->id);
	else if (check == 4 && !philo->data->dead)
		printf("%ld %d is thinking\n", (ft_time() - philo->data->start), philo->id);
	pthread_mutex_unlock(&philo->data->c);
}


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
	int check;

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
			pthread_mutex_unlock(&data->t);
			return (NULL);
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
			return (ft_unlock(philo->right_fork, philo->left_fork), pthread_mutex_unlock(&philo->data->c), 1);
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
		pthread_mutex_lock(philo->left_fork);
		ft_print_mutex(1, philo);
		pthread_mutex_lock(philo->right_fork);
		if (ft_dead_check(philo, 2) || ft_routine_check(philo->data))
			return (NULL);
		ft_print_mutex(1, philo);
		if (ft_dead_check(philo, 2) || ft_routine_check(philo->data))
			return (NULL);
		ft_print_mutex(2, philo);
		if (ft_sleep(philo->data->t_eat, philo->data))
			return (ft_unlock(philo->left_fork, philo->right_fork), NULL);
		ft_get_last_meal(philo);
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

void	ft_data_initializer(t_dt *data)
{
	pthread_mutex_init(&data->p, NULL);
	pthread_mutex_init(&data->c, NULL);
	pthread_mutex_init(&data->t, NULL);
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
	if (data->t_die < 0 || data->t_eat < 0 || data->t_sleep < 0 || data->t_num_eat < -1)
		return (free(data), NULL);
	data->dead = 0;
	data->num = num;
	data->end_sim = 0;
	ft_data_initializer(data);
	return (data);
	// pthread_mutex_init(&data->p, NULL);
	// pthread_mutex_init(&data->c, NULL);
	// pthread_mutex_init(&data->t, NULL);
}

int	ft_alloc_philo(t_ph **philo, int num, char **argv)
{
	int	i;
	t_dt	*data;

	i = 0;
	// data = malloc(sizeof(t_dt));
	// if (!data)
	// 	return (1);
	// data->start = ft_time();
	// data->t_die = ft_atoi(argv[2]);
	// if (data->t_die < 0)
	// 	return (1);
	// data->t_eat = ft_atoi(argv[3]);
	// if (data->t_eat < 0)
	// 	return (1);
	// data->t_sleep = ft_atoi(argv[4]);
	// if (data->t_sleep < 0)
	// 	return (1);
	// if (argv[5])
	// 	data->t_num_eat = ft_atoi(argv[5]);
	// else
	// 	data->t_num_eat = -1;
	// if (data->t_num_eat < -1)
	// 	return (1);
	// data->dead = 0;
	// data->num = num;
	// data->end_sim = 0;
	// pthread_mutex_init(&data->p, NULL);
	// pthread_mutex_init(&data->c, NULL);
	// pthread_mutex_init(&data->t, NULL);
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
	pthread_mutex_destroy(&data->c);
	pthread_mutex_destroy(&data->t);
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
	if (num <= 1)
	{
		if (pthread_create(&philo[i]->thread, NULL, one_thread_routine, (void *) philo[i]))
			return (1);
	}
	else
	{
		while (i < num)
		{
			if (pthread_create(&philo[i]->thread, NULL, start_routine, (void *) philo[i]))
				return (1);
			i++;
		}
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
