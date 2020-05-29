#include "../main.h"

bool after_sig = false;
struct file_manager *file_manager;

void init_42sh_with_history(void)
{
    signal(SIGINT, sighandler);

    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;
    struct history *history = open_history();
    file_manager = init_file_manager();
    if (!option->norc_flag)
    {
        char *path = xmalloc(strlen(getenv("HOME")) + 9);
        path = strcpy(path, getenv("HOME"));
        path = strcat(path, "/.42shrc");
        load_file(path, true);
        load_file("/etc/42shrc", true);
    }
    else if (option->file_path)
        load_file(option->file_path, true);
    if (option->cmd)
    {
        if ((lexer = new_lexer(option->cmd)))
        {
            ast = parse(lexer);
            exec_node_input(ast);
            if (option->print_ast_flag)
                print_ast(ast);
        }
        free_garbage_collector();
    }
    else
    {
        int char_after_start = 0;
        char *line = NULL;
        struct buffer *buffer = new_buffer();
        if (print_prompt() == 1)
            return;
        int c;
        if (after_sig)
            return;
        while ((c = getch2()) != 4)
        {
            if (after_sig)
                return;
            if (c == 91) // top arrow
            {
                /*
                ** Top arrow : "[A"
                ** Down arrow : "[B"
                ** Right arrow : "[C"
                ** Left arrow : "[D"
                ** Cette partie vérifie le 2eme caractère
                ** Si A/B/C/D, le A n'a pas besoin d'être append au buffer ni afficher
                **             et on exécute les flèches
                ** Sinon, on rajoute le '[' et le deuxième caractère au buffer et on les affiche
                */
                if ((c = getch2()) == 65)
                {
                    /*
                    ** Efface l'input actuelle affichée sur le shell
                    ** Problème : besoin de rajouter un printf avant sinon ça bug
                    */
                    printf("a");
                    int idx = buffer->index;
                    int cas = char_after_start;
                    while (idx < cas)
                    {
                        putchar('a');
                        idx++;
                    }
                    while (char_after_start > 0)
                    {
                        char_after_start--;
                        delete_last_character();
                    }
                    /*
                    ** Retire l'ancienne input du buffer
                    */
                    flush(buffer);
                    /*
                    ** Récupère l'history
                    ** Affiche et ajoute caractère par caractère la nouvelle input (history)
                    */
                    char *command = write_next_history(history);
                    if (command)
                    {
                        for (size_t i = 0; i < strlen(command); i++)
                        {
                            putchar(command[i]);
                            char_after_start++;
                            append_buffer(buffer, command[i]);
                        }
                    }
                    continue;
                }
                else if (c == 66)
                {
                    // down arrow
                    /*
                    ** Efface l'input actuelle affichée sur le shell
                    ** Problème : besoin de rajouter un printf avant sinon ça bug
                    */
                    printf("a");
                    int idx = buffer->index;
                    int cas = char_after_start;
                    while (idx < cas)
                    {
                        putchar('a');
                        idx++;
                    }
                    while (char_after_start > 0)
                    {
                        char_after_start--;
                        delete_last_character();
                    }
                    /*
                    ** Retire l'ancienne input du buffer
                    */
                    flush(buffer);
                    /*
                    ** Récupère l'history
                    ** Affiche et ajoute caractère par caractère la nouvelle input (history)
                    */
                    char *command = write_prev_history(history);
                    if (command)
                    {
                        for (size_t i = 0; i < strlen(command); i++)
                        {
                            putchar(command[i]);
                            char_after_start++;
                            append_buffer(buffer, command[i]);
                        }
                    }
                    continue;
                }
                else if (c == 67)
                {
                    // right arrow
                    if (buffer->index < char_after_start)
                    {
                        putchar('a');
                        putchar(buffer->buf[buffer->index]);
                        buffer->index++;
                    }
                    else
                        putchar('a');
                    continue;
                }
                else if (c == 68)
                {
                    if (buffer->index > 0)
                    {
                        buffer->index--;
                        putchar('\b');
                    }
                    putchar('a');
                    continue;
                }
                append_buffer(buffer, 91);
                putchar(91);
                char_after_start++;
                append_buffer(buffer, c);
                putchar(c);
                char_after_start++;
                continue;
            }
            else if (c == '\n')
            {
                char *new = xcalloc(1, MAX_STR_LEN);
                strcpy(new, buffer->buf);
                append_history_command(history, new);
                putchar('\n');
                append_buffer(buffer, '\n');

                if ((lexer = new_lexer(new)))
                {
                    flush(buffer);
                    ast = parse(lexer);
                    exec_node_input(ast);
                    if (after_sig)
                        return;
                    if (option->print_ast_flag)
                        print_ast(ast);
                }
                flush(buffer);
                print_prompt();
                char_after_start = 0;
                continue;
            }
            else if (c == 127) // delete
            {
                if (buffer->index > 0)
                {
                    if (buffer->index < char_after_start)
                    {
                        putchar('\b');
                        for (int i = buffer->index - 1;
                            i <= char_after_start + 1;
                             i++)
                        {
                            buffer->buf[i] = buffer->buf[i + 1];
                            putchar(buffer->buf[i]);
                        }
                        putchar(' ');
                        for (int i = buffer->index; i <= char_after_start;
                            i++)
                            putchar('\b');
                        char_after_start--;
                        buffer->index--;
                    }
                    else
                    {
                        delete_last_character();
                        char_after_start--;
                        buffer->buf[--buffer->index] = '\0';
                    }
                }
            }
            else if (c == 4) // Ctrl+D
            {
                free_garbage_collector();
                putchar('\n');
                return;
            }
            else if (c == 9) // Tabulation
            {
                char *best_fit = get_auto_completion(history, buffer->buf);
                if (best_fit)
                {
                    while (char_after_start--)
                        delete_last_character();
                    flush(buffer);

                    for (size_t i = 0; i < strlen(best_fit); i++)
                    {
                        putchar(best_fit[i]);
                        char_after_start++;
                        append_buffer(buffer, best_fit[i]);
                    }
                    // append_string_to_buffer(buffer, best_fit);
                }
            }
            else
            {
                putchar(c);
                if (c != 27)
                {
                    if (buffer->index < char_after_start)
                    {
                        for (int i = char_after_start; i > buffer->index; i--)
                            buffer->buf[i] = buffer->buf[i - 1];
                        buffer->buf[buffer->index++] = c;
                        for (int i = buffer->index; i <= char_after_start;
                             i++)
                            putchar(buffer->buf[i]);
                        for (int i = buffer->index; i <= char_after_start;
                             i++)
                            putchar('\b');
                    }
                    else
                        append_buffer(buffer, c);
                    char_after_start++;
                }
            }
        }
        if (line)
            free(line);
        printf("\n");
    }
}

void init_42sh_without_history(void)
{
    signal(SIGINT, sighandler_without);
    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;
    if (option->cmd)
    {
        if ((lexer = new_lexer(option->cmd)))
        {
            ast = parse(lexer);
            exec_node_input(ast);
            if (option->print_ast_flag)
                print_ast(ast);
        }
        free_garbage_collector();
    }
    else
    {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, stdin)) != -1)
        {
            if ((lexer = new_lexer(line)))
            {
                ast = parse(lexer);
                exec_node_input(ast);

                if (option->print_ast_flag)
                    print_ast(ast);
            }
            close(0);
        }
        if (line)
            free(line);
    }
}

void print_usage(void)
{
    fprintf(stdout, USAGE);
    exit(1);
}

int print_prompt(void)
{
    char *buff = NULL;
    if ((buff = getcwd(NULL, 0)) == NULL)
        exit(1);
    if (isatty(0) == 1)
    {
        dprintf(0, "%s[%s%s%s", START_COLOR, CYAN, buff, END_COLOR);
        dprintf(0, "%s[%s %s> %s", START_COLOR, CYAN, BLINK, END_COLOR);
    }
    else
    {
        init_42sh_without_history();
        free(buff);
        return 1;
    }
    free(buff);
    return 0;
}
int file_exists(const char *filename)
{
    /* try to open file to read */
    FILE *file;
    file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void delete_last_character(void)
{
    putchar('\b');  // se place sur le dernier caractère
    putchar(' ');   // le remplace temporairement par un espace
    putchar('\b');  // se place sur l'espace pour qu'il soit écrasé par le prochain caractère
}


void sighandler(int signum)
{
    signum = signum;
    if (file_manager->fd_to_close != -1)
    {
        close(file_manager->fd_to_close);
        fclose(file_manager->file);
    }
    dup2(file_manager->save_in, 0);
    dup2(file_manager->save_out, 1);
    dup2(file_manager->save_err, 2);
    free_garbage_collector();
    printf("\n");
    init_42sh_with_history();
    after_sig = true;
}

void sighandler_without(int signum)
{
    signum = signum;
    if (file_manager->fd_to_close != -1)
    {
        close(file_manager->fd_to_close);
        fclose(file_manager->file);
    }
    dup2(file_manager->save_in, 0);
    dup2(file_manager->save_out, 1);
    dup2(file_manager->save_err, 2);
    free_garbage_collector();
    printf("\n");
}

int getch2(void)
{
    struct termios org_opts;
    struct termios new_opts;
    int c = 0;
    int res = 0;
    res = tcgetattr(STDIN_FILENO, &org_opts);
    if (res != 0)
    {
        printf("getch2(): first assertion failed\n");
        return -1;
    }
    //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c = getchar();
    //------  restore old settings ---------
    res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    if (res != 0)
    {
        printf("getch2(): first assertion failed\n");
        return -1;
    }
    return(c);
}
