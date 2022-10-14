#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int finder(const char *filename, char *dirname)
{
	// Добавление '/' в конец к названию пути, если нет изначально
	if (dirname[strlen(dirname) - 1] != '/')
	{
		dirname = realloc(dirname, strlen(dirname) + 2);
		dirname = strcat(dirname, "/");
	}
	// Инициализация dir и проверка, открылась ли директория
	DIR *dir = opendir(dirname);
	if (!dir)
	{
		perror("Error");
		free(dirname);
		return (1);
	}
	struct dirent *entry;
	// Считывание данных из директории циклом
	while ((entry = readdir(dir)) != NULL)
	{
		// Пропуск '.' и '..'
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;
        // Сравнение названий файлов
		if (!strcmp(entry->d_name, filename))
			printf("%s%s\n", dirname, entry->d_name);
		// Рекурсивный обход дочерних каталогов
		if (entry->d_type == DT_DIR)
		{
			char *subdir_name = strdup(dirname);
			subdir_name = realloc(subdir_name, strlen(subdir_name) + strlen(entry->d_name) + 1);
			finder(filename, strcat(subdir_name, entry->d_name));
		}
	}
	closedir(dir);
	free(dirname);
	return (0);
}

int main(int argc, char **argv) {
	if (argc != 2 && argc != 3)
	{
	    perror("Error: invalid arguments");
	    return 1;
	}

	int result = argc == 3 ? chdir(argv[2]) : 0;
	if (result != 0)
	{
	    perror("Error");
	    return 1;
	}

	return finder(argv[1], strdup("."));
}
