#include "../headers/header.h"

uint32_t ft_strlen(const char* str)
{
	uint32_t res = 0;

	while (*(str++))
	    res++;
	return res;
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*res_dest;
	unsigned char	*res_src;

	res_dest = (unsigned char *)dest;
	res_src = (unsigned char *)src;
	while (n--)
	{
		*res_dest = *res_src;
		res_dest++;
		res_src++;
	}
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*temp_src;
	unsigned char	*temp_dest;

	temp_src = (unsigned char *) src;
	temp_dest = (unsigned char *) dest;
	i = 0;
	if (temp_dest > temp_src)
	{
		while (i < n)
		{
			temp_dest[n - 1] = temp_src[n - 1];
			n--;
		}
	}
	else
	{
		while (i < n)
		{
			temp_dest[i] = temp_src[i];
			i++;
		}
	}
	return (dest);
}