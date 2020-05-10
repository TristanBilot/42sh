#include "../main.h"

void init_42sh_with_history(struct option_sh *option)
{
    signal(SIGINT, sighandler);
    struct buffer *buffer = new_buffer();

    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;
    struct history *history = open_history();
    if (option->cmd)
    {
        lexer = new_lexer(option->cmd);
        ast = parse(lexer);
        exec_node_input(ast);
        if (option->print_ast_flag)
            print_ast(ast);
        free_garbage_collector();
    }
    else
    {
        int c;   
        
        int char_after_start = 0;
        char * line = NULL;
        struct buffer *buffer = new_buffer();
        if (print_prompt() == 1)
            return;
        
        while ((c = getch2()) != 'q')
        {
            if (c == 91) // top arrow   old value: 27
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
                    while (--char_after_start)
                        delete_last_character();
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
                    while (--char_after_start)
                        delete_last_character();
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
                    continue;
                }
                else if (c == 68)
                {
                    if (char_after_start > 1)
                    {
                        buffer->index -= 2;
                        
                        putchar('\b');
                    }
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

                lexer = new_lexer(buffer->buf);
                flush(buffer);
                ast = parse(lexer);
                
                exec_node_input(ast);
                
                //if(!option->norc_flag)
                    //  load_ressources();
                if (option->print_ast_flag)
                    print_ast(ast);
                flush(buffer);
                print_prompt();
                char_after_start = 0;
                continue;
            }
            else if (c == 127) // delete
            {
                // printf("DELETE ");
                if (buffer->index > 0)
                {
                    delete_last_character();
                    char_after_start--;
                    buffer->buf[buffer->index--] = '\0';
                }
                // (void) getc(stdin);
            }
            else if (c == 4) // EOF :  Ctrl+D
            {
                // printf("CTRL+D ");
                // printf("enter eof: Ctrl+D\n");
                free_garbage_collector();
                putchar('\n');
                return;
            }
            else if (c == 9) // Tabulation
            {
                char *best_fit = get_auto_completion(history, buffer->buf);
                if (best_fit)
                {
                    putchar('\n');
                    printf("\n%s\n", best_fit);
                    flush(buffer);
                    append_string_to_buffer(buffer, best_fit);
                }
            }
            else
            {
                // printf("APPEND ");
                append_buffer(buffer, c);
                putchar(c);
                char_after_start++;
            }
            // printf("%d : in the buffer at exec : //%s//\n", c, buffer->buf);
            // tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
            // if (c != '\n' || c != 'a')
            // {
            //     append_buffer(buffer, c);
            //     putchar(c);
            // }
        }
        /*restore the old settings*/
        // tcsetattr( STDIN_FILENO, TCSANOW, &org_opts);
        if (line)
            free(line);
        printf("\n");
    }
}

void init_42sh_without_history(struct option_sh *option)
{
    signal(SIGINT, sighandler);

    struct lexer *lexer = NULL;
    struct node_input *ast = NULL;
    // struct history *history = open_history();
    if (option->cmd)
    {
        lexer = new_lexer(option->cmd);
        ast = parse(lexer);
        exec_node_input(ast);
        if (option->print_ast_flag)
            print_ast(ast);
        free_garbage_collector();
    }
    else
    {   
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        // print_prompt();
        // struct buffer *buffer = new_buffer();
        while ((read = getline(&line, &len, stdin)) != -1)
        {
                // append_history_command(history, line);
                lexer = new_lexer(line);
                ast = parse(lexer);
                exec_node_input(ast);

                if (option->print_ast_flag)
                    print_ast(ast);

                // print_prompt();
                // write_next_history(history);
        }
        if (line)
            free(line);
            
        // printf("\n");
    }
}

/*void  INThandler(int sig)
{
    signal(sig, SIGINT);
    printf("\n");
    exit(0);
}*/



// void load_ressource_files()
// {
//     char line[LINE_BUFSIZE];
//     int linenr;
//     FILE *pipe;
    
//     /* Get a pipe where the output from the scripts comes in */
//     pipe = popen("./42shrc.sh", "r");
//     if (pipe == NULL) {  /* check for errors */
//         perror(argv[0]); /* report error message */
//         return 1;        /* return with exit code indicating error */
//     }

//     /* Read script output from the pipe line by line */
//     linenr = 1;
//     while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
//         printf("Script output line %d: %s", linenr, line);
//         ++linenr;
//     }
//     /* Once here, out of the loop, the script has ended. */
//     pclose(pipe); /* Close the pipe */
//     return 0;     /* return with exit code indicating success. */
// }


void print_usage()
{
    fprintf(stdout, USAGE);
    exit(1);
}

int print_prompt()
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
        init_42sh_without_history(option);
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

// void load_ressources()
// {
//     char cwd[MAX_STR_LEN];
//     char *args[] = { "load_ressource", NULL };
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//     {
//         printf("path :%s\n", cwd);
//         strcat(cwd, "/load_ressource");
//         printf("path :%s\n", cwd);
//     }
//    else
//    {
//        perror("getcwd() error");
//    }
   
//     if (file_exists(cwd))
//     {
//         chmod(cwd, S_IRWXU);
//         execv(cwd, args);
//     }
//     else
//     {
//         printf("erreur\n");
//     }
// }

void sighandler(int signum)
{
    (void) signum;
    free_garbage_collector();
    printf("\n");
    init_42sh_with_history(option);
}

bool sould_use_history(void)
{
    struct termios org_opts;
    struct termios new_opts;
    int c=0;
      
    int res=0;
    res=tcgetattr(STDIN_FILENO, &org_opts);
    return res != 0;
}

int getch2(void) 
{
    struct termios org_opts;
    struct termios new_opts;
    int c=0;
      
    int res=0;
    res=tcgetattr(STDIN_FILENO, &org_opts);
    if (res != 0)
    {
        printf("getch2(): first assertion failed\n");
        return -1;
    }
        //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO /*| ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL*/);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    
    c=getchar();
        //------  restore old settings ---------
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    if (res != 0)
    {
        printf("getch2(): first assertion failed\n");
        return -1;
    }
    return(c);
}
