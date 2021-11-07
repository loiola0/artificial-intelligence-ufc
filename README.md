# artificial-intelligence-ufc

Neste projeto foram utilizados os seguintes tipos de busca: 
- Busca em Largura;
- Busca em Profundidade;
- Busca Gulosa (Greedy);
- Busca A*.

## *Instruções para serem executadas no terminal*

1. Para executar o jogo, compile o primeiro com o seguinte comando no terminal: 
`g++ Game15Puzzle.cpp -o main`

2. Agora com o jogo compilado, digite `./main` para executar jogo.

3. Digite a configuração inicial. Exemplo:

| 1 | 2 | 3 | 4 |
|--- |--- |--- |--- |
| 5 | 6 | 8 | 12 |
| 13 | 9 | 0 | 7 |
| 14 | 11 | 10 | 15 |

4. Agora digite como sua solução deve ficar. Exemplo:

| 1 | 2 | 3 | 4 |
|--- |--- |--- |--- |
| 5 | 6 | 7 | 8 |
| 9 | 10 | 11 | 12 |
| 13 | 14 | 15 | 0 |

- Caso queira testar uma configuração sem solução, você pode usar a seguinte: 

| 1 | 2 | 3 | 4 |
|--- |--- |--- |--- |
| 13 | 6 | 8 | 12 |
| 5 | 9 | 0 | 7 |
| 14 | 11 | 10 | 15 |
