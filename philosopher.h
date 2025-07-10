/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakroud <aakroud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:58:15 by aakroud           #+#    #+#             */
/*   Updated: 2025/07/10 19:15:36 by aakroud          ###   ########.fr       */
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
	pthread_mutex_t	l_mutex;
	pthread_mutex_t	r_mutex;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			t_num_eat;
	long			start;
	int				dead;
	pthread_mutex_t	p;
	pthread_mutex_t	c;
	pthread_mutex_t t;
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
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_dt			*data;
} t_ph;


long	ft_atoi(const char *str);
int		ft_isdigit(int c);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *s);


#endif