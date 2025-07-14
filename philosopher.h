/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:58:15 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/14 14:41:54 by aakroud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_dt
{
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			t_num_eat;
	long			start;
	int				dead;
	int				end_sim;
	int				num;
	pthread_mutex_t	p;
	pthread_mutex_t	c;
	pthread_mutex_t t;
	// pthread_mutex_t eat_finish;
	struct s_ph		**philo;
} t_dt;

typedef struct s_mt
{
	pthread_mutex_t fork;
} t_mt;

typedef struct s_ph
{
	int				id;
	long			l_meal;
	int				count_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_dt			*data;
} t_ph;


long	ft_atoi(const char *str);
int		ft_isdigit(int c);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *s);
void	ft_mutex_destroyer(t_mt **mutex, int num, t_dt *data);
long	ft_time(void);
void	ft_unlock(pthread_mutex_t *thread1, pthread_mutex_t *thread2);
int		check_empty(char *str);
int		check_input(char **argv, int argc);
void	ft_print_mutex(int check, t_ph *philo);
int		ft_routine_check(t_dt *data);
int		ft_dead_check(t_ph *philo, int check);
void	*start_routine(void *arg);
void	*one_thread_routine(void *arg);
void	ft_free_mutex(t_mt **mutex);
int		ft_mutex_initializer(t_mt **mutex, int num, t_dt *data);
void	ft_free_array(t_ph **philo, int num);
int		ft_data_initializer(t_dt *data);
t_dt	*ft_alloc_helper(char **argv, int num);
int		ft_alloc_philo(t_ph **philo, int num, char **argv);
void	ft_mutex_destroyer(t_mt **mutex, int num, t_dt *data);
void	ft_m_philo(t_ph **philo, t_mt **mutex);
int		ft_sleep(long time_to_sleep, t_dt *data);
void	*monitor(void *arg);
void	*ft_monitor_helper(t_dt *data, int i);
int		ft_meal_check(t_ph **philo, t_dt *data);
void	ft_get_last_meal(t_ph *philo);
int		ft_scan_input(t_ph **philo, char **argv, int num, t_mt **mutex);



#endif