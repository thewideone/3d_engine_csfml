#include "external_dependencies.h"

static void (*drawLine_func_ptr) ( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

static void (*putText_func_ptr) ( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

void engine3D_register_drawLine( void (*function_ptr) ){
    drawLine_func_ptr = function_ptr;
}

void engine3D_register_putText( void (*function_ptr) ){
    putText_func_ptr = function_ptr;
}

void engine3D_putText( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            ){
    putText_func_ptr( str, x, y, size
#ifndef COLOUR_MONOCHROME
            , colour
#endif
               );
}

void engine3D_drawLine( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            ){
    drawLine_func_ptr( x0, y0, x1, y1
#ifndef COLOUR_MONOCHROME
            , colour
#endif
                );
}

#ifdef USE_LOADING_FROM_OBJ
// Used only by getline() below
ssize_t getdelim(char **linep, size_t *n, int delim, FILE *fp){
    int ch;
    size_t i = 0;
    if(!linep || !n || !fp){
        errno = EINVAL;
        return -1;
    }
    if(*linep == NULL){
        if(NULL==(*linep = malloc(*n=128))){
            *n = 0;
            errno = ENOMEM;
            return -1;
        }
    }
    while((ch = fgetc(fp)) != EOF){
        if(i + 1 >= *n){
            char *temp = realloc(*linep, *n + 128);
            if(!temp){
                errno = ENOMEM;
                return -1;
            }
            *n += 128;
            *linep = temp;
        }
        (*linep)[i++] = ch;
        if(ch == delim)
            break;
    }
    (*linep)[i] = '\0';
    return !i && ch == EOF ? -1 : i;
}

// Used only for loading meshes from .obj files
ssize_t getline(char **linep, size_t *n, FILE *fp){
    return getdelim(linep, n, '\n', fp);
}
#endif