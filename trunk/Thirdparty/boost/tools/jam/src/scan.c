/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

#include "jam.h"
#include "lists.h"
#include "parse.h"
#include "scan.h"
#include "jamgram.h"
#include "jambase.h"
#include "newstr.h"

/*
 * scan.c - the jam yacc scanner
 *
 * 12/26/93 (seiwald) - bump buf in yylex to 10240 - yuk.
 * 09/16/94 (seiwald) - check for overflows, unmatched {}'s, etc.
 *          Also handle tokens abutting EOF by remembering
 *          to return EOF now matter how many times yylex()
 *          reinvokes yyline().
 * 02/11/95 (seiwald) - honor only punctuation keywords if SCAN_PUNCT.
 * 07/27/95 (seiwald) - Include jamgram.h after scan.h, so that YYSTYPE is
 *          defined before Linux's yacc tries to redefine it.
 */

struct keyword
{
    char * word;
    int    type;
} keywords[] =
{
#include "jamgramtab.h"
    { 0, 0 }
};

struct include
{
    struct include   * next;       /* next serial include file */
    char             * string;     /* pointer into current line */
    char           * * strings;    /* for yyfparse() -- text to parse */
    FILE             * file;       /* for yyfparse() -- file being read */
    char             * fname;      /* for yyfparse() -- file name */
    int                line;       /* line counter for error messages */
    char               buf[ 512 ]; /* for yyfparse() -- line buffer */
};

static struct include * incp = 0; /* current file; head of chain */

static int scanmode = SCAN_NORMAL;
static int anyerrors = 0;


static char * symdump( YYSTYPE * );

#define BIGGEST_TOKEN 10240  /* no single token can be larger */


/*
 * Set parser mode: normal, string, or keyword.
 */

void yymode( int n )
{
    scanmode = n;
}


void yyerror( char * s )
{
    /* We use yylval instead of incp to access the error location information as
     * the incp pointer will already be reset to 0 in case the error occurred at
     * EOF.
     *
     * The two may differ only if we get an error while reading a lexical token
     * spanning muliple lines, e.g. a multi-line string literal or action body,
     * in which case yylval location information will hold the information about
     * where this token started while incp will hold the information about where
     * reading it broke.
     *
     * TODO: Test the theory about when yylval and incp location information are
     * the same and when they differ.
     */
    printf( "%s:%d: %s at %s\n", yylval.file, yylval.line, s, symdump( &yylval ) );
    ++anyerrors;
}


int yyanyerrors()
{
    return anyerrors != 0;
}


void yyfparse( char * s )
{
    struct include * i = (struct include *)BJAM_MALLOC( sizeof( *i ) );

    /* Push this onto the incp chain. */
    i->string = "";
    i->strings = 0;
    i->file = 0;
    i->fname = copystr( s );
    i->line = 0;
    i->next = incp;
    incp = i;

    /* If the filename is "+", it means use the internal jambase. */
    if ( !strcmp( s, "+" ) )
        i->strings = jambase;
}


/*
 * yyline() - read new line and return first character.
 *
 * Fabricates a continuous stream of characters across include files, returning
 * EOF at the bitter end.
 */

int yyline()
{
    struct include * i = incp;

    if ( !incp )
        return EOF;

    /* Once we start reading from the input stream, we reset the include
     * insertion point so that the next include file becomes the head of the
     * list.
     */

    /* If there is more data in this line, return it. */
    if ( *i->string )
        return *i->string++;

    /* If we are reading from an internal string list, go to the next string. */
    if ( i->strings )
    {
        if ( *i->strings )
        {
            ++i->line;
            i->string = *(i->strings++);
            return *i->string++;
        }
    }
    else
    {
        /* If necessary, open the file. */
        if ( !i->file )
        {
            FILE * f = stdin;
            if ( strcmp( i->fname, "-" ) && !( f = fopen( i->fname, "r" ) ) )
                perror( i->fname );
            i->file = f;
        }

        /* If there is another line in this file, start it. */
        if ( i->file && fgets( i->buf, sizeof( i->buf ), i->file ) )
        {
            ++i->line;
            i->string = i->buf;
            return *i->string++;
        }
    }

    /* This include is done. Free it up and return EOF so yyparse() returns to
     * parse_file().
     */

    incp = i->next;

    /* Close file, free name. */
    if ( i->file && ( i->file != stdin ) )
        fclose( i->file );
    freestr( i->fname );
    BJAM_FREE( (char *)i );

    return EOF;
}


/*
 * yylex() - set yylval to current token; return its type.
 *
 * Macros to move things along:
 *
 *  yychar() - return and advance character; invalid after EOF.
 *  yyprev() - back up one character; invalid before yychar().
 *
 * yychar() returns a continuous stream of characters, until it hits the EOF of
 * the current include file.
 */

#define yychar() ( *incp->string ? *incp->string++ : yyline() )
#define yyprev() ( incp->string-- )

int yylex()
{
    int c;
    char buf[ BIGGEST_TOKEN ];
    char * b = buf;

    if ( !incp )
        goto eof;

    /* Get first character (whitespace or of token). */
    c = yychar();

    if ( scanmode == SCAN_STRING )
    {
        /* If scanning for a string (action's {}'s), look for the closing brace.
         * We handle matching braces, if they match.
         */

        int nest = 1;

        while ( ( c != EOF ) && ( b < buf + sizeof( buf ) ) )
        {
            if ( c == '{' )
                ++nest;

            if ( ( c == '}' ) && !--nest )
                break;

            *b++ = c;

            c = yychar();

            /* Turn trailing "\r\n" sequences into plain "\n" for Cygwin. */
            if ( ( c == '\n' ) && ( b[ -1 ] == '\r' ) )
                --b;
        }

        /* We ate the ending brace -- regurgitate it. */
        if ( c != EOF )
            yyprev();

        /* Check for obvious errors. */
        if ( b == buf + sizeof( buf ) )
        {
            yyerror( "action block too big" );
            goto eof;
        }

        if ( nest )
        {
            yyerror( "unmatched {} in action block" );
            goto eof;
        }

        *b = 0;
        yylval.type = STRING;
        yylval.string = newstr( buf );
        yylval.file = incp->fname;
        yylval.line = incp->line;
    }
    else
    {
        char * b = buf;
        struct keyword * k;
        int inquote = 0;
        int notkeyword;

        /* Eat white space. */
        for ( ;; )
        {
            /* Skip past white space. */
            while ( ( c != EOF ) && isspace( c ) )
                c = yychar();

            /* Not a comment? */
            if ( c != '#' )
                break;

            /* Swallow up comment line. */
            while ( ( ( c = yychar() ) != EOF ) && ( c != '\n' ) ) ;
        }

        /* c now points to the first character of a token. */
        if ( c == EOF )
            goto eof;

        yylval.file = incp->fname;
        yylval.line = incp->line;

        /* While scanning the word, disqualify it for (expensive) keyword lookup
         * when we can: $anything, "anything", \anything
         */
        notkeyword = c == '$';

        /* Look for white space to delimit word. "'s get stripped but preserve
         * white space. \ protects next character.
         */
        while
        (
            ( c != EOF ) &&
            ( b < buf + sizeof( buf ) ) &&
            ( inquote || !isspace( c ) )
        )
        {
            if ( c == '"' )
            {
                /* begin or end " */
                inquote = !inquote;
                notkeyword = 1;
            }
            else if ( c != '\\' )
            {
                /* normal char */
                *b++ = c;
            }
            else if ( ( c = yychar() ) != EOF )
            {
                /* \c */
                if (c == 'n')
                    c = '\n';
                else if (c == 'r')
                    c = '\r';
                *b++ = c;
                notkeyword = 1;
            }
            else
            {
                /* \EOF */
                break;
            }

            c = yychar();
        }

        /* Check obvious errors. */
        if ( b == buf + sizeof( buf ) )
        {
            yyerror( "string too big" );
            goto eof;
        }

        if ( inquote )
        {
            yyerror( "unmatched \" in string" );
            goto eof;
        }

        /* We looked ahead a character - back up. */
        if ( c != EOF )
            yyprev();

        /* Scan token table. Do not scan if it is obviously not a keyword or if
         * it is an alphabetic when were looking for punctuation.
         */

        *b = 0;
        yylval.type = ARG;

        if ( !notkeyword && !( isalpha( *buf ) && ( scanmode == SCAN_PUNCT ) ) )
            for ( k = keywords; k->word; ++k )
                if ( ( *buf == *k->word ) && !strcmp( k->word, buf ) )
                { 
                    yylval.type = k->type;
                    yylval.string = k->word;  /* used by symdump */
                    break;
                }

        if ( yylval.type == ARG )
            yylval.string = newstr( buf );
    }

    if ( DEBUG_SCAN )
        printf( "scan %s\n", symdump( &yylval ) );

    return yylval.type;

eof:
    /* We do not reset yylval.file & yylval.line here so unexpected EOF error
     * messages would include correct error location information.
     */
    yylval.type = EOF;
    return yylval.type;
}


static char * symdump( YYSTYPE * s )
{
    static char buf[ BIGGEST_TOKEN + 20 ];
    switch ( s->type )
    {
        case EOF   : sprintf( buf, "EOF"                          ); break;
        case 0     : sprintf( buf, "unknown symbol %s", s->string ); break;
        case ARG   : sprintf( buf, "argument %s"      , s->string ); break;
        case STRING: sprintf( buf, "string \"%s\""    , s->string ); break;
        default    : sprintf( buf, "keyword %s"       , s->string ); break;
    }
    return buf;
}


/*
 * Get information about the current file and line, for those epsilon
 * transitions that produce a parse.
 */

void yyinput_stream( char * * name, int * line )
{
    if ( incp )
    {
        *name = incp->fname;
        *line = incp->line;
    }
    else
    {
        *name = "(builtin)";
        *line = -1;
    }
}
