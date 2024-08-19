## Descrição do Trabalho

O projeto descreve um programa em C que é capaz de escrever dados em formato de texto em páginas de memória virtual do sistema operacional.

### 1. Estrutura do código

- #### Inclusão de bibliotecas
```c 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
```
#### O código começa incluindo as bibliotecas necessárias. Cada uma dessas bibliotecas fornece funções essenciais para a operação:

`string.h:` Para manipulação de strings.
<br>
`sys/mman.h:` Para funções relacionadas à manipulação de memória, como mmap e munmap.
<br>
`sys/stat.h:` Para operações com atributos de arquivos, como fstat.
<br>
`fcntl.h:` Para operações de controle de arquivos, como open.
<br>
`unistd.h:` Para funções padrão do sistema Unix, como close e read.

### 2. Verificação dos Argumentos

```c
if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    exit(EXIT_FAILURE);
}
```
Este bloco de código verifica se o programa foi chamado com o argumento necessário, que é o nome do arquivo de entrada. Caso contrário, ele exibe uma mensagem de uso e encerra o programa.

### 3. Abertura do Arquivo

```c
int fd = open(input_file, O_RDONLY);
if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
```
Aqui, o arquivo é aberto no modo de leitura (O_RDONLY). Se houver um erro na abertura do arquivo, a função perror exibe uma mensagem de erro, e o programa termina.

### 4. Obtendo o Tamanho do Arquivo 

```c
struct stat file_stat;
if (fstat(fd, &file_stat) == -1) {
    perror("fstat");
    close(fd);
    exit(EXIT_FAILURE);
}
size_t file_size = file_stat.st_size;
```
O código utiliza a função fstat para obter informações sobre o arquivo, especificamente seu tamanho. Isso é necessário para saber quanto espaço de memória deve ser alocado para armazenar o conteúdo do arquivo.

### 5. Mapeamento de Memória 
```c
void *mapped_mem = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
if (mapped_mem == MAP_FAILED) {
    perror("mmap");
    close(fd);
    exit(EXIT_FAILURE);
}
```
A função mmap é usada para mapear um arquivo ou dispositivo na memória. Neste caso, o código mapeia um espaço de memória anônimo e privado com permissão de leitura e escrita. Se a função falhar, uma mensagem de erro é exibida, e o programa é encerrado.

### 6. Leitura e Cópia do Arquivo para a Memória

```c
ssize_t bytes_read = read(fd, mapped_mem, file_size);
if (bytes_read == -1) {
    perror("read");
    munmap(mapped_mem, file_size);
    close(fd);
    exit(EXIT_FAILURE);
}
```
Este bloco de código lê o conteúdo do arquivo e copia para a memória mapeada. A função read retorna o número de bytes lidos. Se ocorrer um erro durante a leitura, o programa desmapeia a memória, fecha o arquivo e termina a execução.

### 7. Exibição dos Dados Lidos

```c
printf("Data written to virtual memory:\n%.*s\n", (int)file_size, (char *)mapped_mem);
```
Aqui, o conteúdo lido do arquivo e armazenado na memória mapeada é exibido no console.

### 8. Desmapeamento da Memória e Fechamento do Arquivo
```c
if (munmap(mapped_mem, file_size) == -1) {
    perror("munmap");
}

close(fd);
```
Finalmente, a memória mapeada é desmapeada usando munmap e o arquivo é fechado.

### Como Executar o Código

#### Para compilar e executar o código, siga os passos abaixo:

1. Compile o código usando o GCC:
```sh
gcc -o mmap_example mmap_example.c
```
2. Execute o programa fornecendo o nome de um arquivo de entrada:
```sh
./mmap_example <input_file>
```
Certifique-se de substituir `<input_file>` pelo caminho do arquivo que você deseja mapear na memória.

### Considerações Finais
Este código é um exemplo básico para ilustrar o conceito de mapeamento de memória em ambientes Unix/Linux. Ele não cobre todas as possíveis verificações de erro e não lida com arquivos extremamente grandes que poderiam não caber na memória. Para produção, melhorias adicionais e manuseio adequado de erros seriam necessários.