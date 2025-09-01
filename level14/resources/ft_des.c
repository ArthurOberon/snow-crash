/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_des.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoberon <aoberon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 11:39:47 by aoberon           #+#    #+#             */
/*   Updated: 2025/08/29 13:44:21 by aoberon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

char * ft_des(char *s)
{
	unsigned int	i;
	int	key_i;
	int	j;
	int	k;
	int	len;
	char	*str;
	char	key[] = "0123456";	

	str = strdup(s);
	len = strlen(str);
	key_i = 0;
	for ( i = 0; len > i; ++i )
	{
		if ( key_i == 6 )
			key_i = 0;
		if ( (i & 1) != 0 )
		{
			for ( j = 0; key[key_i] > j; ++j )
			{
				if ( ++str[i] == 127 )
				str[i] = 32;
			}
		}
		else
		{
			for ( k = 0; key[key_i] > k; ++k )
			{
			if ( --str[i] == 31 )
				str[i] = 126;
			}
		}
		++key_i;
	}
	return str;
}

int	main(void)
{
	printf("%s", ft_des("g <t61:|4_|!@IF.-62FH&G~DCK/Ekrvvdwz?v|"));
}