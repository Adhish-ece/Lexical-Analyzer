#include"utility.h"


const char *KEYWORDS[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while", "main"
};

const int NUM_KEYWORDS = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);


const char *OPERATORS[] = {
    "+", "-", "*", "/", "%", "++", "--", "=",
    "==", "!=", ">", "<", ">=", "<=", "+=", "-=", "*=", "/=",
    "&", "|", "^", "~", "<<", ">>", "&&", "||", "!", "?", ":"
};
const int NUM_OPERATORS = sizeof(OPERATORS) / sizeof(OPERATORS[0]);


const char DELIMITERS[] = {'(', ')', '{', '}', '[', ']', ';', ',', '.', ':'};
const int NUM_DELIMITERS = sizeof(DELIMITERS) / sizeof(DELIMITERS[0]);


char *readFileContent(char *fname)
{
    FILE *fp = fopen(fname,"rb");
    if(!fp)
    {
        perror("Error opening file");
        return NULL;
    }

    long file_size;
    char *buffer = NULL;
    fseek(fp,0,SEEK_END);
    file_size = ftell(fp);
    rewind(fp);
    buffer = (char *)malloc(file_size+1);
    if(buffer==NULL)
    {
        perror("Error allocating memory");
        fclose(fp);
        return NULL;
    }
    if(fread(buffer,1,file_size,fp)!=file_size)
    {
        printf("Error reading the file\n");
        free(buffer);
        fclose(fp);
        return NULL;
    }
    buffer[file_size] = '\0';
    fclose(fp);
    return buffer;
}

int isNumericConstant(char *str)
{
    if(!str || strlen(str)==0)
    return 0;
    int dotCount =0;
    int digitCount =0;
    for(int i =0;str[i]!='\0';i++)
    {
        if(i==0 && (str[i]=='+' || str[i]=='-'))
        {
            continue;
        }
        if(str[i]=='.')
        dotCount++;
        else if(!isdigit(str[i]))
        {
            return 0;
        }  
        else
        {
            digitCount++;
        }
    }
    return (digitCount>0 && dotCount<=1);
}

int isIdentifier(char *str)
{
    if(!str || strlen(str)==0)
    return 0;
    if(!(isalpha(str[0]) || str[0]=='_'))
    return 0;
    for(int i =1;i<strlen(str);i++)
    {
        if(!(isalnum(str[i]) || str[i]=='_'))
        return 0;
    }
    return !isKeyword(str);
}

Status isDelimeter(char ch)
{
    for(int i =0;i<NUM_DELIMITERS;i++)
    {
        if(ch==DELIMITERS[i])
        return True;
    }
    return False;
}

Status isKeyword(char *str)
{
    if(!str || strlen(str)==0)
    return False;
    for(int i =0;i<NUM_KEYWORDS;i++)
    {
        if(strcmp(str,KEYWORDS[i])==0)
        {
            return True;
        }
    }
    return False;
}

int matchOperator(char *src,char *operator_token)
{
    if(!src || *src == '\0')
    {
        return 0;
    }
    if(*(src+1)!='\0')
    {
        char two_char_op[3] ={src[0],src[1],'\0'};
        for(int i =0;i<NUM_OPERATORS;i++)
        {
            if(strlen(OPERATORS[i])==2 && strcmp(two_char_op,OPERATORS[i])==0)
            {
                strcpy(operator_token,two_char_op);
                return 2;
            }
        }
    }
    char one_char_op[2]={src[0],'\0'};
    for(int i=0;i<NUM_OPERATORS;i++)
    {
        if(strlen(OPERATORS[i])==1 && strcmp(one_char_op,OPERATORS[i])==0)
        {
            strcpy(operator_token,one_char_op);
            return 1;
        }
    }
    return 0;
}
void lexicalAnalyzer(char *code)
{
    char *current = code;
    char token_buffer[MAX_TOKEN_LEN];
    int brace_balance = 0;
    int paren_balance =0;
    //int line = 0;
    char op_token[3];
    printf("\n--- C Lexical Analyzer ---\n");
   
    printf("|----------------------------------------|-------------------------|\n");
    printf("| %-38s | %-23s |\n", "LEXEME", "TOKEN TYPE");
    printf("|----------------------------------------|-------------------------|\n");

    while(*current !='\0')
    {
        if(isspace(*current))
        {
            current++;
            continue;
        }
        if(*current == '#')
        {
            current++;
            int i =0;
            while(*current !='\0' && !isspace(*current) && i<MAX_TOKEN_LEN-1)
            {
                token_buffer[i++]=*current++;
            }
            token_buffer[i]='\0';
            printf("| %-38s | %-23s |\n", "#", "Preprocessor Directive");
            while(isspace(*current))
            {
                current++;
            }
            if(*current=='<' || *current == '"')
            {
                char close_char = (*current=='<')?'>':'"';
                char *filename_start = current;
                while(*current!=close_char && *current !='\n' && *current !='\0')
                {
                    current++;
                }
                if(*current == close_char)
                {
                    current++;
                    int len = current - filename_start;
                    if(len<MAX_TOKEN_LEN)
                    {
                        strncpy(token_buffer,filename_start,len);
                        token_buffer[len]='\0';
                        printf("| %-38s | %-23s |\n", token_buffer, "File Literal");
                        
                    }
                }
            }
            else
            {
                printf("| %-38s | %-23s |\n", token_buffer, "Preprocessor Command");
            }
            continue;
        }

        if(*current == '/')
        {
            if(*(current+1)=='/')
            {
                while(*current!='\n' && *current != '\0')
                {
                    current++;
                }
            }
        }
        else if(*(current+1)=='*')
        {
            current+=2;
            while(!(*current=='*' && *(current+1) =='/') && *current !='\0');
            {
                current++;
            }
            if(*current!='\0')
            {
                current+=2;
            }
            continue;
        }  

        if(*current =='"' || *current=='\'')
        {
            char quote_char = *current;
            current++;
            int i =0;
            char full_literal[MAX_TOKEN_LEN+2];
            while(*current !=quote_char && *current !='\n' && *current!='\0' && i<MAX_TOKEN_LEN-1)
            {
                if(*current =='\\' && *(current+1)!='\0')
                {
                    token_buffer[i++] = *current++;
                }
                token_buffer[i++] = *current++;
            }
            token_buffer[i]='\0';
            if(*current==quote_char)
            {
                current++;
                char *type =(quote_char=='"')?"String Literal":"Character Literal";
                snprintf(full_literal,sizeof(full_literal),"%c%s%c",quote_char,token_buffer,quote_char);
                printf("| %-38s | %-23s |\n", full_literal, type);
            }
            continue;
        }

        if(isDelimeter(*current))
        {
            char delimiter[2] = {*current,'\0'};
            printf("| %-38s | %-23s |\n", delimiter, "Delimiter");
            
            if (*current == '{') brace_balance++;
            if (*current == '}') brace_balance--;
            if (*current == '(') paren_balance++;
            if (*current == ')') paren_balance--;
            
            current++;
            continue;
        }

        int op_len = matchOperator(current,op_token);
        if(op_len>0)
        {
            printf("| %-38s | %-23s |\n", op_token, "Operator");
            current += op_len;
            continue;
        }

        int i = 0;
        while (*current !='\0' && !isspace(*current) && !isDelimeter(*current) &&!matchOperator(current,op_token))
        {
            token_buffer[i++] = *current++;
        }

        token_buffer[i] ='\0';
        if(i==0)
        {
            current++;
            continue;
        }

        if(isKeyword(token_buffer))
        {
            printf("| %-38s | %-23s |\n", token_buffer, "Keyword");
        }
        else if(isNumericConstant(token_buffer))
        {
            printf("| %-38s | %-23s |\n", token_buffer, "Numeric Constant");
        }
        else if(isIdentifier(token_buffer))
        {
            printf("| %-38s | %-23s |\n", token_buffer, "Identifier");
        }
        else 
        {
            printf("| %-38s | %-23s |\n", token_buffer, "Unknown");
        }
    }
    printf("|________________________________________|_________________________|\n");
    
    if(brace_balance!=0 || paren_balance !=0)
    {
        
        printf("\n|---------------------------------------------------------------|\n");
        printf("| %-61s |\n", "SYNTAX BALANCE CHECK");
        printf("|---------------------------------------------------------------|\n");
        printf("| %-45s %-15d |\n", "[WARN] Unbalanced Braces {} count difference:",brace_balance);
        printf("| %-50s %-10d |\n","[WARN] Unbalanced Parentheses () count difference:", paren_balance);
        printf("|_______________________________________________________________|\n");
    }


}