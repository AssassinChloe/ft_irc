#include "ft_irc.hpp"

static int	ft_is_in_charset(char s, char *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		if (c[i] == s)
			return (1);
		i++;
	}
	return (0);
}

static int	ft_strcount(const char *s, char *c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (ft_is_in_charset(s[i], c) == 0)
		{
			j++;
			while (ft_is_in_charset(s[i], c) == 0 && s[i])
				i++;
		}
		else
			i++;
	}
	return (j);
}

static char	*ft_strcpy(const char *s, char *c, unsigned int *i)
{
	char			*str;
	unsigned int	j;
	int				len;

	j = 0;
	len = 0;
	while (ft_is_in_charset(s[*i], c) == 1)
		*i = *i + 1;
	while (s[len + *i] && ft_is_in_charset(s[len + *i], c) == 0)
		len++;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!(str))
		return (NULL);
	while (s[*i] && len > 0)
	{
		str[j] = s[*i];
		j++;
		len--;
		*i = *i + 1;
	}
	str[j] = '\0';
	return (str);
}

static char	**ft_free_tab(char **tab, int j)
{
	while (j > 0)
	{
		j--;
		free(tab[j]);
	}
	free(tab);
	tab = NULL;
	return (tab);
}

char	**ft_split(const char *s, char *c)
{
	char			**tab;
	unsigned int	i;
	int				j;
	int				len;

	if (!(s))
		return (NULL);
	i = 0;
	j = 0;
	len = ft_strcount(s, c);
	tab = (char **)malloc(sizeof(char *) * (len + 1));
	if (!(tab))
		return (NULL);
	while (j < len)
	{
		tab[j] = ft_strcpy(s, c, &i);
		if (tab[j] == NULL)
			return (ft_free_tab(tab, j));
		j++;
	}
	tab[j] = NULL;
	return (tab);
}