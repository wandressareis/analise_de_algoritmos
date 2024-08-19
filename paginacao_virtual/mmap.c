#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[1];

    // Abrindo o arquivo para leitura
    int fd = open(input_file, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Obtendo o tamanho do arquivo
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }
    size_t file_size = file_stat.st_size;

    // Mapeando a memória
    void *mapped_mem = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mapped_mem == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Lendo o arquivo e copiando para a memória mapeada
    ssize_t bytes_read = read(fd, mapped_mem, file_size);
    if (bytes_read == -1) {
        perror("read");
        munmap(mapped_mem, file_size);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Exibindo os dados que foram escritos na memória
    printf("Data written to virtual memory:\n%.*s\n", (int)file_size, (char *)mapped_mem);

    // Desmapeando a memória
    if (munmap(mapped_mem, file_size) == -1) {
        perror("munmap");
    }

    close(fd);
    return 0;
}
