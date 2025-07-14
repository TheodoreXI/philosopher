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

int	ft_scan_input_helper(t_ph **philo, int num, t_mt **mutex, pthread_t monitor_thread)
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

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (s == NULL || fd < 0)
		return ;
	write(fd, s, ft_strlen(s));
}

static int	ft_space(const char **str, int sign)
{
	while ((**str >= 9 && **str <= 13) || **str == 32)
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign *= -1;
		(*str)++;
	}
	return (sign);
}

long	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	long			res;
	long			tmp;

	sign = 1;
	i = 0;
	res = 0;
	tmp = 0;
	sign = ft_space(&str, sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - 48);
		if (res / 10 != tmp && sign == 1)
			return (-2);
		else if (res / 10 != tmp && sign == -1)
			return (-2);
		tmp = res;
		i++;
	}
	if (res * sign > 2147483647 || res * sign < -2147483648)
		return (-2);
	return (res * sign);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
