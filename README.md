# minishell

# TEST
```
make ltest
./test
```
## テストケースの変更
- srcs/lexer/tester.cのファイル名を書き換える

## Builtin関数のそれぞれのテスト用
```
gcc -I includes -I libft/includes libft/libft.a srcs/builtins/ft_cd.c
```
