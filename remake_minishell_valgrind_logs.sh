make re && make clean && valgrind -s --leak-check=full --show-reachable=yes --track-fds=yes --trace-children=yes --log-file=logs/%pvalgrind.log --suppressions=minishell_valgrind.supp ./minishell
