# Asynchronous Socket Connection
-----
Projeto desenvolvido para atender os requisitos de um desafio técnico.

## Desafio
Desenvolver um software utilizando a linguagem C++ para ambientes Linux.

O software deve receber dados proveniente da rede e salvar estes dados em arquivos localmente, ou seja, será um servidor que receberá dados via stream (socket) e
salvará os dados em arquivos de até X bytes. Deve-se desenvolver também o serviço cliente que envia as informações para esse servidor. 

**Premissas**
1) A porta que o software recebe os dados deve ser configurável por um arquivo de configuração;
2) O tamanho dos arquivos salvos deve ser configurável;
3) Comunicação TCP;
4) Container;

**Requisitos**
1. Básicos
    1. A porta que o software receberá os dados deverá ser configurável por um arquivo de configuração; Fazer um SH que modifica as variáveis de ambiente.
    2. O tamanho dos arquivos salvos deve ser configurável;
    3. Comunicação TCP;
    4. TODOS os dados transmitidos devem ser armazenados corretamente;
    5. Os arquivos não podem exceder o tamanho máximo em qualquer hipótese;

2. Intermediários
    1. Caso haja a necessidade de armazenar os dados em mais de um arquivo, o arquivo imediatamente anterior deve ter exatamente o tamanho máximo configurado, ou seja, não pode ser menor do que o limite máximo definido;
    2. O nome do arquivo deve ser configurável, como um prefixo, ao qual deve ser anexado uma marca de tempo do momento da abertura do arquivo, e.g.:
        ```
        nome_arquivo = PREFIXO
        Arquivos gerados:
        PREFIXO_20180730145530
        ```
    3. O servidor deve ativar um timer configurável e, caso o cliente não transmita dados por um tempo igual ou maior a este período, cancelar a conexão;

3. Avançados
    1. Permitir a conexão simultânea de vários clientes;
    2. Garantir que os dados de cada conexão sejam armazenados em arquivos separados, cada qual com sua própria sequência.

Observações: Se faz necessário o uso da Boost como biblioteca para desenvolvimento das comunicações em rede.

-----

## Sobre o desenvolvimento
```
Eduardo Alves da Silva
Iniciado em: 13/10/2024
```

### Dependências
 > 1. Boost 1.86.0
 > 2. STL C++/11

### Tempo aproximado de desenvolvimento
* Codificação: 4 horas
* Testes e ajustes: 12 horas
* TOTAL: 16 horas (2 dias).

### Recursos utilizados
**BOOST:**
1. Ponto de partida/Start point: https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
2. Configuração/Setup: https://think-async.com/Asio/asio-1.10.6/doc/asio/using.html
3. Temporizador/timers: https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/tutorial/tuttimer1.html
4. Documentação boost - leitura assíncrona/asynchronous read: http://charette.no-ip.com:81/programming/doxygen/boost/group__async__read.html e http://charette.no-ip.com:81/programming/doxygen/boost/group__async__read__until.html
5. Documentação boost - servidor assíncrono/asynchronous server e timers: https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio/tutorial.html
6. Documentação boost - buffers: https://beta.boost.org/doc/libs/1_52_0/doc/html/boost_asio/reference/buffer.html

Também consultei fóruns como StackOverflow e afins, mas não anotei todas as consultas.

**Variáveis de ambiente em C++:**
1. https://en.cppreference.com/w/cpp/utility/program/getenv

### Dificuldades encontradas
1. Não havia usado a BOOST previamente, então demorei um pouco para entender como funciona o tratamento das operações assíncronas na biblioteca e o fluxo das operações.
2. A maior dificuldade foi trabalhar com "buffers" dinâmicos para recepção no servidor do conteúdo enviado pelo cliente de "tamanho desconhecido", pois a maioria das fontes buscadas indicava o uso de buffers de tamanho limitado.
3. A última parte feita foi a adição do timer para atender ao requisito de "Timeout". Foi mais na linha de encaixar o temporizador na estrutura de código que já realizava todas as demais operações.

Em resumo, trabalhar com a boost pela primeira vez.

### Facilidades
1. Entender a dinâmica de funcionamento de comunicação TCP/IP;
2. Noção da orientação a objetos para separar as tarefas no código;
3. Trabalhar com arquivos;
4. Lógica das operações.

-------

## Instruções

Há 2 projetos no repositório, o [`servidor`](https://github.com/edualvss/cog_async_socket/tree/main/cog_server) e o [`cliente`](https://github.com/edualvss/cog_async_socket/tree/main/cog_client).

É necessário compilar os 2 projetos! Isso pode ser feito separadamente, pois há um CMakeLists para cada projeto, ou na raiz do repositório com o CMakeLists.txt que gera os 2 executáveis.

### Variáveis de ambiente

Há um arquivo de configuração das `variáveis de ambiente`, dentro do diretório do servidor chamado de [`setenv.sh`](https://github.com/edualvss/cog_async_socket/blob/main/cog_server/setenv.sh).

Também há um link simbólico no projeto do cliente que aponta para o mesmo arquivo de configuração das variáveis de ambiente do servidor, embora o cliente precise apenas de 1 variável configurada.

Para configurar as variáveis de ambiente, no diretório raiz do projeto do servidor execute o comando:
> `source setenv.sh`

Isso também pode ser feito no diretório do cliente devido ao link simbólico.

As variáveis de ambiente necessárias para o servidor são:
> `COG_FILESIZE_LIMIT`=51200  # Tamanho limite dos arquivos em bytes. Por exemplo, 51200 bytes corresponde a arquivos de 50Kb como tamanho limite.
> 
> `COG_FILENAME_PREFIX`=EduardoChallenge # Uma string que será prefixada ao nome dos arquivos gerados com o conteúdo que o servidor recebe das conexões dos clientes.
> 
> `COG_SERVER_PORT`=1024 # Porta em que o servidor ficará "escutando" (listen)
> 
> `COG_SERVER_TIMEOUT`=5000 # Tempo em que a conexão com o cliente é encerrada ao permanecer ociosa (sem envio de dados e nem conclusão da conexão)

### Execução

#### Servidor

Com as variáveis configuradas, basta executar o servidor: 
> `./server`


#### Cliente

O cliente está "hard-coded" para conectar em um serviço endereçado na máquina local (localhost). 

Dessa forma, é necessário apenas configurar a porta de comunicação pela variável de ambiente `COG_SERVER_PORT`. 

O valor variável de ambiente `COG_SERVER_PORT` deve ser o mesmo da configuração feita para o servidor. 

A configuração de ambiente pode ser feita no diretório do cliente pelo link simbólico do arquivo de configuração do servidor.

Com a variável configurada e o servidor **"online"**, o cliente pode ser executado de 2 formas:
> 1. `./client`: Envia um texto qualquer (lorem ipsum) para o servidor com 1025 caracteres/bytes.
> 
> 2. `./cliente <arquivo>`: Envia o conteúdo do arquivo passado como argumento na linha de comando para o servidor.

--------------

Qualquer dúvida, get in touch: eduardo.univali@gmail.com
