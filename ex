comando: echo ">" > output
Matriz:
{"echo", ">"}
{">", "output"}

comando: ls > output -la
Matriz:
{"ls", "-la"};
{">", "output"}

comando: < ex cat
Matriz:
{"<", "ex"}
{"cat"}

comando: ls -la | grep Makefile
Matriz:
{"ls", "-la"}
{"|"}
{"grep", "Makefile"}
